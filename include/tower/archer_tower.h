/**
 * @file archer_tower.h
 * @brief 弓箭手塔类，继承自Tower基类，实现远程攻击功能
 */

#ifndef _ARCHER_TOWER_H_
#define _ARCHER_TOWER_H_

#include "tower.h"
#include "resources_manager.h"

/**
 * @class ArcherTower
 * @brief 弓箭手塔类，具有远程攻击能力，使用箭矢作为攻击方式
 */
class ArcherTower : public Tower
{
public:
     /**
      * @brief 构造函数，初始化弓箭手塔的属性和动画
      */
     ArcherTower()
     {
          // 获取弓箭手塔的纹理
          static SDL_Texture *tex_archer = ResourcesManager::instance()
                                               ->get_texture_pool()
                                               .find(ResID::Tex_Archer)
                                               ->second;

          // 定义各个方向的动画帧索引
          static const std::vector<int> idx_list_idle_up = {3, 4};          // 向上待机动画帧
          static const std::vector<int> idx_list_idle_down = {0, 1};        // 向下待机动画帧
          static const std::vector<int> idx_list_idle_left = {6, 7};        // 向左待机动画帧
          static const std::vector<int> idx_list_idle_right = {9, 10};      // 向右待机动画帧
          static const std::vector<int> idx_list_fire_up = {15, 16, 17};    // 向上攻击动画帧
          static const std::vector<int> idx_list_fire_down = {12, 13, 14};  // 向下攻击动画帧
          static const std::vector<int> idx_list_fire_left = {18, 19, 20};  // 向左攻击动画帧
          static const std::vector<int> idx_list_fire_right = {21, 22, 23}; // 向右攻击动画帧

          // 设置各个方向的动画数据
          anim_idle_up.set_frame_data(tex_archer, 3, 8, idx_list_idle_up);
          anim_idle_down.set_frame_data(tex_archer, 3, 8, idx_list_idle_down);
          anim_idle_left.set_frame_data(tex_archer, 3, 8, idx_list_idle_left);
          anim_idle_right.set_frame_data(tex_archer, 3, 8, idx_list_idle_right);
          anim_fire_up.set_frame_data(tex_archer, 3, 8, idx_list_fire_up);
          anim_fire_down.set_frame_data(tex_archer, 3, 8, idx_list_fire_down);
          anim_fire_left.set_frame_data(tex_archer, 3, 8, idx_list_fire_left);
          anim_fire_right.set_frame_data(tex_archer, 3, 8, idx_list_fire_right);

          // 设置塔的尺寸
          size.x = 48, size.y = 48;

          // 设置塔的类型
          tower_type = TowerType::Archer;

          // 设置攻击属性
          fire_speed = 6;                  // 箭矢飞行速度
          bullet_type = BulletType::Arrow; // 使用箭矢作为攻击方式
     }

     ~ArcherTower() = default;
};

#endif // !_ARCHER_TOWER_H_
