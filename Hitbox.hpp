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

        // Indica si el ataque es bajo (los ataques realizados mientras el
        // personaje est� agachado ser�n efectivos a�n si el otro personaje est�
        // protegi�ndose)
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
