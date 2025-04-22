#pragma once

#include "Animacion.hpp"
#include "SFML/Graphics.hpp"
#include <string>

/*
    Esta clase define animaciones que empiezan siendo inifinitamente
    pequeñas (ni se ven) y se agrandan hasta conseguir
    alcanzar su tamaño original, para luego hacerse otra vez más pequeñas tras
    esperar un rato teniendo el tamaño original
*/
class AnimacionAgrandable : public Animacion {
    private:
        // Indica, del 0 al 1, cómo de agrandado está el objeto con respecto a su
        // tamaño original
        float escalaActual;

        // Indica si el sprite se está agrandando (si está a false puede que se esté disminuyendo
        // o puede que esté manteniéndose en su máximo tamaño, depende de si contadorMaxTamano es menor
        // que framesMaxTamaño (máximo tamaño) o igual (disminuyendo))
        bool seEstaAgrandando;

        // Indica cuántos frames debe esperar el frame en tamaño máximo antes de volver a
        // disminuir de tamaño
        int framesEspera;

        // Indica cuántos frames lleva el sprite con su tamaño máximo
        int contadorEspera;

    public:
        AnimacionAgrandable(int framesMaxTamano, sf::Texture& textura, std::string rutaSonido);

        // Actualiza la animación (avanza un frame), reproduciendo el sonido si es necesario. En nuevasAnimaciones
        // se insertan las nuevas animaciones que serán introducidas
        void actualizar(std::list<std::shared_ptr<Animacion>> &nuevasAnimaciones);

        // Resetea la animación al estado inicial
        void resetear();

        // Le da la vuelta a la animación
        void voltear();

        // Devuelve una lista con las hitboxes
        std::list<Hitbox> getHitboxes();

        // Determina si una animación ha terminado
        bool haTerminado();

        // Determina si la animación se ha agrandado ya hasta su tope
        bool haTerminadoDeAgrandarse();

        // Devuelve un clon de la animación
        std::shared_ptr<Animacion> clonar();

        // Dibuja la animación agrandable, aunque puede parecer que no se dibuja si
        // la escala es 0 y es infinitamente pequeña
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

