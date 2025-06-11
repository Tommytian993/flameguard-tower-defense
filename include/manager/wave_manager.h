#ifndef _WAVE_MANAGER_H_
#define _WAVE_MANAGER_H_

// 包含必要的头文件
#include "timer.h"          // 计时器类，用于控制波次和敌人生成的时间
#include "manager.h"        // 基础管理器模板，实现单例模式
#include "config_manager.h" // 配置管理器，用于读取波次配置
#include "enemy_manager.h"  // 敌人生成管理器，用于生成敌人
#include "coin_manager.h"   // 金币管理器，用于增加金币奖励

// WaveManager 类：负责管理游戏波次和敌人生成
// 该类继承自 Manager 模板类，实现了单例模式
class WaveManager : public Manager<WaveManager>
{
     // 声明 Manager 模板类为友元，允许其访问私有成员
     friend class Manager<WaveManager>;

public:
     // 更新函数：处理波次和敌人生成的逻辑
     // @param delta: 时间增量，单位：秒
     void on_update(double delta)
     {
          // 获取配置管理器实例
          static ConfigManager *instance = ConfigManager::instance();

          // 如果游戏结束，直接返回
          if (instance->is_game_over)
               return;

          // 如果波次未开始，更新波次开始计时器
          if (!is_wave_started)
               timer_start_wave.on_update(delta);
          // 否则，更新敌人生成计时器
          else
               timer_spawn_enemy.on_update(delta);

          // 如果最后一个敌人已生成且所有敌人已清除
          if (is_spawned_last_enemy && EnemyManager::instance()->check_cleared())
          {
               // 增加金币奖励
               CoinManager::instance()->increase_coin(instance->wave_list[idx_wave].rawards);

               // 进入下一波次
               idx_wave++;

               // 如果所有波次已完成，设置游戏胜利并结束
               if (idx_wave >= instance->wave_list.size())
               {
                    instance->is_game_win = true;
                    instance->is_game_over = true;
               }
               // 否则，准备下一波次
               else
               {
                    // 重置生成事件索引
                    idx_spawn_event = 0;
                    // 标记波次已开始
                    is_wave_started = true;
                    // 标记最后一个敌人未生成
                    is_spawned_last_enemy = false;

                    // 获取下一波次配置
                    const Wave &wave = instance->wave_list[idx_wave];
                    // 设置波次开始等待时间
                    timer_start_wave.set_wait_time(wave.interval);
                    // 重启波次开始计时器
                    timer_start_wave.restart();
               }
          }
     }

protected:
     // 构造函数：初始化波次和敌人生成的计时器
     WaveManager()
     {
          // 获取波次配置列表
          static const std::vector<Wave> &wave_list = ConfigManager::instance()->wave_list;

          // 设置波次开始计时器为一次性计时器
          timer_start_wave.set_one_shot(true);
          // 设置波次开始等待时间
          timer_start_wave.set_wait_time(wave_list[0].interval);
          // 设置波次开始计时器超时回调
          timer_start_wave.set_on_timeout(
              [&]()
              {
                   // 标记波次已开始
                   is_wave_started = true;
                   // 设置敌人生成等待时间
                   timer_spawn_enemy.set_wait_time(wave_list[idx_wave].spawn_event_list[0].interval);
                   // 重启敌人生成计时器
                   timer_spawn_enemy.restart();
              });

          // 设置敌人生成计时器为一次性计时器
          timer_spawn_enemy.set_one_shot(true);
          // 设置敌人生成计时器超时回调
          timer_spawn_enemy.set_on_timeout(
              [&]()
              {
                   // 获取当前波次的生成事件列表
                   const std::vector<Wave::SpawnEvent> &spawn_event_list = wave_list[idx_wave].spawn_event_list;
                   // 获取当前生成事件
                   const Wave::SpawnEvent &spawn_event = spawn_event_list[idx_spawn_event];

                   // 生成敌人
                   EnemyManager::instance()->spawn_enemy(spawn_event.enemy_type, spawn_event.spawn_point);

                   // 进入下一个生成事件
                   idx_spawn_event++;

                   // 如果所有生成事件已完成，标记最后一个敌人已生成
                   if (idx_spawn_event >= spawn_event_list.size())
                   {
                        is_spawned_last_enemy = true;
                        return;
                   }

                   // 设置下一个生成事件的等待时间
                   timer_spawn_enemy.set_wait_time(spawn_event_list[idx_spawn_event].interval);
                   // 重启敌人生成计时器
                   timer_spawn_enemy.restart();
              });
     }

     // 析构函数：使用默认实现
     ~WaveManager() = default;

private:
     int idx_wave = 0;                   // 当前波次索引
     int idx_spawn_event = 0;            // 当前生成事件索引
     Timer timer_start_wave;             // 波次开始计时器
     Timer timer_spawn_enemy;            // 敌人生成计时器
     bool is_wave_started = false;       // 波次是否已开始
     bool is_spawned_last_enemy = false; // 最后一个敌人是否已生成
};

#endif // !_WAVE_MANAGER_H_
