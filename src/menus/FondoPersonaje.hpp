#pragma once

#include "Enums.hpp"
#include <SFML/Graphics.hpp>

/*
    Esta es una clase auxiliar para las imágenes grandes que salen
    para cada personaje en el menú de selección de personaje
*/
class FondoPersonaje : public sf::Drawable
{
    private:
        // El sprite asociado al fondo
        sf::Sprite spriteFondo;

        // Indica el nombre del personaje al que hace referencia este fondo
        std::string nombrePersonaje;
        
        // La posición relativa a la que se encuentra este fondo. Debería
        // ser la misma que la posición relativa a la que se encuentra el
        // selector del personaje al que hace referencia este fondo
        int posicionRelativa;

        // El jugador al que pertenece este fondo
        Jugador jugador;

        // El shader usado para que el fondo se vuelva blanco al estar seleccionado
        sf::Shader shader;

        // Indica si el selector asociado al fondo fue seleccionado o no
        bool seleccionado;
    
    public:
        // Se crea el fondo
        FondoPersonaje(const sf::Texture& texturaFondo, const std::string& nombrePersonaje, Jugador jugador, int posicionRelativa);

        // Se reestablece el fondo a su estado inicial (excepto la posición relativa,
        // eso se lo tienes que poner tú antes de resetear)
        void resetear(int posicionRelativa);

        // Se elige el selector asociado al fondo
        void seleccionar();

        // Se cancela la elección del selector asociado al fondo
        void quitarSeleccion();

        // Se actualiza la posición y la transparencia del fondo
        void actualizar();

        // Permite dibujar el fondo
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        // Establece el valor de la posición relativa
        void setPosicionRelativa(int posicionRelativa)
        {
            this->posicionRelativa = posicionRelativa;
        }

        // Devuelve el nombre del personaje
        std::string getNombrePersonaje()
        {
            return nombrePersonaje;
        }

        // Devuelve una referencia al sprite del fondo
        sf::Sprite& getSprite()
        {
            return spriteFondo;
        }
};
