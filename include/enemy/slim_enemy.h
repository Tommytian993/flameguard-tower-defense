#ifndef _SLIM_ENEMY_H_
#define _SLIM_ENEMY_H_

#include "enemy/enemy.h"
#include "manager/config_manager.h"
#include "manager/resources_manager.h"

// 史莱姆敌人类
// 继承自Enemy基类，实现基础敌人的行为
// 特点：
// 1. 基础属性：生命值中等，移动速度较慢
// 2. 动画：四个方向的行走动画，每个方向6帧
// 3. 无特殊技能：作为最基础的敌人单位
class SlimEnemy : public Enemy
{
public:
	SlimEnemy()
	{
		// 获取资源管理器中的纹理资源
		static const ResourcesManager::TexturePool &texture_pool = ResourcesManager::instance()->get_texture_pool();
		static SDL_Texture *tex_slime = texture_pool.find(ResID::Tex_Slime)->second;
		static SDL_Texture *tex_slime_sketch = texture_pool.find(ResID::Tex_SlimeSketch)->second;

		// 获取配置管理器中的史莱姆模板配置
		static ConfigManager::EnemyTemplate &slim_template = ConfigManager::instance()->slim_template;

		// 定义四个方向的动画帧索引
		// 每个方向6帧，从精灵图中提取
		static const std::vector<int> idx_list_up = {6, 7, 8, 9, 10, 11};	   // 向上行走的帧
		static const std::vector<int> idx_list_down = {0, 1, 2, 3, 4, 5};	   // 向下行走的帧
		static const std::vector<int> idx_list_left = {18, 19, 20, 21, 22, 23};  // 向左行走的帧
		static const std::vector<int> idx_list_right = {12, 13, 14, 15, 16, 17}; // 向右行走的帧

		// 设置正常状态下的动画
		// 每个方向设置循环播放，间隔0.1秒
		anim_up.set_loop(true);
		anim_up.set_interval(0.1);
		anim_up.set_frame_data(tex_slime, 6, 4, idx_list_up);
		anim_down.set_loop(true);
		anim_down.set_interval(0.1);
		anim_down.set_frame_data(tex_slime, 6, 4, idx_list_down);
		anim_left.set_loop(true);
		anim_left.set_interval(0.1);
		anim_left.set_frame_data(tex_slime, 6, 4, idx_list_left);
		anim_right.set_loop(true);
		anim_right.set_interval(0.1);
		anim_right.set_frame_data(tex_slime, 6, 4, idx_list_right);

		// 设置草图状态下的动画（用于预览或特殊效果）
		// 使用相同的帧索引，但使用草图纹理
		anim_up_sketch.set_loop(true);
		anim_up_sketch.set_interval(0.1);
		anim_up_sketch.set_frame_data(tex_slime_sketch, 6, 4, idx_list_up);
		anim_down_sketch.set_loop(true);
		anim_down_sketch.set_interval(0.1);
		anim_down_sketch.set_frame_data(tex_slime_sketch, 6, 4, idx_list_down);
		anim_left_sketch.set_loop(true);
		anim_left_sketch.set_interval(0.1);
		anim_left_sketch.set_frame_data(tex_slime_sketch, 6, 4, idx_list_left);
		anim_right_sketch.set_loop(true);
		anim_right_sketch.set_interval(0.1);
		anim_right_sketch.set_frame_data(tex_slime_sketch, 6, 4, idx_list_right);

		// 从配置模板中加载敌人属性
		max_hp = slim_template.hp;					   // 最大生命值
		max_speed = slim_template.speed;				   // 最大移动速度
		damage = slim_template.damage;				   // 攻击伤害
		reward_ratio = slim_template.reward_ratio;		   // 击杀奖励倍率
		recover_interval = slim_template.recover_interval;   // 恢复技能间隔（未使用）
		recover_range = slim_template.recover_range;		   // 恢复技能范围（未使用）
		recover_intensity = slim_template.recover_intensity; // 恢复技能强度（未使用）

		// 设置敌人大小和初始状态
		size.x = 48, size.y = 48;	  // 设置碰撞箱大小
		hp = max_hp, speed = max_speed; // 初始化当前生命值和速度
	}

	~SlimEnemy() = default;
};

#endif // !_SLIM_ENEMY_H_
