#pragma once // Evita inclusiones múltiples del archivo

 // Dibuja texto en la ventana del juego
void draw_text(bool i_center, unsigned short i_x, unsigned short i_y, const std::string& i_text, sf::RenderWindow& i_window);
// Renderiza una cadena de texto en una posición específica con opciones de centrado