#pragma once // Evita inclusiones múltiples del archivo

// Clase Log
class Log // Representa un tronco flotante en el río
{
    bool direction; // Dirección del movimiento: 0 = derecha, 1 = izquierda

    short x; // Posición horizontal en píxeles (puede ser negativa)

    unsigned char animation_timer; // Temporizador para controlar la velocidad de animación del sprite
    unsigned char current_frame; // Frame actual de la animación (para sprites animados)
    unsigned char size; // Tamaño del tronco (número de celdas de ancho)
    unsigned char speed; // Velocidad de movimiento del tronco en píxeles por frame
    unsigned char type; // Tipo de tronco (determina qué sprite usar)

    unsigned short y; // Posición vertical en píxeles

    sf::Sprite sprite; // Sprite de SFML para dibujar el tronco
    sf::Texture texture; // Textura que contiene la imagen del tronco

public:
    //Constructor de Log
    Log(unsigned char i_type, unsigned char i_x, unsigned char i_y); // Inicializa un tronco con su tipo, posición y características
    bool check_frog(const Frog& i_frog) const; // Verifica si la rana está sobre este tronco
    void draw(sf::RenderWindow& i_window); // Dibuja el tronco en la ventana
    void update(bool i_move_frog, Frog& i_frog); // Actualiza la posición del tronco y mueve la rana si está sobre él
};