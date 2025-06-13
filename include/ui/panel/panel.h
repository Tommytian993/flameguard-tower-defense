/**
 * @file panel.h
 * @brief 面板基类，定义了游戏中UI面板的基本属性和行为
 */

#ifndef _PANEL_H_
#define _PANEL_H_

#include "tile.h"
#include "resources_manager.h"

#include <SDL2/SDL.h>
#include <string>

/**
 * @class Panel
 * @brief 面板基类，提供了UI面板的基本功能，包括显示、交互和渲染
 */
class Panel
{
public:
     /**
      * @brief 构造函数，初始化面板的选择光标纹理
      */
     Panel()
     {
          tex_select_cursor = ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_UISelectCursor)->second;
     }

     /**
      * @brief 析构函数，清理文本纹理资源
      */
     ~Panel()
     {
          SDL_DestroyTexture(tex_text_background);
          SDL_DestroyTexture(tex_text_foreground);
     }

     /**
      * @brief 显示面板
      */
     void show()
     {
          visible = true;
     }

     /**
      * @brief 设置选中的瓦片索引
      * @param idx 瓦片的网格索引
      */
     void set_idx_tile(const SDL_Point &idx)
     {
          idx_tile_selected = idx;
     }

     /**
      * @brief 设置面板的中心位置
      * @param pos 中心点坐标
      */
     void set_center_pos(const SDL_Point &pos)
     {
          center_pos = pos;
     }

     /**
      * @brief 处理输入事件
      * @param event SDL事件
      */
     void on_input(const SDL_Event &event)
     {
          if (!visible)
               return;

          switch (event.type)
          {
          case SDL_MOUSEMOTION:
          {
               // 检查鼠标是否悬停在面板的各个区域
               SDL_Point pos_cursor = {event.motion.x, event.motion.y};
               SDL_Rect rect_target = {0, 0, size_button, size_button};

               // 检查顶部区域
               rect_target.x = center_pos.x - width / 2 + offset_top.x;
               rect_target.y = center_pos.y - width / 2 + offset_top.y;
               if (SDL_PointInRect(&pos_cursor, &rect_target))
               {
                    hovered_target = HoveredTarget::Top;
                    return;
               }

               // 检查左侧区域
               rect_target.x = center_pos.x - width / 2 + offset_left.x;
               rect_target.y = center_pos.y - width / 2 + offset_left.y;
               if (SDL_PointInRect(&pos_cursor, &rect_target))
               {
                    hovered_target = HoveredTarget::Left;
                    return;
               }

               // 检查右侧区域
               rect_target.x = center_pos.x - width / 2 + offset_right.x;
               rect_target.y = center_pos.y - width / 2 + offset_right.y;
               if (SDL_PointInRect(&pos_cursor, &rect_target))
               {
                    hovered_target = HoveredTarget::Right;
                    return;
               }

               hovered_target = HoveredTarget::None;
          }
          break;
          case SDL_MOUSEBUTTONUP:
          {
               // 处理鼠标点击事件
               switch (hovered_target)
               {
               case Panel::HoveredTarget::Top:
                    on_click_top_area();
                    break;
               case Panel::HoveredTarget::Left:
                    on_click_left_area();
                    break;
               case Panel::HoveredTarget::Right:
                    on_click_right_area();
                    break;
               }

               visible = false;
          }
          break;
          default:
               break;
          }
     }

     /**
      * @brief 更新面板状态
      * @param renderer SDL渲染器指针
      */
     virtual void on_update(SDL_Renderer *renderer)
     {
          static TTF_Font *font = ResourcesManager::instance()->get_font_pool().find(ResID::Font_Main)->second;

          if (hovered_target == HoveredTarget::None)
               return;

          // 获取当前悬停区域的值
          int val = 0;
          switch (hovered_target)
          {
          case Panel::HoveredTarget::Top:
               val = val_top;
               break;
          case Panel::HoveredTarget::Left:
               val = val_left;
               break;
          case Panel::HoveredTarget::Right:
               val = val_right;
               break;
          }

          // 清理旧的文本纹理
          SDL_DestroyTexture(tex_text_background);
          tex_text_background = nullptr;
          SDL_DestroyTexture(tex_text_foreground);
          tex_text_foreground = nullptr;

          // 创建新的文本纹理
          std::string str_val = val < 0 ? "MAX" : std::to_string(val);
          SDL_Surface *suf_text_background = TTF_RenderText_Blended(font, str_val.c_str(), color_text_background);
          SDL_Surface *suf_text_foreground = TTF_RenderText_Blended(font, str_val.c_str(), color_text_foreground);

          width_text = suf_text_background->w, height_text = suf_text_background->h;
          tex_text_background = SDL_CreateTextureFromSurface(renderer, suf_text_background);
          tex_text_foreground = SDL_CreateTextureFromSurface(renderer, suf_text_foreground);

          SDL_FreeSurface(suf_text_background);
          SDL_FreeSurface(suf_text_foreground);
     }

     /**
      * @brief 渲染面板
      * @param renderer SDL渲染器指针
      */
     virtual void on_render(SDL_Renderer *renderer)
     {
          if (!visible)
               return;

          // 渲染选择光标
          SDL_Rect rect_dst_cursor =
              {
                  center_pos.x - SIZE_TILE / 2,
                  center_pos.y - SIZE_TILE / 2,
                  SIZE_TILE, SIZE_TILE};
          SDL_RenderCopy(renderer, tex_select_cursor, nullptr, &rect_dst_cursor);

          // 渲染面板背景
          SDL_Rect rect_dst_panel =
              {
                  center_pos.x - width / 2,
                  center_pos.y - height / 2,
                  width, height};

          // 根据悬停状态选择面板纹理
          SDL_Texture *tex_panel = nullptr;
          switch (hovered_target)
          {
          case Panel::HoveredTarget::None:
               tex_panel = tex_idle;
               break;
          case Panel::HoveredTarget::Top:
               tex_panel = tex_hovered_top;
               break;
          case Panel::HoveredTarget::Left:
               tex_panel = tex_hovered_left;
               break;
          case Panel::HoveredTarget::Right:
               tex_panel = tex_hovered_right;
               break;
          }

          SDL_RenderCopy(renderer, tex_panel, nullptr, &rect_dst_panel);

          if (hovered_target == HoveredTarget::None)
               return;

          // 渲染文本（带阴影效果）
          SDL_Rect rect_dst_text;

          // 渲染文本阴影
          rect_dst_text.x = center_pos.x - width_text / 2 + offset_shadow.x;
          rect_dst_text.y = center_pos.y + height / 2 + offset_shadow.y;
          rect_dst_text.w = width_text, rect_dst_text.h = height_text;
          SDL_RenderCopy(renderer, tex_text_background, nullptr, &rect_dst_text);

          // 渲染文本前景
          rect_dst_text.x -= offset_shadow.x;
          rect_dst_text.y -= offset_shadow.y;
          SDL_RenderCopy(renderer, tex_text_foreground, nullptr, &rect_dst_text);
     }

