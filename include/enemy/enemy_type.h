#ifndef _ENEMY_TYPE_H_
#define _ENEMY_TYPE_H_

enum class EnemyType
{
     Slim, // 基础敌人：史莱姆
           // 特点：生命值中等，移动速度较慢，无特殊技能
           // 用途：作为游戏中最基础的敌人单位，用于前期波次
           // 属性：hp=100, speed=1, damage=1, reward_ratio=0.5

     KingSlim, // 精英敌人：史莱姆王
               // 特点：生命值高，移动速度中等，具有范围治疗技能
               // 用途：作为精英单位出现，可以治疗周围的其他敌人
               // 属性：hp=200, speed=1.2, damage=2, recover_interval=10, recover_range=3, recover_intensity=25

     Skeleton, // 远程敌人：骷髅
               // 特点：生命值较低，移动速度快，具有远程攻击能力
               // 用途：作为快速单位出现，可以绕过防御塔的攻击范围
               // 属性：hp=80, speed=1.5, damage=1.5, reward_ratio=0.7

     Goblin, // 突击敌人：哥布林
             // 特点：生命值低，移动速度极快，具有冲锋技能
             // 用途：作为突击单位出现，可以快速突破防线
             // 属性：hp=60, speed=2.0, damage=1, reward_ratio=0.8

     GoblinPriest // 支援敌人：哥布林祭司
                  // 特点：生命值中等，移动速度慢，具有强力治疗和增益技能
                  // 用途：作为支援单位出现，可以大幅提升其他敌人的生存能力
                  // 属性：hp=120, speed=0.8, damage=0.5, recover_interval=5, recover_range=5, recover_intensity=40
};

#endif // !_ENEMY_TYPE_H_
