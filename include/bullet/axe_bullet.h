#ifndef _AXE_BULLET_H_
#define _AXE_BULLET_H_

// 包含必要的头文件
#include "bullet/bullet.h"             // 子弹基类
#include "manager/resources_manager.h" // 资源管理器，用于加载纹理和音效

// 斧头子弹类：继承自Bullet基类
// 特点：
// 1. 单体伤害
// 2. 命中时使敌人减速
// 3. 随机命中音效
// 4. 8帧旋转动画
class AxeBullet : public Bullet
{
public:
     // 构造函数：初始化斧头子弹的属性
     // 功能：
     // 1. 加载斧头纹理
     // 2. 设置动画参数
     // 3. 设置尺寸
     AxeBullet()
     {
          // 从资源管理器获取斧头纹理
          static SDL_Texture *tex_axe = ResourcesManager::instance()
                                            ->get_texture_pool()
                                            .find(ResID::Tex_BulletAxe)
                                            ->second;

          // 定义动画帧序列：使用0-8帧
          static const std::vector<int> idx_list = {0, 1, 2, 3, 4, 5, 6, 7, 8};

          // 设置动画参数
          animation.set_loop(true);                          // 循环播放
          animation.set_interval(0.1);                       // 每帧间隔0.1秒
          animation.set_frame_data(tex_axe, 4, 2, idx_list); // 4列2行的纹理，使用指定帧

          size.x = 48, size.y = 48; // 设置尺寸为48x48像素
     }

     ~AxeBullet() = default;

     // 处理与敌人的碰撞
     // @param enemy: 碰撞的敌人
     // 功能：
     // 1. 随机播放三种命中音效之一
     // 2. 使敌人减速
     // 3. 调用基类的碰撞处理
     void on_collide(Enemy *enemy) override
     {
          // 获取音效池
          static const ResourcesManager::SoundPool &sound_pool = ResourcesManager::instance()->get_sound_pool();

          // 随机选择并播放命中音效
          switch (rand() % 3)
          {
          case 0:
               Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeHit_1)->second, 0);
               break;
          case 1:
               Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeHit_2)->second, 0);
               break;
          case 2:
               Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeHit_3)->second, 0);
               break;
          }

          // 使敌人减速
          enemy->slow_down();

          // 调用基类的碰撞处理
          Bullet::on_collide(enemy);
     }
};

#endif // !_AXE_BULLET_H_
