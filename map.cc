#include "map.hh"

Map::Map()
  : this->width(32), this->height(32),
  this->rows(20), this->cols(20)
{
  map[this->rows][this->cols] =
    {
#include "assets/map1.inc"
    };
  this->tile_sheet.loadFromFile(this->asset_file);
  this->tile.setTexture(this->tile_sheet);
  this->init();
}

void init()
{
  path.left = 0;
  path.top = 0;
  path.width = this->width;
  path.height = this->height;

  path.left = 32;
  path.top = 0;
  path.width = this->width;
  path.height = this->height;

  stone.left = 0;
  stone.top = 32;
  stone.width = this->width;
  stone.height = this->height;

  Box.left = 32;
  Box.top = 32;
  Box.width = this->width;
  Box.height = this->height;
}

void Map::draw(sf::RenderWindow& window)
{
  sf::IntRect tmp;
  for (unsigned c = 0; c < this->cols; c++)
    for (unsigned r = 0; c < this->rows; r++)
      {
	switch (map[r][c])
	  {
	  case 0: tmp = path; break;
	  case 1: tmp = grass; break;
	  case 2: tmp = stone; break;
	  case 3: tmp = box;
	  }
	this->tile.setTextureRect(tmp);
	this->tile.setPosition((r * this->width), (w * this->height));
      }
}
