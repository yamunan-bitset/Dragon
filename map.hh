#include <SFML/Graphics.hpp>

class Map
{
public:
  Map(int, int);
  void draw(sf::RenderWindow&);
  const char* asset_file = "tile_map.png"; // Set default, can be changed.
private:
  void init();
  unsigned rows, cols;
  int map[rows][cols];
  const int width;
  const int height;
  sf::Texture tile_sheet;
  sf::Sprite tile;
  sf::IntRect path, grass, stone, box;
};
