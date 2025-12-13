#include <array>
#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Car.hpp"
#include "Frog.hpp"
#include "CarsManager.hpp"
#include "DrawMap.hpp"
#include "DrawText.hpp"
#include "Global.hpp"
#include "Log.hpp"
#include "Turtle.hpp"
#include "RiverManager.hpp"

int main() // Función principal del juego
{
    bool next_level = 0;
    unsigned char level = 0;

    unsigned short timer = TIMER_INITIAL_DURATION;
    unsigned short timer_duration = TIMER_INITIAL_DURATION;

    std::array<bool, 5 > swamp ={0};
    std::chrono::microseconds lag(0);
    std::chrono::steady_clock::time_point previous_time;
    
    sf::Event event;
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, SCREEN_RESIZE * (FRONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)), "Frogger", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FRONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));

    CarsManager cars_manager(level);

    Frog frog;

    RiverManager river_manager(level);

    previous_time = std::chrono::steady_clock::now();

    while (1 == window.isOpen()) // Acciona el bucle principal mientras la ventana esté abierta
    {
        std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);
        lag += delta_time;
        previous_time += delta_time;

        while (FRAME_DURATION <= lag) // Procesa la lógica del juego en intervalos fijos
        {
            lag -= FRAME_DURATION;

            while (1 == window.pollEvent(event)) // Procesa todos los eventos de la ventana
            {
                switch (event.type) // Maneja diferentes tipos de eventos
                {
                    case sf::Event::Closed: // Que hacer en caso de que el usuario cierra la ventana
                    {
                        window.close();
                    }
                }
            }

            if (1 == next_level) // Que hacer si se completó un nivel
            {
                if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) // Espera Enter para continuar
                {
                    next_level = 0;
                }
            }
            else // Lógica de juego activa o no en siguiente nivel
            {
                if (0 == frog.get_dead()) // Continuar si la rana está viva
                {
                    if (0 == timer) // Si se acabó el tiempo
                    {
                        frog.set_dead();
                    }
                    else // Si aún hay tiempo
                    {
                        timer--;
                    }
                }

                frog.update();
                cars_manager.update(frog);
                river_manager.update(frog);
            }

            if (1 == frog.get_dead()) // Si la rana murió
            {
                if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) // Presiona Enter para reiniciar
                {
                    level = 0;

                    timer =TIMER_INITIAL_DURATION;
                    timer_duration = TIMER_INITIAL_DURATION;
                    swamp.fill(0);
                    cars_manager.generate_level(level);
                    river_manager.generate_level(level);
                    frog.reset();
                }
            }
            else if (1 == frog.update_swamp(swamp)) // Si la rana alcanzó un pantano
            {
                bool swamp_full = 1;

                for (unsigned char a = 0; a < swamp. size(); a++) // Verifica si todos los pantanos están llenos
                {
                    if (0 == swamp[a]) // Si encuentra un pantano vacío
                    {
                        swamp_full = 0;
                        break;
                    }
                }

                if (1 == swamp_full) // Si todos los pantanos están llenos
                {
                    next_level = 1;
                    level++;
                    timer_duration = std::max<unsigned short>(floor(0.25f *TIMER_INITIAL_DURATION), timer_duration - TIMER_REDUCTION);
                    timer = timer_duration;
// Modificacion del nivel ----------------------------------------------------------------------------------------------------------------------------------------
                    if (TOTAL_LEVELS == level) // Si alcanzó el último nivel
                    {
                        level = static_cast<unsigned char>(floor(0.5f * TOTAL_LEVELS)); // Regresa a la mitad de los niveles
                    }
                    swamp.fill(0);
                    cars_manager.generate_level(level);
                    river_manager.generate_level(level);
                }
                else // Si aún faltan pantanos por llenar
                {
                    timer = std::min<unsigned short>(timer_duration, timer + floor(0.5f * timer_duration));
                }
                 frog.reset();
            }
            if (FRAME_DURATION > lag) // Si es momento de renderizar
            {
                window.clear();

                if (1 == next_level) // Muestra pantalla de siguiente nivel
                {
                    draw_text(1, 0, 0, "NEXT LEVEL!", window);
                }
                else // Renderizado normal del juego
                {
                    draw_map(swamp, window);

                    if (0 == frog.get_dead()) // Si la rana está viva, dibuja río primero
                    {
                        river_manager.draw(window);
                        frog.draw(window);
                    }
                    else // Si la rana está muerta, dibuja rana primero
                    {
                        frog.draw(window);
                        river_manager.draw(window);
                    }

                    cars_manager.draw(window);
                    draw_text(0, 0, CELL_SIZE * MAP_HEIGHT, "Time: " + std::to_string(static_cast<unsigned short>(floor(timer / 64.f))), window);
                }

                window.display();
            }
        }
    }
}