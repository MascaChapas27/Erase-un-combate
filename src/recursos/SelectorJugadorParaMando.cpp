#include "SelectorJugadorParaMando.hpp"
#include "Constantes.hpp"
#include "VentanaPrincipal.hpp"
#include "ContenedorDeRecursos.hpp"
#include "ReproductorDeMusica.hpp"
#include "ReproductorDeSonidos.hpp"
#include "GestorDeControles.hpp"
#include "Utilidades.hpp"
#include "Configuracion.hpp"
#include "ClienteDiscord.hpp"

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

SelectorJugadorParaMando::SelectorJugadorParaMando() : spriteJugador1(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/selector-mando/jugador1.png"), FACTOR_APROXIMACION_SPRITES_SELECTOR_JUGADOR_MANDO),
                                                       spriteJugador2(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/selector-mando/jugador2.png"), FACTOR_APROXIMACION_SPRITES_SELECTOR_JUGADOR_MANDO),
                                                       spriteNadie(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/selector-mando/nadie.png"), FACTOR_APROXIMACION_SPRITES_SELECTOR_JUGADOR_MANDO)
{
    spriteJugador1.getSprite().setColor(sf::Color::Transparent);
    spriteJugador2.getSprite().setColor(sf::Color::Transparent);
    spriteNadie.getSprite().setColor(sf::Color::Transparent);

}

Jugador SelectorJugadorParaMando::decidirJugador(Control c)
{
    // Se guarda la canción que se estaba reproduciendo antes y se pausa
    std::string cancionAnterior = ReproductorDeMusica::unicaInstancia()->getCancionActual();
    ReproductorDeMusica::unicaInstancia()->pausar();

    // Se saca el tamaño de la ventana y se crea la textura con ese tamaño
    sf::Vector2u tamanoVentana = VentanaPrincipal::unicaInstancia()->getSize();
    sf::Texture texturaFondo({tamanoVentana.x, tamanoVentana.y});

    // Una vez se tiene la textura, se copia el contenido de la ventana
    texturaFondo.update(*VentanaPrincipal::unicaInstancia());

    // Se asigna la textura al sprite y se escala según el zoom para que el tamaño sea correcto
    sf::Sprite spriteFondo(texturaFondo);
    spriteFondo.scale(sf::Vector2f(1.0 / VentanaPrincipal::getZoom(), 1.0 / VentanaPrincipal::getZoom()));
    spriteFondo.setPosition({0, 0});

    // Se crea un rectángulo negro del tamaño de la ventana para poder oscurecer el fondo
    sf::RectangleShape rectanguloOscuro(sf::Vector2f(VENTANA_ANCHURA,VENTANA_ALTURA));
    rectanguloOscuro.setFillColor(COLOR_INICIAL_RECTANGULO_OSCURO_SELECTOR_JUGADOR_MANDO);

    sf::RenderWindow *ventana = VentanaPrincipal::unicaInstancia();

    Jugador jugadorSeleccionado = Jugador::JUGADOR1;

    bool jugadorDecidido = false;

    ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/seleccionar-mando/aparece-ventana.ogg");
    ReproductorDeMusica::unicaInstancia()->reproducir("musica/selector-mando.ogg");

    while (!jugadorDecidido && ventana->isOpen())
    {
        // Se prepara un reloj para ver cuánto tiempo pasa entre fotogramas
        sf::Clock reloj;

        // Se ejecutan cosillas de Discord en cada frame
        discordpp::RunCallbacks();

        // Se comprueban los eventos como de costumbre
        while (const std::optional evento = ventana->pollEvent())
        {
            if(evento->is<sf::Event::Closed>())
            {
                exit(EXIT_SUCCESS);
            }
            else
            {
                InfoEvento infoEvento = GestorDeControles::unicaInstancia()->comprobarEvento(evento);
                if (infoEvento.accion == Accion::ESCAPE)
                {
                    exit(EXIT_SUCCESS);
                }
                else if (infoEvento.accion == Accion::ATACAR && infoEvento.realizada)
                {
                    jugadorDecidido = true;
                }
                else if ((infoEvento.accion == Accion::ABAJO) && infoEvento.realizada)
                {
                    switch(jugadorSeleccionado)
                    {
                        case Jugador::JUGADOR1:
                            jugadorSeleccionado = Jugador::JUGADOR2;
                            break;
                        
                        case Jugador::JUGADOR2:
                            jugadorSeleccionado = Jugador::NADIE;
                            break;

                        case Jugador::NADIE:
                            jugadorSeleccionado = Jugador::JUGADOR1;
                            break;
                    }

                    // Se reproduce el sonido de cambiar selección
                    ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/seleccionar-mando/cambiar-seleccion.ogg");
                }
                else if ((infoEvento.accion == Accion::ARRIBA) && infoEvento.realizada)
                {
                    switch(jugadorSeleccionado)
                    {
                        case Jugador::JUGADOR1:
                            jugadorSeleccionado = Jugador::NADIE;
                            break;
                        
                        case Jugador::JUGADOR2:
                            jugadorSeleccionado = Jugador::JUGADOR1;
                            break;

                        case Jugador::NADIE:
                            jugadorSeleccionado = Jugador::JUGADOR2;
                            break;
                    }

                    // Se reproduce el sonido de cambiar selección
                    ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/seleccionar-mando/cambiar-seleccion.ogg");
                }
            }
        }

        // Se pone más claro el sprite de selección de jugador
        spriteJugador1.actualizar();
        spriteJugador2.actualizar();
        spriteNadie.actualizar();

        // Se pone más oscuro el rectángulo negro del fondo
        rectanguloOscuro.setFillColor(util::aproximarColor(rectanguloOscuro.getFillColor(),COLOR_FINAL_RECTANGULO_OSCURO_SELECTOR_JUGADOR_MANDO,FACTOR_APROXIMACION_SPRITES_SELECTOR_JUGADOR_MANDO));

        // Se dibujan las cosas
        ventana->clear();
        ventana->draw(spriteFondo);
        ventana->draw(rectanguloOscuro);
        ventana->draw(jugadorSeleccionado == Jugador::JUGADOR1 ? spriteJugador1 : (jugadorSeleccionado == Jugador::JUGADOR2 ? spriteJugador2 : spriteNadie));
        ventana->display();

        // El juego se duerme hasta que dé tiempo a dibujar el siguiente fotograma, teniendo en cuenta
        // que se deben dibujar 60 fotogramas por segundo y que cada fotograma además necesita un tiempo
        // previo de preparación para actualizar y dibujar y tal
        sf::sleep(sf::seconds(1.f / Configuracion::unicaInstancia()->getFPS()) - reloj.reset());
    }

    // Si se ha seleccionado un jugador para el mando, se reproduce un sonidito
    if(jugadorSeleccionado != Jugador::NADIE)
        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/seleccionar-mando/jugador-seleccionado.ogg");
    else
        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/seleccionar-mando/nadie-seleccionado.ogg");

    // Se para esta canción y se reanuda la anterior
    ReproductorDeMusica::unicaInstancia()->detener();
    ReproductorDeMusica::unicaInstancia()->reanudar(cancionAnterior);

    return jugadorSeleccionado;
}