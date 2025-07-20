#ifndef __HITBOX_HPP__
#define __HITBOX_HPP__

#include <SFML/Graphics.hpp>

/*
    Clase que implementa una caja de colisiones. Esta caja es un rect�ngulo
    que indica el �rea que abarca un ataque o un personaje, TOMANDO SIEMPRE COMO
    REFERENCIA LA ESQUINA SUPERIOR IZQUIERDA DEL SPRITE, SEA CUAL SEA
*/
class Hitbox {
    private:

        // Rect�ngulo de la hitbox
        sf::IntRect rectangulo;

        // Da�o que quita el ataque de la hitbox (si es un ataque)
        int fuerzaAtaque;

    public:

        // Constructor
        Hitbox(sf::IntRect rectangulo, int fuerzaAtaque);

        // Devuelve el rect�ngulo
        sf::IntRect getRectangulo();

        // Devuelve la fuerza de ataque
        int getFuerzaAtaque();

};

#endif // __HITBOX_HPP__
