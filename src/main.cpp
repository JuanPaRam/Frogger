#include <array>
#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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
    bool instructions_showing = 1;
    bool intro_playing = 0;
    bool title_showing = 0;
    bool next_level = 0;
    bool game_over = 0;
    unsigned char level = 0;
    unsigned char lives = 3;
    unsigned char intro_frame = 1;
    unsigned char intro_animation_timer = 0;
    unsigned short instructions_timer = 240;
    unsigned short title_timer = 0;
    constexpr unsigned char INTRO_TOTAL_FRAMES = 76;
    constexpr unsigned char INTRO_FRAME_SPEED = 2;
    constexpr unsigned short INSTRUCTIONS_DURATION = 240;
    constexpr unsigned short TITLE_DURATION = 60;

    unsigned short timer = TIMER_INITIAL_DURATION;
    unsigned short timer_duration = TIMER_INITIAL_DURATION;

    std::array<bool, 5 > swamp ={0};
    std::chrono::microseconds lag(0);
    std::chrono::steady_clock::time_point previous_time;
    std::chrono::steady_clock::time_point game_over_time;
    
    sf::Texture intro_texture;
    sf::Sprite intro_sprite;
    sf::Texture title_texture;
    sf::Sprite title_sprite;
    title_texture.loadFromFile("assets/Title.png");
    
    sf::Texture arrows_texture;
    sf::Sprite arrows_sprite;
    arrows_texture.loadFromFile("assets/Arrows.png");
    arrows_sprite.setTexture(arrows_texture);
    
    sf::Texture enter_texture;
    sf::Sprite enter_sprite;
    enter_texture.loadFromFile("assets/Enter.png");
    enter_sprite.setTexture(enter_texture);
    
    sf::SoundBuffer blink_buffer;
    sf::Sound blink_sound;
    blink_buffer.loadFromFile("assets/Audio/Blink.mp3");
    blink_sound.setBuffer(blink_buffer);
    
    sf::SoundBuffer tongue_buffer;
    sf::Sound tongue_sound;
    tongue_buffer.loadFromFile("assets/Audio/Tongue.mp3");
    tongue_sound.setBuffer(tongue_buffer);
    
    sf::SoundBuffer gameover_buffer;
    sf::Sound gameover_sound;
    gameover_buffer.loadFromFile("assets/Audio/GameOver.mp3");
    gameover_sound.setBuffer(gameover_buffer);
    
    sf::SoundBuffer rushed_buffer;
    sf::Sound rushed_sound;
    rushed_buffer.loadFromFile("assets/Audio/Rushed.mp3");
    rushed_sound.setBuffer(rushed_buffer);
    
    sf::SoundBuffer splash_buffer;
    sf::Sound splash_sound;
    splash_buffer.loadFromFile("assets/Audio/Splash.mp3");
    splash_sound.setBuffer(splash_buffer);
    
    sf::SoundBuffer point_buffer;
    sf::Sound point_sound;
    point_buffer.loadFromFile("assets/Audio/Point.mp3");
    point_sound.setBuffer(point_buffer);
    
    sf::Music gameplay_music;
    gameplay_music.openFromFile("assets/Audio/GamePlay.mp3");
    gameplay_music.setLoop(true);
    
    sf::Event event;
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, SCREEN_RESIZE * (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)), "Frogger", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));
    
    // Establecer icono de la ventana
    sf::Image icon;
    if (icon.loadFromFile("assets/icon.png"))
    {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    CarsManager cars_manager(level);

    Frog frog;
    
    // Configurar los sonidos de muerte en la clase Frog
    Frog::set_death_sounds(&rushed_sound, &splash_sound);

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

            if (1 == instructions_showing) // Pantalla de instrucciones
            {
                if (instructions_timer > 0)
                {
                    instructions_timer--;
                }
                else
                {
                    instructions_showing = 0;
                    intro_playing = 1;
                }
            }
            else if (1 == intro_playing) // Animación de introducción
            {
                if (0 == intro_animation_timer)
                {
                    intro_animation_timer = INTRO_FRAME_SPEED;
                    intro_frame++;
                    
                    // Reproducir sonido Blink cuando la rana parpadea (cada 10 frames aproximadamente)
                    if (intro_frame % 10 == 0 && intro_frame > 0 && intro_frame < INTRO_TOTAL_FRAMES)
                    {
                        blink_sound.play();
                    }
                    
                    // Reproducir sonido Tongue cuando se alcanza el último frame
                    if (intro_frame == INTRO_TOTAL_FRAMES)
                    {
                        tongue_sound.play();
                    }
                    
                    if (intro_frame > INTRO_TOTAL_FRAMES)
                    {
                        intro_playing = 0;
                        title_showing = 1;
                        title_timer = TITLE_DURATION;
                        gameplay_music.play();
                    }
                }
                else
                {
                    intro_animation_timer--;
                }
                
                // Saltar intro con Enter
                if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    intro_playing = 0;
                    title_showing = 1;
                    title_timer = TITLE_DURATION;
                    gameplay_music.play();
                }
            }
            else if (1 == game_over) // Manejo del Game Over
            {
                // Detener música de gameplay
                if (gameplay_music.getStatus() == sf::Music::Playing)
                {
                    gameplay_music.stop();
                }
                
                // Verifica que hayan pasado al menos 3 segundos desde game over
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - game_over_time);
                if (elapsed.count() >= 3000) // Reinicia automáticamente después de 3 segundos
                {
                    game_over = 0;
                    lives = 3;
                    level = 0;
                    timer = TIMER_INITIAL_DURATION;
                    timer_duration = TIMER_INITIAL_DURATION;
                    swamp.fill(0);
                    cars_manager.generate_level(level);
                    river_manager.generate_level(level);
                    frog.reset();
                    gameplay_music.play();
                }
            }
            else if (1 == next_level) // Que hacer si se completó un nivel
            {
                if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) // Espera Enter para continuar
                {
                    next_level = 0;
                }
            }
            else if (0 == game_over) // Lógica de juego activa solo si no está en game over
            {
                // Actualizar temporizador del título si está mostrándose
                if (1 == title_showing)
                {
                    if (title_timer > 0)
                    {
                        title_timer--;
                    }
                    else
                    {
                        title_showing = 0;
                    }
                    
                    // Saltar título con Enter
                    if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                    {
                        title_showing = 0;
                    }
                }
                
                if (0 == frog.get_dead()) // Continuar si la rana está viva
                {
                    if (0 == timer) // Si se acabó el tiempo
                    {
                        frog.set_dead();
                        if (0 == lives) // Si no quedan vidas
                        {
                            game_over = 1;
                            game_over_time = std::chrono::steady_clock::now();
                            gameover_sound.play();
                        }
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

            if (0 == game_over && 1 == frog.get_dead()) // Si la rana murió y no está en game over
            {
                // Si no quedan vidas, activa game over inmediatamente
                if (0 == lives)
                {
                    game_over = 1;
                    game_over_time = std::chrono::steady_clock::now();
                    gameover_sound.play();
                }
                else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) // Si hay vidas, espera Enter
                {
                    lives--;
                    timer = timer_duration;
                    frog.reset();
                    
                    // Verifica si después de decrementar no quedan vidas
                    if (0 == lives)
                    {
                        game_over = 1;
                        game_over_time = std::chrono::steady_clock::now();
                        gameover_sound.play();
                    }
                }
            }
            else if (1 == frog.update_swamp(swamp)) // Si la rana alcanzó un pantano
            {
                point_sound.play();
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
        }
        
        // Renderizado - fuera del bucle de actualización
        window.clear();

        if (1 == instructions_showing) // Muestra pantalla de instrucciones
        {
            // Calcular centro de la ventana
            float centerX = CELL_SIZE * MAP_WIDTH / 2.0f;
            float centerY = (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) / 2.0f;
            
            // Ancho aproximado de cada texto en píxeles (cada letra ~8px)
            float text1Width = 15 * 8.0f; // "To navigate use"
            float text2Width = 14 * 8.0f; // "To respawn use"
            
            // Ancho de las imágenes (se obtiene de las texturas)
            float arrowsWidth = arrows_texture.getSize().x;
            float enterWidth = enter_texture.getSize().x;
            
            // Espacio entre texto e imagen
            float spacing = 10.0f;
            
            // Calcular ancho total de cada línea
            float line1Width = text1Width + spacing + arrowsWidth;
            float line2Width = text2Width + spacing + enterWidth;
            
            // Posicionar primera línea centrada
            float line1StartX = centerX - (line1Width / 2.0f);
            draw_text(0, line1StartX, centerY - 20, "To navigate use", window);
            arrows_sprite.setPosition(line1StartX + text1Width + spacing, centerY - 30);
            window.draw(arrows_sprite);
            
            // Posicionar segunda línea centrada
            float line2StartX = centerX - (line2Width / 2.0f);
            draw_text(0, line2StartX, centerY + 12, "To respawn use", window);
            enter_sprite.setPosition(line2StartX + text2Width + spacing, centerY + 12);
            window.draw(enter_sprite);
        }
        else if (1 == intro_playing) // Muestra animación de introducción
        {
            intro_texture.loadFromFile("assets/intro/AnimacionPantallaCarga" + std::to_string(intro_frame) + ".png");
            intro_sprite.setTexture(intro_texture, true);
            
            // Escalar la imagen para que ocupe toda la ventana
            float scaleX = (CELL_SIZE * MAP_WIDTH) / static_cast<float>(intro_texture.getSize().x);
            float scaleY = (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) / static_cast<float>(intro_texture.getSize().y);
            intro_sprite.setScale(scaleX, scaleY);
            intro_sprite.setPosition(0, 0);
            window.draw(intro_sprite);
        }
        else if (1 == game_over) // Muestra pantalla de Game Over
        {
            // Calcula el tiempo restante
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - game_over_time);
            float time_remaining = std::max(0.0f, (3000.0f - elapsed.count()) / 1000.0f);
            
            std::string counter_text = std::to_string(static_cast<int>(std::ceil(time_remaining)));
            
            draw_text(1, 0, 0, "Game Over :(\n\n" + counter_text + "\n\nThe game will restart\n\nin a moment", window);
        }
        else if (1 == next_level) // Muestra pantalla de siguiente nivel
        {
            draw_text(1, 0.5f * CELL_SIZE * MAP_WIDTH, 0.5f * CELL_SIZE * MAP_HEIGHT, "NEXT LEVEL!", window);
        }
        else // Renderizado normal del juego
        {
            draw_map(swamp, window);

            river_manager.draw(window);
            cars_manager.draw(window);
            
            // Dibujar la rana siempre al final para que sea visible
            frog.draw(window);
            
            draw_text(0, 0, CELL_SIZE * MAP_HEIGHT, "Time: " + std::to_string(static_cast<unsigned short>(floor(timer / 64.f))), window);
            draw_text(0, CELL_SIZE * MAP_WIDTH / 2 - 40, CELL_SIZE * MAP_HEIGHT, "Level: " + std::to_string(level + 1), window);
            draw_text(0, CELL_SIZE * MAP_WIDTH - 80, CELL_SIZE * MAP_HEIGHT, "Lives: " + std::to_string(lives), window);
            
            // Dibujar título encima del juego si está activo
            if (1 == title_showing)
            {
                title_sprite.setTexture(title_texture, true);
                
                // Escalar la imagen para que ocupe toda la ventana
                float scaleX = (CELL_SIZE * MAP_WIDTH) / static_cast<float>(title_texture.getSize().x);
                float scaleY = (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) / static_cast<float>(title_texture.getSize().y);
                title_sprite.setScale(scaleX, scaleY);
                title_sprite.setPosition(0, 0);
                window.draw(title_sprite);
            }
        }

        window.display();
    }
}