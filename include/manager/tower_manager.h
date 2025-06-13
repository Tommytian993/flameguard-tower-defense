/**
 * @file tower_manager.h
 * @brief 塔防游戏中的塔管理器，负责管理所有防御塔的创建、升级和更新
 */

#ifndef _TOWER_MANAGER_H_
#define _TOWER_MANAGER_H_

#include "tower/tower.h"
#include "tower/tower_type.h"
#include "manager.h"
#include "tower/archer_tower.h"
#include "tower/axeman_tower.h"
#include "tower/gunner_tower.h"
#include "manager/config_manager.h"
#include "manager/resources_manager.h"

#include <vector>

/**
 * @class TowerManager
 * @brief 塔管理器类，继承自Manager模板类，用于管理游戏中的所有防御塔
 */
class TowerManager : public Manager<TowerManager>
{
     friend class Manager<TowerManager>;

public:
     /**
      * @brief 更新所有防御塔的状态
      * @param delta 时间增量
      */
     void on_update(double delta)
     {
          for (Tower *tower : tower_list)
               tower->on_update(delta);
     }

     /**
      * @brief 渲染所有防御塔
      * @param renderer SDL渲染器指针
      */
     void on_render(SDL_Renderer *renderer)
     {
          for (Tower *tower : tower_list)
               tower->on_render(renderer);
     }

     /**
      * @brief 获取指定类型防御塔的放置成本
      * @param type 防御塔类型
      * @return 放置成本
      */
     double get_place_cost(TowerType type)
     {
          static ConfigManager *instance = ConfigManager::instance();

          switch (type)
          {
          case Archer:
               return instance->archer_template.cost[instance->level_archer];
               break;
          case Axeman:
               return instance->axeman_template.cost[instance->level_axeman];
               break;
          case Gunner:
               return instance->gunner_template.cost[instance->level_gunner];
               break;
          }

          return 0;
     }

     /**
      * @brief 获取指定类型防御塔的升级成本
      * @param type 防御塔类型
      * @return 升级成本，如果已达到最高等级则返回-1
      */
     double get_upgrade_cost(TowerType type)
     {
          static ConfigManager *instance = ConfigManager::instance();

          switch (type)
          {
          case Archer:
               return instance->level_archer == 9 ? -1 : instance->archer_template.upgrade_cost[instance->level_archer];
               break;
          case Axeman:
               return instance->level_axeman == 9 ? -1 : instance->axeman_template.upgrade_cost[instance->level_axeman];
               break;
          case Gunner:
               return instance->level_gunner == 9 ? -1 : instance->gunner_template.upgrade_cost[instance->level_gunner];
               break;
          }

          return 0;
     }

     /**
      * @brief 获取指定类型防御塔的攻击范围
      * @param type 防御塔类型
      * @return 攻击范围
      */
     double get_damage_range(TowerType type)
     {
          static ConfigManager *instance = ConfigManager::instance();

          switch (type)
          {
          case Archer:
               return instance->archer_template.view_range[instance->level_archer];
               break;
          case Axeman:
               return instance->axeman_template.view_range[instance->level_axeman];
               break;
          case Gunner:
               return instance->gunner_template.view_range[instance->level_gunner];
               break;
          }

          return 0;
     }

     /**
      * @brief 在指定位置放置防御塔
      * @param type 防御塔类型
      * @param idx 放置位置的网格索引
      */
     void place_tower(TowerType type, const SDL_Point &idx)
     {
          Tower *tower = nullptr;

          switch (type)
          {
          case Archer:
               tower = new ArcherTower();
               break;
          case Axeman:
               tower = new AxemanTower();
               break;
          case Gunner:
               tower = new GunnerTower();
               break;
          default:
               tower = new ArcherTower();
               break;
          }

          static Vector2 position;
          static const SDL_Rect &rect = ConfigManager::instance()->rect_tile_map;

          position.x = rect.x + idx.x * SIZE_TILE + SIZE_TILE / 2;
          position.y = rect.y + idx.y * SIZE_TILE + SIZE_TILE / 2;
          tower->set_position(position);
          tower_list.push_back(tower);

          ConfigManager::instance()->map.place_tower(idx);

          static const ResourcesManager::SoundPool &sound_pool = ResourcesManager::instance()->get_sound_pool();

          Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_PlaceTower)->second, 0);
     }

     /**
      * @brief 升级指定类型的防御塔
      * @param type 防御塔类型
      */
     void upgrade_tower(TowerType type)
     {
          static ConfigManager *instance = ConfigManager::instance();

          switch (type)
          {
          case Archer:
               instance->level_archer = instance->level_archer >= 9 ? 9 : instance->level_archer + 1;
               break;
          case Axeman:
               instance->level_axeman = instance->level_axeman >= 9 ? 9 : instance->level_axeman + 1;
               break;
          case Gunner:
               instance->level_gunner = instance->level_gunner >= 9 ? 9 : instance->level_gunner + 1;
               break;
          }

          static const ResourcesManager::SoundPool &sound_pool = ResourcesManager::instance()->get_sound_pool();

          Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_TowerLevelUp)->second, 0);
     }

protected:
     TowerManager() = default;
     ~TowerManager() = default;

private:
     std::vector<Tower *> tower_list; ///< 存储所有防御塔的列表
};

#endif // !_TOWER_MANAGER_H_
