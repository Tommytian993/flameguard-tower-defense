/**
 * @file axeman_tower.h
 * @brief 斧手塔类，继承自Tower基类，实现近战攻击功能
 */

#ifndef _AXEMAX_H_
#define _AXEMAX_H_

#include "tower.h"
#include "manager/resources_manager.h"

/**
 * @class AxemanTower
 * @brief 斧手塔类，具有近战攻击能力，使用斧头作为攻击方式
 */
class AxemanTower : public Tower
{
public:
     /**
      * @brief 构造函数，初始化斧手塔的属性和动画
      */
     AxemanTower()
     {
          // 获取斧手塔的纹理
          static SDL_Texture *tex_axeman = ResourcesManager::instance()
                                               ->get_texture_pool()
                                               .find(ResID::Tex_Axeman)
                                               ->second;

          // 定义各个方向的动画帧索引
          static const std::vector<int> idx_list_idle_up = {3, 4};          // 向上待机动画帧
          static const std::vector<int> idx_list_idle_down = {0, 1};        // 向下待机动画帧
          static const std::vector<int> idx_list_idle_left = {9, 10};       // 向左待机动画帧
          static const std::vector<int> idx_list_idle_right = {6, 7};       // 向右待机动画帧
          static const std::vector<int> idx_list_fire_up = {15, 16, 17};    // 向上攻击动画帧
          static const std::vector<int> idx_list_fire_down = {12, 13, 14};  // 向下攻击动画帧
          static const std::vector<int> idx_list_fire_left = {21, 22, 23};  // 向左攻击动画帧
          static const std::vector<int> idx_list_fire_right = {18, 19, 20}; // 向右攻击动画帧

          // 设置各个方向的动画数据
          anim_idle_up.set_frame_data(tex_axeman, 3, 8, idx_list_idle_up);
          anim_idle_down.set_frame_data(tex_axeman, 3, 8, idx_list_idle_down);
          anim_idle_left.set_frame_data(tex_axeman, 3, 8, idx_list_idle_left);
          anim_idle_right.set_frame_data(tex_axeman, 3, 8, idx_list_idle_right);
          anim_fire_up.set_frame_data(tex_axeman, 3, 8, idx_list_fire_up);
          anim_fire_down.set_frame_data(tex_axeman, 3, 8, idx_list_fire_down);
          anim_fire_left.set_frame_data(tex_axeman, 3, 8, idx_list_fire_left);
          anim_fire_right.set_frame_data(tex_axeman, 3, 8, idx_list_fire_right);

          // 设置塔的尺寸
          size.x = 48, size.y = 48;

          // 设置塔的类型
          tower_type = TowerType::Axeman;

          // 设置攻击属性
          fire_speed = 5;                // 斧头飞行速度
          bullet_type = BulletType::Axe; // 使用斧头作为攻击方式
     }

     ~AxemanTower() = default;
};

#endif // !_AXEMAX_H_
