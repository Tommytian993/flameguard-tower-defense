/**
 * @file tower.h
 * @brief 定义游戏中防御塔的基础类，包含塔的基本属性和行为
 */

#ifndef _TOWER_H_
#define _TOWER_H_

#include "facing.h"
#include "vector2.h"
#include "animation.h"
#include "tower_type.h"
#include "enemy_manager.h"
#include "bullet_manager.h"

/**
 * @class Tower
 * @brief 防御塔基类，定义了所有防御塔共有的属性和行为
 */
class Tower
{
public:
     /**
      * @brief 构造函数，初始化塔的动画和计时器
      */
     Tower()
     {
          timer_fire.set_one_shot(true);
          timer_fire.set_on_timeout(
              [&]()
              {
                   can_fire = true;
              });

          // 初始化待机动画
          anim_idle_up.set_loop(true);
          anim_idle_up.set_interval(0.2);
          anim_idle_down.set_loop(true);
          anim_idle_down.set_interval(0.2);
          anim_idle_left.set_loop(true);
          anim_idle_left.set_interval(0.2);
          anim_idle_right.set_loop(true);
          anim_idle_right.set_interval(0.2);

          // 初始化攻击动画
          anim_fire_up.set_loop(false);
          anim_fire_up.set_interval(0.2);
          anim_fire_up.set_on_finished(
              [&]()
              {
                   update_idle_animation();
              });

          anim_fire_down.set_loop(false);
          anim_fire_down.set_interval(0.2);
          anim_fire_down.set_on_finished(
              [&]()
              {
                   update_idle_animation();
              });

          anim_fire_left.set_loop(false);
          anim_fire_left.set_interval(0.2);
          anim_fire_left.set_on_finished(
              [&]()
              {
                   update_idle_animation();
              });

          anim_fire_right.set_loop(false);
          anim_fire_right.set_interval(0.2);
          anim_fire_right.set_on_finished(
              [&]()
              {
                   update_idle_animation();
              });
     }

     ~Tower() = default;

     /**
      * @brief 设置塔的位置
      * @param position 新的位置坐标
      */
     void set_position(const Vector2 &position)
     {
          this->position = position;
     }

     /**
      * @brief 获取塔的尺寸
      * @return 塔的尺寸向量
      */
     const Vector2 &get_size() const
     {
          return size;
     }

     /**
      * @brief 获取塔的位置
      * @return 塔的位置向量
      */
     const Vector2 &get_position() const
     {
          return size;
     }

     /**
      * @brief 更新塔的状态
      * @param delta 时间增量
      */
     void on_update(double delta)
     {
          timer_fire.on_update(delta);
          anim_current->on_update(delta);

          if (can_fire)
               on_fire();
     }

     /**
      * @brief 渲染塔
      * @param renderer SDL渲染器指针
      */
     void on_render(SDL_Renderer *renderer)
     {
          static SDL_Point point;

          point.x = (int)(position.x - size.x / 2);
          point.y = (int)(position.y - size.y / 2);

          anim_current->on_render(renderer, point);
     }

protected:
     Vector2 size; ///< 塔的尺寸

     Animation anim_idle_up;    ///< 向上待机动画
     Animation anim_idle_down;  ///< 向下待机动画
     Animation anim_idle_left;  ///< 向左待机动画
     Animation anim_idle_right; ///< 向右待机动画
     Animation anim_fire_up;    ///< 向上攻击动画
     Animation anim_fire_down;  ///< 向下攻击动画
     Animation anim_fire_left;  ///< 向左攻击动画
     Animation anim_fire_right; ///< 向右攻击动画

