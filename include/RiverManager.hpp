#pragma once // Evita inclusiones múltiples del archivo

// Clase RiverManager
class RiverManager // Gestor de todos los elementos del río en el juego
{
    std::vector<Log> logs; // Vector que contiene todos los troncos activos en el nivel actual

    std::vector<Turtle> turtles; // Vector que contiene todas las tortugas activas en el nivel actual
public:
    RiverManager(unsigned char i_level); // Constructor de RiverManager

    void draw(sf::RenderWindow& i_window); // Dibuja todos los troncos y tortugas en la ventana

    void generate_level(unsigned char i_level); // Genera la configuración de troncos y tortugas para un nivel específico
    
    // Actualiza todos los elementos del río y verifica si la rana está sobre alguno
    void update(Frog& i_frog); // Si la rana no está sobre ningún tronco/tortuga y está en el río, la marca como muerta
};