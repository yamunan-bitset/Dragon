#include <SFML/Graphics.hpp>

#define ROWS 20
#define COLS 20

class Map
{
public:
  Map();
  int width = 32, height = 32;
  int map[ROWS][COLS];
  sf::Sprite tile;
  sf::IntRect path, grass, stone, box;
private:
  sf::Texture tile_sheet;
  void init();
};
