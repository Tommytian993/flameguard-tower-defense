#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include "../map.h"
#include "../wave.h"
#include "manager.h"

#include <SDL.h>
#include <string>
#include <cJSON.h>
#include <fstream>
#include <sstream>
#include <iostream>

// 配置管理器：负责加载和管理游戏配置数据
class ConfigManager : public Manager<ConfigManager>
{
     friend class Manager<ConfigManager>;

public:
     // 基础配置模板：包含窗口和游戏基本设置
     struct BasicTemplate
     {
          std::string window_title = u8"Tower Defense Game!"; // 窗口标题
          int window_width = 1280;                            // 窗口宽度
          int window_height = 720;                            // 窗口高度
     };

     // 玩家配置模板：包含玩家属性和技能设置
     struct PlayerTemplate
     {
          double speed = 3;                    // 移动速度
          double normal_attack_interval = 0.5; // 普通攻击间隔（秒）
          double normal_attack_damage = 0;     // 普通攻击伤害
          double skill_interval = 10;          // 技能冷却时间（秒）
          double skill_damage = 1;             // 技能伤害
     };

     // 防御塔配置模板：包含防御塔属性和升级设置
     struct TowerTemplate
     {
          double interval[10] = {1};     // 攻击间隔（秒），索引为等级
          double damage[10] = {25};      // 攻击伤害，索引为等级
          double view_range[10] = {5};   // 视野范围，索引为等级
          double cost[10] = {50};        // 建造费用，索引为等级
          double upgrade_cost[9] = {75}; // 升级费用，索引为当前等级
     };

     // 敌人配置模板：包含敌人属性和特殊能力设置
     struct EnemyTemplate
     {
          double hp = 100;               // 生命值
          double speed = 1;              // 移动速度
          double damage = 1;             // 攻击伤害
          double reward_ratio = 0.5;     // 击杀奖励倍率
          double recover_interval = 10;  // 恢复技能间隔（秒）
          double recover_range = 0;      // 恢复技能范围
          double recover_intensity = 25; // 恢复技能强度
     };

public:
     Map map;                     // 当前地图数据
     std::vector<Wave> wave_list; // 波次列表

     // 防御塔等级
     int level_archer = 0; // 弓箭手等级
     int level_axeman = 0; // 斧手等级
     int level_gunner = 0; // 枪手等级

     // 游戏状态
     bool is_game_win = true;      // 游戏胜利标志
     bool is_game_over = false;    // 游戏结束标志
     SDL_Rect rect_tile_map = {0}; // 地图显示区域

     // 配置模板实例
     BasicTemplate basic_template; // 基础配置

     PlayerTemplate player_template; // 玩家配置

     // 防御塔配置
     TowerTemplate archer_template; // 弓箭手配置
     TowerTemplate axeman_template; // 斧手配置
     TowerTemplate gunner_template; // 枪手配置

     // 敌人配置
     EnemyTemplate slim_template;          // 史莱姆配置
     EnemyTemplate king_slim_template;     // 史莱姆王配置
     EnemyTemplate skeleton_template;      // 骷髅配置
     EnemyTemplate goblin_template;        // 哥布林配置
     EnemyTemplate goblin_priest_template; // 哥布林祭司配置

