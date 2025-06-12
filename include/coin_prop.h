#ifndef _COIN_PROP_H_
#define _COIN_PROP_H_

// 包含必要的头文件
#include "tile.h"              // 瓦片类，用于获取瓦片大小常量
#include "timer.h"             // 计时器类，用于控制金币动画和消失时间
#include "vector2.h"           // 向量类，用于处理位置和速度
#include "resources_manager.h" // 资源管理器，用于获取金币纹理

#include <SDL.h>

// 金币道具类：实现金币的物理效果和动画
// 功能包括：
// 1. 金币的跳跃动画
// 2. 金币的漂浮动画
// 3. 金币的自动消失
class CoinProp
{
public:
     // 构造函数：初始化金币的物理属性和计时器
     CoinProp()
     {
          // 设置跳跃计时器为一次性计时器
          timer_jump.set_one_shot(true);
          timer_jump.set_wait_time(interval_jump);
          timer_jump.set_on_timeout(
              [&]()
              {
                   is_jumping = false; // 跳跃结束后切换到漂浮状态
              });

          // 设置消失计时器为一次性计时器
          timer_disappear.set_one_shot(true);
          timer_disappear.set_wait_time(interval_disappear);
          timer_disappear.set_on_timeout(
              [&]()
              {
                   is_valid = false; // 标记金币无效，可以被移除
              });

          // 设置初始速度：随机水平方向，向上跳跃
          velocity.x = (rand() % 2 ? 1 : -1) * 2 * SIZE_TILE;
          velocity.y = -3 * SIZE_TILE;
     }

     ~CoinProp() = default;

     // 设置金币位置
     void set_position(const Vector2 &position)
     {
          this->position = position;
     }

     // 获取金币位置
     const Vector2 &get_position() const
     {
          return position;
     }

     // 获取金币大小
     const Vector2 &get_size() const
     {
          return size;
     }

     // 使金币无效（可以被移除）
     void make_invalid()
     {
          is_valid = false;
     }

     // 检查金币是否可以被移除
     bool can_remove()
     {
          return !is_valid;
     }

     // 更新金币状态
     // @param delta: 时间增量，单位：秒
     void on_update(double delta)
     {
          // 更新计时器
          timer_jump.on_update(delta);
          timer_disappear.on_update(delta);

          if (is_jumping)
          {
               // 跳跃状态：应用重力
               velocity.y += gravity * delta;
          }
          else
          {
               // 漂浮状态：水平静止，垂直正弦运动
               velocity.x = 0;
               velocity.y = sin(SDL_GetTicks64() / 1000.0 * 4) * 30;
          }

          // 更新位置
          position += velocity * delta;
     }

     // 渲染金币
     // @param renderer: SDL渲染器
     void on_render(SDL_Renderer *renderer)
     {
          // 创建目标矩形
          static SDL_Rect rect = {0, 0, (int)size.x, (int)size.y};
          // 获取金币纹理
          static SDL_Texture *tex_coin = ResourcesManager::instance()
                                             ->get_texture_pool()
                                             .find(ResID::Tex_Coin)
                                             ->second;

          // 设置渲染位置（居中）
          rect.x = (int)(position.x - size.x / 2);
          rect.y = (int)(position.y - size.y / 2);

          // 渲染金币
          SDL_RenderCopy(renderer, tex_coin, nullptr, &rect);
     }

private:
     Vector2 position; // 金币位置
     Vector2 velocity; // 金币速度

     Timer timer_jump;      // 跳跃计时器
     Timer timer_disappear; // 消失计时器

     bool is_valid = true;   // 金币是否有效
     bool is_jumping = true; // 金币是否在跳跃状态

     double gravity = 490;           // 重力加速度
     double interval_jump = 0.75;    // 跳跃持续时间
     Vector2 size = {16, 16};        // 金币大小
     double interval_disappear = 10; // 消失时间
};

#endif // !_COIN_PROP_H_
