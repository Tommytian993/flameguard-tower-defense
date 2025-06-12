#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include "../game_map/map.h"
#include "../game_map/wave.h"
#include "manager.h"

#include <SDL.h>
#include <string>
#include <cJSON.h>
#include <fstream>
#include <sstream>
#include <iostream>

// 配置管理器：负责加载和管理游戏配置数据
// 该类继承自Manager模板类，实现了单例模式
// 主要功能：
// 1. 加载和管理游戏基础配置（窗口大小、标题等）
// 2. 管理玩家属性配置（移动速度、攻击属性等）
// 3. 管理防御塔配置（攻击间隔、伤害、视野范围等）
// 4. 管理敌人配置（生命值、速度、特殊技能等）
// 5. 加载和管理关卡波次配置
class ConfigManager : public Manager<ConfigManager>
{
     friend class Manager<ConfigManager>;

public:
     // 基础配置模板：包含窗口和游戏基本设置
     // 这些设置影响游戏的基本显示和运行环境
     struct BasicTemplate
     {
          std::string window_title = u8"Tower Defense Game!"; // 游戏窗口标题，使用UTF-8编码支持中文
          int window_width = 1280;                            // 游戏窗口宽度（像素）
          int window_height = 720;                            // 游戏窗口高度（像素）
     };

     // 玩家配置模板：包含玩家属性和技能设置
     // 这些属性决定了玩家角色的基本能力和战斗特性
     struct PlayerTemplate
     {
          double speed = 3;                    // 玩家移动速度（单位：格/秒）
          double normal_attack_interval = 0.5; // 普通攻击间隔（单位：秒）
          double normal_attack_damage = 0;     // 普通攻击伤害值
          double skill_interval = 10;          // 技能冷却时间（单位：秒）
          double skill_damage = 1;             // 技能伤害值
     };

     // 防御塔配置模板：包含防御塔属性和升级设置
     // 每个属性都是一个数组，索引代表防御塔等级（0-9）
     // 例如：damage[0]表示1级防御塔的伤害，damage[1]表示2级防御塔的伤害
     struct TowerTemplate
     {
          double interval[10] = {1};     // 攻击间隔（单位：秒），不同等级可能有不同的攻击速度
          double damage[10] = {25};      // 攻击伤害值，随等级提升而增加
          double view_range[10] = {5};   // 视野范围（单位：格），决定防御塔的攻击范围
          double cost[10] = {50};        // 建造费用（单位：金币），不同等级可能有不同的建造成本
          double upgrade_cost[9] = {75}; // 升级费用（单位：金币），索引表示从当前等级升级到下一级的费用
     };

     // 敌人配置模板：包含敌人属性和特殊能力设置
     // 这些属性决定了敌人的基本属性和特殊技能效果
     struct EnemyTemplate
     {
          double hp = 100;               // 生命值，决定敌人的生存能力
          double speed = 1;              // 移动速度（单位：格/秒）
          double damage = 1;             // 攻击伤害值，当敌人到达终点时对玩家造成的伤害
          double reward_ratio = 0.5;     // 击杀奖励倍率，影响击杀该敌人获得的金币数量
          double recover_interval = 10;  // 恢复技能间隔（单位：秒），特殊敌人的治疗技能冷却时间
          double recover_range = 0;      // 恢复技能范围（单位：格），决定治疗技能的影响范围
          double recover_intensity = 25; // 恢复技能强度，决定每次治疗恢复的生命值
     };

public:
     Map map;                     // 当前地图数据，包含地形、路径等信息
     std::vector<Wave> wave_list; // 波次列表，存储关卡中的所有敌人波次信息

     // 防御塔等级记录
     int level_archer = 0; // 弓箭手防御塔当前等级
     int level_axeman = 0; // 斧手防御塔当前等级
     int level_gunner = 0; // 枪手防御塔当前等级

     // 游戏状态标志
     bool is_game_win = true;      // 游戏胜利标志，true表示玩家获胜
     bool is_game_over = false;    // 游戏结束标志，true表示游戏结束
     SDL_Rect rect_tile_map = {0}; // 地图显示区域，用于确定地图在屏幕上的位置和大小

