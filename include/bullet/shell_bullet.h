#ifndef _SHELL_BULLET_H_
#define _SHELL_BULLET_H_

// 包含必要的头文件
#include "bullet.h"            // 子弹基类
#include "resources_manager.h" // 资源管理器，用于加载纹理和音效

// 炮弹子弹类：继承自Bullet基类
// 特点：
// 1. 范围伤害（96像素范围）
// 2. 爆炸动画效果
// 3. 命中音效
// 4. 两阶段动画：飞行和爆炸
class ShellBullet : public Bullet
{
public:
     // 构造函数：初始化炮弹子弹的属性
     // 功能：
     // 1. 加载炮弹和爆炸纹理
     // 2. 设置飞行和爆炸动画参数
     // 3. 设置伤害范围
     // 4. 设置尺寸
     ShellBullet()
     {
          // 从资源管理器获取炮弹纹理
          static SDL_Texture *tex_shell = ResourcesManager::instance()
                                              ->get_texture_pool()
                                              .find(ResID::Tex_BulletShell)
                                              ->second;
          // 从资源管理器获取爆炸效果纹理
          static SDL_Texture *tex_explode = ResourcesManager::instance()
                                                ->get_texture_pool()
                                                .find(ResID::Tex_EffectExplode)
                                                ->second;

          // 定义动画帧序列
          static const std::vector<int> idx_list = {0, 1};                  // 炮弹飞行动画：2帧
          static const std::vector<int> idx_explode_list = {0, 1, 2, 3, 4}; // 爆炸动画：5帧

          // 设置炮弹飞行动画参数
          animation.set_loop(true);                            // 循环播放
          animation.set_interval(0.1);                         // 每帧间隔0.1秒
          animation.set_frame_data(tex_shell, 2, 1, idx_list); // 2列1行的纹理，使用指定帧

          // 设置爆炸动画参数
          animation_explode.set_loop(false);                                     // 不循环播放
          animation_explode.set_interval(0.1);                                   // 每帧间隔0.1秒
          animation_explode.set_frame_data(tex_explode, 5, 1, idx_explode_list); // 5列1行的纹理，使用指定帧
          // 爆炸动画结束时使子弹失效
          animation_explode.set_on_finished(
              [&]()
              {
                   make_invalid();
              });

          damage_range = 96;        // 设置伤害范围为96像素
          size.x = 48, size.y = 48; // 设置尺寸为48x48像素
     }

     ~ShellBullet() = default;

     // 更新炮弹状态
     // @param delta: 时间增量，单位：秒
     // 功能：
     // 1. 如果可碰撞，更新飞行状态
     // 2. 如果不可碰撞（已爆炸），更新爆炸动画
     void on_update(double delta) override
     {
          if (can_collide())
          {
               Bullet::on_update(delta); // 更新飞行状态
               return;
          }

          animation_explode.on_update(delta); // 更新爆炸动画
     }

     // 渲染炮弹
     // @param renderer: SDL渲染器
     // 功能：
     // 1. 如果可碰撞，渲染飞行状态
     // 2. 如果不可碰撞（已爆炸），渲染爆炸动画
     void on_render(SDL_Renderer *renderer) override
     {
          if (can_collide())
          {
               Bullet::on_render(renderer); // 渲染飞行状态
               return;
          }

          // 计算爆炸效果渲染位置（居中显示）
          static SDL_Point point;
          point.x = (int)(position.x - 96 / 2); // 爆炸效果尺寸为96x96
          point.y = (int)(position.y - 96 / 2);

          animation_explode.on_render(renderer, point); // 渲染爆炸动画
     }

     // 处理与敌人的碰撞
     // @param enemy: 碰撞的敌人
     // 功能：
     // 1. 播放命中音效
     // 2. 禁用碰撞检测，开始爆炸动画
     void on_collide(Enemy *enemy) override
     {
          // 获取音效池
          static const ResourcesManager::SoundPool &sound_pool = ResourcesManager::instance()->get_sound_pool();

          // 播放命中音效
          Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ShellHit)->second, 0);

          // 禁用碰撞检测，开始爆炸动画
          disable_collide();
     }

private:
     Animation animation_explode; // 爆炸动画
};

#endif // !_SHELL_BULLET_H_