#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Enums.hpp"

// Esta clase define la ventanita que sale cuando pulsas el boton de un mando cuando
// los dos jugadores están usando el teclado, por lo que es necesario elegir para qué
// jugador va a ser el mando

class SelectorJugadorParaMando {
    private:
        // El constructor es privado porque la clase es Singleton
        SelectorJugadorParaMando();

        ~SelectorJugadorParaMando();

        // La única instancia
        static SelectorJugadorParaMando * selectorJugadorParaMando;

        // Imagen que aparece cuando seleccionas la opción del jugador 1
        sf::Sprite spriteJugador1;

        // Imagen que aparece cuando seleccionas la opción del jugador 2
        sf::Sprite spriteJugador2;

        // Sonido que suena cuando aparece la ventana
        sf::Sound sonidoAparecer;

        // Sonido que suena cuando se cambia el jugador seleccionado
        sf::Sound sonidoCambiarSeleccion;

        // Sonido que suena cuando desaparece la ventana
        sf::Sound sonidoDesaparecer;
    
    public:
        // En base a un control dado, se pregunta al usuario qué jugador va a ser
        // quien utilice ese control (que debe ser un Joystick, pero se da por
        // sentado (precondición como lo llama la chaviza))
        Jugador decidirJugador(Control c);

        // NUNCA SE COPIA UN SINGLETON
        SelectorJugadorParaMando(SelectorJugadorParaMando &otro) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const SelectorJugadorParaMando &) = delete;

        // Devuelve la única instancia
        static SelectorJugadorParaMando * unicaInstancia();
};