     // 配置模板实例
     BasicTemplate basic_template;   // 基础配置实例，存储游戏基本设置
     PlayerTemplate player_template; // 玩家配置实例，存储玩家属性设置

     // 防御塔配置实例
     TowerTemplate archer_template; // 弓箭手防御塔配置
     TowerTemplate axeman_template; // 斧手防御塔配置
     TowerTemplate gunner_template; // 枪手防御塔配置

     // 敌人配置实例
     EnemyTemplate slim_template;          // 史莱姆配置，基础敌人
     EnemyTemplate king_slim_template;     // 史莱姆王配置，精英敌人
     EnemyTemplate skeleton_template;      // 骷髅配置，远程敌人
     EnemyTemplate goblin_template;        // 哥布林配置，快速敌人
     EnemyTemplate goblin_priest_template; // 哥布林祭司配置，治疗型敌人

     // 游戏初始值设置
     const double num_initial_hp = 10;    // 玩家初始生命值
     const double num_initial_coin = 100; // 玩家初始金币数量
     const double num_coin_per_prop = 10; // 每个道具提供的金币数量

public:
     // 加载关卡配置
     // 从JSON文件中读取关卡配置，包括波次信息、敌人生成规则等
     // 配置文件格式要求：
     // 1. 根节点必须是一个JSON数组，每个元素代表一个波次
     // 2. 每个波次必须包含以下字段：
     //    - rewards: 完成该波次的奖励金币数
     //    - interval: 该波次开始前的等待时间（秒）
     //    - spawn_list: 敌人生成事件数组
     // 3. 每个生成事件必须包含以下字段：
     //    - interval: 生成间隔时间（秒）
     //    - point: 生成点编号
     //    - enemy: 敌人类型（"Slim"/"KingSlim"/"Skeleton"/"Goblin"/"GoblinPriest"）
     // @param path: 关卡配置文件路径
     // @return: 加载成功返回true，否则返回false
     bool load_level_config(const std::string &path)
     {
          // 打开配置文件
          std::ifstream file(path);
          if (!file.good())
               return false;

          // 读取文件内容到字符串流
          std::stringstream str_stream;
          str_stream << file.rdbuf();
          file.close();

          // 解析JSON字符串为JSON对象
          cJSON *json_root = cJSON_Parse(str_stream.str().c_str());
          if (!json_root)
               return false;

          // 验证根节点是否为数组类型
          if (json_root->type != cJSON_Array)
          {
               cJSON_Delete(json_root);
               return false;
          }

          // 遍历每个波次配置
          cJSON *json_wave = nullptr;
          cJSON_ArrayForEach(json_wave, json_root)
          {
               // 验证波次节点是否为对象类型
               if (json_wave->type != cJSON_Object)
                    continue;

               // 创建新的波次对象
               wave_list.emplace_back();
               Wave &wave = wave_list.back();

               // 解析波次奖励金币数
               cJSON *json_wave_rewards = cJSON_GetObjectItem(json_wave, "rewards");
               if (json_wave_rewards && json_wave_rewards->type == cJSON_Number)
                    wave.rawards = json_wave_rewards->valuedouble;

               // 解析波次开始前的等待时间
               cJSON *json_wave_interval = cJSON_GetObjectItem(json_wave, "interval");
               if (json_wave_interval && json_wave_interval->type == cJSON_Number)
                    wave.interval = json_wave_interval->valuedouble;

               // 解析敌人生成事件列表
               cJSON *json_wave_spawn_list = cJSON_GetObjectItem(json_wave, "spawn_list");
               if (json_wave_spawn_list && json_wave_spawn_list->type == cJSON_Array)
               {
                    // 遍历每个生成事件
                    cJSON *json_spawn_event = nullptr;
                    cJSON_ArrayForEach(json_spawn_event, json_wave_spawn_list)
                    {
                         // 验证生成事件节点是否为对象类型
                         if (json_spawn_event->type != cJSON_Object)
                              continue;

                         // 创建新的生成事件对象
                         wave.spawn_event_list.emplace_back();
                         Wave::SpawnEvent &spawn_event = wave.spawn_event_list.back();

                         // 解析生成事件间隔时间
                         cJSON *json_spawn_event_interval = cJSON_GetObjectItem(json_spawn_event, "interval");
                         if (json_spawn_event_interval && json_spawn_event_interval->type == cJSON_Number)
                              spawn_event.interval = json_spawn_event_interval->valuedouble;

                         // 解析生成点编号
                         cJSON *json_spawn_event_spawn_point = cJSON_GetObjectItem(json_spawn_event, "point");
                         if (json_spawn_event_spawn_point && json_spawn_event_spawn_point->type == cJSON_Number)
                              spawn_event.spawn_point = json_spawn_event_spawn_point->valueint;

                         // 解析敌人类型
                         cJSON *json_spawn_event_enemy_type = cJSON_GetObjectItem(json_spawn_event, "enemy");
                         if (json_spawn_event_enemy_type && json_spawn_event_enemy_type->type == cJSON_String)
                         {
                              // 根据敌人类型字符串设置对应的枚举值
                              const std::string str_enemy_type = json_spawn_event_enemy_type->valuestring;
                              if (str_enemy_type == "Slim")
                                   spawn_event.enemy_type = EnemyType::Slim;
                              else if (str_enemy_type == "KingSlim")
                                   spawn_event.enemy_type = EnemyType::KingSlim;
                              else if (str_enemy_type == "Skeleton")
                                   spawn_event.enemy_type = EnemyType::Skeleton;
                              else if (str_enemy_type == "Goblin")
                                   spawn_event.enemy_type = EnemyType::Goblin;
                              else if (str_enemy_type == "GoblinPriest")
                                   spawn_event.enemy_type = EnemyType::GoblinPriest;
                         }
                    }

                    // 如果波次没有生成事件，则移除该波次
                    if (wave.spawn_event_list.empty())
                         wave_list.pop_back();
               }
          }

          // 释放JSON对象
          cJSON_Delete(json_root);

          // 验证是否成功加载了至少一个波次
          if (wave_list.empty())
               return false;

          return true;
     }

