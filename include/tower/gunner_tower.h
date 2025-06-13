/**
 * @file gunner_tower.h
 * @brief 枪手塔类，继承自Tower基类，实现远程范围攻击功能
 */

#ifndef _GUNNER_TOWER_H_
#define _GUNNER_TOWER_H_

#include "tower.h"
#include "manager/resources_manager.h"

/**
 * @class GunnerTower
 * @brief 枪手塔类，具有远程范围攻击能力，使用炮弹作为攻击方式
 */
class GunnerTower : public Tower
{
public:
     /**
      * @brief 构造函数，初始化枪手塔的属性和动画
      */
     GunnerTower()
     {
          // 获取枪手塔的纹理
          static SDL_Texture *tex_gunner = ResourcesManager::instance()
                                               ->get_texture_pool()
                                               .find(ResID::Tex_Gunner)
                                               ->second;

          // 定义各个方向的动画帧索引
          static const std::vector<int> idx_list_idle_up = {4, 5};              // 向上待机动画帧
          static const std::vector<int> idx_list_idle_down = {0, 1};            // 向下待机动画帧
          static const std::vector<int> idx_list_idle_left = {12, 13};          // 向左待机动画帧
          static const std::vector<int> idx_list_idle_right = {8, 9};           // 向右待机动画帧
          static const std::vector<int> idx_list_fire_up = {20, 21, 22, 23};    // 向上攻击动画帧
          static const std::vector<int> idx_list_fire_down = {16, 17, 18, 19};  // 向下攻击动画帧
          static const std::vector<int> idx_list_fire_left = {28, 29, 30, 31};  // 向左攻击动画帧
          static const std::vector<int> idx_list_fire_right = {24, 25, 26, 27}; // 向右攻击动画帧

          // 设置各个方向的动画数据
          anim_idle_up.set_frame_data(tex_gunner, 4, 8, idx_list_idle_up);
          anim_idle_down.set_frame_data(tex_gunner, 4, 8, idx_list_idle_down);
          anim_idle_left.set_frame_data(tex_gunner, 4, 8, idx_list_idle_left);
          anim_idle_right.set_frame_data(tex_gunner, 4, 8, idx_list_idle_right);
          anim_fire_up.set_frame_data(tex_gunner, 4, 8, idx_list_fire_up);
          anim_fire_down.set_frame_data(tex_gunner, 4, 8, idx_list_fire_down);
          anim_fire_left.set_frame_data(tex_gunner, 4, 8, idx_list_fire_left);
          anim_fire_right.set_frame_data(tex_gunner, 4, 8, idx_list_fire_right);

          // 设置塔的尺寸
          size.x = 48, size.y = 48;

          // 设置塔的类型
          tower_type = TowerType::Gunner;

          // 设置攻击属性
          fire_speed = 6;                  // 炮弹飞行速度
          bullet_type = BulletType::Shell; // 使用炮弹作为攻击方式
     }

     ~GunnerTower() = default;
};

#endif // !_GUNNER_TOWER_H_
