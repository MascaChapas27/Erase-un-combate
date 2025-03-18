#include "SelectorJugadorParaMando.hpp"
#include "Constantes.hpp"
#include "VentanaPrincipal.hpp"
#include "ContenedorDeRecursos.hpp"
#include "ReproductorDeMusica.hpp"
#include <iostream>

// La instancia es nula al principio
SelectorJugadorParaMando *SelectorJugadorParaMando::selectorJugadorParaMando = nullptr;

SelectorJugadorParaMando *SelectorJugadorParaMando::unicaInstancia()
{
    if (selectorJugadorParaMando == nullptr)
        selectorJugadorParaMando = new SelectorJugadorParaMando();
    return selectorJugadorParaMando;
}

SelectorJugadorParaMando::~SelectorJugadorParaMando()
{
    if (selectorJugadorParaMando != nullptr)
        delete selectorJugadorParaMando;
}

SelectorJugadorParaMando::SelectorJugadorParaMando() : spriteJugador1(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/selector-mando/jugador1.png")),
                                                       spriteJugador2(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/selector-mando/jugador2.png")),
                                                       sonidoAparecer(ContenedorDeSonidos::unicaInstancia()->obtener("sonidos/seleccionar-mando/aparece-ventana.wav")),
                                                       sonidoCambiarSeleccion(ContenedorDeSonidos::unicaInstancia()->obtener("sonidos/seleccionar-mando/cambiar-seleccion.wav")),
                                                       sonidoDesaparecer(ContenedorDeSonidos::unicaInstancia()->obtener("sonidos/seleccionar-mando/desaparece-ventana.wav"))
{
}

Jugador SelectorJugadorParaMando::decidirJugador(Control c)
{

    // Se guarda el volumen original y se baja el volumen de la música a la mitad
    float volumenOriginal = ReproductorDeMusica::unicaInstancia()->getVolumen();
    ReproductorDeMusica::unicaInstancia()->setVolumen(volumenOriginal / 2);

    // Se saca el tamaño de la ventana y se crea la textura con ese tamaño
    sf::Vector2u tamanoVentana = VentanaPrincipal::unicaInstancia()->getSize();
    sf::Texture texturaFondo({tamanoVentana.x, tamanoVentana.y});

    // Una vez se tiene la textura, se copia el contenido de la ventana
    texturaFondo.update(*VentanaPrincipal::unicaInstancia());

    // Se asigna la textura al sprite y se escala según el zoom para que el tamaño sea correcto
    sf::Sprite spriteFondo(texturaFondo);
    spriteFondo.scale(sf::Vector2f(1.0 / VentanaPrincipal::getZoom(), 1.0 / VentanaPrincipal::getZoom()));
    spriteFondo.setPosition({0, 0});

    sf::RenderWindow *ventana = VentanaPrincipal::unicaInstancia();

    Jugador jugadorSeleccionado = Jugador::JUGADOR1;

    bool jugadorDecidido = false;

    bool joystickMovido = false;

    sonidoAparecer.play();

    while (!jugadorDecidido && ventana->isOpen())
    {

        // Se comprueban los eventos como de costumbre
        while (const std::optional evento = ventana->pollEvent())
        {
            if (evento->is<sf::Event::Closed>())
            {
                exit(EXIT_SUCCESS);
            }
            else if (evento->is<sf::Event::JoystickButtonPressed>())
            {
                jugadorDecidido = true;
            }
            else if (evento->is<sf::Event::JoystickMoved>() &&
                     (evento->getIf<sf::Event::JoystickMoved>()->axis == sf::Joystick::Axis::PovX ||
                      evento->getIf<sf::Event::JoystickMoved>()->axis == sf::Joystick::Axis::X ||
                      evento->getIf<sf::Event::JoystickMoved>()->axis == sf::Joystick::Axis::R))
            {

                if (!joystickMovido && std::abs(evento->getIf<sf::Event::JoystickMoved>()->position) > UMBRAL_JOYSTICK)
                {
                    joystickMovido = true;
                    jugadorSeleccionado = (jugadorSeleccionado == Jugador::JUGADOR1 ? Jugador::JUGADOR2 : Jugador::JUGADOR1);
                    sonidoCambiarSeleccion.play();
                }
                else if (joystickMovido && std::abs(evento->getIf<sf::Event::JoystickMoved>()->position) < UMBRAL_JOYSTICK)
                {
                    joystickMovido = false;
                }
            }
        }

        // Se dibujan las cosas
        ventana->clear();
        ventana->draw(spriteFondo);
        ventana->draw(jugadorSeleccionado == Jugador::JUGADOR1 ? spriteJugador1 : spriteJugador2);
        ventana->display();
    }

    sonidoDesaparecer.play();

    // Se reestablece el volumen original
    ReproductorDeMusica::unicaInstancia()->setVolumen(volumenOriginal);

    return jugadorSeleccionado;
}