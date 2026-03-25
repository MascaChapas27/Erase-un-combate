#pragma once

#include "Animacion.hpp"
#include <vector>

/*
    Esta clase define un tipo muy específico de animación
    que sirve para los fondos que se mueven infinitamente
    de forma lineal
*/
class AnimacionFondoBucleLineal : public Animacion
{
    private:
        // Dirección en la que se mueve el fondo
        Direccion direccionBucle;

    public:
        // Constructor que toma la textura para el fondo y la
        // dirección en la que se moverá
        AnimacionFondoBucleLineal(sf::Texture& texturaFondo, const Direccion direccionBucle);

        // Actualiza la animación, moviéndola un poco en la dirección correcta. En nuevasAnimaciones
        // se deberían colocar animaciones generadas por el fondo, pero como no genera ninguna, lo
        // más seguro es que no se coloque nada
        void actualizar(std::list<std::shared_ptr<Animacion>>& nuevasAnimaciones);

        // Resetea la animación al estado inicial
        void resetear();

        // Le da la vuelta a la animación
        void voltear();

        // Devuelve un vector con las hitboxes
        std::vector<Hitbox> getHitboxes();

        // Determina si la animación ha terminado
        bool haTerminado();

        // Devuelve un clon de la animación
        std::shared_ptr<Animacion> clonar();

        // Esto se usaría para indicar a la animación que ha chocado
        // con una hitbox, pero los fondos no chocan con nada
        void chocar(Hitbox hitbox);

        // Las clases que heredan de sf::Drawable deben implementar draw
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