     // 游戏初始值
     const double num_initial_hp = 10;    // 初始生命值
     const double num_initial_coin = 100; // 初始金币
     const double num_coin_per_prop = 10; // 每个道具的金币数

public:
     // 加载关卡配置
     // @param path: 关卡配置文件路径
     // @return: 加载成功返回true，否则返回false
     bool load_level_config(const std::string &path)
     {
          std::ifstream file(path);

          if (!file.good())
               return false;

          // 读取文件内容
          std::stringstream str_stream;
          str_stream << file.rdbuf();
          file.close();

          // 解析JSON
          cJSON *json_root = cJSON_Parse(str_stream.str().c_str());
          if (!json_root)
               return false;

          if (json_root->type != cJSON_Array)
          {
               cJSON_Delete(json_root);
               return false;
          }

          // 解析每个波次
          cJSON *json_wave = nullptr;
          cJSON_ArrayForEach(json_wave, json_root)
          {
               if (json_wave->type != cJSON_Object)
                    continue;

               wave_list.emplace_back();
               Wave &wave = wave_list.back();

               // 解析波次奖励
               cJSON *json_wave_rewards = cJSON_GetObjectItem(json_wave, "rewards");
               if (json_wave_rewards && json_wave_rewards->type == cJSON_Number)
                    wave.rawards = json_wave_rewards->valuedouble;

               // 解析波次间隔
               cJSON *json_wave_interval = cJSON_GetObjectItem(json_wave, "interval");
               if (json_wave_interval && json_wave_interval->type == cJSON_Number)
                    wave.interval = json_wave_interval->valuedouble;

               // 解析生成事件列表
               cJSON *json_wave_spawn_list = cJSON_GetObjectItem(json_wave, "spawn_list");
               if (json_wave_spawn_list && json_wave_spawn_list->type == cJSON_Array)
               {
                    cJSON *json_spawn_event = nullptr;
                    cJSON_ArrayForEach(json_spawn_event, json_wave_spawn_list)
                    {
                         if (json_spawn_event->type != cJSON_Object)
                              continue;

                         wave.spawn_event_list.emplace_back();
                         Wave::SpawnEvent &spawn_event = wave.spawn_event_list.back();

                         // 解析生成事件间隔
                         cJSON *json_spawn_event_interval = cJSON_GetObjectItem(json_spawn_event, "interval");
                         if (json_spawn_event_interval && json_spawn_event_interval->type == cJSON_Number)
                              spawn_event.interval = json_spawn_event_interval->valuedouble;

                         // 解析生成点
                         cJSON *json_spawn_event_spawn_point = cJSON_GetObjectItem(json_spawn_event, "point");
                         if (json_spawn_event_spawn_point && json_spawn_event_spawn_point->type == cJSON_Number)
                              spawn_event.spawn_point = json_spawn_event_spawn_point->valueint;

                         // 解析敌人类型
                         cJSON *json_spawn_event_enemy_type = cJSON_GetObjectItem(json_spawn_event, "enemy");
                         if (json_spawn_event_enemy_type && json_spawn_event_enemy_type->type == cJSON_String)
                         {
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

          cJSON_Delete(json_root);

          if (wave_list.empty())
               return false;

          return true;
     }

     // 加载游戏配置
     // @param path: 游戏配置文件路径
     // @return: 加载成功返回true，否则返回false
     bool load_game_config(const std::string &path)
     {
          std::ifstream file(path);
          if (!file.good())
               return false;

          // 读取文件内容
          std::stringstream str_stream;
          str_stream << file.rdbuf();
          file.close();

          // 解析JSON
          cJSON *json_root = cJSON_Parse(str_stream.str().c_str());
          if (!json_root || json_root->type != cJSON_Object)
               return false;

          // 获取各个配置节点
          cJSON *json_basic = cJSON_GetObjectItem(json_root, "basic");
          cJSON *json_player = cJSON_GetObjectItem(json_root, "player");
          cJSON *json_tower = cJSON_GetObjectItem(json_root, "tower");
          cJSON *json_enemy = cJSON_GetObjectItem(json_root, "enemy");

          // 验证配置节点
          if (!json_basic || !json_player || !json_tower || !json_enemy ||
              json_basic->type != cJSON_Object || json_player->type != cJSON_Object ||
              json_tower->type != cJSON_Object || json_enemy->type != cJSON_Object)
          {
               cJSON_Delete(json_root);
               return false;
          }

          // 解析各个配置模板
          parse_basic_template(basic_template, json_basic);
          parse_player_template(player_template, json_player);
          parse_tower_template(archer_template, cJSON_GetObjectItem(json_tower, "archer"));
          parse_tower_template(axeman_template, cJSON_GetObjectItem(json_tower, "axeman"));
          parse_tower_template(gunner_template, cJSON_GetObjectItem(json_tower, "gunner"));
          parse_enemy_template(slim_template, cJSON_GetObjectItem(json_enemy, "slim"));
          parse_enemy_template(king_slim_template, cJSON_GetObjectItem(json_enemy, "king_slim"));
          parse_enemy_template(skeleton_template, cJSON_GetObjectItem(json_enemy, "skeleton"));
          parse_enemy_template(goblin_template, cJSON_GetObjectItem(json_enemy, "goblin"));
          parse_enemy_template(goblin_priest_template, cJSON_GetObjectItem(json_enemy, "goblin_priest"));

          cJSON_Delete(json_root);
          return true;
     }

protected:
     ConfigManager() = default;
     ~ConfigManager() = default;

private:
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