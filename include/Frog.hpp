#pragma once // Evita inclusiones múltiples del archivo

// Forward declarations
namespace sf {
    class Sound;
}

// Clase Frog
class Frog // Representa la rana jugable en el juego Frogger
{
    bool dead; // Estado de la rana: true = muerta, false = viva

    short x; // Posición horizontal en píxeles (puede ser negativa)

    unsigned char animation_timer; // Temporizador para controlar la velocidad de animación
    unsigned char current_frame; // Frame actual de la animación del sprite
    unsigned char direction; // hacia donde voltea la ranita

    unsigned short y; // Posición vertical en píxeles

    //Esto hace que la rana solo avance un espacio
    std::array<bool, 4> control_keys; // Array para controlar las teclas presionadas (arriba, abajo, izquierda, derecha)

    sf::Sprite sprite; // Sprite de SFML para dibujar la rana
    sf::Texture texture; // Textura que contiene la imagen de la rana

public:
    
    // Constructor de Frog
    Frog(); // Inicializa la rana en su posición inicial
    bool get_dead() const; // Obtiene el estado de muerte de la rana
    bool update_swamp(std::array<bool, 5>& i_swamp);// Actualiza el estado del pantano cuando la rana alcanza uno
    unsigned short get_y() const; // Obtiene la posición vertical de la rana
    void draw(sf::RenderWindow& i_window); // Dibuja la rana en la ventana
    void move(char i_value); // Mueve la rana horizontalmente
    void reset(); // Reinicia la rana a su posición y estado inicial
    void set_dead(unsigned char death_type = 0); // Marca la rana como muerta (0=ahogada, 1=atropellada)
    void update(); // Actualiza el estado de la rana
    sf::IntRect get_rect() const; // Obtiene el rectángulo delimitador de la rana
    
    // Método estático para configurar los sonidos de muerte
    static void set_death_sounds(sf::Sound* rushed, sf::Sound* splash);
};