#ifndef _COIN_MANAGER_H_
#define _COIN_MANAGER_H_

// 包含必要的头文件
#include "coin_prop.h"      // 金币道具类
#include "manager.h"        // 基础管理器模板，实现单例模式
#include "config_manager.h" // 配置管理器，用于读取初始金币数量

#include <vector>

// 金币管理器类：负责管理游戏中的金币系统
// 功能包括：
// 1. 管理玩家金币数量（增加/减少）
// 2. 管理金币道具的生成和销毁
// 3. 更新和渲染所有金币道具
class CoinManager : public Manager<CoinManager>
{
     // 声明 Manager 模板类为友元，允许其访问私有成员
     friend class Manager<CoinManager>;

public:
     // 金币道具列表类型定义
     typedef std::vector<CoinProp *> CoinPropList;

public:
     // 增加金币数量
     // @param val: 要增加的金币数量
     void increase_coin(double val)
     {
          num_coin += val;
     }

     // 减少金币数量
     // @param val: 要减少的金币数量
     void decrease_coin(double val)
     {
          num_coin -= val;

          // 确保金币数量不会小于0
          if (num_coin < 0)
               num_coin = 0;
     }

     // 更新所有金币道具的状态
     // @param delta: 时间增量，单位：秒
     void on_update(double delta)
     {
          // 更新每个金币道具
          for (CoinProp *coin_prop : coin_prop_list)
               coin_prop->on_update(delta);

          // 移除并删除已失效的金币道具
          coin_prop_list.erase(std::remove_if(coin_prop_list.begin(), coin_prop_list.end(),
                                              [](CoinProp *coin_prop)
                                              {
                                                   bool deletable = coin_prop->can_remove();
                                                   if (deletable)
                                                        delete coin_prop;
                                                   return deletable;
                                              }),
                               coin_prop_list.end());
     }

     // 渲染所有金币道具
     // @param renderer: SDL渲染器
     void on_render(SDL_Renderer *renderer)
     {
          for (CoinProp *coin_prop : coin_prop_list)
               coin_prop->on_render(renderer);
     }

     // 获取当前金币数量
     // @return: 当前金币数量
     double get_current_coin_num()
     {
          return num_coin;
     }

     // 获取金币道具列表
     // @return: 金币道具列表的引用
     CoinPropList &get_coin_prop_list()
     {
          return coin_prop_list;
     }

     // 在指定位置生成金币道具
     // @param position: 生成位置
     void spawn_coin_prop(const Vector2 &position)
     {
          // 创建新的金币道具
          CoinProp *coin_prop = new CoinProp();
          coin_prop->set_position(position);

          // 添加到金币道具列表
          coin_prop_list.push_back(coin_prop);
     }

protected:
     // 构造函数：初始化金币数量为配置文件中设置的初始值
     CoinManager()
     {
          num_coin = ConfigManager::instance()->num_initial_coin;
     }

     // 析构函数：清理所有金币道具
     ~CoinManager()
     {
          for (CoinProp *coin_prop : coin_prop_list)
               delete coin_prop;
     }

private:
     double num_coin = 0;         // 当前金币数量
     CoinPropList coin_prop_list; // 金币道具列表
};

#endif // !_COIN_MANAGER_H_