     // 加载游戏配置
     // 从JSON文件中读取游戏基本配置，包括窗口设置、玩家属性、防御塔属性、敌人属性等
     // 配置文件格式要求：
     // 1. 根节点必须是一个JSON对象
     // 2. 必须包含以下字段：
     //    - basic: 基础配置对象
     //    - player: 玩家配置对象
     //    - tower: 防御塔配置对象
     //    - enemy: 敌人配置对象
     // 3. 每个配置对象包含相应的属性设置
     // @param path: 游戏配置文件路径
     // @return: 加载成功返回true，否则返回false
     bool load_game_config(const std::string &path)
     {
          // 打开配置文件
          std::ifstream file(path);
          if (!file.good())
               return false;

          // 读取文件内容到字符串流
          std::stringstream str_stream;
          str_stream << file.rdbuf();
          file.close();

          // 解析JSON字符串为JSON对象
          cJSON *json_root = cJSON_Parse(str_stream.str().c_str());
          if (!json_root || json_root->type != cJSON_Object)
               return false;

          // 获取各个配置节点
          cJSON *json_basic = cJSON_GetObjectItem(json_root, "basic");
          cJSON *json_player = cJSON_GetObjectItem(json_root, "player");
          cJSON *json_tower = cJSON_GetObjectItem(json_root, "tower");
          cJSON *json_enemy = cJSON_GetObjectItem(json_root, "enemy");

          // 验证所有必需的配置节点是否存在且类型正确
          if (!json_basic || !json_player || !json_tower || !json_enemy ||
              json_basic->type != cJSON_Object || json_player->type != cJSON_Object ||
              json_tower->type != cJSON_Object || json_enemy->type != cJSON_Object)
          {
               cJSON_Delete(json_root);
               return false;
          }

          // 解析各个配置模板
          // 基础配置：包含窗口标题、大小等设置
          parse_basic_template(basic_template, json_basic);

          // 玩家配置：包含移动速度、攻击属性等设置
          parse_player_template(player_template, json_player);

          // 防御塔配置：包含三种类型防御塔的属性设置
          parse_tower_template(archer_template, cJSON_GetObjectItem(json_tower, "archer"));
          parse_tower_template(axeman_template, cJSON_GetObjectItem(json_tower, "axeman"));
          parse_tower_template(gunner_template, cJSON_GetObjectItem(json_tower, "gunner"));

          // 敌人配置：包含五种类型敌人的属性设置
          parse_enemy_template(slim_template, cJSON_GetObjectItem(json_enemy, "slim"));
          parse_enemy_template(king_slim_template, cJSON_GetObjectItem(json_enemy, "king_slim"));
          parse_enemy_template(skeleton_template, cJSON_GetObjectItem(json_enemy, "skeleton"));
          parse_enemy_template(goblin_template, cJSON_GetObjectItem(json_enemy, "goblin"));
          parse_enemy_template(goblin_priest_template, cJSON_GetObjectItem(json_enemy, "goblin_priest"));

          // 释放JSON对象
          cJSON_Delete(json_root);
          return true;
     }

protected:
     // 构造函数和析构函数设为protected，确保只能通过单例模式访问
     ConfigManager() = default;
     ~ConfigManager() = default;

private:
     // 解析基础配置模板
     // 从JSON对象中读取并设置窗口标题、宽度、高度等基本配置
     // @param tpl: 要填充的基础配置模板
     // @param json_root: 包含基础配置的JSON对象
     void parse_basic_template(BasicTemplate &tpl, cJSON *json_root)
     {
          if (!json_root || json_root->type != cJSON_Object)
               return;

          cJSON *json_window_title = cJSON_GetObjectItem(json_root, "window_title");
          cJSON *json_window_width = cJSON_GetObjectItem(json_root, "window_width");
          cJSON *json_window_height = cJSON_GetObjectItem(json_root, "window_height");

          if (json_window_title && json_window_title->type == cJSON_String)
               tpl.window_title = json_window_title->valuestring;
          if (json_window_width && json_window_width->type == cJSON_Number)
               tpl.window_width = json_window_width->valueint;
          if (json_window_height && json_window_height->type == cJSON_Number)
               tpl.window_height = json_window_height->valueint;
     }

