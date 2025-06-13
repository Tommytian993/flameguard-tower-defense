#ifndef _BULLET_MANAGER_H_
#define _BULLET_MANAGER_H_

// 包含必要的头文件
#include "bullet/bullet.h"       // 子弹基类
#include "manager.h"             // 管理器基类
#include "bullet/arrow_bullet.h" // 箭矢子弹
#include "bullet/axe_bullet.h"   // 斧头子弹
#include "bullet/shell_bullet.h" // 炮弹子弹
#include "bullet/bullet_type.h"  // 子弹类型枚举

#include <vector>
#include <iostream>

// 子弹管理器类：继承自Manager基类
// 功能：
// 1. 管理所有子弹的生命周期
// 2. 创建不同类型的子弹
// 3. 更新和渲染所有子弹
// 4. 自动清理无效子弹
class BulletManager : public Manager<BulletManager>
{
     friend class Manager<BulletManager>;

public:
     // 子弹列表类型定义
     typedef std::vector<Bullet *> BulletList;

public:
     // 更新所有子弹的状态
     // @param delta: 时间增量（秒）
     // 功能：
     // 1. 更新每个子弹的位置、动画等
     // 2. 清理无效的子弹
     void on_update(double delta)
     {
          for (Bullet *bullet : bullet_list)
               bullet->on_update(delta);

          remove_invalid_bullet();
     }

     // 渲染所有子弹
     // @param renderer: SDL渲染器
     // 功能：调用每个子弹的渲染方法
     void on_render(SDL_Renderer *renderer)
     {
          for (Bullet *bullet : bullet_list)
               bullet->on_render(renderer);
     }

     // 获取子弹列表
     // @return: 当前所有子弹的列表
     BulletList &get_bullet_list()
     {
          return bullet_list;
     }

     // 创建新的子弹
     // @param type: 子弹类型（箭矢/斧头/炮弹）
     // @param position: 子弹的初始位置
     // @param target_position: 子弹的目标位置
     // @param damage: 子弹的伤害值
     // @param damage_range: 子弹的伤害范围（-1表示单体伤害）
     // @param velocity: 子弹的速度向量
     // 功能：
     // 1. 根据类型创建对应的子弹对象
     // 2. 设置子弹的初始属性
     // 3. 将子弹添加到管理列表中
     void create_bullet(BulletType type, const Vector2 &position, const Vector2 &target_position, double damage, double damage_range = -1, const Vector2 &velocity = Vector2())
     {
          Bullet *bullet = nullptr;

          // 根据类型创建对应的子弹
          switch (type)
          {
          case Arrow:
               bullet = new ArrowBullet();
               break;
          case Axe:
               bullet = new AxeBullet();
               break;
          case Shell:
               bullet = new ShellBullet();
               break;
          }

          // 设置子弹的基本属性
          bullet->set_position(position);
          if (velocity.length() > 0)
               bullet->set_velocity(velocity);
          else
               bullet->set_target_position(target_position);
          bullet->set_damage(damage);
          bullet->set_damage_range(damage_range);

          // 添加到管理列表
          bullet_list.push_back(bullet);
     }

protected:
     // 构造函数：默认构造
     BulletManager() = default;

     // 析构函数：清理所有子弹对象
     ~BulletManager()
     {
          for (Bullet *bullet : bullet_list)
               delete bullet;
     }

private:
     // 子弹列表：存储所有活跃的子弹
     BulletList bullet_list;

     // 移除无效的子弹
     // 功能：
     // 1. 遍历子弹列表
     // 2. 删除并移除已标记为无效的子弹
     // 3. 释放子弹对象的内存
     void remove_invalid_bullet()
     {
          bullet_list.erase(std::remove_if(
                                bullet_list.begin(), bullet_list.end(),
                                [](const Bullet *bullet)
                                {
                                     bool deletable = !bullet->is_valid();
                                     if (deletable)
                                          delete bullet;
                                     return deletable;
                                }),
                            bullet_list.end());
     }
};

#endif // !_BULLET_MANAGER_H_
