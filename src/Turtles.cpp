#include <array> // Librería para usar std::array
#include <chrono> // Librería para manejo de tiempo
#include <cmath> // Librería para funciones matemáticas como floor
#include <SFML/Graphics.hpp> // Librería SFML para gráficos

#include "include/Frog.hpp" // Incluye la clase Frog
#include "include/Global.hpp" // Incluye constantes globales del juego
#include "include/Turtle.hpp" // Incluye la definición de la clase Turtle


// Constructor de la clase Turtle inicializando una tortuga con su posición, tamaño, dirección y comportamiento de inmersión
Turtle::Turtle(bool i_diving, unsigned char i_size, unsigned char i_x, unsigned char i_y) :
        direction(i_y % 2), // Determina dirección: 0 = derecha, 1 = izquierda (según si i_y es par o impar)
        diving(i_diving), // Indica si esta tortuga se sumerge (true) o no (false)
        diving_down(1), // Estado inicial: la tortuga comenzará sumergiéndose
        x(CELL_SIZE * i_x), // Posición horizontal inicial en píxeles
        animation_timer(CROCODILE_ANIMATION_SPEED), // Temporizador para animar el sprite
        current_frame(0), // Frame actual de la animación (0 o 1)
        diving_state(0), // Estado de inmersión: 0 = superficie, 1-3 = sumergiéndose
        diving_timer(TURTLE_DIVING_SPEED), // Temporizador para controlar la velocidad de inmersión
        size(i_size), // Número de tortugas en este grupo (1-4)
        y(CELL_SIZE*(2+ i_y)) // Posición vertical en píxeles (fila del río)
{        
        if (0 == direction) // Que hacer si la tortuga se mueve hacia la derecha
        {
            if(x >= CELL_SIZE * MAP_WIDTH) // Que hacer si la posición inicial está fuera del mapa por la derecha
            {
                x -= static_cast<short>(CELL_SIZE * floor(1.5 * MAP_WIDTH)); // La reposiciona dentro del mapa
            }
        }
        else // Que hacer si la tortuga se mueve hacia la izquierda
        {
            if(x <= -CELL_SIZE * floor(0.5f * MAP_WIDTH)) // Que hacer si la posición inicial está fuera del mapa por la izquierda
            {
                x += static_cast<short>(CELL_SIZE * floor(1.5f * MAP_WIDTH)); // La reposiciona dentro del mapa
            }
        }
        if(2 > i_y) // Que hacer si está en las filas 0 o 1 del río
        {
            speed = SLOW_SPEED; // Asigna velocidad lenta
        }
        else if (4 > i_y) // Que hacer si está en las filas 2 o 3 del río
        {
            speed = NORMAL_SPEED; // Asigna velocidad normal
        }
        else // Que hacer si está en las filas 4 o superiores
        {
            speed = FAST_SPEED; // Asigna velocidad rápida
        }
}

bool Turtle::check_frog(const Frog& i_frog) const // Verifica si la rana está sobre esta tortuga
{
    if (1 < diving_state) // Si la tortuga está sumergida (estado 2 o 3)
    {
        return 0; // La rana no puede pararse sobre una tortuga sumergida
    }
    else // Si la tortuga está en la superficie o comenzando a sumergirse
    {
        /* Verifica si el rectángulo de la rana intersecta con el área de las tortugas
        se ajusta el área con márgenes (0.25 y 0.5 del tamaño de celda) */
        return i_frog.get_rect().intersects(sf::IntRect(static_cast<short>(x + floor(0.25f * CELL_SIZE)), y, static_cast<short>(CELL_SIZE * size - floor(0.5f CELL_SIZE)), CELL_SIZE));

    }
}