     // 解析玩家配置模板
     // 从JSON对象中读取并设置玩家移动速度、攻击属性等配置
     // @param tpl: 要填充的玩家配置模板
     // @param json_root: 包含玩家配置的JSON对象
     void parse_player_template(PlayerTemplate &tpl, cJSON *json_root)
     {
          if (!json_root || json_root->type != cJSON_Object)
               return;

          cJSON *json_speed = cJSON_GetObjectItem(json_root, "speed");
          cJSON *json_normal_attack_interval = cJSON_GetObjectItem(json_root, "normal_attack_interval");
          cJSON *json_normal_attack_damage = cJSON_GetObjectItem(json_root, "normal_attack_damage");
          cJSON *json_skill_interval = cJSON_GetObjectItem(json_root, "skill_interval");
          cJSON *json_skill_damage = cJSON_GetObjectItem(json_root, "skill_damage");

          if (json_speed && json_speed->type == cJSON_Number)
               tpl.speed = json_speed->valuedouble;
          if (json_normal_attack_interval && json_normal_attack_interval->type == cJSON_Number)
               tpl.normal_attack_interval = json_normal_attack_interval->valuedouble;
          if (json_normal_attack_damage && json_normal_attack_damage->type == cJSON_Number)
               tpl.normal_attack_damage = json_normal_attack_damage->valuedouble;
          if (json_skill_interval && json_skill_interval->type == cJSON_Number)
               tpl.skill_interval = json_skill_interval->valuedouble;
          if (json_skill_damage && json_skill_damage->type == cJSON_Number)
               tpl.skill_damage = json_skill_damage->valuedouble;
     }

