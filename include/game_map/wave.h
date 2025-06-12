#ifndef _WAVE_H_
#define _WAVE_H_

#include "enemy_type.h"

#include <vector>

// 波次系统：管理一波敌人的生成
struct Wave
{
     // 单个敌人生成事件
     struct SpawnEvent
     {
          double interval = 0;                    // 与上一个敌人生成的时间间隔（秒）
          int spawn_point = 1;                    // 敌人生成的路径点索引
          EnemyType enemy_type = EnemyType::Slim; // 生成的敌人类型
     };

     double rawards = 0;                       // 完成这波次后的奖励金币数
     double interval = 0;                      // 这波次开始前的等待时间（秒）
     std::vector<SpawnEvent> spawn_event_list; // 这波次中的所有敌人生成事件
};

#endif // !_WAVE_H_
