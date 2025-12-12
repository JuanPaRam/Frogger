#pragma once  // Evita inclusiones múltiples del archivo

// Dibuja el mapa completo del juego Frogger
void draw_map(const std::array<bool, 5>& i_swamp, sf::RenderWindow& i_window); 
// Renderiza las diferentes secciones del mapa (carretera, río, zonas seguras)