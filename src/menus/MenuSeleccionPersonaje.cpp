#include "MenuSeleccionPersonaje.hpp"
#include "Constantes.hpp"
#include "ReproductorDeMusica.hpp"
#include "VentanaPrincipal.hpp"
#include "GestorDeControles.hpp"
#include "Configuracion.hpp"
#include "Utilidades.hpp"
#include "ContenedorDePersonajes.hpp"
#include "ReproductorDeSonidos.hpp"
#include "ClienteDiscord.hpp"

#include <algorithm>

#include <iostream>

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
indiceJugador1(0), indiceJugador2(1), personajeElegidoJugador1(false), personajeElegidoJugador2(false), contadorSaliendo(0),
spriteMarco(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/marco.png")),
spriteEsc(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/esc-salir.png")),
fondoCuadriculado(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/fondo-cuadricula.png"),Direccion::ARRIBA_IZQUIERDA,VELOCIDAD_FONDO_CUADRICULADO_SELECCION_PERSONAJE),
fondoJ1Elegido(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/fondo-j1-seleccionado.png"),Direccion::ARRIBA,VELOCIDAD_FONDO_PERSONAJE_SELECCIONADO,sf::Vector2i(VENTANA_ANCHURA/2,0)),
fondoJ2Elegido(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/fondo-j2-seleccionado.png"),Direccion::ARRIBA,VELOCIDAD_FONDO_PERSONAJE_SELECCIONADO,sf::Vector2i(VENTANA_ANCHURA/2,0)),
fondoJ1ElegidoBolitas(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/fondo-j1-seleccionado-bolitas.png"),Direccion::ARRIBA,VELOCIDAD_FONDO_PERSONAJE_SELECCIONADO*2.f,sf::Vector2i(VENTANA_ANCHURA/2,0)),
fondoJ2ElegidoBolitas(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/fondo-j2-seleccionado-bolitas.png"),Direccion::ARRIBA,VELOCIDAD_FONDO_PERSONAJE_SELECCIONADO*2.f,sf::Vector2i(VENTANA_ANCHURA/2,0)),
rectanguloNegro({VENTANA_ANCHURA,VENTANA_ALTURA}),
spriteTeclaIzquierdaJ1(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-izquierda-a.png")),
spriteTeclaDerechaJ1(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-derecha-d.png")),
spriteTeclaSeleccionarJ1(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-seleccionar-shift.png")),
spriteTeclaIzquierdaJ2(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-izquierda-j.png")),
spriteTeclaDerechaJ2(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-derecha-l.png")),
spriteTeclaSeleccionarJ2(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-seleccionar-espacio.png")),
controlUtilizadoParaLosSpritesJ1(GestorDeControles::unicaInstancia()->obtenerControlUsadoPorJugador(Jugador::JUGADOR1)),
controlUtilizadoParaLosSpritesJ2(GestorDeControles::unicaInstancia()->obtenerControlUsadoPorJugador(Jugador::JUGADOR2))
{
    rectanguloNegro.setFillColor(sf::Color::Black);

    fondoJ2Elegido.setOrigen({-VENTANA_ANCHURA/2.f, 0.f});
    fondoJ2Elegido.setPosicion({0.f,0.f});
    fondoJ2ElegidoBolitas.setOrigen({-VENTANA_ANCHURA/2.f, 0.f});
    fondoJ2ElegidoBolitas.setPosicion({0.f,0.f});

    spriteEsc.setPosition(POSICION_SPRITE_ESC_SELECCION_PERSONAJE);

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

    rectanguloBlancoJ1.setFillColor(COLOR_RECTANGULO_BLANCO_TRANSPARENTE_FONDO_PERSONAJE);
    rectanguloBlancoJ1.setSize(sf::Vector2f(fondosPersonajeJugador1[0].getSprite().getTextureRect().size));
    rectanguloBlancoJ1.setPosition({POSICION_X_FONDO_PERSONAJE_J1,POSICION_Y_FONDO_PERSONAJE});

    rectanguloBlancoJ2.setFillColor(COLOR_RECTANGULO_BLANCO_TRANSPARENTE_FONDO_PERSONAJE);
    rectanguloBlancoJ2.setSize(sf::Vector2f(fondosPersonajeJugador2[0].getSprite().getTextureRect().size));
    rectanguloBlancoJ2.setPosition({POSICION_X_FONDO_PERSONAJE_J2,POSICION_Y_FONDO_PERSONAJE});

    resetear();
}

void MenuSeleccionPersonaje::resetear()
{
    indiceJugador1 = 0;
    indiceJugador2 = 1;
    personajeElegidoJugador1 = false;
    personajeElegidoJugador2 = false;
    contadorSaliendo = 0;

    for(int i=0;i<selectoresPersonajeJugador1.size();i++)
    {
        selectoresPersonajeJugador1[i].resetear(i-indiceJugador1);
        selectoresPersonajeJugador2[i].resetear(i-indiceJugador2);

        fondosPersonajeJugador1[i].resetear(i-indiceJugador1);
        fondosPersonajeJugador2[i].resetear(i-indiceJugador2);
    }

    fondoCuadriculado.resetear();

    rectanguloBlancoJ1.setFillColor(COLOR_RECTANGULO_BLANCO_TRANSPARENTE_FONDO_PERSONAJE);
    rectanguloBlancoJ2.setFillColor(COLOR_RECTANGULO_BLANCO_TRANSPARENTE_FONDO_PERSONAJE);

    cambiarSpritesTeclas(Jugador::JUGADOR1, controlUtilizadoParaLosSpritesJ1);
    cambiarSpritesTeclas(Jugador::JUGADOR2, controlUtilizadoParaLosSpritesJ2);

    // Los sprites de las teclas o botones para ir hacia la izquierda y hacia
    // la derecha se actualizan para tener el valor correcto de transparencia
    sf::Color colorTeclaIzquierdaJ1 = spriteTeclaIzquierdaJ1.getColor();
    colorTeclaIzquierdaJ1.a = indiceJugador1 == 0 ? 0 : 255;
    spriteTeclaIzquierdaJ1.setColor(colorTeclaIzquierdaJ1);

    sf::Color colorTeclaDerechaJ1 = spriteTeclaDerechaJ1.getColor();
    colorTeclaDerechaJ1.a = indiceJugador1 == selectoresPersonajeJugador1.size()-1 ? 0 : 255;
    spriteTeclaDerechaJ1.setColor(colorTeclaDerechaJ1);

    sf::Color colorTeclaIzquierdaJ2 = spriteTeclaIzquierdaJ2.getColor();
    colorTeclaIzquierdaJ2.a = indiceJugador2 == 0 ? 0 : 255;
    spriteTeclaIzquierdaJ2.setColor(colorTeclaIzquierdaJ2);

    sf::Color colorTeclaDerechaJ2 = spriteTeclaDerechaJ2.getColor();
    colorTeclaDerechaJ2.a = indiceJugador2 == selectoresPersonajeJugador2.size()-1 ? 0 : 255;
    spriteTeclaDerechaJ2.setColor(colorTeclaDerechaJ2);
}

void MenuSeleccionPersonaje::cambiarSpritesTeclas(Jugador j, Control c)
{
    switch(c)
    {
        case Control::TECLADO_IZQUIERDA:
            if(j == Jugador::JUGADOR1)
            {
                spriteTeclaIzquierdaJ1.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-izquierda-a.png"));
                spriteTeclaDerechaJ1.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-derecha-d.png"));
                spriteTeclaSeleccionarJ1.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-seleccionar-shift.png"));
            }
            else if(j == Jugador::JUGADOR2)
            {
                spriteTeclaIzquierdaJ2.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-izquierda-j.png"));
                spriteTeclaDerechaJ2.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-derecha-l.png"));
                spriteTeclaSeleccionarJ2.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-seleccionar-espacio.png"));
            }
            break;
        
        case Control::MANDO0:
        case Control::MANDO1:
        case Control::MANDO2:
        case Control::MANDO3:
        case Control::MANDO4:
        case Control::MANDO5:
        case Control::MANDO6:
        case Control::MANDO7:
            if(j == Jugador::JUGADOR1)
            {
                spriteTeclaIzquierdaJ1.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/boton-izquierda-d-pad.png"));
                spriteTeclaDerechaJ1.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/boton-derecha-d-pad.png"));
                spriteTeclaSeleccionarJ1.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/boton-seleccionar.png"));
            }
            else if(j == Jugador::JUGADOR2)
            {
                spriteTeclaIzquierdaJ2.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/boton-izquierda-d-pad.png"));
                spriteTeclaDerechaJ2.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/boton-derecha-d-pad.png"));
                spriteTeclaSeleccionarJ2.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/boton-seleccionar.png"));
            }
            break;
    }

    // Se vuelve a establecer el TextureRect para que el sprite no se muestre cortado si
    // la nueva textura es de un tamaño distinto a la antigua
    if(j == Jugador::JUGADOR1)
    {
        spriteTeclaIzquierdaJ1.setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaIzquierdaJ1.getTexture().getSize())));
        spriteTeclaDerechaJ1.setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaDerechaJ1.getTexture().getSize())));
        spriteTeclaSeleccionarJ1.setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaSeleccionarJ1.getTexture().getSize())));
    }
    else if(j == Jugador::JUGADOR2)
    {
        spriteTeclaIzquierdaJ2.setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaIzquierdaJ2.getTexture().getSize())));
        spriteTeclaDerechaJ2.setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaDerechaJ2.getTexture().getSize())));
        spriteTeclaSeleccionarJ2.setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaSeleccionarJ2.getTexture().getSize())));
    }

    // Se establece el origen de los sprites de las teclas o botones en el centro
    spriteTeclaIzquierdaJ1.setOrigin(static_cast<sf::Vector2f>(spriteTeclaIzquierdaJ1.getTextureRect().size)/2.f);
    spriteTeclaDerechaJ1.setOrigin(static_cast<sf::Vector2f>(spriteTeclaDerechaJ1.getTextureRect().size)/2.f);
    spriteTeclaSeleccionarJ1.setOrigin(static_cast<sf::Vector2f>(spriteTeclaSeleccionarJ1.getTextureRect().size)/2.f);

    spriteTeclaIzquierdaJ2.setOrigin(static_cast<sf::Vector2f>(spriteTeclaIzquierdaJ2.getTextureRect().size)/2.f);
    spriteTeclaDerechaJ2.setOrigin(static_cast<sf::Vector2f>(spriteTeclaDerechaJ2.getTextureRect().size)/2.f);
    spriteTeclaSeleccionarJ2.setOrigin(static_cast<sf::Vector2f>(spriteTeclaSeleccionarJ2.getTextureRect().size)/2.f);

    // Ahora, para calcular correctamente el lugar exacto en el que poner los sprites que indican qué tecla
    // o botón tiene que pulsar cada jugador para poder hacer cosas, hay que tener en cuenta dónde está el
    // selector de personaje actual y dónde están los selectores de personaje que tiene al lado.
    float posicionSelectorPersonajeActualJ1EjeX = POSICION_X_SELECTOR_PERSONAJE_J1;
    float posicionSelectorPersonajeSiguienteJ1EjeX = POSICION_X_SELECTOR_PERSONAJE_J1 + (1-DIFERENCIA_ESCALA_SELECTOR_PERSONAJE)*DIFERENCIA_POSICION_X_SELECTOR_PERSONAJE;
    float diferenciaPosicionSelectoresPersonajesAdyacentesEjeX = posicionSelectorPersonajeSiguienteJ1EjeX - posicionSelectorPersonajeActualJ1EjeX;

    // También hay que tener en cuenta el tamaño de cada selector
    float tamanoSelectorActualX = selectoresPersonajeJugador1[0].getSprite().getTextureRect().size.x;
    float tamanoSelectorSiguienteX = tamanoSelectorActualX*(1-DIFERENCIA_ESCALA_SELECTOR_PERSONAJE);

    // Ahora, se puede calcular cuál es la diferencia exacta en el eje X entre el selector actual y el
    // sprite que indica la tecla a pulsar
    float diferenciaEntreSelectorYSpriteTecla = tamanoSelectorActualX/2.f + (diferenciaPosicionSelectoresPersonajesAdyacentesEjeX - tamanoSelectorActualX/2.f - tamanoSelectorSiguienteX/2.f)/2.f;

    // Finalmente, se ponen los sprites en su posición correcta
    spriteTeclaIzquierdaJ1.setPosition({POSICION_X_SELECTOR_PERSONAJE_J1-diferenciaEntreSelectorYSpriteTecla,POSICION_Y_SELECTOR_PERSONAJE});
    spriteTeclaDerechaJ1.setPosition({POSICION_X_SELECTOR_PERSONAJE_J1+diferenciaEntreSelectorYSpriteTecla,POSICION_Y_SELECTOR_PERSONAJE});

    spriteTeclaIzquierdaJ2.setPosition({POSICION_X_SELECTOR_PERSONAJE_J2-diferenciaEntreSelectorYSpriteTecla,POSICION_Y_SELECTOR_PERSONAJE});
    spriteTeclaDerechaJ2.setPosition({POSICION_X_SELECTOR_PERSONAJE_J2+diferenciaEntreSelectorYSpriteTecla,POSICION_Y_SELECTOR_PERSONAJE});
}

