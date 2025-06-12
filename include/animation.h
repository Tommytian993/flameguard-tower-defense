#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "timer.h"

#include <SDL.h>
#include <vector>
#include <functional>

/**
 * @brief 动画类，用于管理精灵帧动画
 *
 * 该类使用Timer来控制帧动画的播放速度，支持循环播放和单次播放，
 * 可以设置动画完成时的回调函数。
 */
class Animation
{
public:
     /**
      * @brief 动画播放完成时的回调函数类型
      */
     typedef std::function<void()> PlayCallback;

public:
     /**
      * @brief 构造函数
      *
      * 初始化计时器，设置计时器的超时回调用于推进动画帧
      */
     Animation()
     {
          timer.set_one_shot(false);
          timer.set_on_timeout(
              [&]()
              {
                   idx_frame++;
                   if (idx_frame >= rect_src_list.size())
                   {
                        idx_frame = is_loop ? 0 : rect_src_list.size() - 1;
                        if (!is_loop && on_finished)
                             on_finished();
                   }
              });
     }

     ~Animation() = default;

     /**
      * @brief 重置动画状态
      *
      * 重启计时器并将当前帧索引重置为0
      */
     void reset()
     {
          timer.restart();

          idx_frame = 0;
     }

     /**
      * @brief 设置动画帧数据
      * @param texture 包含所有动画帧的纹理
      * @param num_h 水平方向的帧数
      * @param num_v 垂直方向的帧数
      * @param idx_list 要使用的帧索引列表
      *
      * 根据给定的精灵表（spritesheet）和索引列表计算每个帧的源矩形
      */
     void set_frame_data(SDL_Texture *texture, int num_h, int num_v, const std::vector<int> &idx_list)
     {
          // 获取纹理的宽度和高度
          int width_tex, height_tex;
          this->texture = texture;
          SDL_QueryTexture(texture, nullptr, nullptr, &width_tex, &height_tex);

          // 计算每一帧的宽度和高度
          // 例如：如果纹理是 4x2 的精灵图，num_h=4, num_v=2
          width_frame = width_tex / num_h;   // 每帧宽度 = 总宽度 / 水平帧数
          height_frame = height_tex / num_v; // 每帧高度 = 总高度 / 垂直帧数

          // 为每一帧创建源矩形（SDL_Rect）
          rect_src_list.resize(idx_list.size());
          for (size_t i = 0; i < idx_list.size(); i++)
          {
               int idx = idx_list[i]; // 获取当前帧的索引
               SDL_Rect &rect_src = rect_src_list[i];

               // 计算当前帧在精灵图中的位置
               rect_src.x = (idx % num_h) * width_frame;  // x = (索引 % 水平帧数) * 帧宽度
               rect_src.y = (idx / num_h) * height_frame; // y = (索引 / 水平帧数) * 帧高度
               rect_src.w = width_frame;                  // 设置帧宽度
               rect_src.h = height_frame;                 // 设置帧高度
          }
     }

     /**
      * @brief 设置动画是否循环播放
      * @param is_loop 如果为true，动画将循环播放
      */
     void set_loop(bool is_loop)
     {
          this->is_loop = is_loop;
     }

     /**
      * @brief 设置帧间隔时间
      * @param interval 每帧之间的时间间隔（秒）
      */
     void set_interval(double interval)
     {
          timer.set_wait_time(interval);
     }

     /**
      * @brief 设置动画播放完成时的回调函数
      * @param on_finished 动画完成时要调用的函数
      *
      * 只有在非循环模式下才会触发此回调
      */
     void set_on_finished(PlayCallback on_finished)
     {
          this->on_finished = on_finished;
     }

     /**
      * @brief 更新动画状态
      * @param delta 自上次更新以来经过的时间（秒）
      *
      * 此方法应当在游戏循环的更新阶段调用
      */
     void on_update(double delta)
     {
          timer.on_update(delta);
     }

     /**
      * @brief 渲染当前动画帧
      * @param renderer SDL渲染器
      * @param pos_dst 目标位置
      * @param angle 旋转角度（默认为0）
      *
      * 此方法应当在游戏循环的渲染阶段调用
      */
     void on_render(SDL_Renderer *renderer, const SDL_Point &pos_dst, double angle = 0) const
     {
          static SDL_Rect rect_dst;

          rect_dst.x = pos_dst.x, rect_dst.y = pos_dst.y;
          rect_dst.w = width_frame, rect_dst.h = height_frame;

          SDL_RenderCopyEx(renderer, texture, &rect_src_list[idx_frame], &rect_dst, angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
     }

private:
     Timer timer;                           // 控制帧率的计时器
     bool is_loop = true;                   // 是否循环播放动画
     size_t idx_frame = 0;                  // 当前帧索引
     PlayCallback on_finished;              // 动画完成时的回调函数
     SDL_Texture *texture = nullptr;        // 包含所有帧的纹理
     std::vector<SDL_Rect> rect_src_list;   // 每一帧的源矩形列表
     int width_frame = 0, height_frame = 0; // 单个帧的宽度和高度
};

#endif // !_ANIMATION_H_
