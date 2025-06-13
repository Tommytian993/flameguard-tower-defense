#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "game_map/tile.h"

#include <SDL.h>
#include <vector>

// 路径类：表示从生成点到终点的路径
class Route
{
public:
     // 路径点列表类型定义
     typedef std::vector<SDL_Point> IdxList;

public:
     Route() = default;

     // 构造函数：根据地图和起始点生成路径
     // @param map: 地图数据
     // @param idx_origin: 路径的起始点坐标
     Route(const TileMap &map, const SDL_Point &idx_origin)
     {
          size_t width_map = map[0].size();
          size_t height_map = map.size();
          SDL_Point idx_next = idx_origin;

          // 沿着地图上的方向标记生成路径
          while (true)
          {
               // 检查是否超出地图边界
               if (idx_next.x >= width_map || idx_next.y >= height_map)
                    break;

               // 检查是否出现重复路径点
               if (check_duplicate_idx(idx_next))
                    break;
               else
                    idx_list.push_back(idx_next);

               bool is_next_dir_exist = true;
               const Tile &tile = map[idx_next.y][idx_next.x];

               // 到达终点（特殊标记为0的点）
               if (tile.special_flag == 0)
                    break;

               // 根据瓦片的方向确定下一个路径点
               switch (tile.direction)
               {
               case Tile::Direction::Up:
                    idx_next.y--;
                    break;
               case Tile::Direction::Down:
                    idx_next.y++;
                    break;
               case Tile::Direction::Left:
                    idx_next.x--;
                    break;
               case Tile::Direction::Right:
                    idx_next.x++;
                    break;
               default:
                    is_next_dir_exist = false;
                    break;
               }

               if (!is_next_dir_exist)
                    break;
          }
     }

     ~Route() = default;

     // 获取路径点列表
     const IdxList &get_idx_list() const
     {
          return idx_list;
     }

private:
     IdxList idx_list; // 存储路径上的所有点

private:
     // 检查路径点是否重复
     // @param target_idx: 要检查的路径点
     // @return: 如果路径点已存在返回true，否则返回false
     bool check_duplicate_idx(const SDL_Point &target_idx)
     {
          for (const SDL_Point &idx : idx_list)
          {
               if (idx.x == target_idx.x && idx.y == target_idx.y)
                    return true;
          }

          return false;
     }
};

#endif // !_ROUTE_H_