     // 解析数值数组
     // 从JSON数组中读取数值并填充到目标数组
     // @param ary: 目标数组
     // @param max_len: 数组最大长度
     // @param json_root: 包含数值的JSON数组
     void parse_number_array(double *ary, int max_len, cJSON *json_root)
     {
          if (!json_root || json_root->type != cJSON_Array)
               return;

          int idx = -1;
          cJSON *json_element = nullptr;
          cJSON_ArrayForEach(json_element, json_root)
          {
               idx++;
               if (json_element->type != cJSON_Number || idx >= max_len)
                    continue;

               ary[idx] = json_element->valuedouble;
          }
     }

     // 解析防御塔配置模板
     // 从JSON对象中读取并设置防御塔的攻击间隔、伤害、视野范围等属性
     // @param tpl: 要填充的防御塔配置模板
     // @param json_root: 包含防御塔配置的JSON对象
     void parse_tower_template(TowerTemplate &tpl, cJSON *json_root)
     {
          if (!json_root || json_root->type != cJSON_Object)
               return;

          cJSON *json_interval = cJSON_GetObjectItem(json_root, "interval");
          cJSON *json_damage = cJSON_GetObjectItem(json_root, "damage");
          cJSON *json_view_range = cJSON_GetObjectItem(json_root, "view_range");
          cJSON *json_cost = cJSON_GetObjectItem(json_root, "cost");
          cJSON *json_upgrade_cost = cJSON_GetObjectItem(json_root, "upgrade_cost");

          parse_number_array(tpl.interval, 10, json_interval);
          parse_number_array(tpl.damage, 10, json_damage);
          parse_number_array(tpl.view_range, 10, json_view_range);
          parse_number_array(tpl.cost, 10, json_cost);
          parse_number_array(tpl.upgrade_cost, 9, json_upgrade_cost);
     }

     // 解析敌人配置模板
     // 从JSON对象中读取并设置敌人的生命值、速度、特殊技能等属性
     // @param tpl: 要填充的敌人配置模板
     // @param json_root: 包含敌人配置的JSON对象
     void parse_enemy_template(EnemyTemplate &tpl, cJSON *json_root)
     {
          if (!json_root || json_root->type != cJSON_Object)
               return;

          cJSON *json_hp = cJSON_GetObjectItem(json_root, "hp");
          cJSON *json_speed = cJSON_GetObjectItem(json_root, "speed");
          cJSON *json_damage = cJSON_GetObjectItem(json_root, "damage");
          cJSON *json_reward_ratio = cJSON_GetObjectItem(json_root, "reward_ratio");
          cJSON *json_recover_interval = cJSON_GetObjectItem(json_root, "recover_interval");
          cJSON *json_recover_range = cJSON_GetObjectItem(json_root, "recover_range");
          cJSON *json_recover_intensity = cJSON_GetObjectItem(json_root, "recover_intensity");

          if (json_hp && json_hp->type == cJSON_Number)
               tpl.hp = json_hp->valuedouble;
          if (json_speed && json_speed->type == cJSON_Number)
               tpl.speed = json_speed->valuedouble;
          if (json_damage && json_damage->type == cJSON_Number)
               tpl.damage = json_damage->valuedouble;
          if (json_reward_ratio && json_reward_ratio->type == cJSON_Number)
               tpl.reward_ratio = json_reward_ratio->valuedouble;
          if (json_recover_interval && json_recover_interval->type == cJSON_Number)
               tpl.recover_interval = json_recover_interval->valuedouble;
          if (json_recover_range && json_recover_range->type == cJSON_Number)
               tpl.recover_range = json_recover_range->valuedouble;
          if (json_recover_intensity && json_recover_intensity->type == cJSON_Number)
               tpl.recover_intensity = json_recover_intensity->valuedouble;
     }
};

#endif // !_CONFIG_MANAGER_H_