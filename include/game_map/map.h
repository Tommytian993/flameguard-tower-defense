#ifndef _MAP_H_
#define _MAP_H_

#include "game_map/tile.h"
#include "game_map/route.h"

#include <SDL.h>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iostream>

// 地图类：管理游戏地图的加载和访问
class Map
{
public:
     // 生成点路径池类型定义：键为生成点ID，值为对应的路径
     typedef std::unordered_map<int, Route> SpawnerRoutePool;

public:
     Map() = default;
     ~Map() = default;

     // 从文件加载地图
     // @param path: 地图文件路径
     // @return: 加载成功返回true，否则返回false
     bool load(const std::string &path)
     {
          std::ifstream file(path);
          if (!file.good())
          {
               std::cerr << "Failed to open map file: " << path << std::endl;
               return false;
          }

          TileMap tile_map_temp;

          int idx_x = -1, idx_y = -1;

          // 逐行读取地图文件
          std::string str_line;
          while (std::getline(file, str_line))
          {
               str_line = trim_str(str_line);
               if (str_line.empty())
                    continue;

               idx_x = -1, idx_y++;
               tile_map_temp.emplace_back();

               // 解析每行的瓦片数据
               std::string str_tile;
               std::stringstream str_stream(str_line);
               while (std::getline(str_stream, str_tile, ','))
               {
                    idx_x++;
                    tile_map_temp[idx_y].emplace_back();
                    Tile &tile = tile_map_temp[idx_y].back();
                    load_tile_from_string(tile, str_tile);
                    std::cout << "Loaded tile at (" << idx_x << "," << idx_y << "): " << str_tile << std::endl;
               }
          }

          file.close();

          if (tile_map_temp.empty() || tile_map_temp[0].empty())
          {
               std::cerr << "Map file is empty or invalid." << std::endl;
               return false;
          }

          tile_map = tile_map_temp;

          // 生成地图缓存（终点和路径）
          generate_map_cache();

          return true;
     }

     // 获取地图宽度
     size_t get_width() const
     {
          if (tile_map.empty())
               return 0;

          return tile_map[0].size();
     }

     // 获取地图高度
     size_t get_height() const
     {
          return tile_map.size();
     }

     // 获取瓦片地图数据
     const TileMap &get_tile_map() const
     {
          return tile_map;
     }

     // 获取终点坐标
     const SDL_Point &get_idx_home() const
     {
          return idx_home;
     }

     // 获取生成点路径池
     const SpawnerRoutePool &get_idx_spawner_pool() const
     {
          return spwaner_route_pool;
     }

     // 在指定位置放置防御塔
     // @param idx_tile: 防御塔放置的瓦片坐标
     void place_tower(const SDL_Point &idx_tile)
     {
          tile_map[idx_tile.y][idx_tile.x].has_tower = true;
     }

private:
     TileMap tile_map;                    // 瓦片地图数据
     SDL_Point idx_home = {0};            // 终点坐标
     SpawnerRoutePool spwaner_route_pool; // 生成点路径池

private:
     // 去除字符串首尾的空白字符
     // @param str: 输入字符串
     // @return: 处理后的字符串
     std::string trim_str(const std::string &str)
     {
          size_t begin_idx = str.find_first_not_of(" \t");
          if (begin_idx == std::string::npos)
               return "";
          size_t end_idx = str.find_last_not_of(" \t");
          size_t idx_range = end_idx - begin_idx + 1;

          return str.substr(begin_idx, idx_range);
     }

     // 从字符串加载瓦片数据
     // @param tile: 目标瓦片
     // @param str: 瓦片数据字符串，格式：地形\装饰\方向\特殊标记
     void load_tile_from_string(Tile &tile, const std::string &str)
     {
          std::string str_tidy = trim_str(str);

          std::string str_value;
          std::vector<int> values;
          std::stringstream str_stream(str_tidy);

          // 解析瓦片属性
          while (std::getline(str_stream, str_value, '\\'))
          {
               int value;
               try
               {
                    value = std::stoi(str_value);
               }
               catch (const std::invalid_argument &)
               {
                    value = -1;
               }
               values.push_back(value);
          }

          // 设置瓦片属性
          tile.terrian = (values.size() < 1 || values[0] < 0) ? 0 : values[0];
          tile.decoration = (values.size() < 2) ? -1 : values[1];
          tile.direction = (Tile::Direction)((values.size() < 3 || values[2] < 0) ? 0 : values[2]);
          tile.special_flag = (values.size() <= 3) ? -1 : values[3];
     }

     // 生成地图缓存：记录终点位置和生成点路径
     void generate_map_cache()
     {
          for (int y = 0; y < get_height(); y++)
          {
               for (int x = 0; x < get_width(); x++)
               {
                    const Tile &tile = tile_map[y][x];
                    if (tile.special_flag < 0)
                         continue;

                    // 记录终点位置
                    if (tile.special_flag == 0)
                    {
                         idx_home.x = x;
                         idx_home.y = y;
                    }
                    // 生成路径并加入路径池
                    else
                    {
                         spwaner_route_pool[tile.special_flag] = Route(tile_map, {x, y});
                    }
               }
          }
     }
};

#endif // !_MAP_H_
