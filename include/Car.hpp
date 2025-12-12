#pragma once  // Evita inclusiones múltiples del archivo

class Car // Maneja el movimiento, renderizado y colisiones de los vehículos
{
    bool direction; // Dirección del movimiento: 0 = derecha, 1 = izquierda

    short x; // Posición horizontal en píxeles (puede ser negativa)

    unsigned char size; // Tamaño del coche en píxeles (ancho)
    unsigned char speed; // Velocidad de movimiento del coche    
    unsigned char type; // Tipo de coche (determina qué sprite usar)

    unsigned short y; // Posición vertical en píxeles

    sf::Sprite sprite; // Sprite de SFML para dibujar el coche

    sf::Texture texture; // Textura que contiene la imagen del coche
public:
    // Constructor de Car
    Car(unsigned char i_x, unsigned short i_y);

    void draw(sf::RenderWindow& i_window); // Dibuja el coche en la ventana de renderizado

    void update(); // Actualiza la posición del coche

    sf::IntRect get_rect() const; // Obtiene el rectángulo delimitador del coche
};