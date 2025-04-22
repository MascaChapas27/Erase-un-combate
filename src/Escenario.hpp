#pragma once

#include "Personaje.hpp"
#include "Animacion.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>

/*
    Esta clase define un escenario en el que pueden pelear los luchadores. Los escenarios
    están formados por una imagen de fondo que se va moviendo según se mueven los personajes.
    El origen de este sprite se coloca justo enmedio en el eje X, y en el eje Y arriba del todo (y=0)
*/

class Escenario : public sf::Drawable {

    private:

        // Sprite que forma el fondo del escenario
        sf::Sprite spriteFondo;

    public:
        // Construye un escenario utilizando la textura del fondo
        Escenario(sf::Texture& texturaFondo);

        // Actualiza el escenario según la posición de los personajes (también puede actualizar
        // la posición de los personajes y los efectos para que se muevan con el fondo)
        void actualizar(Personaje& personaje1, Personaje& personaje2, std::list<std::shared_ptr<Animacion>>& efectos);

        // Coloca el escenario en el centro
        void resetear();

        // Dibuja el fondo
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

