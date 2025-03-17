#include "GestorDeControles.hpp"
#include "Constantes.hpp"
#include "SelectorJugadorParaMando.hpp"
#include <iostream>

// La instancia es nula al principio
GestorDeControles * GestorDeControles::gestorDeControles = nullptr;

GestorDeControles * GestorDeControles::unicaInstancia()
{
    if(gestorDeControles == nullptr)
        gestorDeControles = new GestorDeControles();
    return gestorDeControles;
}

GestorDeControles::~GestorDeControles(){
    if(gestorDeControles != nullptr) delete gestorDeControles;
}

GestorDeControles::GestorDeControles()
{
    // Teclas, controles y acciones para la parte izquierda del teclado
    teclaAControlYAccion[sf::Keyboard::Scancode::S] = std::pair<Control,Accion>(Control::TECLADO_IZQUIERDA,Accion::ABAJO);
    teclaAControlYAccion[sf::Keyboard::Scancode::W] = std::pair<Control,Accion>(Control::TECLADO_IZQUIERDA,Accion::ARRIBA);
    teclaAControlYAccion[sf::Keyboard::Scancode::A] = std::pair<Control,Accion>(Control::TECLADO_IZQUIERDA,Accion::IZQUIERDA);
    teclaAControlYAccion[sf::Keyboard::Scancode::D] = std::pair<Control,Accion>(Control::TECLADO_IZQUIERDA,Accion::DERECHA);
    teclaAControlYAccion[sf::Keyboard::Scancode::LShift] = std::pair<Control,Accion>(Control::TECLADO_IZQUIERDA,Accion::ATACAR);

    // Teclas, controles y acciones para la parte izquierda del teclado
    teclaAControlYAccion[sf::Keyboard::Scancode::K] = std::pair<Control,Accion>(Control::TECLADO_DERECHA,Accion::ABAJO);
    teclaAControlYAccion[sf::Keyboard::Scancode::I] = std::pair<Control,Accion>(Control::TECLADO_DERECHA,Accion::ARRIBA);
    teclaAControlYAccion[sf::Keyboard::Scancode::J] = std::pair<Control,Accion>(Control::TECLADO_DERECHA,Accion::IZQUIERDA);
    teclaAControlYAccion[sf::Keyboard::Scancode::L] = std::pair<Control,Accion>(Control::TECLADO_DERECHA,Accion::DERECHA);
    teclaAControlYAccion[sf::Keyboard::Scancode::Space] = std::pair<Control,Accion>(Control::TECLADO_DERECHA,Accion::ATACAR);

    // En un principio ningún jugador está moviendo el joystick porque ninguno usa mando
    // (y aunque lo usaran, en un principio nadie debería estar haciendo nada)
    jugadorRealizandoAccionJoystick[Jugador::JUGADOR1][Accion::ABAJO] = false;
    jugadorRealizandoAccionJoystick[Jugador::JUGADOR1][Accion::IZQUIERDA] = false;
    jugadorRealizandoAccionJoystick[Jugador::JUGADOR1][Accion::DERECHA] = false;
    jugadorRealizandoAccionJoystick[Jugador::JUGADOR1][Accion::ARRIBA] = false;
    jugadorRealizandoAccionJoystick[Jugador::JUGADOR2][Accion::ABAJO] = false;
    jugadorRealizandoAccionJoystick[Jugador::JUGADOR2][Accion::IZQUIERDA] = false;
    jugadorRealizandoAccionJoystick[Jugador::JUGADOR2][Accion::DERECHA] = false;
    jugadorRealizandoAccionJoystick[Jugador::JUGADOR2][Accion::ARRIBA] = false;

    // Al principio los controles no están asociados a nadie
    for(int i=0;i<NUMERO_CONTROLES;i++){
        controlAJugador[static_cast<Control>(i)] = Jugador::NADIE;
    }

    // Bueno sí, hay dos que sí están asociados al principio
    controlAJugador[Control::TECLADO_IZQUIERDA] = Jugador::JUGADOR1;
    controlAJugador[Control::TECLADO_DERECHA] = Jugador::JUGADOR2;
}

bool GestorDeControles::estaLibre(Control c){
    return controlAJugador[c] == Jugador::NADIE;
}

