#ifndef _KINNG_SLIM_ENEMY_H_
#define _KINNG_SLIM_ENEMY_H_

#include "enemy/enemy.h"
#include "manager/config_manager.h"
#include "manager/resources_manager.h"

// 史莱姆王敌人类
// 继承自Enemy基类，实现基础敌人的行为
// 特点：
// 1. 基础属性：生命值高，移动速度中等
// 2. 动画：四个方向的行走动画，每个方向6帧，使用特殊的精英纹理
class KingSlimeEnemy : public Enemy
{
public:
	KingSlimeEnemy()
	{
		// 获取资源管理器中的纹理资源
		static const ResourcesManager::TexturePool &texture_pool = ResourcesManager::instance()->get_texture_pool();
		static SDL_Texture *tex_king_slime = texture_pool.find(ResID::Tex_KingSlime)->second;
		static SDL_Texture *tex_king_slime_sketch = texture_pool.find(ResID::Tex_KingSlimeSketch)->second;

		// 获取配置管理器中的史莱姆王模板配置
		static ConfigManager::EnemyTemplate &king_slim_template = ConfigManager::instance()->king_slim_template;

		// 定义四个方向的动画帧索引
		// 每个方向6帧，从精灵图中提取
		static const std::vector<int> idx_list_up = {18, 19, 20, 21, 22, 23};	   // 向上行走的帧
		static const std::vector<int> idx_list_down = {0, 1, 2, 3, 4, 5};	   // 向下行走的帧
		static const std::vector<int> idx_list_left = {6, 7, 8, 9, 10, 11};	   // 向左行走的帧
		static const std::vector<int> idx_list_right = {12, 13, 14, 15, 16, 17}; // 向右行走的帧

		// 设置正常状态下的动画
		// 每个方向设置循环播放，间隔0.1秒
		anim_up.set_loop(true);
		anim_up.set_interval(0.1);
		anim_up.set_frame_data(tex_king_slime, 6, 4, idx_list_up);
		anim_down.set_loop(true);
		anim_down.set_interval(0.1);
		anim_down.set_frame_data(tex_king_slime, 6, 4, idx_list_down);
		anim_left.set_loop(true);
		anim_left.set_interval(0.1);
		anim_left.set_frame_data(tex_king_slime, 6, 4, idx_list_left);
		anim_right.set_loop(true);
		anim_right.set_interval(0.1);
		anim_right.set_frame_data(tex_king_slime, 6, 4, idx_list_right);

		// 设置草图状态下的动画（用于预览或特殊效果）
		// 使用相同的帧索引，但使用草图纹理
		anim_up_sketch.set_loop(true);
		anim_up_sketch.set_interval(0.1);
		anim_up_sketch.set_frame_data(tex_king_slime_sketch, 6, 4, idx_list_up);
		anim_down_sketch.set_loop(true);
		anim_down_sketch.set_interval(0.1);
		anim_down_sketch.set_frame_data(tex_king_slime_sketch, 6, 4, idx_list_down);
		anim_left_sketch.set_loop(true);
		anim_left_sketch.set_interval(0.1);
		anim_left_sketch.set_frame_data(tex_king_slime_sketch, 6, 4, idx_list_left);
		anim_right_sketch.set_loop(true);
		anim_right_sketch.set_interval(0.1);
		anim_right_sketch.set_frame_data(tex_king_slime_sketch, 6, 4, idx_list_right);

		// 从配置模板中加载敌人属性
		max_hp = king_slim_template.hp;					   // 最大生命值
		max_speed = king_slim_template.speed;				   // 最大移动速度
		damage = king_slim_template.damage;				   // 攻击伤害
		reward_ratio = king_slim_template.reward_ratio;		   // 击杀奖励倍率
		recover_interval = king_slim_template.recover_interval;   // 恢复技能间隔
		recover_range = king_slim_template.recover_range;		   // 恢复技能范围
		recover_intensity = king_slim_template.recover_intensity; // 恢复技能强度

		// 设置敌人大小和初始状态
		size.x = 48, size.y = 48;	  // 设置碰撞箱大小
		hp = max_hp, speed = max_speed; // 初始化当前生命值和速度
	}

	~KingSlimeEnemy() = default;
};

#endif // !_KINNG_SLIM_ENEMY_H_
