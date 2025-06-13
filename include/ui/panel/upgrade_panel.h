/**
 * @file upgrade_panel.h
 * @brief 升级面板类，用于显示和管理防御塔的升级界面
 */

#ifndef _UPGRADE_PANEL_H_
#define _UPGRADE_PANEL_H_

#include "panel.h"
#include "tower_manager.h"
#include "resources_manager.h"

/**
 * @class UpgradePanel
 * @brief 升级面板类，继承自Panel基类，用于显示防御塔的升级选项和相关信息
 */
class UpgradePanel : public Panel
{
public:
     /**
      * @brief 构造函数，初始化升级面板的纹理和状态
      */
     UpgradePanel()
     {
          const ResourcesManager::TexturePool &texture_pool = ResourcesManager::instance()->get_texture_pool();

          tex_idle = texture_pool.find(ResID::Tex_UIUpgradeIdle)->second;
          tex_hovered_top = texture_pool.find(ResID::Tex_UIUpgradeHoveredTop)->second;
          tex_hovered_left = texture_pool.find(ResID::Tex_UIUpgradeHoveredLeft)->second;
          tex_hovered_right = texture_pool.find(ResID::Tex_UIUpgradeHoveredRight)->second;
     }

     ~UpgradePanel() = default;

     /**
      * @brief 更新面板状态
      * @param renderer SDL渲染器指针
      */
     void on_update(SDL_Renderer *renderer)
     {
          static TowerManager *instance = TowerManager::instance();

          val_top = (int)instance->get_upgrade_cost(TowerType::Axeman);
          val_left = (int)instance->get_upgrade_cost(TowerType::Archer);
          val_right = (int)instance->get_upgrade_cost(TowerType::Gunner);

          Panel::on_update(renderer);
     }

protected:
     /**
      * @brief 处理点击顶部区域的事件（升级防御塔）
      */
     void on_click_top_area() override
     {
          CoinManager *instance = CoinManager::instance();

          if (val_top > 0 && val_top <= instance->get_current_coin_num())
          {
               TowerManager::instance()->upgrade_tower(TowerType::Axeman);
               instance->decrease_coin(val_top);
          }
     }

     /**
      * @brief 处理点击左侧区域的事件（出售防御塔）
      */
     void on_click_left_area() override
     {
          CoinManager *instance = CoinManager::instance();

          if (val_left > 0 && val_left <= instance->get_current_coin_num())
          {
               TowerManager::instance()->upgrade_tower(TowerType::Axeman);
               instance->decrease_coin(val_left);
          }
     }

     /**
      * @brief 处理点击右侧区域的事件（无操作，因为已达到最高等级）
      */
     void on_click_right_area() override
     {
          CoinManager *instance = CoinManager::instance();

          if (val_right > 0 && val_right <= instance->get_current_coin_num())
          {
               TowerManager::instance()->upgrade_tower(TowerType::Axeman);
               instance->decrease_coin(val_right);
          }
     }
};

#endif // !_UPGRADE_PANEL_H_
