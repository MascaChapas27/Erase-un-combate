#pragma once

#include "Personaje.hpp"
#include "Enums.hpp"
#include "GUIPersonaje.hpp"
#include "Escenario.hpp"
#include "AnimacionAgrandable.hpp"
#include "ConectorOnline.hpp"
#include <optional>
#include <SFML/Network.hpp>

/*
    Esta clase define un combate, formado por dos jugadores, cada
    uno manejando un personaje. El combate tiene varias rondas,
    las cuales terminan cuando uno de los personajes llega a 0 puntos de
    vida. A las 2 rondas ganadas por parte de uno de los personajes, el
    combate termina.
*/
class Combate{
    private:

        // Personaje asignado al jugador 1
        Personaje personajeJugador1;

        // Personaje asignado al jugador 2
        Personaje personajeJugador2;

        // GUI para el personaje asignado al jugador 1
        GUIPersonaje GUIJugador1;

        // GUI para el personaje asignado al jugador 2
        GUIPersonaje GUIJugador2;

        // Escenario para el combate
        Escenario escenario;

        // Rectángulo negro que aparece antes de que empiece el combate y entre rondas
        sf::RectangleShape rectanguloOscuro;

        // Ronda actual
        int numeroRonda;

        // Cartel que aparece al principio de la ronda en el que pone "¿Todo listo?"
        std::shared_ptr<Animacion> cartelTodoListo;

        // Cartel que aparece al principio de la ronda en el que pone "¡A pelear!"
        std::shared_ptr<Animacion> cartelAPelear;

        // Cartel que aparece al terminar una ronda en el que pone "Jugador 1 gana"
        std::shared_ptr<Animacion> cartelJugador1Gana;

        // Cartel que aparece al terminar una ronda en el que pone "Jugador 2 gana"
        std::shared_ptr<Animacion> cartelJugador2Gana;

        // Cartel que aparece al terminar una ronda en el que pone "Empate"
        std::shared_ptr<Animacion> cartelEmpate;

        // Conector para jugar online
        std::optional<ConectorOnline> conector;

    private:
        // Devuelve todo a la normalidad para la siguiente ronda (excepto el contador
        // de rondas ganadas de cada GUI de personaje)
        void resetear();

        // Actualiza un frame del combate, pero solo cuando al menos uno de los personajes está preparando
        // su súper ataque (el fondo se oscurece y nadie se puede mover)
        void actualizarFramePreparandoSuper(std::list<std::shared_ptr<Animacion>> &efectos);

        // Actualiza un frame completamente normal y corriente del combate
        void actualizarFrameNormal(std::list<std::shared_ptr<Animacion>> &efectos);

        // Actualiza un frame cuando una ronda termina y uno de los personajes la gana o hay empate
        void actualizarFrameCelebracion(std::list<std::shared_ptr<Animacion>> &efectos);

        // Sé que el nombre es largo de cojones pero no se me ocurre otra cosa mejor. Actualiza como su propio nombre indica
        // los dos personajes, los efectos, las GUIs, el escenario y la ventana. No comprueba colisiones, y no inserta nuevos
        // efectos en la lista de efectos (los efectos nuevos se quedan en nuevosEfectos hasta que se vayan a meter luego)
        void actualizarPersonajesEfectosGuisEscenarioVentana(std::list<std::shared_ptr<Animacion>> &efectos, std::list<std::shared_ptr<Animacion>> &nuevosEfectos);

        // Procesa los eventos generados en la ventana actual e informa a los personajes para que se muevan si se está jugando offline
        void recibirEntradaPlayerVSPlayerOffline();

        // Como recibirEntradaPlayerVSPlayerOffline() pero esta vez se sincroniza con el rival para mandarle la entrada y que nos mande su entrada
        void recibirEntradaPlayerVSPlayerOnline();

        // Como recibirEntradaPlayerVSPlayerOffline() pero esta vez el jugador 2 es un bot
        void recibirEntradaPlayerVSBot();

    public:

        // Construye el combate en base a los nombres de los personajes
        // y del escenario en el que se va a pelear. También es necesario especificar la dirección IP del jugador
        // contra el que se va a luchar. Por defecto es un string vacío, por lo que será un combate offline. Además, el booleano lider
        // permite indicar si el jugador actual será el líder del combate (jugará en el lado izquierdo y su ordenador coordinará el combate)
        // o si será el invitado (jugará en el lado derecho y su ordenador seguirá las órdenes del líder)
        Combate(std::string nombrePersonajeJ1, std::string nombrePersonajeJ2, std::string nombreEscenario, sf::IpAddress direccionIP = sf::IpAddress(0,0,0,0), bool lider = false);

        // Comienza el combate (la clase pasa a tomar el control de
        // la ventana principal hasta que termine el combate)
        void comenzar();

};