     TowerType tower_type = TowerType::Archer;   ///< 塔的类型
     double fire_speed = 0;                      ///< 子弹速度
     BulletType bullet_type = BulletType::Arrow; ///< 子弹类型

private:
     Timer timer_fire;                           ///< 攻击冷却计时器
     Vector2 position;                           ///< 塔的位置
     bool can_fire = true;                       ///< 是否可以攻击
     Facing facing = Facing::Right;              ///< 塔的朝向
     Animation *anim_current = &anim_idle_right; ///< 当前播放的动画

private:
     /**
      * @brief 更新待机动画
      */
     void update_idle_animation()
     {
          switch (facing)
          {
          case Left:
               anim_current = &anim_idle_left;
               break;
          case Right:
               anim_current = &anim_idle_right;
               break;
          case Up:
               anim_current = &anim_idle_up;
               break;
          case Down:
               anim_current = &anim_idle_down;
               break;
          }
     }

     /**
      * @brief 更新攻击动画
      */
     void update_fire_animation()
     {
          switch (facing)
          {
          case Left:
               anim_current = &anim_fire_left;
               break;
          case Right:
               anim_current = &anim_fire_right;
               break;
          case Up:
               anim_current = &anim_fire_up;
               break;
          case Down:
               anim_current = &anim_fire_down;
               break;
          }
     }

     /**
      * @brief 寻找目标敌人
      * @return 找到的目标敌人，如果没有找到则返回nullptr
      */
     Enemy *find_target_enemy()
     {
          double process = -1;
          double view_range = 0;
          Enemy *enemy_target = nullptr;

          static ConfigManager *instance = ConfigManager::instance();

          // 根据塔的类型获取视野范围
          switch (tower_type)
          {
          case Archer:
               view_range = instance->archer_template.view_range[instance->level_archer];
               break;
          case Axeman:
               view_range = instance->axeman_template.view_range[instance->level_axeman];
               break;
          case Gunner:
               view_range = instance->gunner_template.view_range[instance->level_gunner];
               break;
          }

          EnemyManager::EnemyList &enemy_list = EnemyManager::instance()->get_enemy_list();

          // 在视野范围内寻找进度最远的敌人
          for (Enemy *enemy : enemy_list)
          {
               if ((enemy->get_position() - position).length() <= view_range * SIZE_TILE)
               {
                    double new_process = enemy->get_route_process();
                    if (new_process > process)
                    {
                         enemy_target = enemy;
                         process = new_process;
                    }
               }
          }

          return enemy_target;
     }

     /**
      * @brief 处理攻击行为
      */
     void on_fire()
     {
          Enemy *target_enemy = find_target_enemy();

          if (!target_enemy)
               return;

          can_fire = false;
          static ConfigManager *instance = ConfigManager::instance();
          static const ResourcesManager::SoundPool &sound_pool = ResourcesManager::instance()->get_sound_pool();

          // 根据塔的类型设置攻击间隔和伤害
          double interval = 0, damage = 0;
          switch (tower_type)
          {
          case Archer:
               interval = instance->archer_template.interval[instance->level_archer];
               damage = instance->archer_template.damage[instance->level_archer];
               switch (rand() % 2)
               {
               case 0:
                    Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowFire_1)->second, 0);
                    break;
               case 1:
                    Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowFire_2)->second, 0);
                    break;
               }
               break;
          case Axeman:
               interval = instance->axeman_template.interval[instance->level_axeman];
               damage = instance->axeman_template.damage[instance->level_axeman];
               Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeFire)->second, 0);
               break;
          case Gunner:
               interval = instance->gunner_template.interval[instance->level_gunner];
               damage = instance->gunner_template.damage[instance->level_gunner];
               Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ShellFire)->second, 0);
               break;
          }
          timer_fire.set_wait_time(interval);
          timer_fire.restart();

          // 计算子弹方向和速度
          Vector2 direction = target_enemy->get_position() - position;
          BulletManager::instance()->fire_bullet(bullet_type, position, direction.normalize() * fire_speed * SIZE_TILE, damage);

          // 更新塔的朝向
          bool is_show_x_anim = abs(direction.x) >= abs(direction.y);
          if (is_show_x_anim)
               facing = direction.x > 0 ? Facing::Right : Facing::Left;
          else
               facing = direction.y > 0 ? Facing::Down : Facing::Up;

          update_fire_animation();
     }
};

#endif // !_TOWER_H_
