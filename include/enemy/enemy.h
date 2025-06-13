#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "timer.h"
#include "game_map/route.h"
#include "game_map/vector2.h"
#include "animation.h"
#include "manager/config_manager.h"

#include <functional>

// 敌人类：游戏中的敌人实体
// 功能包括：
// 1. 沿指定路径移动：敌人会按照预设的路径点进行移动，支持平滑过渡
// 2. 生命值管理：包含最大生命值、当前生命值，以及生命值变化时的视觉效果
// 3. 技能系统：支持定时释放技能，可自定义技能效果
// 4. 动画状态管理：根据移动方向和状态（正常/受伤）切换不同的动画
// 5. 减速效果：支持临时减速，并在指定时间后恢复
// 6. 奖励系统：击杀敌人可获得奖励，支持自定义奖励倍率
// 7. 恢复系统：支持在指定范围内恢复友方单位生命值
class Enemy
{
public:
     // 技能回调函数类型定义
     // 当技能冷却结束时，会调用此函数执行技能效果
     typedef std::function<void(Enemy *enemy)> SkillCallback;

public:
     // 构造函数：初始化计时器和回调函数
     // 设置三个计时器：
     // 1. 技能计时器：循环模式，用于控制技能释放间隔
     // 2. 受伤闪烁计时器：单次模式，控制受伤时的闪烁效果持续时间
     // 3. 减速恢复计时器：单次模式，控制减速效果的持续时间
     Enemy()
     {
          // 设置技能计时器为循环模式
          timer_skill.set_one_shot(false);
          timer_skill.set_on_timeout([&]()
                                     { on_skill_released(this); });

          // 设置受伤闪烁计时器为单次模式，持续0.075秒
          timer_sketch.set_one_shot(true);
          timer_sketch.set_wait_time(0.075);
          timer_sketch.set_on_timeout([&]()
                                      { is_show_sketch = false; });

          // 设置减速恢复计时器为单次模式，持续1秒
          timer_restore_speed.set_one_shot(true);
          timer_restore_speed.set_on_timeout([&]()
                                             { speed = max_speed; });
     }

     ~Enemy() = default;

     // 更新敌人状态
     // @param delta: 时间增量，单位：秒
     // 功能：
     // 1. 更新所有计时器状态
     // 2. 计算并更新敌人位置
     // 3. 处理路径点切换
     // 4. 更新速度向量
     // 5. 根据移动方向和状态选择并更新动画
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

          // 更新速度向量：速度 = 方向 * 速度值 * 瓦片大小
          velocity.x = direction.x * speed * SIZE_TILE;
          velocity.y = direction.y * speed * SIZE_TILE;

          // 根据移动方向选择动画
          // 如果水平速度大于垂直速度，使用水平动画
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
     // 功能：
     // 1. 渲染当前动画帧
     // 2. 如果生命值不满，渲染血条
     // 3. 血条包含边框和内容两部分，内容长度根据当前生命值比例计算
     void on_render(SDL_Renderer *renderer)
     {
          // 静态变量定义
          static SDL_Rect rect;
          static SDL_Point point;
          static const int offset_y = 2;                               // 血条与敌人的垂直偏移
          static const Vector2 size_hp_bar = {40, 8};                  // 血条尺寸
          static const SDL_Color color_border = {116, 185, 124, 255};  // 血条边框颜色
          static const SDL_Color color_content = {226, 255, 194, 255}; // 血条内容颜色

          // 计算渲染位置（居中显示）
          point.x = (int)(position.x - size.x / 2);
          point.y = (int)(position.y - size.y / 2);

          // 渲染当前动画
          anim_current->on_render(renderer, point);

          // 如果生命值不满，渲染血条
          if (hp < max_hp)
          {
               // 渲染血条内容（绿色填充）
               rect.x = (int)(position.x - size_hp_bar.x / 2);
               rect.y = (int)(position.y - size.y / 2 - size_hp_bar.y - offset_y);
               rect.w = (int)(size_hp_bar.x * (hp / max_hp)); // 根据生命值比例计算宽度
               rect.h = (int)size_hp_bar.y;
               SDL_SetRenderDrawColor(renderer, color_content.r, color_content.g, color_content.b, color_content.a);
               SDL_RenderFillRect(renderer, &rect);

               // 渲染血条边框（深绿色）
               rect.w = (int)size_hp_bar.x;
               SDL_SetRenderDrawColor(renderer, color_border.r, color_border.g, color_border.b, color_border.a);
               SDL_RenderDrawRect(renderer, &rect);
          }
     }

