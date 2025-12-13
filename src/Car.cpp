#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Car.hpp"
#include "Global.hpp"

//Constructor de la clase Car por si se genera el carrito afuera del mapa
Car::Car(unsigned char i_x, unsigned short i_y) : direction(i_y % 2), x(CELL_SIZE * i_x), size(CELL_SIZE * (1 + (2 > i_y))), type(i_y), y(static_cast<unsigned short>(CELL_SIZE * floor(1 + i_y + 0.5f * MAP_HEIGHT)))
{
    //Inicializa un coche con su posición, dirección, tamaño y velocidad
    if (0 == direction)
    {
        if (x >= CELL_SIZE * MAP_WIDTH) //Posición horizontal inicial en celdas
        {
            x -= static_cast<short>(CELL_SIZE * floor(1.5f * MAP_WIDTH));
        }
    }
    else
    {
        if (x <= -CELL_SIZE * floor(0.5f * MAP_WIDTH))
        {
            x += static_cast<short>(CELL_SIZE * floor(1.5f * MAP_WIDTH));
        }
    }

    if (2 > i_y) //Fila del coche (determina tipo, dirección y posición vertical)
    {
        speed = NORMAL_SPEED;
    }
    else if (4 > i_y)
    {
        speed = FAST_SPEED;
    }
    else
    {
        speed = SLOW_SPEED;
    }
}

//Dibuja el coche en la ventana de renderizado
void Car::draw(sf::RenderWindow& i_window) 
{
    texture.loadFromFile("assets/Cars.png"); //Carga la textura correspondiente y la dibuja en la posición actual del coche

    sprite.setPosition(x, y); //La orientación del sprite depende de la dirección del movimiento
    sprite.setTexture(texture);

    if (0 == direction)
    {
        sprite.setTextureRect(sf::IntRect(0, type * CELL_SIZE, size, CELL_SIZE));
    }
    else
    {
        sprite.setTextureRect(sf::IntRect(size, type * CELL_SIZE, -size, CELL_SIZE));
    }

    i_window.draw(sprite); //Ventana de SFML donde se dibujará el coche
}

// Actualiza la posición del coche según su velocidad y dirección
void Car::update()
{
    if (0 == direction) // Implementa movimiento circular: cuando el coche sale del mapa por un lado
    {
        x += speed;
        if (x > CELL_SIZE * MAP_WIDTH)
        {
            x = -static_cast<short>(CELL_SIZE * floor(1.5f * MAP_WIDTH));
        }
    }
    else // reaparece por el otro lado
    {
        x -= speed;
        if (x < -CELL_SIZE * floor(0.5f * MAP_WIDTH))
        {
            x += static_cast<short>(CELL_SIZE * floor(1.5f * MAP_WIDTH));
        }
    }
}

// Obtiene el rectángulo delimitador del coche
sf::IntRect Car::get_rect() const // Utilizado para detección de colisiones con otros objetos
{
    return sf::IntRect(x, y, size, CELL_SIZE); // sf::IntRect Rectángulo que representa el área ocupada por el coche
}