#ifndef __ESCENARIO_HPP__
#define __ESCENARIO_HPP__

#include "Personaje.hpp"
#include "Animacion.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>

/*
    Esta clase define un escenario en el que pueden pelear los luchadores. Los escenarios
    están formados por una parte trasera, situada detrás de los personajes, y una parte delantera,
    situada delante. El origen de ambos sprites se coloca justo enmedio en el eje X, y en el eje Y arriba del todo (y=0)
*/

class Escenario {

    private:

        // Sprite que forma el fondo del escenario
        sf::Sprite spriteFondo;

        // Sprite que forma la parte frontal del escenario, que puede colocarse encima de los personajes
        // (por ejemplo, farolas o árboles)
        sf::Sprite spriteFrente;

        // Textura para el suelo
        sf::Texture& texturaSuelo;

        // Suelo hecho especialmente para que se vaya torciendo con el movimiento. Está formado por cuatro
        // vértices de los cuales los dos de abajo se van moviento más de la cuenta para conseguir el efecto
        sf::VertexArray suelo;

    public:
        // Construye un escenario utilizando la textura del fondo y la textura del frente
        Escenario(sf::Texture& texturaFondo, sf::Texture& texturaFrente, sf::Texture& texturaSuelo);

        // Actualiza el escenario según la posición de los personajes (también puede actualizar
        // la posición de los personajes y los efectos para que se muevan con el fondo)
        void actualizar(Personaje& personaje1, Personaje& personaje2, std::list<std::shared_ptr<Animacion>>& efectos);

        // Coloca el escenario en el centro
        void resetear();

        // Dibuja el fondo del escenario
        void dibujarFondo(sf::RenderTarget& target, sf::RenderStates states) const;

        // Dibuja el frente del escenario
        void dibujarFrente(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif