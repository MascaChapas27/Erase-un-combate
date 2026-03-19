#include "MenuSeleccionPersonaje.hpp"
#include "Constantes.hpp"
#include "ReproductorDeMusica.hpp"
#include "VentanaPrincipal.hpp"
#include "GestorDeControles.hpp"
#include "Configuracion.hpp"
#include "Utilidades.hpp"
#include "ContenedorDePersonajes.hpp"
#include "ReproductorDeSonidos.hpp"

#include <algorithm>

// La instancia es nula al principio
MenuSeleccionPersonaje * MenuSeleccionPersonaje::menuSeleccionPersonaje = nullptr;

MenuSeleccionPersonaje * MenuSeleccionPersonaje::unicaInstancia()
{
    if(menuSeleccionPersonaje == nullptr)
        menuSeleccionPersonaje = new MenuSeleccionPersonaje();
    return menuSeleccionPersonaje;
}

MenuSeleccionPersonaje::~MenuSeleccionPersonaje(){
    if(menuSeleccionPersonaje != nullptr) delete menuSeleccionPersonaje;
}

MenuSeleccionPersonaje::MenuSeleccionPersonaje() :
indiceJugador1(0), indiceJugador2(1), personajeElegidoJugador1(false), personajeElegidoJugador2(false),
spriteMarco(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/marco.png")),
rectanguloNegro({VENTANA_ANCHURA,VENTANA_ALTURA})
{
    rectanguloNegro.setFillColor(sf::Color::Black);

    // A la hora de colocar los selectores de personaje, la posición relativa
    // del primero para el jugador 1 será 0, y la posición relativa del primero
    // para el jugador 2 será -1. Luego se irán aumentando en 1 las posiciones
    // relativas por cada personaje introducido
    int posicionRelativaJugador1 = -indiceJugador1;
    int posicionRelativaJugador2 = -indiceJugador2;

    for(const std::string& nombrePersonaje : ContenedorDePersonajes::unicaInstancia()->obtenerNombresPersonajes())
    {
        sf::Texture& texturaPortraitPersonaje = ContenedorDeTexturas::unicaInstancia()->obtener("sprites/personajes/"+nombrePersonaje+"/portrait.png");

        selectoresPersonajeJugador1.emplace_back(texturaPortraitPersonaje, nombrePersonaje, Jugador::JUGADOR1, posicionRelativaJugador1);
        selectoresPersonajeJugador2.emplace_back(texturaPortraitPersonaje, nombrePersonaje, Jugador::JUGADOR2, posicionRelativaJugador2);

        sf::Texture& texturaFondoPersonaje = ContenedorDeTexturas::unicaInstancia()->obtener("sprites/personajes/"+nombrePersonaje+"/fondo-seleccion-personaje.png");

        fondosPersonajeJugador1.emplace_back(texturaFondoPersonaje, nombrePersonaje, Jugador::JUGADOR1, posicionRelativaJugador1);
        fondosPersonajeJugador2.emplace_back(texturaFondoPersonaje, nombrePersonaje, Jugador::JUGADOR2, posicionRelativaJugador2);

        posicionRelativaJugador1++;
        posicionRelativaJugador2++;
    }

    resetear();
}

void MenuSeleccionPersonaje::resetear()
{
    indiceJugador1 = 0;
    indiceJugador2 = 1;
    personajeElegidoJugador1 = false;
    personajeElegidoJugador2 = false;

    for(int i=0;i<selectoresPersonajeJugador1.size();i++)
    {
        selectoresPersonajeJugador1[i].resetear(i-indiceJugador1);
        selectoresPersonajeJugador2[i].resetear(i-indiceJugador2);

        fondosPersonajeJugador1[i].resetear(i-indiceJugador1);
        fondosPersonajeJugador2[i].resetear(i-indiceJugador2);
    }
}

std::unordered_map<Jugador,std::string> MenuSeleccionPersonaje::comenzarEleccionDoble()
{
    // Se resetea todo antes de empezar por si acaso
    resetear();

    ReproductorDeMusica::unicaInstancia()->reproducir("musica/menu-eleccion-personaje.ogg");

    sf::RenderWindow * ventana = VentanaPrincipal::unicaInstancia();

    // Esta variable indica si se está saliendo del menú de selección de personaje, ya sea
    // porque se ha cancelado la selección y se ha vuelto hacia atrás, o porque se han
    // elegido a los personajes y va a comenzar el combate
    bool saliendo = false;

    // El mapa que se va a devolver, con el personaje que ha elegido cada jugador
    std::unordered_map<Jugador,std::string> personajesElegidos;

    // Aquí se guardan las animaciones que se van mostrando
    std::list<std::shared_ptr<Animacion>> animaciones;

    while(!(saliendo && rectanguloNegro.getFillColor().a == 255)){
        // Se prepara un reloj para ver cuánto tiempo pasa entre fotogramas
        sf::Clock reloj;

        while(const std::optional evento = ventana->pollEvent())
        {
            if(evento->is<sf::Event::Closed>()){
                exit(EXIT_SUCCESS);
            }

            InfoEvento infoEvento(GestorDeControles::unicaInstancia()->comprobarEvento(evento));

            if(infoEvento.accion == Accion::ESCAPE && infoEvento.realizada)
            {
                saliendo = true;
                ReproductorDeMusica::unicaInstancia()->detener();
            }
            else if(infoEvento.accion == Accion::DERECHA && infoEvento.realizada)
            {
                if(infoEvento.jugador == Jugador::JUGADOR1 && !personajeElegidoJugador1 && indiceJugador1 < selectoresPersonajeJugador1.size()-1)
                {
                    ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-1-derecha.ogg");
                    indiceJugador1++;
                }
                else if(infoEvento.jugador == Jugador::JUGADOR2 && !personajeElegidoJugador2 && indiceJugador2 < selectoresPersonajeJugador2.size()-1)
                {
                    ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-2-derecha.ogg");
                    indiceJugador2++;
                }
            }
            else if(infoEvento.accion == Accion::IZQUIERDA && infoEvento.realizada)
            {
                if(infoEvento.jugador == Jugador::JUGADOR1 && !personajeElegidoJugador1 && indiceJugador1 > 0)
                {
                    ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-1-izquierda.ogg");
                    indiceJugador1--;
                }
                else if(infoEvento.jugador == Jugador::JUGADOR2 && !personajeElegidoJugador2 && indiceJugador2 > 0)
                {
                    ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-2-izquierda.ogg");
                    indiceJugador2--;
                }
            }
            else if(infoEvento.accion == Accion::ATACAR && infoEvento.realizada)
            {
                if(infoEvento.jugador == Jugador::JUGADOR1)
                {
                    if(!personajeElegidoJugador1)
                    {
                        std::list<std::shared_ptr<Animacion>> nuevasAnimaciones;

                        personajeElegidoJugador1 = selectoresPersonajeJugador1[indiceJugador1].seleccionar(nuevasAnimaciones);
                        
                        animaciones.splice(animaciones.end(),nuevasAnimaciones);

                        if(personajeElegidoJugador1)
                        {
                            fondosPersonajeJugador1[indiceJugador1].seleccionar();

                            ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-1-elegir.ogg");
                            
                            personajesElegidos[Jugador::JUGADOR1] = selectoresPersonajeJugador1[indiceJugador1].getNombrePersonaje();
                        }
                    }
                    else if(!saliendo)
                    {
                        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-1-rechazar.ogg");
                        personajesElegidos.erase(Jugador::JUGADOR1);
                        personajeElegidoJugador1 = false;
                    }
                }
                else if(infoEvento.jugador == Jugador::JUGADOR2)
                {
                    if(!personajeElegidoJugador2)
                    {
                        std::list<std::shared_ptr<Animacion>> nuevasAnimaciones;

                        personajeElegidoJugador2 = selectoresPersonajeJugador2[indiceJugador2].seleccionar(nuevasAnimaciones);

                        animaciones.splice(animaciones.end(),nuevasAnimaciones);

                        if(personajeElegidoJugador2)
                        {
                            fondosPersonajeJugador2[indiceJugador2].seleccionar();

                            ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-2-elegir.ogg");
                            
                            personajesElegidos[Jugador::JUGADOR2] = selectoresPersonajeJugador2[indiceJugador2].getNombrePersonaje();
                        }
                    }
                    else if(!saliendo)
                    {
                        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-2-rechazar.ogg");
                        personajesElegidos.erase(Jugador::JUGADOR2);
                        personajeElegidoJugador2 = false;
                    }
                }
            }
        }

        // Asumimos que selectoresPersonajeJugador1 tiene la misma longitud
        // que selectoresPersonajeJugador2 porque se supone que ambos jugadores
        // tienen acceso a los mismos personajes
        for(int i=0; i<selectoresPersonajeJugador1.size(); i++)
        {
            selectoresPersonajeJugador1[i].setPosicionRelativa(i-indiceJugador1);
            selectoresPersonajeJugador2[i].setPosicionRelativa(i-indiceJugador2);

            selectoresPersonajeJugador1[i].actualizar();
            selectoresPersonajeJugador2[i].actualizar();

            fondosPersonajeJugador1[i].setPosicionRelativa(i-indiceJugador1);
            fondosPersonajeJugador2[i].setPosicionRelativa(i-indiceJugador2);

            fondosPersonajeJugador1[i].actualizar();
            fondosPersonajeJugador2[i].actualizar();
        }

        // Si ambos jugadores han elegido ya a su personaje, nos vamos yendo
        if(personajeElegidoJugador1 && personajeElegidoJugador2)
        {
            ReproductorDeMusica::unicaInstancia()->detener();
            saliendo = true;
        }

        // Se cambia la opacidad del rectángulo negro según sea necesario
        if(rectanguloNegro.getFillColor().a > 0 && !saliendo)
        {
            rectanguloNegro.setFillColor(sf::Color(rectanguloNegro.getFillColor().r, rectanguloNegro.getFillColor().g, rectanguloNegro.getFillColor().b, rectanguloNegro.getFillColor().a-5));
        }
        else if (saliendo && animaciones.empty() && rectanguloNegro.getFillColor().a < 255)
        {
            rectanguloNegro.setFillColor(sf::Color(rectanguloNegro.getFillColor().r, rectanguloNegro.getFillColor().g, rectanguloNegro.getFillColor().b, rectanguloNegro.getFillColor().a+5));
        }

        // Se actualizan las animaciones
        std::list<std::shared_ptr<Animacion>> nuevasAnimaciones;

        for(std::shared_ptr<Animacion> &a : animaciones){
            a->actualizar(nuevasAnimaciones);
        }

        // Se itera por cada animación de la lista de animaciones y se van
        // eliminando las que hayan terminado
        std::list<std::shared_ptr<Animacion>>::iterator it = animaciones.begin();

        while(it != animaciones.end())
        {
            if((*it)->haTerminado())
                it = animaciones.erase(it);
            else
                it++;
        }

        ventana->clear(sf::Color(0,0,0));

        for(int i=0;i<fondosPersonajeJugador1.size();i++)
        {
            ventana->draw(fondosPersonajeJugador1[i]);
            ventana->draw(fondosPersonajeJugador2[i]);
        }

        ventana->draw(spriteMarco);

        for(int i=0;i<selectoresPersonajeJugador1.size();i++)
        {
            ventana->draw(selectoresPersonajeJugador1[i]);
            ventana->draw(selectoresPersonajeJugador2[i]);
        }

        for(std::shared_ptr<Animacion> &a : animaciones){
            ventana->draw(*a);
        }

        ventana->draw(rectanguloNegro);
        
        ventana->display();

        sf::sleep(sf::seconds(1.f / Configuracion::unicaInstancia()->getFPS()) - reloj.reset());
    }

    return personajesElegidos;
}