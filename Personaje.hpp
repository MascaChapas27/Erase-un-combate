#ifndef __PERSONAJE_HPP__
#define __PERSONAJE_HPP__

#include "AnimacionPorFrames.hpp"
#include "Enums.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include <list>

/*
    Esta clase abstracta define las cualidades de un personaje gen�rico
    Los personajes creados deben heredar de esta clase obligatoriamente e implementar
    todas las funciones por su cuenta, incluyendo el constructor
*/
class Personaje : public sf::Drawable {

    private:

        // Cantidad actual de puntos de vida
        int puntosDeVida;

        // Jugador al que est� asociado el personaje
        Jugador jugador;

        // Estado en el que se encuentra el personaje
        EstadoPersonaje estado;

        // Animaci�n del personaje seg�n el estado
        std::map<EstadoPersonaje,Animacion*> animaciones;

        // Indica qu� acciones est�n siendo realizadas
        std::map<Accion,bool> accionesRealizadas;

    public:

        // Construye el personaje
        Personaje(std::map<EstadoPersonaje,Animacion*> animaciones);

        // Indica que se est� pulsando un bot�n
        void realizarAccion(Accion accion);

        // Indica que se ha soltado un bot�n
        void detenerAccion(Accion accion);

        // Actualiza la posici�n del personaje y dem�s seg�n los botones que est�n pulsados
        virtual void actualizar() = 0;

        // Comprueba si la hitbox del personaje ha colisionado con alg�n ataque enemigo y
        // reacciona de forma adecuada
        virtual void comprobarColisiones(std::list<Animacion*> &animaciones) = 0;

        // Devuelve los puntos de vida actuales
        int getPuntosDeVida();

        // Establece el jugador asociado
        void setJugador(Jugador jugador);

        // Cambia el estado
        void cambiarEstado(EstadoPersonaje estadoNuevo);

        // Devuelve un puntero a un clon del personaje
        virtual Personaje * clonar() = 0;

        // Las clases que heredan de sf::Drawable deben implementar draw
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

};

#endif // __PERSONAJE_HPP__
