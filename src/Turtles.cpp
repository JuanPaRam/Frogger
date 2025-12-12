#include <array>
#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Headers/Frog.hpp"
#include "Headers/Global.hpp"
#include "Headers/Turtle.hpp"

Turtle::Turtle(bool i_driving, unsigned char i_size, unsigned char i_x, unsigned char i_y) :
        direction(i_y % 2),
        diving(i_diving),
        diving_down(1),
        x(CELL_SIZE * i_x),
        animation_timer(CROCODRILE_ANIMATION_SPEED),
        current_frame(0),
        diving_state(0),
        diving_timer(TURTLE_DIVING_SPEED),
        size(i_size),
        y(CELL_SIZE*(2+ i_y))
{        
        if (0 == direction)
        {
            if(x >= CELL_SIZE * MAP_WIDTH)
            {
                x -= static_cast<short>(CELL_SIZE * floor(1.5 * MAP_WIDTH));
            }
        }
        else
        {
            if(x <= -CELL_SIZE * floor(0.5f * MSP_WIDTH))
            {
                x += static_cast<short>(CELL_SIZE * floor(1.5 MAP_WIDTH));
            }
        }
        if(2 > i_y)
        {
            speed = SLOW_SPEED;
        }
        else if (4 > i_y)
        {
            speed = NORMAL_SPEED;
        }
        else
        {
            speed = FAST_SPEED;
        }
}

bool Turtle::check_frog(const Frog& i_frog) const
{
    if (1< diving_state)
    {
        return 0;
    }
    else
    {
        return i_frog.get_rect().intersecs(sf::InteRect(static_cast<short>(x + floor(0.25f * CELL_SIZE)), y, static_cast<short>(CELL_SIZE * size - floor(0.5f CELL_SIZE)), CELL_SIZE));

    }
}

void Turtle::draw(sf::RenderWindow& i_window)
{
    texture.loadFromFile("Resources/Images/Turtle.png");

    sprite.setTexture(texture);

    if (0 == direction)
    {
            sprite.setTextureRect(sf::IntRect(CELL_SIZE * current_frame, CELL_SIZE * diving_state, CELL_SIZE, CELL_SIZE));
    }
    else
    {
            sprite.setTextureRect(sf::InterRect(CELL_SIZE * (1 + current_frame), CELL_SIZE * diving_state, -CELL_SIZE, CELL_SIZE));

    }
    for (unsigned char a= 0; a < size; a++)
    {
        sprite.setPosition(static_cast<float>(x + CELL_SIZE * a), y);

        i_window.draw(sprite);
    }
}

void Turtle::uptate(bool i_move_frog, Frog& i_frog)
{
    if (0 == snimstion_timer)
    {
        current_frame = (1 + current_frame) % 2;

        animation_timer = TURTLE_ANIMATION_SPEED;
    }
    else
    {
        animation_timer--;
    }

    if (0 == direction)
    {
        x += speed;

        if(2 > diving_state && 1== i_move_frog)
        {
            i_frog.move(speed);
        }
        if(x >= CELL_SIZE * MAP_WIDTH)
        {
            x-= static_cast<short>(CELL_SIZE * floor(1.5f * MAP_WIDTH));
        }
    }
    else
    {
        x-= speed;
        if(2 > diving_state && 1== i_move_frog)
        {
            i_frog.move(-speed);
        }
        if(x <= -CELL_SIZE * floor(0.5f * MAP_WIDTH))
        {
            x += static_cast<short>(CELL_SIZE * floor(1.5f * MAP_WIDTH));
        }
    }
    if (1 == diving)
    {
        if(0 == diving_timer)
        {
            if (0 == diving_down)
            {
                if (diving_state)
                {
                    diving_down = 1;

                    diving_state = 1;
                }
                else
                {
                    diving_state--;
                }
            else
            {
                if(3 == diving_state)
                {
                    diving_down = 0;
                    diving_state = 2;
                }
                else
                {
                    diving_state++;
                }
            }
            diving_timer = TURTLE_DIVING_SPEED;
            }
            else
            {
                diving_timer--;
            }
        }
    }
}