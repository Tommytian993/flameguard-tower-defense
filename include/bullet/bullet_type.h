#ifndef _BULLET_TYPE_H_
#define _BULLET_TYPE_H_

// 子弹类型枚举：定义游戏中不同类型的子弹
// 1. Arrow: 箭矢，直线飞行，单体伤害
// 2. Axe: 斧头，直线飞行，范围伤害
// 3. Shell: 炮弹，直线飞行，范围伤害
enum BulletType
{
     Arrow, // 箭矢：基础子弹类型，单体伤害
     Axe,   // 斧头：范围伤害，可旋转
     Shell  // 炮弹：范围伤害，爆炸效果
};

#endif // !_BULLET_TYPE_H_
