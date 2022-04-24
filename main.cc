#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "map.hh"

#define RIGHT true
#define LEFT  false
#define XMOVE true
#define YMOVE false
#define POS   true
#define NEG   false

#if 0 // 1
#define __DRAGON__DEBUG__
#endif
#ifdef __DRAGON__DEBUG__
#define DEBUG(x) std::cout << "Reached: " << x << std::endl
#else
#define DEBUG(x) std::cout << ""
#endif

inline void randbool(bool& var)
{
  if (std::rand()%2 == 0) var = true;
  else                    var = false;
}

int main(int argc, char** argv)
{
  std::srand(std::time(NULL));
  // Render Window
  sf::Vector2i res = sf::Vector2i(800, 600);
  sf::RenderWindow window(sf::VideoMode(res.x, res.y), "Dragon");
  sf::FloatRect window_bounds(sf::Vector2f(0.f, 0.f), window.getDefaultView().getSize());

  // BGM
  sf::Music bgm;
  bgm.openFromFile("assets/sfx/bgm.wav");
  bgm.setLoop(true);
  bgm.play();

  // Dragon Sprite
  DEBUG("Dragon Sprite");
  sf::Texture dragon_t;
  dragon_t.loadFromFile("assets/gfx/dragon.png");
  sf::Sprite dragon(dragon_t);

  // Fire
  DEBUG("Fire Texture");
  sf::Texture fire_t;
  fire_t.loadFromFile("assets/gfx/fire.png");
  sf::Sprite fire(fire_t);
  // Fire Sound
  sf::SoundBuffer fire_buffer;
  sf::Sound fire_sfx;
  fire_buffer.loadFromFile("assets/sfx/fire.wav");
  fire_sfx.setBuffer(fire_buffer);
  
  // Zombies
  DEBUG("Zombie Sprite");
  sf::Texture zombie_t;
  zombie_t.loadFromFile("assets/gfx/zombie.png");
  sf::Sprite zombie(zombie_t);
  zombie.setPosition(res.x/2, res.y/2);
  sf::Shader z_shader;
  z_shader.loadFromFile("assets/gfx/zombie.frag", sf::Shader::Fragment);
  float opacity = 1.0f;
  z_shader.setParameter("texture", sf::Shader::CurrentTexture);
  z_shader.setParameter("opacity", opacity);
  sf::Texture blank_zombie_dead;
  blank_zombie_dead.create(32, 32);
  // Second Spawn Zombie
  sf::Sprite zombie1(zombie_t);
  sf::Sprite zombie2(zombie_t);
  // Zombie Sound
  sf::SoundBuffer zombie_buffer;
  sf::Sound zombie_sfx;
  zombie_buffer.loadFromFile("assets/sfx/zombie.wav");
  zombie_sfx.setBuffer(zombie_buffer);
  zombie_sfx.play();
  
  // Map
  DEBUG("Map Setup");
  Map map;
  sf::IntRect tmp;
  
  // Runtime Variables
  bool blow = false, dir, dir_z, move_z, hit = false, hit2_dead = false, second_spawn = false;
  randbool(dir_z);
  randbool(move_z);
  sf::Event event;
  sf::Clock f_clock, z_clock;
  int new_v = (std::rand()%40), vel = 0;
  
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
	      case sf::Keyboard::Left:
		dir = LEFT;
		dragon.setScale(-1.0f, 1.0f);
		dragon.move(-32, 0);
		break;
	      case sf::Keyboard::Right:
		dir = RIGHT;
		dragon.setScale(1.0f, 1.0f);
		dragon.move(32, 0);
		break;
	      case sf::Keyboard::Up:
		dragon.move(0, -32);
		break;
	      case sf::Keyboard::Down:
		dragon.move(0, 32);
		break;
	      case sf::Keyboard::Space:
		blow = true;
		break;
	      default:
		break;
	      } break;
	  case sf::Event::Closed: window.close();
	  default: break;
	  }
      // Render to Window
      window.clear(sf::Color::Black);
      // Render Map
      DEBUG("Map Render");
      for (unsigned c = 0; c < COLS ; c++)
	for (unsigned r = 0; r < ROWS ; r++)
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
      // Calculate Fire Position
      DEBUG("Fire Render");
      if (blow)
	{
	  switch (dir)
	    {
	    case RIGHT:
	      fire.setScale(1.0f, 1.0f);
	      fire.setPosition(dragon.getPosition().x+64, dragon.getPosition().y+20);
	      break;
	    case LEFT: 
	      fire.setScale(-1.0f, 1.0f);
	      fire.setPosition(dragon.getPosition().x-64, dragon.getPosition().y+20);
	      break;
	    }
	  // Collision Detection
	  DEBUG("Collision Detection");
	  if (fire.getGlobalBounds().intersects(zombie.getGlobalBounds()))
	    {
	      zombie_sfx.play();
	      DEBUG("Collision!!! Zombie State: Paralysed!!!");
	      if (hit) { hit2_dead = true; std::cout << "Dead!!" << std::endl; }
	      else  hit = true;
	    }
	  // Play Fire Sound
	  fire_sfx.play();
	  // Render Fire
	  window.draw(fire);
	  // Only blow for 1 second
	  if (f_clock.getElapsedTime().asSeconds() > 1.0) { blow = false; f_clock.restart(); }
	}
      if (second_spawn)
	{
	  zombie1.setPosition(zombie.getPosition().x+32, zombie.getPosition().y);
	  zombie2.setPosition(zombie.getPosition().x, zombie.getPosition().y+32);
	  zombie_sfx.play();
	  window.draw(zombie1);
	  window.draw(zombie2);
	}
      else
	{
	  // Render Zombie
	  DEBUG("Zombie Render");
	  // Update Zombie Opacity Shader
	  z_shader.setParameter("opacity", opacity);
	  if (z_clock.getElapsedTime().asSeconds() > 0.01)
	    {
	      opacity -= 0.5;
	      if (opacity < 0.0) opacity = 1.0;
	      z_clock.restart();
	      z_shader.setParameter("opacity", opacity);
	    }
	  vel = 0;
	  switch (dir_z)
	    {
	    case NEG: vel = -1; break;
	    case POS: vel =  1; break;
	    default: break;
	    }
	  switch (move_z)
	    {
	    case XMOVE:
	      zombie.setPosition(zombie.getPosition().x+vel, zombie.getPosition().y);
	      break;
	    case YMOVE:
	      zombie.setPosition(zombie.getPosition().x, zombie.getPosition().y+vel);
	      break;
	    default: break;
	    }
	  new_v--;
	  // Recalculate Velocity (if new_v == 0 or if zombie hits side)
	  if (new_v == 0 ||
	      zombie.getPosition().x <= 0 || zombie.getPosition().x >= res.x ||
	      zombie.getPosition().y <= 0 || zombie.getPosition().y >= res.y)
	    {
	      DEBUG("Recalculate Velocity");
	      new_v = (std::rand()%40);
	      randbool(dir_z);
	      randbool(move_z);
	    }
	  if (hit)
	    if (hit2_dead)
	      {
		second_spawn = true;
		zombie.setTexture(blank_zombie_dead);
	      }
	    else window.draw(zombie, &z_shader);
	  else window.draw(zombie);
	}
      // Render Window
      window.display();
    }
  return 0;
}
