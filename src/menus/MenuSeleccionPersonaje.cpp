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
spriteEsc(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/esc-salir.png"),FACTOR_APROXIMACION_SPRITES_TECLAS),
fondoCuadriculado(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/fondo-cuadricula.png"),Direccion::ARRIBA_IZQUIERDA,VELOCIDAD_FONDO_CUADRICULADO_SELECCION_PERSONAJE),
fondoJ1Elegido(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/fondo-j1-seleccionado.png"),Direccion::ARRIBA,VELOCIDAD_FONDO_PERSONAJE_SELECCIONADO,sf::Vector2i(VENTANA_ANCHURA/2,0)),
fondoJ2Elegido(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/fondo-j2-seleccionado.png"),Direccion::ARRIBA,VELOCIDAD_FONDO_PERSONAJE_SELECCIONADO,sf::Vector2i(VENTANA_ANCHURA/2,0)),
fondoJ1ElegidoBolitas(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/fondo-j1-seleccionado-bolitas.png"),Direccion::ARRIBA,VELOCIDAD_FONDO_PERSONAJE_SELECCIONADO*2.f,sf::Vector2i(VENTANA_ANCHURA/2,0)),
fondoJ2ElegidoBolitas(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/fondo-j2-seleccionado-bolitas.png"),Direccion::ARRIBA,VELOCIDAD_FONDO_PERSONAJE_SELECCIONADO*2.f,sf::Vector2i(VENTANA_ANCHURA/2,0)),
rectanguloNegro({VENTANA_ANCHURA,VENTANA_ALTURA}),
spriteTeclaIzquierdaJ1(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-izquierda-a.png"),FACTOR_APROXIMACION_SPRITES_TECLAS),
spriteTeclaDerechaJ1(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-derecha-d.png"),FACTOR_APROXIMACION_SPRITES_TECLAS),
spriteTeclaSeleccionarJ1(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-seleccionar-shift.png"),FACTOR_APROXIMACION_SPRITES_TECLAS),
spriteTeclaIzquierdaJ2(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-izquierda-j.png"),FACTOR_APROXIMACION_SPRITES_TECLAS),
spriteTeclaDerechaJ2(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-derecha-l.png"),FACTOR_APROXIMACION_SPRITES_TECLAS),
spriteTeclaSeleccionarJ2(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-seleccionar-espacio.png"),FACTOR_APROXIMACION_SPRITES_TECLAS),
controlUtilizadoParaLosSpritesJ1(GestorDeControles::unicaInstancia()->obtenerControlUsadoPorJugador(Jugador::JUGADOR1)),
controlUtilizadoParaLosSpritesJ2(GestorDeControles::unicaInstancia()->obtenerControlUsadoPorJugador(Jugador::JUGADOR2))
{
    rectanguloNegro.setFillColor(sf::Color::Black);

    fondoJ2Elegido.setOrigen({-VENTANA_ANCHURA/2.f, 0.f});
    fondoJ2Elegido.setPosicion({0.f,0.f});
    fondoJ2ElegidoBolitas.setOrigen({-VENTANA_ANCHURA/2.f, 0.f});
    fondoJ2ElegidoBolitas.setPosicion({0.f,0.f});

    spriteEsc.setPosicionDeseada(POSICION_SPRITE_ESC_SELECCION_PERSONAJE);
    spriteEsc.actualizarPosicionInmediatamente();

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
    sf::Color colorTeclaIzquierdaJ1 = spriteTeclaIzquierdaJ1.getSprite().getColor();
    colorTeclaIzquierdaJ1.a = indiceJugador1 == 0 ? 0 : 255;
    spriteTeclaIzquierdaJ1.setColorDeseado(colorTeclaIzquierdaJ1);
    spriteTeclaIzquierdaJ1.actualizarColorInmediatamente();

    sf::Color colorTeclaDerechaJ1 = spriteTeclaDerechaJ1.getSprite().getColor();
    colorTeclaDerechaJ1.a = indiceJugador1 == selectoresPersonajeJugador1.size()-1 ? 0 : 255;
    spriteTeclaDerechaJ1.setColorDeseado(colorTeclaDerechaJ1);
    spriteTeclaDerechaJ1.actualizarColorInmediatamente();

    sf::Color colorTeclaIzquierdaJ2 = spriteTeclaIzquierdaJ2.getSprite().getColor();
    colorTeclaIzquierdaJ2.a = indiceJugador2 == 0 ? 0 : 255;
    spriteTeclaIzquierdaJ2.setColorDeseado(colorTeclaIzquierdaJ2);
    spriteTeclaIzquierdaJ2.actualizarColorInmediatamente();

    sf::Color colorTeclaDerechaJ2 = spriteTeclaDerechaJ2.getSprite().getColor();
    colorTeclaDerechaJ2.a = indiceJugador2 == selectoresPersonajeJugador2.size()-1 ? 0 : 255;
    spriteTeclaDerechaJ2.setColorDeseado(colorTeclaDerechaJ2);
    spriteTeclaDerechaJ2.actualizarColorInmediatamente();
}

void MenuSeleccionPersonaje::cambiarSpritesTeclas(Jugador j, Control c)
{
    switch(c)
    {
        case Control::TECLADO_IZQUIERDA:
            if(j == Jugador::JUGADOR1)
            {
                spriteTeclaIzquierdaJ1.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-izquierda-a.png"));
                spriteTeclaDerechaJ1.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-derecha-d.png"));
                spriteTeclaSeleccionarJ1.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-seleccionar-shift.png"));
            }
            else if(j == Jugador::JUGADOR2)
            {
                spriteTeclaIzquierdaJ2.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-izquierda-j.png"));
                spriteTeclaDerechaJ2.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-derecha-l.png"));
                spriteTeclaSeleccionarJ2.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/tecla-seleccionar-espacio.png"));
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
                spriteTeclaIzquierdaJ1.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/boton-izquierda-d-pad.png"));
                spriteTeclaDerechaJ1.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/boton-derecha-d-pad.png"));
                spriteTeclaSeleccionarJ1.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/boton-seleccionar.png"));
            }
            else if(j == Jugador::JUGADOR2)
            {
                spriteTeclaIzquierdaJ2.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/boton-izquierda-d-pad.png"));
                spriteTeclaDerechaJ2.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/boton-derecha-d-pad.png"));
                spriteTeclaSeleccionarJ2.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/boton-seleccionar.png"));
            }
            break;
    }

    // Se vuelve a establecer el TextureRect para que el sprite no se muestre cortado si
    // la nueva textura es de un tamaño distinto a la antigua
    if(j == Jugador::JUGADOR1)
    {
        spriteTeclaIzquierdaJ1.getSprite().setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaIzquierdaJ1.getSprite().getTexture().getSize())));
        spriteTeclaDerechaJ1.getSprite().setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaDerechaJ1.getSprite().getTexture().getSize())));
        spriteTeclaSeleccionarJ1.getSprite().setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaSeleccionarJ1.getSprite().getTexture().getSize())));
    }
    else if(j == Jugador::JUGADOR2)
    {
        spriteTeclaIzquierdaJ2.getSprite().setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaIzquierdaJ2.getSprite().getTexture().getSize())));
        spriteTeclaDerechaJ2.getSprite().setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaDerechaJ2.getSprite().getTexture().getSize())));
        spriteTeclaSeleccionarJ2.getSprite().setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaSeleccionarJ2.getSprite().getTexture().getSize())));
    }

    // Se establece el origen de los sprites de las teclas o botones en el centro
    spriteTeclaIzquierdaJ1.getSprite().setOrigin(static_cast<sf::Vector2f>(spriteTeclaIzquierdaJ1.getSprite().getTextureRect().size)/2.f);
    spriteTeclaDerechaJ1.getSprite().setOrigin(static_cast<sf::Vector2f>(spriteTeclaDerechaJ1.getSprite().getTextureRect().size)/2.f);
    spriteTeclaSeleccionarJ1.getSprite().setOrigin(static_cast<sf::Vector2f>(spriteTeclaSeleccionarJ1.getSprite().getTextureRect().size)/2.f);

    spriteTeclaIzquierdaJ2.getSprite().setOrigin(static_cast<sf::Vector2f>(spriteTeclaIzquierdaJ2.getSprite().getTextureRect().size)/2.f);
    spriteTeclaDerechaJ2.getSprite().setOrigin(static_cast<sf::Vector2f>(spriteTeclaDerechaJ2.getSprite().getTextureRect().size)/2.f);
    spriteTeclaSeleccionarJ2.getSprite().setOrigin(static_cast<sf::Vector2f>(spriteTeclaSeleccionarJ2.getSprite().getTextureRect().size)/2.f);

    // Ahora, para calcular correctamente el lugar exacto en el que poner los sprites que indican qué tecla
    // o botón tiene que pulsar cada jugador para poder hacer cosas, hay que tener en cuenta dónde está el
    // selector de personaje actual y dónde están los selectores de personaje que tiene al lado.
    float posicionSelectorPersonajeActualJ1EjeX = POSICION_X_SELECTOR_PERSONAJE_J1;
    float posicionSelectorPersonajeSiguienteJ1EjeX = POSICION_X_SELECTOR_PERSONAJE_J1 + (1-DIFERENCIA_ESCALA_SELECTOR_PERSONAJE)*DIFERENCIA_POSICION_X_SELECTOR_PERSONAJE;
    float diferenciaPosicionSelectoresPersonajesAdyacentesEjeX = posicionSelectorPersonajeSiguienteJ1EjeX - posicionSelectorPersonajeActualJ1EjeX;

    // También hay que tener en cuenta el tamaño de cada selector
    float tamanoSelectorActualX = selectoresPersonajeJugador1[indiceJugador1].getSprite().getTextureRect().size.x;
    float tamanoSelectorSiguienteX = tamanoSelectorActualX*(1-DIFERENCIA_ESCALA_SELECTOR_PERSONAJE);

    // Ahora, se puede calcular cuál es la diferencia exacta en el eje X entre el selector actual y el
    // sprite que indica la tecla a pulsar para moverse a uno de los lados
    float diferenciaEntreSelectorYSpriteTeclaX = tamanoSelectorActualX/2.f + (diferenciaPosicionSelectoresPersonajesAdyacentesEjeX - tamanoSelectorActualX/2.f - tamanoSelectorSiguienteX/2.f)/2.f;

    // Finalmente, se ponen los sprites en su posición correcta
    spriteTeclaIzquierdaJ1.setPosicionDeseada({POSICION_X_SELECTOR_PERSONAJE_J1-diferenciaEntreSelectorYSpriteTeclaX,POSICION_Y_SELECTOR_PERSONAJE});
    spriteTeclaIzquierdaJ1.actualizarPosicionInmediatamente();
    spriteTeclaDerechaJ1.setPosicionDeseada({POSICION_X_SELECTOR_PERSONAJE_J1+diferenciaEntreSelectorYSpriteTeclaX,POSICION_Y_SELECTOR_PERSONAJE});
    spriteTeclaDerechaJ1.actualizarPosicionInmediatamente();

    spriteTeclaIzquierdaJ2.setPosicionDeseada({POSICION_X_SELECTOR_PERSONAJE_J2-diferenciaEntreSelectorYSpriteTeclaX,POSICION_Y_SELECTOR_PERSONAJE});
    spriteTeclaIzquierdaJ2.actualizarPosicionInmediatamente();
    spriteTeclaDerechaJ2.setPosicionDeseada({POSICION_X_SELECTOR_PERSONAJE_J2+diferenciaEntreSelectorYSpriteTeclaX,POSICION_Y_SELECTOR_PERSONAJE});
    spriteTeclaDerechaJ2.actualizarPosicionInmediatamente();
    
    // Ahora se hace algo parecido pero con el eje Y, para así poder situar el sprite que indica
    // la tecla a pulsar para seleccionar o quitar la selección del personaje al que se está
    // apuntando actualmente
    float tamanoSelectorActualY = selectoresPersonajeJugador1[indiceJugador1].getSprite().getTextureRect().size.y;
    float posicionFinSelectorPersonajeActualEjeY = POSICION_Y_SELECTOR_PERSONAJE + tamanoSelectorActualY/2.f;
    float posicionFinPantallaEjeY = VENTANA_ALTURA;
    float diferenciaPosicionSelectorYFinPantalla = posicionFinPantallaEjeY - posicionFinSelectorPersonajeActualEjeY;
    
    // Ahora, se puede calcular cuál es la diferencia exacta en el eje Y entre el selector actual y el
    // sprite que indica la tecla a pulsar para seleccionar al personaje
    float posicionYSpriteTeclaSeleccionar = posicionFinSelectorPersonajeActualEjeY + diferenciaPosicionSelectorYFinPantalla/2.f;

    // Se utiliza la diferencia calculada para situar el sprite que indica la tecla a pulsar
    spriteTeclaSeleccionarJ1.setPosicionDeseada({POSICION_X_SELECTOR_PERSONAJE_J1,posicionYSpriteTeclaSeleccionar});
    spriteTeclaSeleccionarJ1.actualizarPosicionInmediatamente();
    spriteTeclaSeleccionarJ2.setPosicionDeseada({POSICION_X_SELECTOR_PERSONAJE_J2,posicionYSpriteTeclaSeleccionar});
    spriteTeclaSeleccionarJ2.actualizarPosicionInmediatamente();
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
                spriteEsc.getSprite().move({-NUM_PIXELES_RETROCESO_SPRITES_TECLAS_SELECCION_PERSONAJE,0.f});

                if(personajeElegidoJugador1) desseleccionarPersonaje(Jugador::JUGADOR1,personajesElegidos);
                if(personajeElegidoJugador2) desseleccionarPersonaje(Jugador::JUGADOR2,personajesElegidos);
            }
            else if(infoEvento.accion == Accion::DERECHA && infoEvento.realizada)
            {
                if(infoEvento.jugador == Jugador::JUGADOR1 && !personajeElegidoJugador1 && indiceJugador1 < selectoresPersonajeJugador1.size()-1)
                {
                    ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-1-derecha.ogg");
                    indiceJugador1++;

                    spriteTeclaDerechaJ1.getSprite().move({NUM_PIXELES_RETROCESO_SPRITES_TECLAS_SELECCION_PERSONAJE,0.f});
                    
                    // Como nos hemos movido hacia la derecha, si antes estábamos en el índice 0,
                    // el sprite de la tecla para ir a la izquierda pasa de estar transparente a
                    // estar opaco
                    if (indiceJugador1 == 1)
                        spriteTeclaIzquierdaJ1.setColorDeseado(sf::Color::White);
                    
                    // Si hemos llegado al final, el sprite para ir a la derecha debe volverse
                    // transparente
                    if (indiceJugador1 == selectoresPersonajeJugador1.size()-1)
                        spriteTeclaDerechaJ1.setColorDeseado(sf::Color::Transparent);

                }
                else if(infoEvento.jugador == Jugador::JUGADOR2 && !personajeElegidoJugador2 && indiceJugador2 < selectoresPersonajeJugador2.size()-1)
                {
                    ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-2-derecha.ogg");
                    indiceJugador2++;
                    
                    spriteTeclaDerechaJ2.getSprite().move({NUM_PIXELES_RETROCESO_SPRITES_TECLAS_SELECCION_PERSONAJE,0.f});

                    // Como nos hemos movido hacia la derecha, si antes estábamos en el índice 0,
                    // el sprite de la tecla para ir a la izquierda pasa de estar transparente a
                    // estar opaco
                    if (indiceJugador2 == 1)
                        spriteTeclaIzquierdaJ2.setColorDeseado(sf::Color::White);
                    
                    // Si hemos llegado al final, el sprite para ir a la derecha debe volverse
                    // transparente
                    if (indiceJugador2 == selectoresPersonajeJugador1.size()-1)
                        spriteTeclaDerechaJ2.setColorDeseado(sf::Color::Transparent);
                }
            }
            else if(infoEvento.accion == Accion::IZQUIERDA && infoEvento.realizada)
            {
                if(infoEvento.jugador == Jugador::JUGADOR1 && !personajeElegidoJugador1 && indiceJugador1 > 0)
                {
                    ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-1-izquierda.ogg");
                    indiceJugador1--;

                    spriteTeclaIzquierdaJ1.getSprite().move({-NUM_PIXELES_RETROCESO_SPRITES_TECLAS_SELECCION_PERSONAJE,0.f});

                    // Como nos hemos movido hacia la izquierda, si antes estábamos en el último índice,
                    // el sprite de la tecla para ir a la derecha pasa de estar transparente a
                    // estar opaco
                    if (indiceJugador1 == selectoresPersonajeJugador1.size()-2)
                        spriteTeclaDerechaJ1.setColorDeseado(sf::Color::White);
                    
                    // Si hemos llegado al principio, el sprite para ir a la izquierda debe volverse
                    // transparente
                    if (indiceJugador1 == 0)
                        spriteTeclaIzquierdaJ1.setColorDeseado(sf::Color::Transparent);
                }
                else if(infoEvento.jugador == Jugador::JUGADOR2 && !personajeElegidoJugador2 && indiceJugador2 > 0)
                {
                    ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-seleccion-personaje/jugador-2-izquierda.ogg");
                    indiceJugador2--;

                    spriteTeclaIzquierdaJ2.getSprite().move({-NUM_PIXELES_RETROCESO_SPRITES_TECLAS_SELECCION_PERSONAJE,0.f});

                    // Como nos hemos movido hacia la izquierda, si antes estábamos en el último índice,
                    // el sprite de la tecla para ir a la derecha pasa de estar transparente a
                    // estar opaco
                    if (indiceJugador2 == selectoresPersonajeJugador1.size()-2)
                        spriteTeclaDerechaJ2.setColorDeseado(sf::Color::White);
                    
                    // Si hemos llegado al principio, el sprite para ir a la izquierda debe volverse
                    // transparente
                    if (indiceJugador2 == 0)
                        spriteTeclaIzquierdaJ2.setColorDeseado(sf::Color::Transparent);
                }
            }
            else if(infoEvento.accion == Accion::ATACAR && infoEvento.realizada)
            {
                if(infoEvento.jugador == Jugador::JUGADOR1)
                {
                    if(!personajeElegidoJugador1)
                    {
                        seleccionarPersonaje(infoEvento.jugador,animaciones,personajesElegidos);
                        spriteTeclaSeleccionarJ1.getSprite().setScale({ESCALA_TECLA_SELECCIONAR_MENU_SELECCION_PERSONAJE,ESCALA_TECLA_SELECCIONAR_MENU_SELECCION_PERSONAJE});
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
                        spriteTeclaSeleccionarJ2.getSprite().setScale({ESCALA_TECLA_SELECCIONAR_MENU_SELECCION_PERSONAJE,ESCALA_TECLA_SELECCIONAR_MENU_SELECCION_PERSONAJE});
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

        spriteTeclaIzquierdaJ1.actualizar();
        spriteTeclaDerechaJ1.actualizar();
        spriteTeclaSeleccionarJ1.actualizar();

        spriteTeclaIzquierdaJ2.actualizar();
        spriteTeclaDerechaJ2.actualizar();
        spriteTeclaSeleccionarJ2.actualizar();

        // Si nos estamos saliendo sin elegir personaje, significa que le hemos
        // dado a ESC, por lo que hay que mover el sprite de darle a ESC
        // a su posición original
        if(saliendo && personajesElegidos.empty())
        {
            spriteEsc.actualizar();
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
        ventana->draw(spriteTeclaSeleccionarJ1);

        ventana->draw(spriteTeclaIzquierdaJ2);
        ventana->draw(spriteTeclaDerechaJ2);
        ventana->draw(spriteTeclaSeleccionarJ2);

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