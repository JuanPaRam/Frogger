#include <array>
#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Frog.hpp"
#include "Global.hpp"

// Sonidos globales para las muertes
static sf::Sound* rushed_sound_ptr = nullptr;
static sf::Sound* splash_sound_ptr = nullptr;

void Frog::set_death_sounds(sf::Sound* rushed, sf::Sound* splash)
{
    rushed_sound_ptr = rushed;
    splash_sound_ptr = splash;
}

// Constructor de la clase Frog
Frog::Frog() 
{
    reset();
}

bool Frog::get_dead() const
{
    return dead;
}

bool Frog::update_swamp(std::array<bool, 5>& i_swamp)
{
	if (CELL_SIZE >= y)
	{
		if (floor(0.75f * CELL_SIZE) <= x % (3 * CELL_SIZE) && floor(2.25f * CELL_SIZE) >= x % (3 * CELL_SIZE))
		{
			unsigned char swamp_spot = static_cast<unsigned char>(floor(x / static_cast<float>(3 * CELL_SIZE)));

			if (0 == i_swamp[swamp_spot])
			{
				i_swamp[swamp_spot] = 1;

				return 1;
			}
			else
			{
				set_dead();
			}
		}
		else
		{
			set_dead();
		}
	}

	return 0;
}

unsigned short Frog::get_y() const
{
    return y;
}

void Frog::draw(sf::RenderWindow& i_window)
{
	if (0 == dead)
	{
		texture.loadFromFile("assets/Frog.png");
		sprite.setPosition(x, y);
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(CELL_SIZE * current_frame, CELL_SIZE * direction, CELL_SIZE, CELL_SIZE));
	}
	else
	{
		texture.loadFromFile("assets/DeathFrogRoad.png");
		sprite.setPosition(x, y);
		sprite.setTexture(texture);
		// Usar direction = 0 para animación de muerte (solo tiene una fila)
		sprite.setTextureRect(sf::IntRect(CELL_SIZE * current_frame, 0, CELL_SIZE, CELL_SIZE));
	}

	i_window.draw(sprite);
}

void Frog::move(char i_value)
{
    x = std::clamp(i_value + x, 0, CELL_SIZE * (MAP_WIDTH - 1));
}

void Frog::reset()
{
    dead = 0;
    x = static_cast<short>(CELL_SIZE * floor(0.5f * MAP_WIDTH));

	animation_timer = 0;
	current_frame = 1;
	direction = 1;

	y = CELL_SIZE * (MAP_HEIGHT - 1);

	control_keys[0] = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	control_keys[1] = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	control_keys[2] = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	control_keys[3] = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
}

void Frog::set_dead(unsigned char death_type)
{
	if (0 == dead)
	{
		dead = 1;
		// death_type: 0 = ahogada, 1 = atropellada
		if (death_type == 1 && rushed_sound_ptr != nullptr)
		{
			rushed_sound_ptr->play();
		}
		else if (death_type == 0 && splash_sound_ptr != nullptr)
		{
			splash_sound_ptr->play();
		}

		animation_timer = 0;
		current_frame = 0;
	}
}

void Frog::update()
{
	if (0 == dead)
	{
		bool moved = 0;

		if (0 == control_keys[0] && 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			moved = 1;

			x = std::min(CELL_SIZE + x, CELL_SIZE * (MAP_WIDTH - 1));

			direction = 0;
		}
		else if (0 == control_keys[1] && 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			moved = 1;

			y = std::max(y - CELL_SIZE, 0);

			direction = 1;
		}
		else if (0 == control_keys[2] && 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			moved = 1;

			x = std::max(x - CELL_SIZE, 0);

			direction = 2;
		}
		else if (0 == control_keys[3] && 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			moved = 1;

			y = std::min(CELL_SIZE + y, CELL_SIZE * (MAP_HEIGHT - 1));

			direction = 3;
		}

		control_keys[0] = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
		control_keys[1] = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
		control_keys[2] = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		control_keys[3] = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

		// Animación constante - alterna entre frames 0 y 1
		if (0 == animation_timer)
		{
			animation_timer = FROG_ANIMATION_SPEED;
			current_frame = (current_frame == 0) ? 1 : 0;
		}
		else
		{
			animation_timer--;
		}
	}
	else
	{
		// Animación de muerte - se detiene en el frame 3
		if (y < CELL_SIZE * floor(0.5f * MAP_HEIGHT))
		{
			// Muerte en el agua - anima los 4 frames
			if (current_frame <= 3)
			{
				if (0 == animation_timer)
				{
					animation_timer = FROG_ANIMATION_SPEED;
					if (current_frame < 3)
					{
						current_frame++;
					}
				}
				else
				{
					animation_timer--;
				}
			}
		}
		else
		{
			// Muerte en la carretera - mantiene el frame 0 (aplastado)
			current_frame = 0;
		}
	}
}

sf::IntRect Frog::get_rect() const
{
    return sf::IntRect(x, y, CELL_SIZE, CELL_SIZE);
}