protected:
     /**
      * @brief 悬停目标枚举，定义面板的交互区域
      */
     enum class HoveredTarget
     {
          None, ///< 无悬停
          Top,  ///< 顶部区域
          Left, ///< 左侧区域
          Right ///< 右侧区域
     };

protected:
     bool visible = false;                               ///< 面板是否可见
     SDL_Point idx_tile_selected;                        ///< 选中的瓦片索引
     SDL_Point center_pos = {0};                         ///< 面板中心位置
     SDL_Texture *tex_idle = nullptr;                    ///< 默认状态纹理
     SDL_Texture *tex_hovered_top = nullptr;             ///< 顶部悬停状态纹理
     SDL_Texture *tex_hovered_left = nullptr;            ///< 左侧悬停状态纹理
     SDL_Texture *tex_hovered_right = nullptr;           ///< 右侧悬停状态纹理
     SDL_Texture *tex_select_cursor = nullptr;           ///< 选择光标纹理
     int val_top = 0, val_left = 0, val_right = 0;       ///< 各区域的值
     HoveredTarget hovered_target = HoveredTarget::None; ///< 当前悬停目标

protected:
     /**
      * @brief 点击顶部区域的处理函数
      */
     virtual void on_click_top_area() = 0;
     /**
      * @brief 点击左侧区域的处理函数
      */
     virtual void on_click_left_area() = 0;
     /**
      * @brief 点击右侧区域的处理函数
      */
     virtual void on_click_right_area() = 0;

private:
     const int size_button = 48;                                   ///< 按钮尺寸
     const int width = 144, height = 144;                          ///< 面板尺寸
     const SDL_Point offset_top = {48, 6};                         ///< 顶部区域偏移
     const SDL_Point offset_left = {8, 80};                        ///< 左侧区域偏移
     const SDL_Point offset_right = {90, 80};                      ///< 右侧区域偏移
     const SDL_Point offset_shadow = {3, 3};                       ///< 文本阴影偏移
     const SDL_Color color_text_background = {175, 175, 175, 255}; ///< 文本阴影颜色
     const SDL_Color color_text_foreground = {255, 255, 255, 255}; ///< 文本前景颜色

     int width_text = 0, height_text = 0;        ///< 文本尺寸
     SDL_Texture *tex_text_background = nullptr; ///< 文本阴影纹理
     SDL_Texture *tex_text_foreground = nullptr; ///< 文本前景纹理
};

#endif // !_PANEL_H_
