#pragma once

#include "Hitbox.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <list>
#include <memory>

/*
    Clase abstracta para cualquier tipo de animaciones
*/
class Animacion : public sf::Drawable {
    protected:
        // Sprite para la animación
        sf::Sprite sprite;

        // Ruta del sonido
        std::string rutaSonido;

    public:

        // Constructor para inicializar el sprite o si no el sfml se queja porque no existe constructor por
        // defecto para sf::Sprite
        Animacion(sf::Texture &textura);

        // Actualiza la animación (avanza un frame), reproduciendo el sonido si es necesario. En nuevasAnimaciones
        // se insertan las nuevas animaciones que serán introducidas
        virtual void actualizar(std::list<std::shared_ptr<Animacion>> &nuevasAnimaciones) = 0;

        // Resetea la animación al estado inicial
        virtual void resetear() = 0;

        // Le da la vuelta a la animación
        virtual void voltear() = 0;

        // Coloca el sprite en la posición indicada con respecto a la esquina superior
        // izquierda de la ventana
        void setPosicion(float x, float y);
        void setPosicion(sf::Vector2f posicion);

        // Establece la rotación del sprite en grados centígrados
        void setRotacion(double angulo);

        // Establece el color de la animación
        void setColor(sf::Color color);

        // Devuelve la posición actual del sprite (teniendo en cuenta el origen, sea cual sea)
        sf::Vector2f getPosicion();

        // Devuelve la posición de la esquina superior izquierda del sprite (es decir, no tiene en
        // cuenta el origen que pueda tener)
        sf::Vector2f getPosicionEsqSupIzq() const;

        // Devuelve una lista con las hitboxes
        virtual std::list<Hitbox> getHitboxes() = 0;

        // Mueve la posición del sprite con respecto a la posición actual
        void mover(float x, float y);
        void mover(sf::Vector2f diferencia);

        // Determina si una animación ha terminado
        virtual bool haTerminado() = 0;

        // Devuelve un clon de la animación
        virtual std::shared_ptr<Animacion> clonar() = 0;

        // Indica a la animación que acaba de chocar contra una hitbox (SE SUPONE QUE
        // LA HITBOX VIENE CON SU POSICION RELATIVA A LA PARTE DE ARRIBA A LA IZQUIERDA)
        virtual void chocar(Hitbox hitbox);

        // Las clases que heredan de sf::Drawable deben implementar draw
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};