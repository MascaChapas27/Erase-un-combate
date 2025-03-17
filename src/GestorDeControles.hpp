#ifndef __GESTOR_DE_CONTROLES_HPP__
#define __GESTOR_DE_CONTROLES_HPP__

#include <SFML/Graphics.hpp>
#include "ContenedorDeRecursos.hpp"
#include "Enums.hpp"
#include <map>
#include <vector>

/*
    Clase que se encarga de la correspondencia entre jugadores y controles,
    además de indicar qué significan las teclas y botones que se pulsan
*/
class GestorDeControles
{
    private:
        // El constructor es privado porque la clase es Singleton
        GestorDeControles();

        ~GestorDeControles();

        // La única instancia
        static GestorDeControles * gestorDeControles;

        // Mapa que guarda, para cada control, el jugador al que está asignado
        std::map<Control, Jugador> controlAJugador;

        // Mapa que guarda, para cada tecla válida (excepto la tecla de salir), la parte del teclado y la acción asociada
        std::map<sf::Keyboard::Scancode,std::pair<Control,Accion>> teclaAControlYAccion;

        // Mapa que guarda, para cada jugador, otro mapa que indica qué acciones
        // está realizando con el joystick, para evitar mandar 819791837 eventos por mover
        // un joystick ligeramente un poco más hacia el lado cada vez
        std::map<Jugador,std::map<Accion,bool>> jugadorRealizandoAccionJoystick;

        // Función que indica si un control está libre
        bool estaLibre(Control c);

        // Permite establecer un mando para que lo use un jugador
        bool conectarMando(Jugador j, Control c);

    public:

        // Dado un evento, devuelve el jugador y la acción que está haciendo
        std::pair<Jugador,Accion> comprobarEvento(std::optional<sf::Event> evento);

        // NUNCA SE COPIA UN SINGLETON
        GestorDeControles(GestorDeControles &otro) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const GestorDeControles &) = delete;

        // Devuelve la única instancia
        static GestorDeControles * unicaInstancia();
};

#endif // __GESTOR_DE_CONTROLES_HPP__
