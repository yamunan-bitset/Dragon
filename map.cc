#include "map.hh"

Map::Map()
{
  this->tile_sheet.loadFromFile("assets/tilemap.png");
  this->tile.setTexture(this->tile_sheet);
  this->init();
  int map[ROWS][COLS] =
    {
#include "assets/map1.inc"
    };
}

void Map::init()
{
  this->grass.left = 0;
  this->grass.top = 0;
  this->grass.width = this->width;
  this->grass.height = this->height;

  this->path.left = 32;
  this->path.top = 0;
  this->path.width = this->width;
  this->path.height = this->height;

  this->stone.left = 0;
  this->stone.top = 32;
  this->stone.width = this->width;
  this->stone.height = this->height;

  this->box.left = 32;
  this->box.top = 32;
  this->box.width = this->width;
  this->box.height = this->height;
}
