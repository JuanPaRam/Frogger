#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Car.hpp"
#include "Frog.hpp"
#include "CarsManager.hpp"
#include "Global.hpp"

CarsManager::CarsManager(unsigned char i_level) // Constructor de la clase CarsManager
{
    // Inicializa el gestor de coches y genera el nivel especificado
    generate_level(i_level); // Número del nivel a generar (0-7)
}

void CarsManager::draw(sf::RenderWindow& i_window) // Dibuja todos los coches en la ventana
{
    for (Car& car : cars) // Itera sobre el vector de coches y llama al método draw de cada uno
    {
        car.draw(i_window); // Ventana de SFML donde se dibujarán los coches
    }
}

void CarsManager::generate_level(unsigned char i_level) // Genera la configuración de coches para un nivel específico
{
    cars.clear(); // Limpia los coches existentes y crea nuevos según el nivel seleccionado
    switch (i_level) // Cada nivel tiene un patrón único de posición y cantidad de coches
    {
        case 0: // Número del nivel a generar (0-7, cada uno con diferente dificultad)
        {
            cars.push_back(Car(1, 0));
            cars.push_back(Car(9, 0));

            cars.push_back(Car(7, 1));
            cars.push_back(Car(11, 1));

            cars.push_back(Car(8, 3));
            cars.push_back(Car(10, 2));

            cars.push_back(Car(2, 3));

            cars.push_back(Car(0, 4));
            cars.push_back(Car(6, 4));
            cars.push_back(Car(13, 4));

            cars.push_back(Car(0, 5));
            cars.push_back(Car(7, 5));
            cars.push_back(Car(14, 5));

            break;
        }

        case 1:
        {
            cars.push_back(Car(1, 0));
            cars.push_back(Car(7, 0));
            cars.push_back(Car(13, 0));

            cars.push_back(Car(4, 1));
            cars.push_back(Car(10, 1));
            cars.push_back(Car(14, 1));

            cars.push_back(Car(9, 2));
            cars.push_back(Car(14, 2));

            cars.push_back(Car(5, 3));
            cars.push_back(Car(7, 3));

            cars.push_back(Car(2, 4));
            cars.push_back(Car(10, 4));
            cars.push_back(Car(15, 4));
            cars.push_back(Car(21, 4));

            cars.push_back(Car(0, 5));
            cars.push_back(Car(4, 5));
            cars.push_back(Car(11, 5));
            cars.push_back(Car(18, 5));

            break;
        }

        case 2:
        {
            cars.push_back(Car(1, 0));
            cars.push_back(Car(5, 0));
            cars.push_back(Car(12, 0));
            cars.push_back(Car(19, 0));

            cars.push_back(Car(4, 1));
            cars.push_back(Car(9, 1));
            cars.push_back(Car(18, 1));

            cars.push_back(Car(2, 2));
            cars.push_back(Car(10, 2));
            cars.push_back(Car(18, 2));

            cars.push_back(Car(4, 3));

            cars.push_back(Car(0, 4));
            cars.push_back(Car(9, 4));
            cars.push_back(Car(13, 4));
            cars.push_back(Car(19, 4));
            
            cars.push_back(Car(2, 5));
            cars.push_back(Car(6, 5));
            cars.push_back(Car(15, 5));
            cars.push_back(Car(22, 5));

            break;
        }

        case 3:
        {
            cars.push_back(Car(3, 0));
            cars.push_back(Car(10, 0));
            cars.push_back(Car(16, 0));
            cars.push_back(Car(21, 0));

            cars.push_back(Car(2, 1));
            cars.push_back(Car(7, 1));
            cars.push_back(Car(15, 1));

            cars.push_back(Car(1, 2));
            cars.push_back(Car(5, 2));
            
            cars.push_back(Car(4, 3));
            cars.push_back(Car(13, 3));
            cars.push_back(Car(10, 3));

            cars.push_back(Car(3, 4));
            cars.push_back(Car(6, 4));
            cars.push_back(Car(13, 4));
            cars.push_back(Car(20, 4));

            cars.push_back(Car(1, 5));
            cars.push_back(Car(4, 5));
            cars.push_back(Car(10, 5));
            cars.push_back(Car(16, 5));

            break;
        }

        case 4:
        {
            cars.push_back(Car(0, 0));
            cars.push_back(Car(6, 0));
            cars.push_back(Car(11, 0));
            cars.push_back(Car(21, 0));

            cars.push_back(Car(2, 1));
            cars.push_back(Car(7, 1));
            cars.push_back(Car(16, 1));
            cars.push_back(Car(20, 1));

            cars.push_back(Car(1, 2));
            cars.push_back(Car(9, 2));
            cars.push_back(Car(15, 2));

            cars.push_back(Car(7, 3));
            cars.push_back(Car(13, 3));

            cars.push_back(Car(1, 4));
            cars.push_back(Car(8, 4));
            cars.push_back(Car(12, 4));
            cars.push_back(Car(18, 4));

            cars.push_back(Car(1, 5));
            cars.push_back(Car(8, 5));
            cars.push_back(Car(16, 5));
            cars.push_back(Car(21, 5));

            break;
        }

        case 5:
        {
            cars.push_back(Car(0, 0));
            cars.push_back(Car(8, 0));
            cars.push_back(Car(10, 0));
            cars.push_back(Car(15, 0));

            cars.push_back(Car(1, 1));
            cars.push_back(Car(6, 1));
            cars.push_back(Car(14, 1));
            cars.push_back(Car(18, 1));

            cars.push_back(Car(0, 2));
            cars.push_back(Car(6, 2));

            cars.push_back(Car(7, 3));
            cars.push_back(Car(12, 3));
            cars.push_back(Car(15, 3));

            cars.push_back(Car(0, 4));
            cars.push_back(Car(9, 4));
            cars.push_back(Car(13, 4));
            cars.push_back(Car(20, 4));

            cars.push_back(Car(0, 5));
            cars.push_back(Car(7, 5));
            cars.push_back(Car(15, 5));
            cars.push_back(Car(20, 5));

            break;
        }

        case 6:
        {
            cars.push_back(Car(0, 0));
            cars.push_back(Car(3, 0));
            cars.push_back(Car(8, 0));
            cars.push_back(Car(14, 0));

            cars.push_back(Car(0, 1));
            cars.push_back(Car(7, 1));
            cars.push_back(Car(15, 1));
            cars.push_back(Car(20, 1));

            cars.push_back(Car(1, 2));
            cars.push_back(Car(8, 2));

            cars.push_back(Car(0, 3));
            cars.push_back(Car(10, 3));
            cars.push_back(Car(19, 3));

            cars.push_back(Car(0, 4));
            cars.push_back(Car(7, 4));
            cars.push_back(Car(12, 4));
            cars.push_back(Car(19, 4));
            cars.push_back(Car(21, 4));

            cars.push_back(Car(0, 5));
            cars.push_back(Car(5, 5));
            cars.push_back(Car(16, 5));
            cars.push_back(Car(21, 5));

            break;
        }

        case 7:
        {
            cars.push_back(Car(1, 0));
            cars.push_back(Car(5, 0));
            cars.push_back(Car(8, 0));
            cars.push_back(Car(14, 0));
            cars.push_back(Car(18, 0));

            cars.push_back(Car(0, 1));
            cars.push_back(Car(7, 1));
            cars.push_back(Car(15, 1));
            cars.push_back(Car(20, 1));

            cars.push_back(Car(1, 2));
            cars.push_back(Car(8, 2));
            cars.push_back(Car(15, 2));

            cars.push_back(Car(0, 3));
            cars.push_back(Car(10, 3));
            cars.push_back(Car(19, 3));

            cars.push_back(Car(1, 4));
            cars.push_back(Car(7, 4));
            cars.push_back(Car(11, 4));
            cars.push_back(Car(18, 4));
            cars.push_back(Car(22, 4));

            cars.push_back(Car(0, 5));
            cars.push_back(Car(4, 5));
            cars.push_back(Car(9, 5));
            cars.push_back(Car(16, 5));
            cars.push_back(Car(20, 5));

            break;
        }
    }
}

void CarsManager::update(Frog& i_frog) // Actualiza el estado de todos los coches y verifica colisiones con la rana
{
     // Primero actualiza la posición de cada coche, luego detecta si algún coche colisiona con la rana
    for (Car& car : cars)
    {
        car.update();
    }

    if (0== i_frog.get_dead()) // solo si la rana está viva
    {
        for (Car& car : cars)
        {
             // Referencia a la rana para detectar colisiones y actualizar su estado
            if (1 == car.get_rect().intersects(i_frog.get_rect()))
            {
                i_frog.set_dead(1); // 1 = atropellada
            }
        }
    }
}