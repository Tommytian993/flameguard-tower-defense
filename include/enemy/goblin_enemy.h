#ifndef _GOBLIN_ENEMY_H_
#define _GOBLIN_ENEMY_H_

#include "enemy/enemy.h"
#include "manager/config_manager.h"
#include "manager/resources_manager.h"

// 哥布林敌人类
// 继承自Enemy基类，实现基础敌人的行为
// 特点：
// 1. 基础属性：生命值低，移动速度快
// 2. 动画：四个方向的行走动画，每个方向6帧，使用哥布林特有的纹理
class GoblinEnemy : public Enemy
{
public:
	GoblinEnemy()
	{
		// 获取资源管理器中的纹理资源
		static const ResourcesManager::TexturePool &texture_pool = ResourcesManager::instance()->get_texture_pool();
		static SDL_Texture *tex_goblin = texture_pool.find(ResID::Tex_Goblin)->second;
		static SDL_Texture *tex_goblin_sketch = texture_pool.find(ResID::Tex_GoblinSketch)->second;

		// 获取配置管理器中的哥布林模板配置
		static ConfigManager::EnemyTemplate &goblin_template = ConfigManager::instance()->goblin_template;

		// 定义四个方向的动画帧索引
		// 每个方向6帧，从精灵图中提取
		static const std::vector<int> idx_list_up = {5, 6, 7, 8, 9, 10};		   // 向上行走的帧
		static const std::vector<int> idx_list_down = {0, 1, 2, 3, 4, 5};	   // 向下行走的帧
		static const std::vector<int> idx_list_left = {15, 16, 17, 18, 19, 20};  // 向左行走的帧
		static const std::vector<int> idx_list_right = {10, 11, 12, 13, 14, 15}; // 向右行走的帧

		// 设置正常状态下的动画
		// 每个方向设置循环播放，间隔0.1秒
		anim_up.set_loop(true);
		anim_up.set_interval(0.1);
		anim_up.set_frame_data(tex_goblin, 5, 4, idx_list_up);
		anim_down.set_loop(true);
		anim_down.set_interval(0.1);
		anim_down.set_frame_data(tex_goblin, 5, 4, idx_list_down);
		anim_left.set_loop(true);
		anim_left.set_interval(0.1);
		anim_left.set_frame_data(tex_goblin, 5, 4, idx_list_left);
		anim_right.set_loop(true);
		anim_right.set_interval(0.1);
		anim_right.set_frame_data(tex_goblin, 5, 4, idx_list_right);

		// 设置草图状态下的动画（用于预览或特殊效果）
		// 使用相同的帧索引，但使用草图纹理
		anim_up_sketch.set_loop(true);
		anim_up_sketch.set_interval(0.1);
		anim_up_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_up);
		anim_down_sketch.set_loop(true);
		anim_down_sketch.set_interval(0.1);
		anim_down_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_down);
		anim_left_sketch.set_loop(true);
		anim_left_sketch.set_interval(0.1);
		anim_left_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_left);
		anim_right_sketch.set_loop(true);
		anim_right_sketch.set_interval(0.1);
		anim_right_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_right);

		// 从配置模板中加载敌人属性
		max_hp = goblin_template.hp;						// 最大生命值
		max_speed = goblin_template.speed;					// 最大移动速度
		damage = goblin_template.damage;					// 攻击伤害
		reward_ratio = goblin_template.reward_ratio;			// 击杀奖励倍率
		recover_interval = goblin_template.recover_interval;	// 恢复技能间隔（未使用）
		recover_range = goblin_template.recover_range;		// 恢复技能范围（未使用）
		recover_intensity = goblin_template.recover_intensity; // 恢复技能强度（未使用）

		// 设置敌人大小和初始状态
		size.x = 48, size.y = 48;	  // 设置碰撞箱大小
		hp = max_hp, speed = max_speed; // 初始化当前生命值和速度
	}

	~GoblinEnemy() = default;
};

#endif // !_GOBLIN_ENEMY_H_