void Turtle::draw(sf::RenderWindow& i_window) // Dibuja el grupo de tortugas en la ventana
{
    texture.loadFromFile("Resources/Images/Turtle.png"); // Carga la textura de las tortugas desde el archivo

    sprite.setTexture(texture); // Asigna la textura al sprite

    if (0 == direction) // Si se mueve hacia la derecha
    {
            sprite.setTextureRect(sf::IntRect(CELL_SIZE * current_frame, CELL_SIZE * diving_state, CELL_SIZE, CELL_SIZE)); // Selecciona el frame normal
    }
    else // Si se mueve hacia la izquierda
    {
            sprite.setTextureRect(sf::IntRect(CELL_SIZE * (1 + current_frame), CELL_SIZE * diving_state, -CELL_SIZE, CELL_SIZE)); // Invierte el sprite horizontalmente

    }
    for (unsigned char a= 0; a < size; a++) // Itera por cada tortuga del grupo
    {
        sprite.setPosition(static_cast<float>(x + CELL_SIZE * a), y); // Posiciona cada tortuga consecutivamente

        i_window.draw(sprite); // Dibuja la tortuga en la ventana
    }
}

// Actualiza la posición y estado de las tortugas
void Turtle::update(bool i_move_frog, Frog& i_frog) // Maneja la animación, el movimiento y el ciclo de inmersión
{
    if (0 == animation_timer) // Si el temporizador de animación llegó a cero
    {
        current_frame = (1 + current_frame) % 2; // Alterna entre frame 0 y 1

        animation_timer = TURTLE_ANIMATION_SPEED; // Reinicia el temporizador
    }
    else // Si aún no es momento de cambiar frame
    {
        animation_timer--; // Decrementa el temporizador
    }

    if (0 == direction) // Si se mueve hacia la derecha
    {
        x += speed; // Incrementa la posición horizontal

        if(2 > diving_state && 1== i_move_frog) // Si la tortuga está en superficie y debe mover la rana
        {
            i_frog.move(speed); // Mueve la rana junto con la tortuga hacia la derecha
        }
        if(x >= CELL_SIZE * MAP_WIDTH) // Si salió del mapa por la derecha
        {
            x-= static_cast<short>(CELL_SIZE * floor(1.5f * MAP_WIDTH)); // Reaparece por la izquierda
        }
    }
    else // Si se mueve hacia la izquierda
    {
        x-= speed; // Decrementa la posición horizontal
        if(2 > diving_state && 1== i_move_frog) // Si la tortuga está en superficie y debe mover la rana
        {
            i_frog.move(-speed); // Mueve la rana junto con la tortuga hacia la izquierda
        }
        if(x <= -CELL_SIZE * floor(0.5f * MAP_WIDTH)) // Si salió del mapa por la izquierda
        {
            x += static_cast<short>(CELL_SIZE * floor(1.5f * MAP_WIDTH)); // Reaparece por la derecha
        }
    }
    if (1 == diving) // Si esta tortuga tiene comportamiento de inmersión
    {
        if(0 == diving_timer) // Si el temporizador de inmersión llegó a cero
        {
            if (0 == diving_down) // Si está emergiendo (subiendo)
            {
                if (0 == diving_state) // Si llegó completamente a la superficie
                {
                    diving_down = 1; // Cambia a modo sumergirse

                    diving_state = 1; // Comienza el primer estado de inmersión
                }
                else // Si aún está emergiendo
                {
                    diving_state--; // Disminuye el estado (sube más a la superficie)
                }
            }
            else // Si está sumergiéndose (bajando)
            {
                if(3 == diving_state) // Si llegó al máximo nivel de inmersión
                {
                    diving_down = 0; // Cambia a modo emerger
                    diving_state = 2; // Retrocede un estado para comenzar a emerger
                }
                else // Si aún está sumergiéndose
                {
                    diving_state++; // Aumenta el estado (se sumerge más)
                }
            }
            diving_timer = TURTLE_DIVING_SPEED; // Reinicia el temporizador de inmersión
        }
        else // Si aún no es momento de cambiar estado de inmersión
        {
            diving_timer--; // Decrementa el temporizador
        }
    }
}