void MenuSeleccionPersonaje::seleccionarPersonaje(Jugador jugador, std::list<std::shared_ptr<Animacion>>& animaciones, std::unordered_map<Jugador,std::string>& personajesElegidos)
{
    std::list<std::shared_ptr<Animacion>> nuevasAnimaciones;

    if(jugador == Jugador::JUGADOR1)
    {
        personajeElegidoJugador1 = selectoresPersonajeJugador1[indiceJugador1].seleccionar(nuevasAnimaciones);
        
        animaciones.splice(animaciones.end(),nuevasAnimaciones);

        if(personajeElegidoJugador1)
        {
            fondosPersonajeJugador1[indiceJugador1].seleccionar();

            sf::Color nuevoColor = rectanguloBlancoJ1.getFillColor();
            nuevoColor.a = OPACIDAD_RECTANGULO_BLANCO_FONDO_PERSONAJE_SELECCIONADO;
            rectanguloBlancoJ1.setFillColor(nuevoColor);

            fondoAdicionalCopiaJugador1 = std::make_shared<sf::Sprite>(fondosPersonajeJugador1[indiceJugador1].getSprite());
            fondoAdicionalNegroJugador1 = std::make_shared<sf::Sprite>(fondosPersonajeJugador1[indiceJugador1].getSprite());
            fondoAdicionalNegroJugador1->setColor(sf::Color::Black);

            ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-1-elegir.ogg");
            
            personajesElegidos[Jugador::JUGADOR1] = selectoresPersonajeJugador1[indiceJugador1].getNombrePersonaje();
        }
    }
    else if (jugador == Jugador::JUGADOR2)
    {
        personajeElegidoJugador2 = selectoresPersonajeJugador2[indiceJugador2].seleccionar(nuevasAnimaciones);

        animaciones.splice(animaciones.end(),nuevasAnimaciones);

        if(personajeElegidoJugador2)
        {
            fondosPersonajeJugador2[indiceJugador2].seleccionar();

            sf::Color nuevoColor = rectanguloBlancoJ2.getFillColor();
            nuevoColor.a = 125;
            rectanguloBlancoJ2.setFillColor(nuevoColor);

            fondoAdicionalCopiaJugador2 = std::make_shared<sf::Sprite>(fondosPersonajeJugador2[indiceJugador2].getSprite());
            fondoAdicionalNegroJugador2 = std::make_shared<sf::Sprite>(fondosPersonajeJugador2[indiceJugador2].getSprite());
            fondoAdicionalNegroJugador2->setColor(sf::Color::Black);

            ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-2-elegir.ogg");
            
            personajesElegidos[Jugador::JUGADOR2] = selectoresPersonajeJugador2[indiceJugador2].getNombrePersonaje();
        }
    }
}

