#include "GestorDeControles.hpp"
#include "Constantes.hpp"
#include "SelectorJugadorParaMando.hpp"
#include "Configuracion.hpp"
#include "VentanaPrincipal.hpp"
#include "ReproductorDeMusica.hpp"
#include "ReproductorDeSonidos.hpp"
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

InfoEvento GestorDeControles::comprobarEvento(std::optional<sf::Event> evento)
{
    // Este es el InfoEvento que va a ser devuelto (empieza vacío)
    InfoEvento infoEvento{Jugador::NADIE, Accion::NADA, false};

    if(evento->is<sf::Event::JoystickButtonPressed>() || evento->is<sf::Event::JoystickButtonReleased>())
    {
        // Esto es para debug. Se imprime el código asociado al botón pulsado
        if(evento->is<sf::Event::JoystickButtonPressed>())
            std::cerr << "Botón pulsado: " << evento->getIf<sf::Event::JoystickButtonPressed>()->button << "\n";

        // Alguien ha pulsado un botón de mando (el botón da igual, todos
        // hacen lo mismo). Se le suma 2 al numerito del control porque los dos
        // primeros controles son la parte izquierda del teclado y la parte derecha,
        // por lo que el control 3 es el mando 0 y así sucesivamente

        Control control;
        unsigned int boton;
        if(evento->is<sf::Event::JoystickButtonPressed>()) {
            control = static_cast<Control>(evento->getIf<sf::Event::JoystickButtonPressed>()->joystickId+2);
            boton = evento->getIf<sf::Event::JoystickButtonPressed>()->button;
        } else {
            control = static_cast<Control>(evento->getIf<sf::Event::JoystickButtonReleased>()->joystickId+2);
            boton = evento->getIf<sf::Event::JoystickButtonReleased>()->button;
        }


        infoEvento.jugador = controlAJugador[control];
        if(infoEvento.jugador != Jugador::NADIE){
            if(boton == MANDO_BOTON_ATACAR) infoEvento.accion = Accion::ATACAR;
            else infoEvento.accion = Accion::NADA;
            infoEvento.realizada = evento->is<sf::Event::JoystickButtonPressed>();
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
        infoEvento.jugador = controlAJugador[control];
        if(infoEvento.jugador != Jugador::NADIE){

            // Se saca la posición del joystick de antemano
            float posicionJoystick = evento->getIf<sf::Event::JoystickMoved>()->position;

            // Dependiendo del eje, se decide hacia dónde se mueve
            switch(evento->getIf<sf::Event::JoystickMoved>()->axis){
                // PovX y X son los ejes X de dos posibles entradas
                // PovX es la cruceta y X es el joystick izquierdo
                case sf::Joystick::Axis::X:
                case sf::Joystick::Axis::PovX:

                    if(posicionJoystick > UMBRAL_JOYSTICK && !jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::DERECHA]){
                        infoEvento.accion = Accion::DERECHA;
                        jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::DERECHA] = true;
                        infoEvento.realizada = true;
                    } else if(posicionJoystick < -UMBRAL_JOYSTICK && !jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::IZQUIERDA]){
                        infoEvento.accion = Accion::IZQUIERDA;
                        jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::IZQUIERDA] = true;
                        infoEvento.realizada = true;
                    } else if (std::abs(posicionJoystick) < UMBRAL_JOYSTICK){
                        if(jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::DERECHA]){
                            infoEvento.accion = Accion::DERECHA;
                            jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::DERECHA] = false;
                            infoEvento.realizada = false;
                        } else if(jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::IZQUIERDA]){
                            infoEvento.accion = Accion::IZQUIERDA;
                            jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::IZQUIERDA] = false;
                            infoEvento.realizada = false;
                        }
                    }
                    break;

                // PovY e Y son los ejes Y de dos posibles entradas
                // PovY es la cruceta e Y es el joystick izquierdo
                // Por algún motivo el eje Y está invertido, así que no queda otra
                case sf::Joystick::Axis::Y:
                    posicionJoystick = -posicionJoystick;

                case sf::Joystick::Axis::PovY:
                    if(posicionJoystick < -UMBRAL_JOYSTICK && !jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::ABAJO]){
                        infoEvento.accion = Accion::ABAJO;
                        jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::ABAJO] = true;
                        infoEvento.realizada = true;
                    } else if(posicionJoystick > UMBRAL_JOYSTICK && !jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::ARRIBA]){
                        infoEvento.accion = Accion::ARRIBA;
                        jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::ARRIBA] = true;
                        infoEvento.realizada = true;
                    } else if (std::abs(posicionJoystick) < UMBRAL_JOYSTICK){
                        if(jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::ABAJO]){
                            infoEvento.accion = Accion::ABAJO;
                            jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::ABAJO] = false;
                            infoEvento.realizada = false;
                        } else if(jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::ARRIBA]){
                            infoEvento.accion = Accion::ARRIBA;
                            jugadorRealizandoAccionJoystick[infoEvento.jugador][Accion::ARRIBA] = false;
                            infoEvento.realizada = false;
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

        if(scancode == TECLA_SALIDA){
            infoEvento.jugador = Jugador::JUGADOR1;
            infoEvento.accion = Accion::ESCAPE;
            infoEvento.realizada = evento->is<sf::Event::KeyPressed>();
        } else {
            // Si es otra tecla, hay que ver si es una de las que nos interesa
            if(teclaAControlYAccion.count(scancode)){
                // Si es una tecla que tenemos registrada, se comprueba su control y su acción
                Control c = teclaAControlYAccion[scancode].first;
                Accion a = teclaAControlYAccion[scancode].second;

                // Se asigna el personaje y la acción
                infoEvento.jugador = controlAJugador[c];
                infoEvento.accion = a;
                infoEvento.realizada = evento->is<sf::Event::KeyPressed>();
            }
            // Se comprueba si se ha pulsado una tecla de debug
            else if(evento->is<sf::Event::KeyPressed>())
            {
                switch(scancode)
                {
                    case sf::Keyboard::Scancode::Num1:
                        if(Configuracion::unicaInstancia()->getFPS() != NUMERO_FPS_INICIAL/3)
                        {
                            Configuracion::unicaInstancia()->setFPS(NUMERO_FPS_INICIAL/3);
                            ReproductorDeMusica::unicaInstancia()->setTono(TONO_MUSICA_RALENTIZADA);
                            ReproductorDeSonidos::unicaInstancia()->setTono(TONO_MUSICA_RALENTIZADA);
                        }
                        else
                        {
                            Configuracion::unicaInstancia()->setFPS(NUMERO_FPS_INICIAL);
                            ReproductorDeMusica::unicaInstancia()->setTono(TONO_MUSICA_NORMAL);
                            ReproductorDeSonidos::unicaInstancia()->setTono(TONO_MUSICA_NORMAL);
                        }
                        break;

                    case sf::Keyboard::Scancode::Num2:
                        if(Configuracion::unicaInstancia()->getFPS() != NUMERO_FPS_INICIAL*3)
                        {
                            Configuracion::unicaInstancia()->setFPS(NUMERO_FPS_INICIAL*3);
                            ReproductorDeMusica::unicaInstancia()->setTono(TONO_MUSICA_ACELERADA);
                            ReproductorDeSonidos::unicaInstancia()->setTono(TONO_MUSICA_ACELERADA);
                        }
                        else
                        {
                            Configuracion::unicaInstancia()->setFPS(NUMERO_FPS_INICIAL);
                            ReproductorDeMusica::unicaInstancia()->setTono(TONO_MUSICA_NORMAL);
                            ReproductorDeSonidos::unicaInstancia()->setTono(TONO_MUSICA_NORMAL);
                        }
                        break;
                    
                    case sf::Keyboard::Scancode::Num3:
                        Configuracion::unicaInstancia()->setHitboxesVisibles(!Configuracion::unicaInstancia()->isHitboxesVisibles());
                        break;

                    case sf::Keyboard::Scancode::Num4:
                        Configuracion::unicaInstancia()->setSalidaABitacora(!Configuracion::unicaInstancia()->isSalidaABitacora());
                        break;
                    
                    case sf::Keyboard::Scancode::Down:
                        VentanaPrincipal::disminuirZoom();
                        break;
                    
                    case sf::Keyboard::Scancode::Up:
                        VentanaPrincipal::aumentarZoom();
                        break;
                }
            }
        }
    }

    // Si el personaje es NADIE, la acción sí o sí tiene que ser NADA y viceversa
    if(infoEvento.jugador == Jugador::NADIE){
        infoEvento.accion = Accion::NADA;
    } else if(infoEvento.accion == Accion::NADA){
        infoEvento.jugador = Jugador::NADIE;
    }

    // Después de este pifostio devolvemos el par
    return infoEvento;
}
