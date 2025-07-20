#ifndef __ANIMACION_HPP__
#define __ANIMACION_HPP__

#include <SFML/Graphics.hpp>

/*
    Clase abstracta para cualquier tipo de animaciones
*/
class Animacion : public sf::Drawable {
    protected:
        // Sprite para la animaci�n
        sf::Sprite sprite;

    public:

        // Actualiza la animaci�n (avanza un frame)
        virtual void actualizar() = 0;

        // Resetea la animaci�n al estado inicial
        virtual void resetear() = 0;

        // Le da la vuelta al sprite
        void voltear();

        // Coloca el sprite en la posici�n indicada con respecto a la esquina superior
        // izquierda de la ventana
        void setPosicion(float x, float y);
        void setPosicion(sf::Vector2f posicion);

        // Devuelve la posici�n actual del sprite
        sf::Vector2f getPosicion();

        // Mueve la posici�n del sprite con respecto a la posici�n actual
        void mover(float x, float y);
        void mover(sf::Vector2f diferencia);

        // Devuelve un clon de la animaci�n
        virtual Animacion * clonar() = 0;

        // Las clases que heredan de sf::Drawable deben implementar draw
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // __ANIMACION_HPP__
