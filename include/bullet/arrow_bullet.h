#ifndef _ARROW_BULLET_H_
#define _ARROW_BULLET_H_

// 包含必要的头文件
#include "bullet.h"            // 子弹基类
#include "resources_manager.h" // 资源管理器，用于加载纹理和音效

// 箭矢子弹类：继承自Bullet基类
// 特点：
// 1. 基础单体伤害
// 2. 支持旋转动画
// 3. 随机命中音效
class ArrowBullet : public Bullet
{
public:
     // 构造函数：初始化箭矢子弹的属性
     // 功能：
     // 1. 加载箭矢纹理
     // 2. 设置动画参数
     // 3. 启用旋转
     // 4. 设置尺寸
     ArrowBullet()
     {
          // 从资源管理器获取箭矢纹理
          static SDL_Texture *tex_arrow = ResourcesManager::instance()
                                              ->get_texture_pool()
                                              .find(ResID::Tex_BulletArrow)
                                              ->second;

          // 定义动画帧序列：使用第0和第1帧
          static const std::vector<int> idx_list = {0, 1};

          // 设置动画参数
          animation.set_loop(true);                            // 循环播放
          animation.set_interval(0.1);                         // 每帧间隔0.1秒
          animation.set_frame_data(tex_arrow, 2, 1, idx_list); // 2列1行的纹理，使用指定帧

          can_rotated = true;       // 启用旋转
          size.x = 48, size.y = 48; // 设置尺寸为48x48像素
     }

     ~ArrowBullet() = default;

     // 处理与敌人的碰撞
     // @param enemy: 碰撞的敌人
     // 功能：
     // 1. 随机播放三种命中音效之一
     // 2. 调用基类的碰撞处理
     void on_collide(Enemy *enemy) override
     {
          // 获取音效池
          static const ResourcesManager::SoundPool &sound_pool = ResourcesManager::instance()->get_sound_pool();

          // 随机选择并播放命中音效
          switch (rand() % 3)
          {
          case 0:
               Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_1)->second, 0);
               break;
          case 1:
               Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_2)->second, 0);
               break;
          case 2:
               Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_3)->second, 0);
               break;
          }

          // 调用基类的碰撞处理
          Bullet::on_collide(enemy);
     }
};

#endif // !_ARROW_BULLET_H_
