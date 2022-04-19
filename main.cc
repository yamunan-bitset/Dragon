#include <SFML/Graphics.hpp>
#include <iostream>

#include "map.hh"

#define RIGHT 1
#define LEFT  0

#if 1 // 0
#define __DRAGON__DEBUG__
#ifdef __DRAGON__DEBUG__
#define DEBUG(x) std::cout << "Reached: " << x << std::endl
#else
#define DEBUG(x) std::cout << ""
#endif
#endif

int main(int argc, char** argv)
{
  // Render Window
  sf::Vector2i res = sf::Vector2i(800, 800);
  sf::RenderWindow window(sf::VideoMode(res.x, res.y), "Dragon");

  // Dragon Sprite
  DEBUG("Dragon Sprite");
  sf::Texture dragon_t;
  dragon_t.loadFromFile("assets/dragon.png");
  sf::Sprite dragon(dragon_t);

  // Fire
  DEBUG("Fire Texture");
  sf::Texture fire_t;
  fire_t.loadFromFile("assets/fire.png");
  sf::Sprite fire(fire_t);

  // Zombies
  DEBUG("Zombie Sprite");
  sf::Texture zombie_t;
  zombie_t.loadFromFile("assets/zombie.png");
  sf::Sprite zombie(zombie_t);
  sf::Shader z_shader;
  z_shader.loadFromFile("assets/zombie.frag", sf::Shader::Fragment);
  float opacity = 1.0f;
  z_shader.setParameter("texture", sf::Shader::CurrentTexture);
  z_shader.setParameter("opacity", opacity);

  // Map
  DEBUG("Map Setup");
  Map map;
  sf::IntRect tmp;
  
  // Runtime Variables
  bool blow = false;
  bool dir;
  sf::Event event;
  sf::Clock f_clock, z_clock;

  // Gameloop
  DEBUG("Gameloop");
  while (window.isOpen())
    {
      // Handle Events
      while (window.pollEvent(event))
	switch (event.type)
	  {
	  case sf::Event::KeyPressed:
	    switch (event.key.code)
	      {
	      case sf::Keyboard::Left: dir = LEFT; dragon.setScale(-1.0f, 1.0f); dragon.move(-10, 0); break;
	      case sf::Keyboard::Right: dir = RIGHT; dragon.setScale(1.0f, 1.0f); dragon.move(10, 0); break;
	      case sf::Keyboard::Up: dragon.move(0, -10); break;
	      case sf::Keyboard::Down: dragon.move(0, 10); break;
	      case sf::Keyboard::Space: blow = true;
	      default: break;
	      } break;
	  case sf::Event::Closed: window.close();
	  default: break;
	  }
      // Update Zombie Opacity Shader
      z_shader.setParameter("opacity", opacity);
      // Render to Window
      window.clear(sf::Color::Black);
      // Render Map
      DEBUG("Map Render");
      for (unsigned r = 0; r < ROWS ; r++)
	for (unsigned c = 0; c < COLS ; c++)
	  {
	    switch (map.map[r][c])
	      {
	      case 0: tmp = map.path; break;
	      case 1: tmp = map.grass; break;
	      case 2: tmp = map.stone; break;
	      case 3: tmp = map.box; break;
	      }
	    map.tile.setTextureRect(tmp);
	    map.tile.setPosition((r * map.width), (c * map.height));
	    window.draw(map.tile);
	  }
      // Render Dragon
      DEBUG("Dragon Render");
      window.draw(dragon);
      // Render Fire
      DEBUG("Fire Render");
      if (blow)
	{
	  if (dir == RIGHT)
	    {
	      fire.setScale(1.0f, 1.0f);
	      fire.setPosition(dragon.getPosition().x+64, dragon.getPosition().y+20);
	    }
	  if (dir == LEFT)
	    {
	      fire.setScale(-1.0f, 1.0f);
	      fire.setPosition(dragon.getPosition().x-64, dragon.getPosition().y+20);
	    }
	  window.draw(fire);
	  if (f_clock.getElapsedTime().asSeconds() > (float) 1) { blow = false; f_clock.restart(); }
	}
      DEBUG("Zombie Render");
      // Render Zombie
      if (z_clock.getElapsedTime().asSeconds() > 0.001)
	{
	  opacity -= 0.5;
	  if (opacity < 0.0) opacity = 1.0;
	  z_clock.restart();
	  z_shader.setParameter("opacity", opacity);
	}
      window.draw(zombie, &z_shader);
      // Render Window
      window.display();
    }
  return 0;
}
