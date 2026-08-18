#pragma once

#include <SFML/Graphics.hpp>

/*
    Esta es una clase que, mediante composición, permite
    especificar la posición, el color o la escala deseados
    para un sprite. De esta forma, por ejemplo, se puede
    colocar un sprite en un lugar, y luego decir que su
    posición deseada es otro lugar distinto, haciendo que
    el sprite se mueva al lugar. Esto es útil para crear
    sprites dinámicos que cambien de posición visiblemente
    durante el gameplay, en vez de cambiar de posición
    inmediatamente, lo cual es un poco aburridillo
*/
class SpriteDeseado : public sf::Drawable
{
    private:
        // El sprite normal y corriente
        sf::Sprite sprite;

        // La posición deseada para el sprite
        sf::Vector2f posicionDeseada;

        // La escala deseada para el sprite
        sf::Vector2f escalaDeseada;

        // El color deseado para el sprite
        sf::Color colorDeseado;

        // La fórmula para actualizar el valor actual
        // a un valor más cercano al valor deseado es:
        //
        // valorActual*factorPrimerValor + valorDeseado*(1-factorPrimerValor)
        //
        // Por lo que un valor cercano a 1 hace que las
        // actualizaciones sean más lentas, y un valor cercano a 0 hace
        // que las actualizaciones sean más rápidas.
        float factorPrimerValor;
    
    public:
        // Crea un SpriteDeseado en base a una textura y el factor usado para
        // la actualización de los atributos del sprite creado
        SpriteDeseado(const sf::Texture& textura, float factorPrimerValor);

        // Devuelve una referencia al sprite que se está usando
        sf::Sprite& getSprite();

        // Actualiza los atributos del sprite para que se acerquen
        // a los valores deseados
        void actualizar();

        // Hace que la posición del sprite sea la posición deseada
        void actualizarPosicionInmediatamente();

        // Hace que la escala del sprite sea la escala deseada
        void actualizarEscalaInmediatamente();

        // Hace que el color del sprite sea el color deseado
        void actualizarColorInmediatamente();

        // Permite dibujar el sprite
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;


        // Permite cambiar la posición deseada
        void setPosicionDeseada(const sf::Vector2f& posicionDeseada);

        // Permite cambiar la escala deseada
        void setEscalaDeseada(const sf::Vector2f& escalaDeseada);

        // Permite cambiar el color deseado
        void setColorDeseado(const sf::Color& colorDeseado);


        // Devuelve la posición deseada
        sf::Vector2f getPosicionDeseada();

        // Devuelve la escala deseada
        sf::Vector2f getEscalaDeseada();

        // Devuelve el color deseado
        sf::Color getColorDeseado();
};