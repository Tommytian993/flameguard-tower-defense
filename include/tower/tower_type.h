/**
 * @file tower_type.h
 * @brief 定义游戏中防御塔的类型
 */

#ifndef _TOWER_TYPE_H_
#define _TOWER_TYPE_H_

/**
 * @enum TowerType
 * @brief 防御塔类型枚举，定义了游戏中可用的防御塔类型
 */
enum TowerType
{
     Archer, ///< 弓箭手塔，远程攻击单位
     Axeman, ///< 斧手塔，近战攻击单位
     Gunner  ///< 枪手塔，远程范围攻击单位
};

#endif // !_TOWER_TYPE_H_
