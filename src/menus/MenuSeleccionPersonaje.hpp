#pragma once

#include "SelectorPersonaje.hpp"
#include "FondoPersonaje.hpp"
#include "Enums.hpp"
#include "AnimacionFondoBucleLineal.hpp"

#include <vector>
#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>

/*
    Esta clase representa el menú principal en el que tú y tu rival elegís
    cada uno un personaje (o, si estás jugando contra la máquina, solo tú eliges
    un personaje)
*/
class MenuSeleccionPersonaje {
    private:
        // El constructor es privado porque la clase es Singleton
        MenuSeleccionPersonaje();

        ~MenuSeleccionPersonaje();

        // La única instancia
        static MenuSeleccionPersonaje * menuSeleccionPersonaje;

        // El índice del vector nombresPersonajes donde está el nombre del
        // personaje que está seleccionando cada jugador
        unsigned int indiceJugador1;
        unsigned int indiceJugador2;

        // Indica, para cada jugador, si ya ha elegido a su personaje o aún no
        bool personajeElegidoJugador1;
        bool personajeElegidoJugador2;

        // Un contador que indica desde hace cuántos fotogramas terminó
        // la selección de personaje de forma satisfactoria
        unsigned int contadorSaliendo;

        // El sprite del marco principal del menú de selección de personajes
        sf::Sprite spriteMarco;

        // El sprite que sale arriba a la derecha que indica que si le pulsas
        // a ESC sales al menú anterior
        sf::Sprite spriteEsc;

        // El fondo super chulo cuadriculado que sale por detrás
        AnimacionFondoBucleLineal fondoCuadriculado;

        // El fondo que sale cuando cada jugador elige a su personaje
        AnimacionFondoBucleLineal fondoJ1Elegido;
        AnimacionFondoBucleLineal fondoJ2Elegido;

        // El efecto como de bolitas que sale por encima del fondo
        // que aparece al seleccionar personajes
        AnimacionFondoBucleLineal fondoJ1ElegidoBolitas;
        AnimacionFondoBucleLineal fondoJ2ElegidoBolitas;

        // El rectángulo blanco que aparece encima del fondo de personaje
        // de cada jugador cuando lo selecciona
        sf::RectangleShape rectanguloBlancoJ1;
        sf::RectangleShape rectanguloBlancoJ2;

        // Rectángulo que tapa el menú al entrar y al salir, lo típico
        sf::RectangleShape rectanguloNegro;

        // Vector con todos los selectores de personaje de cada jugador
        std::vector<SelectorPersonaje> selectoresPersonajeJugador1;
        std::vector<SelectorPersonaje> selectoresPersonajeJugador2;

        // Vector con todos los fondos de personaje de cada jugador
        std::vector<FondoPersonaje> fondosPersonajeJugador1;
        std::vector<FondoPersonaje> fondosPersonajeJugador2;

        // Los dos fondos adicionales que se crean para cada jugador
        // al elegir un personaje
        std::shared_ptr<sf::Sprite> fondoAdicionalCopiaJugador1;
        std::shared_ptr<sf::Sprite> fondoAdicionalNegroJugador1;
        std::shared_ptr<sf::Sprite> fondoAdicionalCopiaJugador2;
        std::shared_ptr<sf::Sprite> fondoAdicionalNegroJugador2;

        // Los sprites que indican qué teclas o botones tiene que pulsar
        // el jugador 1 para cambiar al personaje de la derecha, al de
        // la izquierda o para seleccionar al personaje actual
        sf::Sprite spriteTeclaIzquierdaJ1;
        sf::Sprite spriteTeclaDerechaJ1;
        sf::Sprite spriteTeclaSeleccionarJ1;

        // Los sprites que indican qué teclas o botones tiene que pulsar
        // el jugador 2 para cambiar al personaje de la derecha, al de
        // la izquierda o para seleccionar al personaje actual
        sf::Sprite spriteTeclaIzquierdaJ2;
        sf::Sprite spriteTeclaDerechaJ2;
        sf::Sprite spriteTeclaSeleccionarJ2;

        // Estos son los controles que cada jugador está usando. Dependiendo
        // del control, se mostrarán unos sprites u otros indicando qué
        // teclas o botones hay que pulsar para mover el personaje
        // seleccionado y elegir o quitar la selección del personaje actual
        Control controlUtilizadoParaLosSpritesJ1;
        Control controlUtilizadoParaLosSpritesJ2;

        // Se devuelve todo a su posición inicial
        void resetear();

        // Selecciona el personaje al que un jugador está apuntando. Además,
        // mete en animaciones las animaciones que han surgido al seleccionar
        // al personaje, e indica en personajesElegidos cuál ha sido el personaje
        // elegido por el jugador
        void seleccionarPersonaje(Jugador jugador, std::list<std::shared_ptr<Animacion>>& animaciones, std::unordered_map<Jugador,std::string>& personajesElegidos);

        // Se quita la selección del personaje que había elegido un jugador. También
        // elimina la entrada de personajesElegidos para el jugador en cuestión
        void desseleccionarPersonaje(Jugador jugador, std::unordered_map<Jugador,std::string>& personajesElegidos);

        // Cambia los sprites que indican las teclas o botones que hay que
        // pulsar
        void cambiarSpritesTeclas(Jugador j, Control c);

    public:

        // Comienza el funcionamiento del menú, que toma el control
        // de la ventana. Devuelve un mapa de nombres de personaje, donde
        // por cada uno de los dos jugadores se indica el nombre del personaje
        // que ha elegido. Si el mapa está vacío, significa que se ha cancelado
        // la selección de personaje
        std::unordered_map<Jugador,std::string> comenzarEleccionDoble();

        // Es como comenzarEleccionDoble pero solo el jugador 1 elige a su
        // personaje. Se usa, por ejemplo, en el modo escalera
        std::unordered_map<Jugador,std::string> comenzarEleccionSimple();

        // NUNCA SE COPIA UN SINGLETON
        MenuSeleccionPersonaje(MenuSeleccionPersonaje &otro) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const MenuSeleccionPersonaje &) = delete;

        // Devuelve la única instancia
        static MenuSeleccionPersonaje * unicaInstancia();
};