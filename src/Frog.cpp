#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Frog.hpp"
#include "Global.hpp"

// Constructor de la clase Frog
Frog::Frog() : dead(0), x(CELL_SIZE * floor(0.5f * MAP_WIDTH)), animation_timer(0), current_frame(0), direction(0), y(CELL_SIZE * (MAP_HEIGHT - 1)), control_keys({0, 0, 0, 0})
{
    texture.loadFromFile("assets/Frog.png");
    sprite.setTexture(texture);
}

bool Frog::get_dead() const
{
    return dead;
}

bool Frog::update_swamp(std::array<bool, 5>& i_swamp)
{
    // Verifica si la rana llegó a la fila superior del pantano
    if (CELL_SIZE == y)
    {
        // Calcula qué ranura del pantano alcanzó (0-4)
        unsigned char swamp_index = floor((x - CELL_SIZE) / (0.5f * CELL_SIZE * MAP_WIDTH));
        
        // Verifica que el índice esté dentro del rango válido
        if (5 > swamp_index)
        {
            // Si este pantano aún no estaba ocupado
            if (0 == i_swamp[swamp_index])
            {
                i_swamp[swamp_index] = 1;
                
                // Verifica si todos los pantanos están llenos
                bool all_full = 1;
                for (bool swamp : i_swamp)
                {
                    if (0 == swamp)
                    {
                        all_full = 0;
                        break;
                    }
                }
                
                return all_full; // Retorna true si completó todos los pantanos
            }
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
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(CELL_SIZE * current_frame, CELL_SIZE * direction, CELL_SIZE, CELL_SIZE));
    i_window.draw(sprite);
}

void Frog::move(char i_value)
{
    x += i_value;
}

void Frog::reset()
{
    dead = 0;
    x = CELL_SIZE * floor(0.5f * MAP_WIDTH);
    y = CELL_SIZE * (MAP_HEIGHT - 1);
    animation_timer = 0;
    current_frame = 0;
    direction = 0;
    control_keys.fill(0);
}

void Frog::set_dead()
{
    dead = 1;
}

void Frog::update()
{
    // Si la rana está muerta, no procesa nada
    if (1 == dead)
    {
        return;
    }
    
    // Procesa el input del teclado
    if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if (0 == control_keys[0])
        {
            control_keys[0] = 1;
            
            // Mueve hacia arriba si no está en el borde
            if (CELL_SIZE <= y)
            {
                direction = 0;
                y -= CELL_SIZE;
            }
        }
    }
    else
    {
        control_keys[0] = 0;
    }
    
    if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        if (0 == control_keys[1])
        {
            control_keys[1] = 1;
            
            // Mueve hacia abajo si no está en el borde
            if (CELL_SIZE * (MAP_HEIGHT - 1) > y)
            {
                direction = 1;
                y += CELL_SIZE;
            }
        }
    }
    else
    {
        control_keys[1] = 0;
    }
    
    if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (0 == control_keys[2])
        {
            control_keys[2] = 1;
            
            // Mueve hacia la izquierda si no está en el borde
            if (0 < x)
            {
                direction = 2;
                x -= CELL_SIZE;
            }
        }
    }
    else
    {
        control_keys[2] = 0;
    }
    
    if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (0 == control_keys[3])
        {
            control_keys[3] = 1;
            
            // Mueve hacia la derecha si no está en el borde
            if (CELL_SIZE * (MAP_WIDTH - 1) > x)
            {
                direction = 3;
                x += CELL_SIZE;
            }
        }
    }
    else
    {
        control_keys[3] = 0;
    }
    
    // Actualiza la animación
    if (FROG_ANIMATION_SPEED == animation_timer)
    {
        animation_timer = 0;
        current_frame = (current_frame + 1) % 2; // Alterna entre frame 0 y 1
    }
    else
    {
        animation_timer++;
    }
}

sf::IntRect Frog::get_rect() const
{
    return sf::IntRect(x, y, CELL_SIZE, CELL_SIZE);
}