     // 设置技能释放回调函数
     // @param on_skill_released: 技能释放时的回调函数
     // 当技能冷却结束时，会调用此函数执行技能效果
     void set_on_skill_released(SkillCallback on_skill_released)
     {
          this->on_skill_released = on_skill_released;
     }

     // 增加生命值
     // @param val: 要增加的生命值
     // 功能：
     // 1. 增加当前生命值
     // 2. 确保生命值不超过最大值
     void increase_hp(double val)
     {
          hp += val;

          // 确保生命值不超过最大值
          if (hp > max_hp)
               hp = max_hp;
     }

     // 减少生命值
     // @param val: 要减少的生命值
     // 功能：
     // 1. 减少当前生命值
     // 2. 检查是否死亡
     // 3. 触发受伤闪烁效果
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
     // 功能：
     // 1. 将速度降低0.5
     // 2. 设置1秒后恢复原速度
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
     // 功能：
     // 1. 更新路径
     // 2. 刷新目标位置
     void set_route(const Route *route)
     {
          this->route = route;

          refresh_position_target();
     }

     // 使敌人失效
     // 功能：将敌人标记为无效，使其可以被移除
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
     // 返回：恢复效果的作用范围（瓦片大小的倍数）
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
     // 返回：如果敌人已失效（死亡或手动标记），返回true
     bool can_remove() const
     {
          return !is_valid;
     }

     // 获取路径进度
     // 返回：当前路径的完成进度（0-1之间的小数）
     double get_route_process() const
     {
          if (route->get_idx_list().size() == 1)
               return 1;

          return (double)idx_target / (route->get_idx_list().size() - 1);
     }

protected:
     Vector2 size; // 敌人尺寸

     Timer timer_skill; // 技能冷却计时器

     // 四个方向的正常动画
     Animation anim_up;
     Animation anim_down;
     Animation anim_left;
     Animation anim_right;
     // 四个方向的受伤闪烁动画
     Animation anim_up_sketch;
     Animation anim_down_sketch;
     Animation anim_left_sketch;
     Animation anim_right_sketch;

     double hp = 0;                // 当前生命值
     double max_hp = 0;            // 最大生命值
     double speed = 0;             // 当前速度
     double max_speed = 0;         // 最大速度
     double damage = 0;            // 伤害值
     double reward_ratio = 0;      // 奖励倍率
     double recover_interval = 0;  // 恢复间隔
     double recover_range = 0;     // 恢复范围
     double recover_intensity = 0; // 恢复强度

private:
     Vector2 position;  // 当前位置
     Vector2 velocity;  // 当前速度向量
     Vector2 direction; // 当前移动方向

     bool is_valid = true; // 敌人是否有效

     Timer timer_sketch;          // 受伤闪烁计时器
     bool is_show_sketch = false; // 是否显示受伤闪烁效果

     Animation *anim_current = nullptr; // 当前播放的动画

     SkillCallback on_skill_released; // 技能释放回调函数

     Timer timer_restore_speed; // 减速恢复计时器

     const Route *route = nullptr; // 当前移动路径
     int idx_target = 0;           // 当前目标路径点索引
     Vector2 position_target;      // 当前目标位置

private:
     // 刷新目标位置
     // 功能：
     // 1. 根据当前路径点索引获取目标位置
     // 2. 将路径点坐标转换为实际像素坐标
     void refresh_position_target()
     {
          const Route::IdxList &idx_list = route->get_idx_list();

          if (idx_target < idx_list.size())
          {
               const SDL_Point &point = idx_list[idx_target];
               static const SDL_Rect &rect_tile_map = ConfigManager::instance()->rect_tile_map;

               // 计算目标位置（瓦片中心点）
               position_target.x = rect_tile_map.x + point.x * SIZE_TILE + SIZE_TILE / 2;
               position_target.y = rect_tile_map.y + point.y * SIZE_TILE + SIZE_TILE / 2;
          }
     }
};

#endif // !_ENEMY_H_