bool GestorDeControles::conectarMando(Jugador j, Control c){

    // Si no es un mando, nos vamos directamente
    if(c == Control::TECLADO_DERECHA || c == Control::TECLADO_IZQUIERDA)
        return false;

    // Si ya está pillado, también
    if(!estaLibre(c)) return false;

    // Buscamos el control que sea que está usando el jugador y lo deshabilitamos
    // (Gracias a las maravillas de C y C++ puedo evitar usar llaves aquí)
    for(int i=0;i<NUMERO_CONTROLES;i++)
        if(controlAJugador[static_cast<Control>(i)] == j)
            controlAJugador[static_cast<Control>(i)] = Jugador::NADIE;

    // Por último, asignamos el control y ya está
    controlAJugador[c] = j;

    return true;
}

std::pair<Jugador,Accion> GestorDeControles::comprobarEvento(std::optional<sf::Event> evento)
{
    // Este es el par que va a ser devuelto (empieza vacío)
    std::pair<Jugador,Accion> pair(Jugador::NADIE,Accion::NADA);

    if(evento->is<sf::Event::JoystickButtonPressed>() || evento->is<sf::Event::JoystickButtonReleased>()){
        // Alguien ha pulsado un botón de mando (el botón da igual, todos
        // hacen lo mismo). Se le suma 2 al numerito del control porque los dos
        // primeros controles son la parte izquierda del teclado y la parte derecha,
        // por lo que el control 3 es el mando 0 y así sucesivamente

        Control control;
        if(evento->is<sf::Event::JoystickButtonPressed>()) {
            control = static_cast<Control>(evento->getIf<sf::Event::JoystickButtonPressed>()->joystickId+2);
        } else {
            control = static_cast<Control>(evento->getIf<sf::Event::JoystickButtonReleased>()->joystickId+2);
        }


        pair.first = controlAJugador[control];
        if(pair.first != Jugador::NADIE){
            // Si el control es de alguien, se actualiza el par
            pair.second = Accion::ATACAR;
        } else {
            // Si el control no está asignado a un jugador, se comprueba si ninguno tiene mando
            Jugador jugadorTecladoIzq = controlAJugador[Control::TECLADO_IZQUIERDA];
            Jugador jugadorTecladoDer = controlAJugador[Control::TECLADO_DERECHA];

            if(jugadorTecladoIzq != Jugador::NADIE && jugadorTecladoDer == Jugador::NADIE){
                // Si uno tiene mando pero el otro no, se asigna al otro
                conectarMando(jugadorTecladoIzq,control);
            } else if(jugadorTecladoIzq == Jugador::NADIE && jugadorTecladoDer != Jugador::NADIE){
                // Si uno tiene mando pero el otro no, se asigna al otro
                conectarMando(jugadorTecladoDer,control);
            } else if(jugadorTecladoIzq != Jugador::NADIE && jugadorTecladoDer != Jugador::NADIE){
                // Si ninguno tiene mando, se pregunta para quién es el mando
                Jugador jugadorConMando = SelectorJugadorParaMando::unicaInstancia()->decidirJugador(control);
                conectarMando(jugadorConMando,control);
            } 
        }

    } else if (evento->is<sf::Event::JoystickMoved>()){
        // Alguien ha movido un joystick
        Control control = static_cast<Control>(evento->getIf<sf::Event::JoystickMoved>()->joystickId+2);

        // Se saca el jugador correspondiente al mando. Si hay jugador,
        // se hacen cosas
        pair.first = controlAJugador[control];
        if(pair.first != Jugador::NADIE){
            // Dependiendo del eje, se decide hacia dónde se mueve
            switch(evento->getIf<sf::Event::JoystickMoved>()->axis){
                // PovX, X y R son los ejes X de tres posibles entradas
                // PovX es la cruceta, X es el joystick izquierdo y R el joystick derecho
                case sf::Joystick::Axis::PovX:
                case sf::Joystick::Axis::X:
                case sf::Joystick::Axis::R:

                    if(evento->getIf<sf::Event::JoystickMoved>()->position > UMBRAL_JOYSTICK && !jugadorRealizandoAccionJoystick[pair.first][Accion::DERECHA]){
                        pair.second = Accion::DERECHA;
                        jugadorRealizandoAccionJoystick[pair.first][Accion::DERECHA] = true;
                    } else if(evento->getIf<sf::Event::JoystickMoved>()->position < -UMBRAL_JOYSTICK && !jugadorRealizandoAccionJoystick[pair.first][Accion::IZQUIERDA]){
                        pair.second = Accion::IZQUIERDA;
                        jugadorRealizandoAccionJoystick[pair.first][Accion::IZQUIERDA] = true;
                    } else if (std::abs(evento->getIf<sf::Event::JoystickMoved>()->position) < UMBRAL_JOYSTICK){
                        if(jugadorRealizandoAccionJoystick[pair.first][Accion::DERECHA]){
                            pair.second = Accion::DERECHA;
                            jugadorRealizandoAccionJoystick[pair.first][Accion::DERECHA] = false;
                        } else if(jugadorRealizandoAccionJoystick[pair.first][Accion::IZQUIERDA]){
                            pair.second = Accion::IZQUIERDA;
                            jugadorRealizandoAccionJoystick[pair.first][Accion::IZQUIERDA] = false;
                        }
                    }
                    break;

                // PovY, Y y U son los ejes Y de tres posibles entradas
                // PovY es la cruceta, Y es el joystick izquierdo y U el joystick derecho
                case sf::Joystick::Axis::PovY:
                case sf::Joystick::Axis::Y:
                case sf::Joystick::Axis::U:
                    if(evento->getIf<sf::Event::JoystickMoved>()->position < -UMBRAL_JOYSTICK && !jugadorRealizandoAccionJoystick[pair.first][Accion::ABAJO]){
                        pair.second = Accion::ABAJO;
                        jugadorRealizandoAccionJoystick[pair.first][Accion::ABAJO] = true;
                    } else if(evento->getIf<sf::Event::JoystickMoved>()->position > UMBRAL_JOYSTICK && !jugadorRealizandoAccionJoystick[pair.first][Accion::ARRIBA]){
                        pair.second = Accion::ARRIBA;
                        jugadorRealizandoAccionJoystick[pair.first][Accion::ARRIBA] = true;
                    } else if (std::abs(evento->getIf<sf::Event::JoystickMoved>()->position) < UMBRAL_JOYSTICK){
                        if(jugadorRealizandoAccionJoystick[pair.first][Accion::ABAJO]){
                            pair.second = Accion::ABAJO;
                            jugadorRealizandoAccionJoystick[pair.first][Accion::ABAJO] = false;
                        } else if(jugadorRealizandoAccionJoystick[pair.first][Accion::ARRIBA]){
                            pair.second = Accion::ARRIBA;
                            jugadorRealizandoAccionJoystick[pair.first][Accion::ARRIBA] = false;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    } else if (evento->is<sf::Event::KeyPressed>() || evento->is<sf::Event::KeyReleased>()){
        // Alguien ha pulsado una tecla

        // Si es la tecla de salida tiene solución fácil

        sf::Keyboard::Scancode scancode;
        
        if(evento->is<sf::Event::KeyPressed>()){
            scancode = evento->getIf<sf::Event::KeyPressed>()->scancode;
        } else {
            scancode = evento->getIf<sf::Event::KeyReleased>()->scancode;
        }

        if(scancode == TECLA_SALIDA)
            pair.second = Accion::ESCAPE;
        else {
            // Si es otra tecla, hay que ver si es una de las que nos interesa
            if(teclaAControlYAccion.count(scancode)){
                // Si es una tecla que tenemos registrada, se comprueba su control y su acción
                Control c = teclaAControlYAccion[scancode].first;
                Accion a = teclaAControlYAccion[scancode].second;

                // Se asigna el personaje y la acción
                pair.first = controlAJugador[c];
                pair.second = a;
                // Si el personaje es NADIE es porque nadie tiene ese control, así
                // que todo lo que hemos hecho ha sido perder el tiempo
                if(pair.first == Jugador::NADIE){
                    pair.second = Accion::NADA;
                }
            }
        }
    }

    // Después de este pifostio devolvemos el par
    return pair;
}
