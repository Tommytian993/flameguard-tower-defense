#ifndef _HOME_MANAGER_H_
#define _HOME_MANAGER_H_

// 包含必要的头文件
#include "manager.h"           // 基础管理器模板
#include "config_manager.h"    // 配置管理器，用于读取初始生命值
#include "resources_manager.h" // 资源管理器，用于播放音效

// HomeManager 类：负责管理玩家基地的生命值
// 该类继承自 Manager 模板类，实现了单例模式
class HomeManager : public Manager<HomeManager>
{
     // 声明 Manager 模板类为友元，允许其访问私有成员
     friend class Manager<HomeManager>;

public:
     // 获取当前基地生命值
     // @return: 返回当前生命值
     double get_current_hp_num()
     {
          return num_hp;
     }

     // 减少基地生命值
     // @param val: 要减少的生命值数量
     void decrease_hp(double val)
     {
          // 减少生命值
          num_hp -= val;

          // 确保生命值不会小于 0
          if (num_hp < 0)
               num_hp = 0;

          // 获取音效池
          static const ResourcesManager::SoundPool &sound_pool = ResourcesManager::instance()->get_sound_pool();

          // 播放基地受伤音效
          // Mix_PlayChannel 是 SDL_mixer 库的函数，用于播放音效
          // -1 表示自动选择可用声道，0 表示不循环播放
          Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_HomeHurt)->second, 0);
     }

protected:
     // 构造函数：初始化基地生命值为配置文件中设置的初始值
     HomeManager()
     {
          num_hp = ConfigManager::instance()->num_initial_hp;
     }

     // 析构函数：使用默认实现
     ~HomeManager() = default;

private:
     // 基地当前生命值
     double num_hp = 0;
};

#endif // !_HOME_MANAGER_H_
