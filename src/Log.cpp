#include <cmath>
#include <SFML/Graphics.hpp>

#include "Frog.hpp"
#include "Global.hpp"
#include "Log.hpp"

// Constructor de la clase Log
Log::Log(unsigned char i_type, unsigned char i_x, unsigned char i_y) :
    direction(i_y % 2),
    x(CELL_SIZE * i_x),
    animation_timer(0),
    current_frame(0),
    size(i_type),
    type(i_y),
    y(static_cast<unsigned short>(CELL_SIZE * floor(4 + i_y + 0.5f * MAP_HEIGHT)))
{
    // Define la velocidad según el tipo de fila
    if (0 == i_y)
    {
        speed = SLOW_SPEED;
    }
    else if (1 == i_y)
    {
        speed = NORMAL_SPEED;
    }
    else
    {
        speed = FAST_SPEED;
    }
    
    // Ajusta la posición inicial según la dirección
    if (0 == direction)
    {
        if (x >= CELL_SIZE * MAP_WIDTH)
        {
            x = CELL_SIZE * (-static_cast<char>(size));
        }
    }
    else
    {
        if (x < CELL_SIZE * (-static_cast<char>(size)))
        {
            x = CELL_SIZE * MAP_WIDTH;
        }
    }
    
    texture.loadFromFile("assets/Log.png");
    sprite.setTexture(texture);
}

bool Log::check_frog(const Frog& i_frog) const
{
    // Verifica si la rana está a la misma altura que este tronco
    if (y == i_frog.get_y())
    {
        // Verifica si el rectángulo de la rana intersecta con el tronco
        return i_frog.get_rect().intersects(sf::IntRect(static_cast<short>(x + floor(0.25f * CELL_SIZE)), y, static_cast<short>(CELL_SIZE * size - floor(0.5f * CELL_SIZE)), CELL_SIZE));
    }
    
    return 0;
}

void Log::draw(sf::RenderWindow& i_window)
{
    // Dibuja el sprite del tronco
    sprite.setPosition(x, y);
    
    // Dibuja cada segmento del tronco
    for (unsigned char a = 0; a < size; a++)
    {
        if (0 == a)
        {
            // Extremo izquierdo/derecho según la dirección
            sprite.setTextureRect(sf::IntRect(CELL_SIZE * (2 * direction), CELL_SIZE * type, CELL_SIZE, CELL_SIZE));
        }
        else if (a == size - 1)
        {
            // Extremo derecho/izquierdo según la dirección
            sprite.setTextureRect(sf::IntRect(CELL_SIZE * (1 + 2 * direction), CELL_SIZE * type, CELL_SIZE, CELL_SIZE));
        }
        else
        {
            // Segmento del medio
            sprite.setTextureRect(sf::IntRect(CELL_SIZE * 2, CELL_SIZE * type, CELL_SIZE, CELL_SIZE));
        }
        
        sprite.setPosition(x + CELL_SIZE * a, y);
        i_window.draw(sprite);
    }
}

void Log::update(bool i_move_frog, Frog& i_frog)
{
    // Mueve el tronco según su dirección
    if (0 == direction)
    {
        x += speed;
        
        // Si se salió del borde derecho, reaparece por la izquierda
        if (x >= CELL_SIZE * MAP_WIDTH)
        {
            x = CELL_SIZE * (-static_cast<char>(size));
        }
    }
    else
    {
        x -= speed;
        
        // Si se salió del borde izquierdo, reaparece por la derecha
        if (x < CELL_SIZE * (-static_cast<char>(size)))
        {
            x = CELL_SIZE * MAP_WIDTH;
        }
    }
    
    // Si la rana está sobre este tronco, muévela con él
    if (1 == i_move_frog && 1 == check_frog(i_frog))
    {
        if (0 == direction)
        {
            i_frog.move(speed);
        }
        else
        {
            i_frog.move(-speed);
        }
    }
}