void MenuSeleccionPersonaje::desseleccionarPersonaje(Jugador jugador, std::unordered_map<Jugador,std::string>& personajesElegidos)
{
    ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-1-rechazar.ogg");

    personajesElegidos.erase(jugador);

    if(jugador == Jugador::JUGADOR1)
    {
        personajeElegidoJugador1 = false;
        fondoAdicionalNegroJugador1.reset();
        fondoAdicionalCopiaJugador1.reset();
        rectanguloBlancoJ1.setFillColor(COLOR_RECTANGULO_BLANCO_TRANSPARENTE_FONDO_PERSONAJE);
        fondosPersonajeJugador1[indiceJugador1].quitarSeleccion();
    }
    else
    {
        personajeElegidoJugador2 = false;
        fondoAdicionalNegroJugador2.reset();
        fondoAdicionalCopiaJugador2.reset();
        rectanguloBlancoJ2.setFillColor(COLOR_RECTANGULO_BLANCO_TRANSPARENTE_FONDO_PERSONAJE);
        fondosPersonajeJugador2[indiceJugador2].quitarSeleccion();
    }
}

std::unordered_map<Jugador,std::string> MenuSeleccionPersonaje::comenzarEleccionDoble()
{
    ClienteDiscord::unicaInstancia()->actualizarRichPresence("En una Batalla VS", "Eligiendo personajes...");

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

        // Se ejecutan cosillas de Discord en cada frame
        discordpp::RunCallbacks();

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
                ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/salir.ogg");
                spriteEsc.move({-NUM_PIXELES_RETROCESO_SPRITE_ESC,0.f});

                if(personajeElegidoJugador1) desseleccionarPersonaje(Jugador::JUGADOR1,personajesElegidos);
                if(personajeElegidoJugador2) desseleccionarPersonaje(Jugador::JUGADOR2,personajesElegidos);
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
                        seleccionarPersonaje(infoEvento.jugador,animaciones,personajesElegidos);
                    }
                    else if(!saliendo)
                    {
                        desseleccionarPersonaje(infoEvento.jugador,personajesElegidos);
                    }
                }
                else if(infoEvento.jugador == Jugador::JUGADOR2)
                {
                    if(!personajeElegidoJugador2)
                    {
                        seleccionarPersonaje(infoEvento.jugador,animaciones,personajesElegidos);
                    }
                    else if(!saliendo)
                    {
                        desseleccionarPersonaje(infoEvento.jugador,personajesElegidos);
                    }
                }
            }
        }

        // Si el jugador 1 ha cambiado de control, hay que cambiar los sprites
        if(controlUtilizadoParaLosSpritesJ1 != GestorDeControles::unicaInstancia()->obtenerControlUsadoPorJugador(Jugador::JUGADOR1))
        {
            controlUtilizadoParaLosSpritesJ1 = GestorDeControles::unicaInstancia()->obtenerControlUsadoPorJugador(Jugador::JUGADOR1);
            cambiarSpritesTeclas(Jugador::JUGADOR1, controlUtilizadoParaLosSpritesJ1);
        }
        
        // Lo mismo para el jugador 2
        if(controlUtilizadoParaLosSpritesJ2 != GestorDeControles::unicaInstancia()->obtenerControlUsadoPorJugador(Jugador::JUGADOR2))
        {
            controlUtilizadoParaLosSpritesJ2 = GestorDeControles::unicaInstancia()->obtenerControlUsadoPorJugador(Jugador::JUGADOR2);
            cambiarSpritesTeclas(Jugador::JUGADOR2, controlUtilizadoParaLosSpritesJ2);
        }

        // Se actualiza la transparencia del sprite que indica el botón o tecla
        // a pulsar para moverse hacia la izquierda para el jugador 1
        if(spriteTeclaIzquierdaJ1.getColor().a > 0 && indiceJugador1 == 0)
        {
            sf::Color nuevoColor = spriteTeclaIzquierdaJ1.getColor();
            nuevoColor.a -= VELOCIDAD_CAMBIO_COLOR_SPRITES_TECLAS;
            spriteTeclaIzquierdaJ1.setColor(nuevoColor);
        }
        else if (spriteTeclaIzquierdaJ1.getColor().a < 255 && indiceJugador1 > 0)
        {
            sf::Color nuevoColor = spriteTeclaIzquierdaJ1.getColor();
            nuevoColor.a += VELOCIDAD_CAMBIO_COLOR_SPRITES_TECLAS;
            spriteTeclaIzquierdaJ1.setColor(nuevoColor);
        }

        // Se actualiza la transparencia del sprite que indica el botón o tecla
        // a pulsar para moverse hacia la derecha para el jugador 1
        if(spriteTeclaDerechaJ1.getColor().a > 0 && indiceJugador1 == selectoresPersonajeJugador1.size()-1)
        {
            sf::Color nuevoColor = spriteTeclaDerechaJ1.getColor();
            nuevoColor.a -= VELOCIDAD_CAMBIO_COLOR_SPRITES_TECLAS;
            spriteTeclaDerechaJ1.setColor(nuevoColor);
        }
        else if (spriteTeclaDerechaJ1.getColor().a < 255 && indiceJugador1 < selectoresPersonajeJugador1.size()-1)
        {
            sf::Color nuevoColor = spriteTeclaDerechaJ1.getColor();
            nuevoColor.a += VELOCIDAD_CAMBIO_COLOR_SPRITES_TECLAS;
            spriteTeclaDerechaJ1.setColor(nuevoColor);
        }

        // Se actualiza la transparencia del sprite que indica el botón o tecla
        // a pulsar para moverse hacia la izquierda para el jugador 2
        if(spriteTeclaIzquierdaJ2.getColor().a > 0 && indiceJugador2 == 0)
        {
            sf::Color nuevoColor = spriteTeclaIzquierdaJ2.getColor();
            nuevoColor.a -= VELOCIDAD_CAMBIO_COLOR_SPRITES_TECLAS;
            spriteTeclaIzquierdaJ2.setColor(nuevoColor);
        }
        else if (spriteTeclaIzquierdaJ2.getColor().a < 255 && indiceJugador2 > 0)
        {
            sf::Color nuevoColor = spriteTeclaIzquierdaJ2.getColor();
            nuevoColor.a += VELOCIDAD_CAMBIO_COLOR_SPRITES_TECLAS;
            spriteTeclaIzquierdaJ2.setColor(nuevoColor);
        }

        // Se actualiza la transparencia del sprite que indica el botón o tecla
        // a pulsar para moverse hacia la derecha para el jugador 2
        if(spriteTeclaDerechaJ2.getColor().a > 0 && indiceJugador2 == selectoresPersonajeJugador2.size()-1)
        {
            sf::Color nuevoColor = spriteTeclaDerechaJ2.getColor();
            nuevoColor.a -= VELOCIDAD_CAMBIO_COLOR_SPRITES_TECLAS;
            spriteTeclaDerechaJ2.setColor(nuevoColor);
        }
        else if (spriteTeclaDerechaJ2.getColor().a < 255 && indiceJugador2 < selectoresPersonajeJugador2.size()-1)
        {
            sf::Color nuevoColor = spriteTeclaDerechaJ2.getColor();
            nuevoColor.a += VELOCIDAD_CAMBIO_COLOR_SPRITES_TECLAS;
            spriteTeclaDerechaJ2.setColor(nuevoColor);
        }

        // Si estamos saliendo, se aumenta el contador que indica desde hace
        // cuántos frames estamos saliendo
        if(saliendo)
            contadorSaliendo++;

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
        if(!saliendo && personajeElegidoJugador1 && personajeElegidoJugador2)
        {
            ReproductorDeMusica::unicaInstancia()->detener();

            saliendo = true;
        }

        // Si nos estamos saliendo sin elegir personaje, significa que le hemos
        // dado a ESC, por lo que hay que mover el sprite de darle a ESC
        // a su posición original
        if(saliendo && personajesElegidos.empty())
        {
            sf::Vector2f posicionActual = spriteEsc.getPosition();
            sf::Vector2f posicionNueva = util::aproximarVector2f(posicionActual,POSICION_SPRITE_ESC_SELECCION_PERSONAJE,0.9f);
            spriteEsc.setPosition(posicionNueva);
        }

        // El rectángulo negro que cubre la pantalla se vuelve transparente si todavía se está eligiendo un personaje, o si hemos dicho
        // de salir pero no ha pasado el tiempo suficiente
        if(rectanguloNegro.getFillColor().a > 0 && (!saliendo || (saliendo && (contadorSaliendo < FRAMES_ESPERA_SALIR_MENU))))
            rectanguloNegro.setFillColor(sf::Color(rectanguloNegro.getFillColor().r, rectanguloNegro.getFillColor().g, rectanguloNegro.getFillColor().b, rectanguloNegro.getFillColor().a-5));

        // Por el contrario, si ya hemos dicho de salir, se vuelve opaco si pasa suficiente tiempo
        else if (saliendo && contadorSaliendo >= FRAMES_ESPERA_SALIR_MENU)
            rectanguloNegro.setFillColor(sf::Color(rectanguloNegro.getFillColor().r, rectanguloNegro.getFillColor().g, rectanguloNegro.getFillColor().b, rectanguloNegro.getFillColor().a+5));

        // Se actualizan las animaciones
        std::list<std::shared_ptr<Animacion>> nuevasAnimaciones;

        for(std::shared_ptr<Animacion> &a : animaciones){
            a->actualizar(nuevasAnimaciones);
        }

        // Se actualiza el fondo cuadriculado
        fondoCuadriculado.actualizar(nuevasAnimaciones);

        // Se actualizan los fondos que salen cuando se eligen
        // los personajes
        fondoJ1Elegido.actualizar(nuevasAnimaciones);
        fondoJ2Elegido.actualizar(nuevasAnimaciones);
        fondoJ1ElegidoBolitas.actualizar(nuevasAnimaciones);
        fondoJ2ElegidoBolitas.actualizar(nuevasAnimaciones);

        // Se acercan los fondos adicionales del jugador 1 a su sitio
        if(personajeElegidoJugador1){
            sf::Vector2f posicionActual = fondoAdicionalCopiaJugador1->getPosition();
            posicionActual.x = util::aproximarFloat(posicionActual.x,fondosPersonajeJugador1[indiceJugador1].getSprite().getPosition().x-OFFSET_X_FONDO_ADICIONAL_COPIA,0.9);
            fondoAdicionalCopiaJugador1->setPosition(posicionActual);

            posicionActual = fondoAdicionalNegroJugador1->getPosition();
            posicionActual.x = util::aproximarFloat(posicionActual.x,fondosPersonajeJugador1[indiceJugador1].getSprite().getPosition().x-OFFSET_X_FONDO_ADICIONAL_NEGRO,0.9);
            fondoAdicionalNegroJugador1->setPosition(posicionActual);
        }

        // Lo mismo con los fondos adicionales del jugador 2
        if(personajeElegidoJugador2){
            sf::Vector2f posicionActual = fondoAdicionalCopiaJugador2->getPosition();
            posicionActual.x = util::aproximarFloat(posicionActual.x,fondosPersonajeJugador2[indiceJugador2].getSprite().getPosition().x+OFFSET_X_FONDO_ADICIONAL_COPIA,0.9);
            fondoAdicionalCopiaJugador2->setPosition(posicionActual);

            posicionActual = fondoAdicionalNegroJugador2->getPosition();
            posicionActual.x = util::aproximarFloat(posicionActual.x,fondosPersonajeJugador2[indiceJugador2].getSprite().getPosition().x+OFFSET_X_FONDO_ADICIONAL_NEGRO,0.9);
            fondoAdicionalNegroJugador2->setPosition(posicionActual);
        }

        // Se transparenta el rectángulo blanco de cada jugador si su
        // transparencia no es 0
        if(rectanguloBlancoJ1.getFillColor().a > 0){
            sf::Color nuevoColor = rectanguloBlancoJ1.getFillColor();
            nuevoColor.a -= 5;
            rectanguloBlancoJ1.setFillColor(nuevoColor);
        }

        if(rectanguloBlancoJ2.getFillColor().a > 0){
            sf::Color nuevoColor = rectanguloBlancoJ2.getFillColor();
            nuevoColor.a -= 5;
            rectanguloBlancoJ2.setFillColor(nuevoColor);
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
        
        ventana->draw(fondoCuadriculado);

        if(personajeElegidoJugador1)
        {
            ventana->draw(fondoJ1Elegido);
            ventana->draw(fondoJ1ElegidoBolitas);

            ventana->draw(*fondoAdicionalNegroJugador1);
        }

        if(personajeElegidoJugador2)
        {
            ventana->draw(fondoJ2Elegido);
            ventana->draw(fondoJ2ElegidoBolitas);

            ventana->draw(*fondoAdicionalNegroJugador2);
        }

        for(int i=0;i<fondosPersonajeJugador1.size();i++)
        {
            ventana->draw(fondosPersonajeJugador1[i]);
            ventana->draw(fondosPersonajeJugador2[i]);
        }

        if(personajeElegidoJugador1)
        {
            ventana->draw(*fondoAdicionalCopiaJugador1);
        }

        if(personajeElegidoJugador2)
        {
            ventana->draw(*fondoAdicionalCopiaJugador2);
        }

        ventana->draw(rectanguloBlancoJ1);
        ventana->draw(rectanguloBlancoJ2);

        ventana->draw(spriteMarco);
        ventana->draw(spriteEsc);

        ventana->draw(spriteTeclaIzquierdaJ1);
        ventana->draw(spriteTeclaDerechaJ1);

        ventana->draw(spriteTeclaIzquierdaJ2);
        ventana->draw(spriteTeclaDerechaJ2);

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