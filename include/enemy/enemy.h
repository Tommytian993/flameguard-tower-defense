#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "timer.h"
#include "route.h"
#include "vector2.h"
#include "animation.h"
#include "config_manager.h"

#include <functional>

// 敌人类：游戏中的敌人实体
// 功能包括：
// 1. 沿指定路径移动
// 2. 生命值管理
// 3. 技能系统
// 4. 动画状态管理
// 5. 减速效果
class Enemy
{
public:
     // 技能回调函数类型定义
     typedef std::function<void(Enemy *enemy)> SkillCallback;

public:
     // 构造函数：初始化计时器和回调函数
     Enemy()
     {
          // 设置技能计时器为循环模式
          timer_skill.set_one_shot(false);
          timer_skill.set_on_timeout([&]()
                                     { on_skill_released(this); });

          // 设置受伤闪烁计时器为单次模式
          timer_sketch.set_one_shot(true);
          timer_sketch.set_wait_time(0.075);
          timer_sketch.set_on_timeout([&]()
                                      { is_show_sketch = false; });

          // 设置减速恢复计时器为单次模式
          timer_restore_speed.set_one_shot(true);
          timer_restore_speed.set_on_timeout([&]()
                                             { speed = max_speed; });
     }

     ~Enemy() = default;

     // 更新敌人状态
     // @param delta: 时间增量，单位：秒
     void on_update(double delta)
     {
          // 更新所有计时器
          timer_skill.on_update(delta);
          timer_sketch.on_update(delta);
          timer_restore_speed.on_update(delta);

          // 计算移动距离和目标距离
          Vector2 move_distance = velocity * delta;
          Vector2 target_distance = position_target - position;
          // 移动敌人，确保不会超过目标位置
          position += move_distance < target_distance ? move_distance : target_distance;

          // 如果到达目标点，更新下一个目标点
          if (target_distance.approx_zero())
          {
               idx_target++;
               refresh_position_target();

               direction = (position_target - position).normalize();
          }

          // 更新速度向量
          velocity.x = direction.x * speed * SIZE_TILE;
          velocity.y = direction.y * speed * SIZE_TILE;

          // 根据移动方向选择动画
          bool is_show_x_amin = abs(velocity.x) >= abs(velocity.y);

          if (is_show_sketch)
          {
               // 受伤闪烁状态下的动画
               if (is_show_x_amin)
                    anim_current = velocity.x > 0 ? &anim_right_sketch : &anim_left_sketch;
               else
                    anim_current = velocity.y > 0 ? &anim_down_sketch : &anim_up_sketch;
          }
          else
          {
               // 正常状态下的动画
               if (is_show_x_amin)
                    anim_current = velocity.x > 0 ? &anim_right : &anim_left;
               else
                    anim_current = velocity.y > 0 ? &anim_down : &anim_up;
          }

          // 更新当前动画
          anim_current->on_update(delta);
     }

     // 渲染敌人
     // @param renderer: SDL渲染器
     void on_render(SDL_Renderer *renderer)
     {
          // 静态变量定义
          static SDL_Rect rect;
          static SDL_Point point;
          static const int offset_y = 2;
          static const Vector2 size_hp_bar = {40, 8};
          static const SDL_Color color_border = {116, 185, 124, 255};
          static const SDL_Color color_content = {226, 255, 194, 255};

          // 计算渲染位置
          point.x = (int)(position.x - size.x / 2);
          point.y = (int)(position.y - size.y / 2);

          // 渲染当前动画
          anim_current->on_render(renderer, point);

          // 如果生命值不满，渲染血条
          if (hp < max_hp)
          {
               // 渲染血条内容
               rect.x = (int)(position.x - size_hp_bar.x / 2);
               rect.y = (int)(position.y - size.y / 2 - size_hp_bar.y - offset_y);
               rect.w = (int)(size_hp_bar.x * (hp / max_hp));
               rect.h = (int)size_hp_bar.y;
               SDL_SetRenderDrawColor(renderer, color_content.r, color_content.g, color_content.b, color_content.a);
               SDL_RenderFillRect(renderer, &rect);

               // 渲染血条边框
               rect.w = (int)size_hp_bar.x;
               SDL_SetRenderDrawColor(renderer, color_border.r, color_border.g, color_border.b, color_border.a);
               SDL_RenderDrawRect(renderer, &rect);
          }
     }

