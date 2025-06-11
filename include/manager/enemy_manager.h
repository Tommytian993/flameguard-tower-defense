#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include "enemy.h"
#include "manager.h"
#include "config_manager.h"
#include "home_manager.h"
#include "slim_enemy.h"
#include "king_slim_enemy.h"
#include "skeleton_enemy.h"
#include "goblin_enemy.h"
#include "goblin_priest_enemy.h"
#include "bullet_manager.h"
#include "coin_manager.h"

#include <vector>
#include <SDL.h>

// 敌人类管理器
// 负责管理游戏中所有敌人的生命周期、行为、碰撞检测等
// 主要功能：
// 1. 敌人生成：根据波次配置生成不同类型的敌人
// 2. 敌人更新：处理敌人的移动、技能释放等行为
// 3. 碰撞检测：处理敌人与防御塔、基地的碰撞
// 4. 奖励系统：处理敌人死亡后的金币掉落
class EnemyManager : public Manager<EnemyManager>
{
     friend class Manager<EnemyManager>;

public:
     // 敌人列表类型定义：存储所有活跃的敌人对象
     typedef std::vector<Enemy *> EnemyList;

public:
     // 更新所有敌人的状态
     // @param delta: 距离上次更新的时间间隔（秒）
     void on_update(double delta)
     {
          // 更新每个敌人的状态
          for (Enemy *enemy : enemy_list)
               enemy->on_update(delta);

          // 处理与基地的碰撞
          process_home_collision();
          // 处理与子弹的碰撞
          process_bullet_collision();

          // 移除无效的敌人（已死亡或到达终点）
          remove_invalid_enemy();
     }

     // 渲染所有敌人
     // @param renderer: SDL渲染器
     void on_render(SDL_Renderer *renderer)
     {
          for (Enemy *enemy : enemy_list)
               enemy->on_render(renderer);
     }

     // 在指定生成点生成敌人
     // @param type: 敌人类型（史莱姆、史莱姆王、骷髅等）
     // @param idx_spawn_point: 生成点索引
     void spawn_enemy(EnemyType type, int idx_spawn_point)
     {
          static Vector2 position;
          static const SDL_Rect &rect_tile_map = ConfigManager::instance()->rect_tile_map;
          static const Map::SpawnerRoutePool &spawner_route_pool = ConfigManager::instance()->map.get_idx_spawner_pool();

          // 检查生成点是否有效
          const auto &itor = spawner_route_pool.find(idx_spawn_point);
          if (itor == spawner_route_pool.end())
               return;

          Enemy *enemy = nullptr;

          // 根据类型创建对应的敌人对象
          switch (type)
          {
          case EnemyType::Slim:
               enemy = new SlimEnemy();
               break;
          case EnemyType::KingSlim:
               enemy = new KingSlimeEnemy();
               break;
          case EnemyType::Skeleton:
               enemy = new SkeletonEnemy();
               break;
          case EnemyType::Goblin:
               enemy = new GoblinEnemy();
               break;
          case EnemyType::GoblinPriest:
               enemy = new GoblinPriestEnemy();
               break;
          default:
               enemy = new SlimEnemy();
               break;
          }

          // 设置敌人的技能释放回调
          // 当敌人释放技能时，会治疗范围内的其他敌人
          enemy->set_on_skill_released(
              [&](Enemy *enemy_src)
              {
                   double recover_raduis = enemy_src->get_recover_radius();
                   if (recover_raduis < 0)
                        return;

                   const Vector2 pos_src = enemy_src->get_position();
                   for (Enemy *enemy_dst : enemy_list)
                   {
                        const Vector2 &pos_dst = enemy_dst->get_position();
                        double distance = (pos_dst - pos_src).length();
                        if (distance <= recover_raduis)
                             enemy_dst->increase_hp(enemy_src->get_recover_intensity());
                   }
              });

          // 设置敌人的初始位置和路径
          const Route::IdxList &idx_list = itor->second.get_idx_list();
          position.x = rect_tile_map.x + idx_list[0].x * SIZE_TILE + SIZE_TILE / 2;
          position.y = rect_tile_map.y + idx_list[0].y * SIZE_TILE + SIZE_TILE / 2;

          enemy->set_position(position);
          enemy->set_route(&itor->second);

          // 将新生成的敌人添加到列表中
          enemy_list.push_back(enemy);
     }

     // 检查是否所有敌人都已被清除
     // @return: 如果敌人列表为空返回true，否则返回false
     bool check_cleared()
     {
          return enemy_list.empty();
     }

