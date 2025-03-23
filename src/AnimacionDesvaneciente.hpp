#ifndef __ANIMACION_DESVANECIENTE_HPP__
#define __ANIMACION_DESVANECIENTE_HPP__

#include "Animacion.hpp"
#include <SFML/Graphics.hpp>

/*
    Clase para las animaciones que van agrandándose y haciéndose transparentes
    con el tiempo, como cuando le pulsas a una opción del menú principal y
    sale un efecto de destello
*/
class AnimacionDesvaneciente : public Animacion {
    private:

        // Indica por cuánto se quiere multiplicar el tamaño del sprite. Si
        // se indica por ejemplo 2, al terminar la animación el sprite será
        // el doble de grande de lo que era al empezar
        float escalado;

    public:

        // Crea una animación desvaneciente con la textura dada, que se
        // agrandará hasta multiplicar su tamaño por el escalado dado
        AnimacionDesvaneciente(sf::Texture &textura, float escalado);

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

        // Devuelve un clon de la animación (aunque está reseteado)
        std::shared_ptr<Animacion> clonar();
};

#endif