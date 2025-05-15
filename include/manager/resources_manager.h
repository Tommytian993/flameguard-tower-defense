#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_

#include "manager.h"

#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <unordered_map>

// 资源ID枚举：用于标识和管理所有游戏资源
enum class ResID
{
     // 地图资源
     Tex_Tileset, // 瓦片集纹理

     // 角色纹理
     Tex_Player, // 玩家纹理
     Tex_Archer, // 弓箭手防御塔纹理
     Tex_Axeman, // 斧手防御塔纹理
     Tex_Gunner, // 枪手防御塔纹理

     // 敌人纹理
     Tex_Slime,        // 史莱姆纹理
     Tex_KingSlime,    // 史莱姆王纹理
     Tex_Skeleton,     // 骷髅纹理
     Tex_Goblin,       // 哥布林纹理
     Tex_GoblinPriest, // 哥布林祭司纹理
                       // 敌人草图纹理（用于预览）
     Tex_SlimeSketch,
     Tex_KingSlimeSketch,
     Tex_SkeletonSketch,
     Tex_GoblinSketch,
     Tex_GoblinPriestSketch,

     // 子弹纹理
     Tex_BulletArrow, // 箭矢纹理
     Tex_BulletAxe,   // 斧头纹理
     Tex_BulletShell, // 子弹纹理

     // 游戏道具纹理
     Tex_Coin, // 金币纹理
     Tex_Home, // 基地纹理

     // 特效纹理
     Tex_EffectFlash_Up,     // 上方向闪光特效
     Tex_EffectFlash_Down,   // 下方向闪光特效
     Tex_EffectFlash_Left,   // 左方向闪光特效
     Tex_EffectFlash_Right,  // 右方向闪光特效
     Tex_EffectImpact_Up,    // 上方向冲击特效
     Tex_EffectImpact_Down,  // 下方向冲击特效
     Tex_EffectImpact_Left,  // 左方向冲击特效
     Tex_EffectImpact_Right, // 右方向冲击特效
     Tex_EffectExplode,      // 爆炸特效

     // UI纹理
     Tex_UISelectCursor,        // 选择光标
     Tex_UIPlaceIdle,           // 放置按钮默认状态
     Tex_UIPlaceHoveredTop,     // 放置按钮顶部悬停状态
     Tex_UIPlaceHoveredLeft,    // 放置按钮左侧悬停状态
     Tex_UIPlaceHoveredRight,   // 放置按钮右侧悬停状态
     Tex_UIUpgradeIdle,         // 升级按钮默认状态
     Tex_UIUpgradeHoveredTop,   // 升级按钮顶部悬停状态
     Tex_UIUpgradeHoveredLeft,  // 升级按钮左侧悬停状态
     Tex_UIUpgradeHoveredRight, // 升级按钮右侧悬停状态
     Tex_UIHomeAvatar,          // 基地头像
     Tex_UIPlayerAvatar,        // 玩家头像
     Tex_UIHeart,               // 生命值图标
     Tex_UICoin,                // 金币图标
     Tex_UIGameOverBar,         // 游戏结束条
     Tex_UIWinText,             // 胜利文本
     Tex_UILossText,            // 失败文本

     // 音效
     Sound_ArrowFire_1, // 箭矢发射音效1
     Sound_ArrowFire_2, // 箭矢发射音效2
     Sound_AxeFire,     // 斧头发射音效
     Sound_ShellFire,   // 子弹发射音效
     Sound_ArrowHit_1,  // 箭矢命中音效1
     Sound_ArrowHit_2,  // 箭矢命中音效2
     Sound_ArrowHit_3,  // 箭矢命中音效3
     Sound_AxeHit_1,    // 斧头命中音效1
     Sound_AxeHit_2,    // 斧头命中音效2
     Sound_AxeHit_3,    // 斧头命中音效3
     Sound_ShellHit,    // 子弹命中音效

