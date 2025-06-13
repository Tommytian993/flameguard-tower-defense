#ifndef _TILE_H_
#define _TILE_H_

#include <vector>
#include <SDL.h>

// Forward declarations
class ConfigManager;

#define SIZE_TILE 48

struct Tile
{
	enum class Direction
	{
		None = 0,
		Up,
		Down,
		Left,
		Right
	};

	int terrian = 0;
	int decoration = -1;
	int special_flag = -1;
	bool has_tower = false; // during running the game, check if the tile already has a tower
	Direction direction = Direction::None;

	// 根据瓦片索引获取瓦片中心位置
	static SDL_Point get_pos_by_idx(const SDL_Point &idx, const SDL_Rect &rect_tile_map)
	{
		SDL_Point pos;
		pos.x = rect_tile_map.x + idx.x * SIZE_TILE + SIZE_TILE / 2;
		pos.y = rect_tile_map.y + idx.y * SIZE_TILE + SIZE_TILE / 2;
		return pos;
	}
};

typedef std::vector<std::vector<Tile>> TileMap;

#endif // !_TILE_H_
