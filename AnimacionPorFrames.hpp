#ifndef __ANIMACION_POR_FRAMES_HPP__
#define __ANIMACION_POR_FRAMES_HPP__

#include "Enums.hpp"
#include "Hitbox.hpp"
#include "Animacion.hpp"
#include <SFML/Graphics.hpp>
#include <list>
#include <map>

/*
    Esta clase define animaciones para personajes y ataques formadas por varios frames
    Cada animaci�n puede tener asociado cualquier n�mero de hitboxes
*/
class AnimacionPorFrames : public Animacion {
    private:

        // Tipo de repetici�n en bucle
        TipoBucle tipoBucle;

        // Si tipoBucle es PING_PONG, indica si la animaci�n va hacia delante o hacia atr�s
        bool pingPongHaciaDelante;

        // Si tipoBucle no es SIN_BUCLE, indica cu�ntas veces se repetir� la animaci�n (0 es infinito)
        int numRepeticionesTotal;

        // Si numRepeticionesTotal no es 0, indica cu�ntas veces se ha repetido la animaci�n de momento
        int numRepeticionesActual;

        // Correspondencia entre rect�ngulo y hitboxes
        std::map<int,std::list<Hitbox>> hitboxes;

        // Correspondencia entre frame y rect�ngulo (por ejemplo, frame 0 rect�ngulo 0, o frame 5 rect�ngulo 2)
        std::map<int,int> rectanguloCorrespondiente;

        // N�mero de frame actual
        unsigned int frameActual;

    public:

        // Constructor
        AnimacionPorFrames(int posicionX, int posicionY, int origenX, int origenY, int numRectangulos, sf::Texture &textura, TipoBucle tipoBucle, int numRepeticionesTotal, std::map<int,std::list<Hitbox>> hitboxes, std::map<int,int> rectanguloCorrespondiente);

        // Actualiza la animaci�n (avanza un frame)
        virtual void actualizar();

        // Devuelve qu� rect�ngulo se est� mostrando actualmente
        int getNumeroRectangulo();

        // Cambia el tipo de bucle para la animaci�n
        void setTipoBucle(TipoBucle tipoBucle);

        // Resetea la animaci�n al primer frame y el primer rect�ngulo
        virtual void resetear();

        // Devuelve un clon de la animaci�n
        virtual Animacion * clonar();

        // Las clases que heredan de sf::Drawable deben implementar draw
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // __ANIMACION_POR_FRAMES_HPP__
