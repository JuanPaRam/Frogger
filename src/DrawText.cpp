#include <SFML/Graphics.hpp>

#include "DrawText.hpp"
#include "Global.hpp"

void draw_text(bool i_center, unsigned short i_x, unsigned short i_y, const std::string& i_text, sf::RenderWindow& i_window)
{
    sf::Font font;
    sf::Text text;
    
    // Carga la fuente desde el archivo
    if (!font.loadFromFile("Resources/Fonts/Font.ttf"))
    {
        // Si no puede cargar la fuente, intenta con una ruta alternativa
        font.loadFromFile("Font.ttf");
    }
    
    text.setFont(font);
    text.setCharacterSize(FRONT_HEIGHT);
    text.setString(i_text);
    text.setFillColor(sf::Color::White);
    
    // Si se debe centrar el texto
    if (i_center)
    {
        sf::FloatRect text_bounds = text.getLocalBounds();
        text.setPosition(i_x - 0.5f * text_bounds.width, i_y);
    }
    else
    {
        text.setPosition(i_x, i_y);
    }
    
    i_window.draw(text);
}
