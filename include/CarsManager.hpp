#pragma once  // Evita inclusiones múltiples del archivo

class CarsManager // Gestor de todos los coches en el juego
{
    std::vector<Car> cars; // Vector que contiene todos los coches activos en el nivel actual
public:

    CarsManager(unsigned char i_level); // Constructor de CarsManager

    void draw(sf::RenderWindow& i_window); // Dibuja todos los coches en la ventana

    void generate_level(unsigned char i_level); // Genera la configuración de coches para un nivel específico

    void update(Frog& i_frog); // Actualiza el estado de todos los coches y verifica colisiones con la rana
};