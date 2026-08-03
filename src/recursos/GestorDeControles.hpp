#pragma once

#include <SFML/Graphics.hpp>
#include "ContenedorDeRecursos.hpp"
#include "Enums.hpp"
#include <unordered_map>
#include <vector>

// Este struct informa sobre los detalles de un evento que ha sucedido y que permiten
// conocer qué ha ocurrido en el evento y quién lo ha hecho
struct InfoEvento
{
    // Jugador con el que está relacionado este evento
    Jugador jugador;

    // Acción con la que está relacionado este evento
    Accion accion;

    // Indica si la acción fue realizada (se pulsó un botón, por ejemplo)
    // o se dejó de realizar (se soltó un botón, por ejemplo)
    bool realizada;
};

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
    static GestorDeControles *gestorDeControles;

    // Mapa que guarda, para cada control, el jugador al que está asignado
    std::unordered_map<Control, Jugador> controlAJugador;

    // Mapa que guarda, para cada tecla válida (excepto la tecla de salir), la parte del teclado y la acción asociada
    std::unordered_map<sf::Keyboard::Scancode, std::pair<Control, Accion>> teclaAControlYAccion;

    // Mapa que guarda, para cada jugador, otro mapa que indica qué acciones
    // está realizando con el joystick, para evitar mandar 819791837 eventos por mover
    // un joystick ligeramente un poco más hacia el lado cada vez
    std::unordered_map<Jugador, std::unordered_map<Accion, bool>> jugadorRealizandoAccionJoystick;

    // Un flag que indica si actualmente se está asignando un mando a un jugador
    // utilizando la clase SelectorJugadorParaMando. En este caso, el funcionamiento
    // del método comprobarEvento es distinto, ya que se está utilizando un mando
    // que aún no tiene dueño
    bool asignandoJugadorAMando;

    // Si asignandoJugadorAMando está a true, controlSiendoAsignado indica cuál
    // es el mando que está actualmente siendo asignado a un jugador. De esta
    // forma, durante la pantalla de selección de personaje solo se permitirá
    // que este mando controle el juego. Los demás mandos tendrán que esperar
    // a que el mando actual sea asignado a un jugador
    Control controlSiendoAsignado;

    // Función que indica si un control está libre
    bool estaLibre(Control c);

    // Permite establecer un mando para que lo use un jugador
    bool conectarMando(Jugador j, Control c);

public:
    // Dado un evento, devuelve el jugador y la acción que está haciendo
    InfoEvento comprobarEvento(std::optional<sf::Event> evento);

    // Dado un jugador, devuelve qué control está usando actualmente
    Control obtenerControlUsadoPorJugador(Jugador jugador);

    // NUNCA SE COPIA UN SINGLETON
    GestorDeControles(GestorDeControles &otro) = delete;

    // NUNCA SE ASIGNA UN SINGLETON
    void operator=(const GestorDeControles &) = delete;

    // Devuelve la única instancia
    static GestorDeControles *unicaInstancia();
};
