#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

/**
 * @brief 用于调度和管理基于时间的事件的计时器类
 *
 * 该类允许创建计时器，在指定的持续时间后触发回调。
 * 计时器可以配置为触发一次或重复触发，
 * 并且可以根据需要暂停、恢复或重新启动。
 */
class Timer
{
public:
     Timer() = default;
     ~Timer() = default;

     /**
      * @brief 从零重新启动计时器
      *
      * 将经过的时间重置为零并清除触发状态
      */
     void restart()
     {
          pass_time = 0;
          shotted = false;
     }

     /**
      * @brief 设置超时前的等待时间
      * @param val 等待时间（秒）
      */
     void set_wait_time(double val)
     {
          wait_time = val;
     }

     /**
      * @brief 设置计时器是否只触发一次
      * @param flag 如果为true，计时器只会触发一次
      */
     void set_one_shot(bool flag)
     {
          one_shot = flag;
     }

     /**
      * @brief 设置超时时要调用的回调函数
      * @param on_timeout 计时器过期时要调用的函数
      */
     void set_on_timeout(std::function<void()> on_timeout)
     {
          this->on_timeout = on_timeout;
     }

     /**
      * @brief 暂停计时器
      *
      * 暂停时，计时器不会累积时间或触发超时
      */
     void pause()
     {
          paused = true;
     }

     /**
      * @brief 恢复已暂停的计时器
      *
      * 允许计时器继续累积时间并触发超时
      */
     void resume()
     {
          paused = false;
     }

     /**
      * @brief 用上次更新后经过的时间更新计时器
      * @param delta 自上次更新以来经过的时间（秒）
      *
      * 应定期调用此方法（通常每帧一次）以更新计时器。
      * 如果经过了足够的时间，它将触发超时回调。
      */
     void on_update(double delta)
     {
          if (paused)
               return;

          pass_time += delta;
          if (pass_time >= wait_time)
          {
               bool can_shot = (!one_shot || (one_shot && !shotted));
               shotted = true;
               if (can_shot && on_timeout)
                    on_timeout();

               pass_time -= wait_time;
          }
     }

private:
     double pass_time = 0;             // 自上次超时以来累积的时间
     double wait_time = 0;             // 触发超时前等待的时间
     bool paused = false;              // 计时器当前是否暂停
     bool shotted = false;             // 计时器是否已触发（用于一次性计时器）
     bool one_shot = false;            // 计时器是否应该只触发一次
     std::function<void()> on_timeout; // 超时事件的回调函数
};

#endif // !_TIMER_H_
