#ifndef _BULLET_H_
#define _BULLET_H_

// 包含必要的头文件
#include "vector2.h"        // 二维向量类，用于位置和速度
#include "enemy.h"          // 敌人类，用于碰撞检测
#include "animation.h"      // 动画类，用于子弹动画
#include "config_manager.h" // 配置管理器，用于读取地图边界

// 子弹类：游戏中的子弹实体
// 功能包括：
// 1. 物理移动：基于速度的线性运动
// 2. 碰撞检测：与敌人的碰撞处理
// 3. 动画系统：支持旋转和普通动画
// 4. 边界检测：超出地图边界时自动销毁
// 5. 伤害系统：支持范围伤害和单体伤害
class Bullet
{
public:
     Bullet() = default;
     ~Bullet() = default;

     // 设置子弹速度
     // @param velocity: 新的速度向量
     // 功能：
     // 1. 更新速度向量
     // 2. 如果子弹可旋转，计算并设置旋转角度
     void set_velocity(const Vector2 &velocity)
     {
          this->velocity = velocity;

          if (can_rotated)
          {
               // 根据速度方向计算旋转角度（弧度转角度）
               double randian = std::atan2(velocity.y, velocity.x);
               angle_anim_rotated = randian * 180 / 3.14159265;
          }
     }

     // 设置子弹位置
     // @param position: 新的位置
     void set_position(const Vector2 &position)
     {
          this->position = position;
     }

     // 设置子弹伤害
     // @param damage: 新的伤害值
     void set_damage(double damage)
     {
          this->damage = damage;
     }

     // 获取子弹尺寸
     const Vector2 &get_size() const
     {
          return size;
     }

     // 获取子弹位置
     const Vector2 &get_position() const
     {
          return position;
     }

     // 获取子弹伤害
     double get_damage() const
     {
          return damage;
     }

     // 获取子弹伤害范围
     // 返回：伤害范围，-1表示单体伤害
     double get_damage_range() const
     {
          return damage_range;
     }

     // 禁用碰撞检测
     void disable_collide()
     {
          is_collisional = false;
     }

     // 检查是否可以碰撞
     bool can_collide() const
     {
          return is_collisional;
     }

     // 使子弹失效
     // 功能：
     // 1. 标记子弹为无效
     // 2. 禁用碰撞检测
     void make_invalid()
     {
          is_valid = false;
          is_collisional = false;
     }

     // 检查是否可以移除
     // 返回：如果子弹已失效，返回true
     bool can_remove() const
     {
          return !is_valid;
     }

     // 更新子弹状态
     // @param delta: 时间增量，单位：秒
     // 功能：
     // 1. 更新动画
     // 2. 更新位置
     // 3. 检查是否超出地图边界
     virtual void on_update(double delta)
     {
          // 更新动画
          animation.on_update(delta);
          // 更新位置：位置 += 速度 * 时间
          position += velocity * delta;

          // 获取地图边界
          static const SDL_Rect &rect_map = ConfigManager::instance()->rect_tile_map;

          // 检查是否超出地图边界
          if (position.x - size.x / 2 <= rect_map.x || position.x + size.x / 2 >= rect_map.x + rect_map.w || position.y - size.y / 2 <= rect_map.y || position.y + size.y / 2 >= rect_map.y + rect_map.h)
          {
               is_valid = false;
          }
     }

     // 渲染子弹
     // @param renderer: SDL渲染器
     // 功能：
     // 1. 计算渲染位置（居中显示）
     // 2. 渲染当前动画帧，支持旋转
     virtual void on_render(SDL_Renderer *renderer)
     {
          static SDL_Point point;

          // 计算渲染位置（居中显示）
          point.x = (int)(position.x - size.x / 2);
          point.y = (int)(position.y - size.y / 2);

          // 渲染当前动画帧，支持旋转
          animation.on_render(renderer, point, angle_anim_rotated);
     }

     // 处理与敌人的碰撞
     // @param enemy: 碰撞的敌人
     // 功能：
     // 1. 标记子弹为无效
     // 2. 禁用碰撞检测
     virtual void on_collide(Enemy *enemy)
     {
          is_valid = false;
          is_collisional = false;
     }

protected:
     Vector2 size;     // 子弹尺寸
     Vector2 velocity; // 子弹速度
     Vector2 position; // 子弹位置

     Animation animation;      // 子弹动画
     bool can_rotated = false; // 是否可以旋转

     double damage = 0;        // 伤害值
     double damage_range = -1; // 伤害范围，-1表示单体伤害

private:
     bool is_valid = true;          // 子弹是否有效
     bool is_collisional = true;    // 是否可以碰撞
     double angle_anim_rotated = 0; // 动画旋转角度
};

#endif // !_BULLET_H_