     // 设置技能释放回调函数
     // @param on_skill_released: 技能释放时的回调函数
     void set_on_skill_released(SkillCallback on_skill_released)
     {
          this->on_skill_released = on_skill_released;
     }

     // 增加生命值
     // @param val: 要增加的生命值
     void increase_hp(double val)
     {
          hp += val;

          // 确保生命值不超过最大值
          if (hp > max_hp)
               hp = max_hp;
     }

     // 减少生命值
     // @param val: 要减少的生命值
     void decrease_hp(double val)
     {
          hp -= val;

          // 检查是否死亡
          if (hp <= 0)
          {
               hp = 0;
               is_valid = false;
          }

          // 触发受伤闪烁效果
          is_show_sketch = true;
          timer_sketch.restart();
     }

     // 减速效果
     void slow_down()
     {
          speed = max_speed - 0.5;
          timer_restore_speed.set_wait_time(1);
          timer_restore_speed.restart();
     }

     // 设置位置
     // @param position: 新的位置
     void set_position(const Vector2 &position)
     {
          this->position = position;
     }

     // 设置移动路径
     // @param route: 新的路径
     void set_route(const Route *route)
     {
          this->route = route;

          refresh_position_target();
     }

     // 使敌人失效
     void make_invalid()
     {
          is_valid = false;
     }

     // 获取当前生命值
     double get_hp() const
     {
          return hp;
     }

     // 获取敌人尺寸
     const Vector2 &get_size() const
     {
          return size;
     }

     // 获取当前位置
     const Vector2 &get_position() const
     {
          return position;
     }

     // 获取当前速度
     const Vector2 &get_velocity() const
     {
          return velocity;
     }

     // 获取伤害值
     double get_damage() const
     {
          return damage;
     }

     // 获取奖励倍率
     double get_reward_ratio() const
     {
          return reward_ratio;
     }

     // 获取恢复范围
     double get_recover_radius() const
     {
          return SIZE_TILE * recover_range;
     }

     // 获取恢复强度
     double get_recover_intensity() const
     {
          return recover_intensity;
     }

     // 检查是否可以移除
     bool can_remove() const
     {
          return !is_valid;
     }

     double get_route_process() const
     {
          if (route->get_idx_list().size() == 1)
               return 1;

          return (double)idx_target / (route->get_idx_list().size() - 1);
     }

protected:
     Vector2 size;

     Timer timer_skill;

     Animation anim_up;
     Animation anim_down;
     Animation anim_left;
     Animation anim_right;
     Animation anim_up_sketch;
     Animation anim_down_sketch;
     Animation anim_left_sketch;
     Animation anim_right_sketch;

     double hp = 0;
     double max_hp = 0;
     double speed = 0;
     double max_speed = 0;
     double damage = 0;
     double reward_ratio = 0;
     double recover_interval = 0;
     double recover_range = 0;
     double recover_intensity = 0;

private:
     Vector2 position;
     Vector2 velocity;
     Vector2 direction;

     bool is_valid = true;

     Timer timer_sketch;
     bool is_show_sketch = false;

     Animation *anim_current = nullptr;

     SkillCallback on_skill_released;

     Timer timer_restore_speed;

     const Route *route = nullptr;
     int idx_target = 0;
     Vector2 position_target;

private:
     void refresh_position_target()
     {
          const Route::IdxList &idx_list = route->get_idx_list();

          if (idx_target < idx_list.size())
          {
               const SDL_Point &point = idx_list[idx_target];
               static const SDL_Rect &rect_tile_map = ConfigManager::instance()->rect_tile_map;

               position_target.x = rect_tile_map.x + point.x * SIZE_TILE + SIZE_TILE / 2;
               position_target.y = rect_tile_map.y + point.y * SIZE_TILE + SIZE_TILE / 2;
          }
     }
};

#endif // !_ENEMY_H_
