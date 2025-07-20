#ifndef __HITBOX_HPP__
#define __HITBOX_HPP__

#include "Enums.hpp"

#include <SFML/Graphics.hpp>

/*
    Clase que implementa una caja de colisiones. Esta caja es un rectángulo
    que indica el área que abarca un ataque o un personaje, TOMANDO SIEMPRE COMO
    REFERENCIA LA ESQUINA SUPERIOR IZQUIERDA DEL SPRITE, SEA CUAL SEA
*/
class Hitbox {
    private:

        // Rectángulo de la hitbox
        sf::IntRect rectangulo;

        // Daño que quita el ataque de la hitbox (si es un ataque)
        int fuerzaAtaque;

        // Indica si el ataque es bajo (los ataques realizados mientras el
        // personaje está agachado serán efectivos aún si el otro personaje está
        // protegiéndose)
        bool bajo;

    public:

        // Constructor
        Hitbox(sf::IntRect rectangulo, int fuerzaAtaque, bool bajo);

        // Devuelve el rect�ngulo
        sf::IntRect getRectangulo();

        // Devuelve la fuerza de ataque
        int getFuerzaAtaque();

        // Devuelve si el ataque es bajo
        bool esAtaqueBajo();

};

#endif // __HITBOX_HPP__