     // 获取敌人列表
     // @return: 当前所有活跃敌人的列表
     EnemyManager::EnemyList &get_enemy_list()
     {
          return enemy_list;
     }

protected:
     // 构造函数：protected确保只能通过单例模式访问
     EnemyManager() = default;

     // 析构函数：清理所有敌人对象
     ~EnemyManager()
     {
          for (Enemy *enemy : enemy_list)
               delete enemy;
     }

private:
     EnemyList enemy_list; // 存储所有活跃的敌人对象

private:
     // 处理敌人与基地的碰撞
     // 当敌人到达基地时，对基地造成伤害并移除敌人
     void process_home_collision()
     {
          static const SDL_Point &idx_home = ConfigManager::instance()->map.get_idx_home();
          static const SDL_Rect &rect_tile_map = ConfigManager::instance()->rect_tile_map;
          static const Vector2 position_home_tile =
              {
                  (double)rect_tile_map.x + idx_home.x * SIZE_TILE,
                  (double)rect_tile_map.y + idx_home.y * SIZE_TILE};

          for (Enemy *enemy : enemy_list)
          {
               if (enemy->can_remove())
                    continue;

               const Vector2 &position = enemy->get_position();

               // 检查敌人是否与基地发生碰撞
               if (position.x >= position_home_tile.x && position.y >= position_home_tile.y && position.x <= position_home_tile.x + SIZE_TILE && position.y <= position_home_tile.y + SIZE_TILE)
               {
                    enemy->make_invalid();
                    // 对基地造成伤害
                    HomeManager::instance()->decrease_hp(enemy->get_damage());
               }
          }
     }

     // 处理敌人与子弹的碰撞
     // 当子弹击中敌人时，对敌人造成伤害并可能触发范围伤害
     void process_bullet_collision()
     {
          static BulletManager::BulletList &bullet_list = BulletManager::instance()->get_bullet_list();

          for (Enemy *enemy : enemy_list)
          {
               if (enemy->can_remove())
                    continue;

               const Vector2 &size_enemy = enemy->get_size();
               const Vector2 &pos_enemy = enemy->get_position();

               for (Bullet *bullet : bullet_list)
               {
                    if (!bullet->can_collide())
                         continue;

                    const Vector2 &pos_bullet = bullet->get_position();

                    // 检查子弹是否击中敌人
                    if (pos_bullet.x >= pos_enemy.x - size_enemy.x / 2 && pos_bullet.y >= pos_enemy.y - size_enemy.y / 2 && pos_bullet.x <= pos_enemy.x + size_enemy.x / 2 && pos_bullet.y <= pos_enemy.y + size_enemy.y / 2)
                    {
                         double damage = bullet->get_damage();
                         double damage_range = bullet->get_damage_range();
                         if (damage_range < 0)
                         {
                              // 单体伤害
                              enemy->decrease_hp(damage);
                              if (enemy->can_remove())
                                   try_spawn_coin_prop(pos_enemy, enemy->get_reward_ratio());
                         }
                         else
                         {
                              // 范围伤害
                              for (Enemy *target_enemy : enemy_list)
                              {
                                   const Vector2 &pos_target_enemy = target_enemy->get_position();
                                   if ((pos_target_enemy - pos_bullet).length() <= damage_range)
                                   {
                                        target_enemy->decrease_hp(damage);
                                        if (target_enemy->can_remove())
                                             try_spawn_coin_prop(pos_target_enemy, enemy->get_reward_ratio());
                                   }
                              }
                         }

                         bullet->on_collide(enemy);
                    }
               }
          }
     }

     // 移除所有无效的敌人（已死亡或到达终点）
     void remove_invalid_enemy()
     {
          enemy_list.erase(std::remove_if(enemy_list.begin(), enemy_list.end(),
                                          [](const Enemy *enemy)
                                          {
                                               bool deletable = enemy->can_remove();
                                               if (deletable)
                                                    delete enemy;
                                               return deletable;
                                          }),
                           enemy_list.end());
     }

     // 尝试在敌人死亡位置生成金币道具
     // @param position: 生成位置
     // @param ratio: 生成概率（0-1之间）
     void try_spawn_coin_prop(const Vector2 &position, double ratio)
     {
          static CoinManager *instance = CoinManager::instance();

          if ((double)(rand() % 100) / 100 <= ratio)
               instance->spawn_coin_prop(position);
     }
};

#endif // !_ENEMY_MANAGER_H_