     // 特效音效
     Sound_Flash,  // 闪光音效
     Sound_Impact, // 冲击音效

     // 游戏音效
     Sound_Coin,         // 获得金币音效
     Sound_HomeHurt,     // 基地受伤音效
     Sound_PlaceTower,   // 放置防御塔音效
     Sound_TowerLevelUp, // 防御塔升级音效

     // 游戏状态音效
     Sound_Win,  // 胜利音效
     Sound_Loss, // 失败音效

     // 背景音乐
     Music_BGM, // 背景音乐

     // 字体
     Font_Main // 主字体
};

// 资源管理器：负责加载和管理游戏资源
class ResourcesManager : public Manager<ResourcesManager>
{
     friend class Manager<ResourcesManager>;

public:
     // 资源池类型定义
     typedef std::unordered_map<ResID, TTF_Font *> FontPool;       // 字体资源池
     typedef std::unordered_map<ResID, Mix_Chunk *> SoundPool;     // 音效资源池
     typedef std::unordered_map<ResID, Mix_Music *> MusicPool;     // 音乐资源池
     typedef std::unordered_map<ResID, SDL_Texture *> TexturePool; // 纹理资源池

public:
     // 从文件加载所有资源
     // @param renderer: SDL渲染器
     // @return: 加载成功返回true，否则返回false
     bool load_from_file(SDL_Renderer *renderer)
     {
          // 加载地图纹理
          texture_pool[ResID::Tex_Tileset] = IMG_LoadTexture(renderer, "resources/tileset.png");

          // 加载角色纹理
          texture_pool[ResID::Tex_Player] = IMG_LoadTexture(renderer, "resources/player.png");
          texture_pool[ResID::Tex_Archer] = IMG_LoadTexture(renderer, "resources/tower_archer.png");
          texture_pool[ResID::Tex_Axeman] = IMG_LoadTexture(renderer, "resources/tower_axeman.png");
          texture_pool[ResID::Tex_Gunner] = IMG_LoadTexture(renderer, "resources/tower_gunner.png");

          // 加载敌人纹理
          texture_pool[ResID::Tex_Slime] = IMG_LoadTexture(renderer, "resources/enemy_slime.png");
          texture_pool[ResID::Tex_KingSlime] = IMG_LoadTexture(renderer, "resources/enemy_king_slime.png");
          texture_pool[ResID::Tex_Skeleton] = IMG_LoadTexture(renderer, "resources/enemy_skeleton.png");
          texture_pool[ResID::Tex_Goblin] = IMG_LoadTexture(renderer, "resources/enemy_goblin.png");
          texture_pool[ResID::Tex_GoblinPriest] = IMG_LoadTexture(renderer, "resources/enemy_goblin_priest.png");
          texture_pool[ResID::Tex_SlimeSketch] = IMG_LoadTexture(renderer, "resources/enemy_slime_sketch.png");
          texture_pool[ResID::Tex_KingSlimeSketch] = IMG_LoadTexture(renderer, "resources/enemy_king_slime_sketch.png");
          texture_pool[ResID::Tex_SkeletonSketch] = IMG_LoadTexture(renderer, "resources/enemy_skeleton_sketch.png");
          texture_pool[ResID::Tex_GoblinSketch] = IMG_LoadTexture(renderer, "resources/enemy_goblin_sketch.png");
          texture_pool[ResID::Tex_GoblinPriestSketch] = IMG_LoadTexture(renderer, "resources/enemy_goblin_priest_sketch.png");

          // 加载子弹纹理
          texture_pool[ResID::Tex_BulletArrow] = IMG_LoadTexture(renderer, "resources/bullet_arrow.png");
          texture_pool[ResID::Tex_BulletAxe] = IMG_LoadTexture(renderer, "resources/bullet_axe.png");
          texture_pool[ResID::Tex_BulletShell] = IMG_LoadTexture(renderer, "resources/bullet_shell.png");

          // 加载游戏道具纹理
          texture_pool[ResID::Tex_Coin] = IMG_LoadTexture(renderer, "resources/coin.png");
          texture_pool[ResID::Tex_Home] = IMG_LoadTexture(renderer, "resources/home.png");

          // 加载特效纹理
          texture_pool[ResID::Tex_EffectFlash_Up] = IMG_LoadTexture(renderer, "resources/effect_flash_up.png");
          texture_pool[ResID::Tex_EffectFlash_Down] = IMG_LoadTexture(renderer, "resources/effect_flash_down.png");
          texture_pool[ResID::Tex_EffectFlash_Left] = IMG_LoadTexture(renderer, "resources/effect_flash_left.png");
          texture_pool[ResID::Tex_EffectFlash_Right] = IMG_LoadTexture(renderer, "resources/effect_flash_right.png");
          texture_pool[ResID::Tex_EffectImpact_Up] = IMG_LoadTexture(renderer, "resources/effect_impact_up.png");
          texture_pool[ResID::Tex_EffectImpact_Down] = IMG_LoadTexture(renderer, "resources/effect_impact_down.png");
          texture_pool[ResID::Tex_EffectImpact_Left] = IMG_LoadTexture(renderer, "resources/effect_impact_left.png");
          texture_pool[ResID::Tex_EffectImpact_Right] = IMG_LoadTexture(renderer, "resources/effect_impact_right.png");
          texture_pool[ResID::Tex_EffectExplode] = IMG_LoadTexture(renderer, "resources/effect_explode.png");

          // 加载UI纹理
          texture_pool[ResID::Tex_UISelectCursor] = IMG_LoadTexture(renderer, "resources/ui_select_cursor.png");
          texture_pool[ResID::Tex_UIPlaceIdle] = IMG_LoadTexture(renderer, "resources/ui_place_idle.png");
          texture_pool[ResID::Tex_UIPlaceHoveredTop] = IMG_LoadTexture(renderer, "resources/ui_place_hovered_top.png");
          texture_pool[ResID::Tex_UIPlaceHoveredLeft] = IMG_LoadTexture(renderer, "resources/ui_place_hovered_left.png");
          texture_pool[ResID::Tex_UIPlaceHoveredRight] = IMG_LoadTexture(renderer, "resources/ui_place_hovered_right.png");
          texture_pool[ResID::Tex_UIUpgradeIdle] = IMG_LoadTexture(renderer, "resources/ui_upgrade_idle.png");
          texture_pool[ResID::Tex_UIUpgradeHoveredTop] = IMG_LoadTexture(renderer, "resources/ui_upgrade_hovered_top.png");
          texture_pool[ResID::Tex_UIUpgradeHoveredLeft] = IMG_LoadTexture(renderer, "resources/ui_upgrade_hovered_left.png");
          texture_pool[ResID::Tex_UIUpgradeHoveredRight] = IMG_LoadTexture(renderer, "resources/ui_upgrade_hovered_right.png");
          texture_pool[ResID::Tex_UIHomeAvatar] = IMG_LoadTexture(renderer, "resources/ui_home_avatar.png");
          texture_pool[ResID::Tex_UIPlayerAvatar] = IMG_LoadTexture(renderer, "resources/ui_player_avatar.png");
          texture_pool[ResID::Tex_UIHeart] = IMG_LoadTexture(renderer, "resources/ui_heart.png");
          texture_pool[ResID::Tex_UICoin] = IMG_LoadTexture(renderer, "resources/ui_coin.png");
          texture_pool[ResID::Tex_UIGameOverBar] = IMG_LoadTexture(renderer, "resources/ui_game_over_bar.png");
          texture_pool[ResID::Tex_UIWinText] = IMG_LoadTexture(renderer, "resources/ui_win_text.png");
          texture_pool[ResID::Tex_UILossText] = IMG_LoadTexture(renderer, "resources/ui_loss_text.png");

          // 检查纹理加载是否成功
          for (const auto &pair : texture_pool)
               if (!pair.second)
                    return false;

          // 加载音效
          sound_pool[ResID::Sound_ArrowFire_1] = Mix_LoadWAV("resources/sound_arrow_fire_1.mp3");
          sound_pool[ResID::Sound_ArrowFire_2] = Mix_LoadWAV("resources/sound_arrow_fire_2.mp3");
          sound_pool[ResID::Sound_AxeFire] = Mix_LoadWAV("resources/sound_axe_fire.wav");
          sound_pool[ResID::Sound_ShellFire] = Mix_LoadWAV("resources/sound_shell_fire.wav");
          sound_pool[ResID::Sound_ArrowHit_1] = Mix_LoadWAV("resources/sound_arrow_hit_1.mp3");
          sound_pool[ResID::Sound_ArrowHit_2] = Mix_LoadWAV("resources/sound_arrow_hit_2.mp3");
          sound_pool[ResID::Sound_ArrowHit_3] = Mix_LoadWAV("resources/sound_arrow_hit_3.mp3");
          sound_pool[ResID::Sound_AxeHit_1] = Mix_LoadWAV("resources/sound_axe_hit_1.mp3");
          sound_pool[ResID::Sound_AxeHit_2] = Mix_LoadWAV("resources/sound_axe_hit_2.mp3");
          sound_pool[ResID::Sound_AxeHit_3] = Mix_LoadWAV("resources/sound_axe_hit_3.mp3");
          sound_pool[ResID::Sound_ShellHit] = Mix_LoadWAV("resources/sound_shell_hit.mp3");

          // 加载特效音效
          sound_pool[ResID::Sound_Flash] = Mix_LoadWAV("resources/sound_flash.wav");
          sound_pool[ResID::Sound_Impact] = Mix_LoadWAV("resources/sound_impact.wav");

          // 加载游戏音效
          sound_pool[ResID::Sound_Coin] = Mix_LoadWAV("resources/sound_coin.mp3");
          sound_pool[ResID::Sound_HomeHurt] = Mix_LoadWAV("resources/sound_home_hurt.wav");
          sound_pool[ResID::Sound_PlaceTower] = Mix_LoadWAV("resources/sound_place_tower.mp3");
          sound_pool[ResID::Sound_TowerLevelUp] = Mix_LoadWAV("resources/sound_tower_level_up.mp3");

          // 加载游戏状态音效
          sound_pool[ResID::Sound_Win] = Mix_LoadWAV("resources/sound_win.wav");
          sound_pool[ResID::Sound_Loss] = Mix_LoadWAV("resources/sound_loss.mp3");

          // 检查音效加载是否成功
          for (const auto &pair : sound_pool)
               if (!pair.second)
                    return false;

          // 加载背景音乐
          music_pool[ResID::Music_BGM] = Mix_LoadMUS("resources/music_bgm.mp3");

          // 检查音乐加载是否成功
          for (const auto &pair : music_pool)
               if (!pair.second)
                    return false;

          // 加载字体
          font_pool[ResID::Font_Main] = TTF_OpenFont("resources/ipix.ttf", 25);

          // 检查字体加载是否成功
          for (const auto &pair : font_pool)
               if (!pair.second)
                    return false;

          return true;
     }

     const FontPool &get_font_pool()
     {
          return font_pool;
     }

     const SoundPool &get_sound_pool()
     {
          return sound_pool;
     }

     const MusicPool &get_music_pool()
     {
          return music_pool;
     }

     const TexturePool &get_texture_pool()
     {
          return texture_pool;
     }

protected:
     ResourcesManager() = default;
     ~ResourcesManager() = default;

private:
     FontPool font_pool;
     SoundPool sound_pool;
     MusicPool music_pool;
     TexturePool texture_pool;
};

#endif // !_RESOURCES_MANAGER_H_
