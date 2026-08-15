#include "MenuPrincipal.hpp"
#include "ContenedorDeEfectos.hpp"
#include "ContenedorDeRecursos.hpp"
#include "Constantes.hpp"
#include "VentanaPrincipal.hpp"
#include "GestorDeControles.hpp"
#include "ReproductorDeMusica.hpp"
#include "ReproductorDeSonidos.hpp"
#include "Configuracion.hpp"
#include "Utilidades.hpp"
#include "ClienteDiscord.hpp"
#include <cmath>

// La instancia es nula al principio
MenuPrincipal * MenuPrincipal::menuPrincipal = nullptr;

MenuPrincipal * MenuPrincipal::unicaInstancia()
{
    if(menuPrincipal == nullptr)
        menuPrincipal = new MenuPrincipal();
    return menuPrincipal;
}

MenuPrincipal::~MenuPrincipal()
{
    if(menuPrincipal != nullptr) delete menuPrincipal;
}

MenuPrincipal::MenuPrincipal() : seleccionActual(0),
                                 cartelTitulo(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("titulo")),
                                 spriteTeclaArriba(sf::Sprite(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/tecla-arriba-w.png")),FACTOR_APROXIMACION_SPRITES_TECLAS),
                                 spriteTeclaAbajo(sf::Sprite(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/tecla-abajo-s.png")),FACTOR_APROXIMACION_SPRITES_TECLAS),
                                 spriteTeclaSeleccionar(sf::Sprite(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/tecla-seleccionar-shift.png")),FACTOR_APROXIMACION_SPRITES_TECLAS),
                                 controlUtilizadoParaLosSprites(GestorDeControles::unicaInstancia()->obtenerControlUsadoPorJugador(Jugador::JUGADOR1)),
                                 dientesSierraArriba(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/dientes-sierra.png"),Direccion::ARRIBA,DIENTES_SIERRA_MENU_PRINCIPAL_VELOCIDAD),
                                 dientesSierraAbajo(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/dientes-sierra.png"),Direccion::ABAJO,DIENTES_SIERRA_MENU_PRINCIPAL_VELOCIDAD),
                                 selectorPulsado(false), rectanguloNegro({VENTANA_ANCHURA,VENTANA_ALTURA})
{
    // Las coordenadas del punto de origen del cartelito animado del
    // título se pasan a un valor discreto para que no se vea
    // feo si se utiliza una resolución baja
    cartelTitulo->setOrigen({std::floor(cartelTitulo->getOrigen().x),std::floor(cartelTitulo->getOrigen().y)});

    
    selectores.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/selector-modo-historia.png"),
                            TipoSelectorMenuPrincipal::MODO_HISTORIA);
    
    selectores.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/selector-batalla-vs.png"),
                            TipoSelectorMenuPrincipal::BATALLA_VS);

    selectores.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/selector-opciones.png"),
                            TipoSelectorMenuPrincipal::OPCIONES);
    

    fondos.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/captura-modo-historia.png"),
                            TipoSelectorMenuPrincipal::MODO_HISTORIA);
        
    fondos.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/captura-batalla-vs.png"),
                            TipoSelectorMenuPrincipal::BATALLA_VS);

    fondos.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/captura-opciones.png"),
                            TipoSelectorMenuPrincipal::OPCIONES);

    rectanguloNegro.setFillColor(sf::Color::Black);

    // Se coloca todo en su posición inicial
    resetear();
}

void MenuPrincipal::resetear()
{
    // El cartel del título se pone en su sitio
    cartelTitulo->setPosicion(POSICION_TITULO);

    // Se resetean todos los selectores
    for(SelectorMenuPrincipal& selector : selectores)
    {
        selector.resetear();
    }

    // Se resetean los fondos correspondientes a cada selector
    for(FondoMenuPrincipal& fondo : fondos)
    {
        fondo.resetear();
    }

    // Se indica que se está seleccionando la primera opción
    seleccionActual = 0;

    cambiarSpritesTeclas(controlUtilizadoParaLosSprites);

    // Los sprites de las teclas o botones para ir arriba o abajo se
    // actualizan para tener el valor correcto de transparencia
    sf::Color colorTeclaArriba = spriteTeclaArriba.getSprite().getColor();
    colorTeclaArriba.a = 0;
    spriteTeclaArriba.setColorDeseado(colorTeclaArriba);
    spriteTeclaArriba.actualizarColorInmediatamente();

    sf::Color colorTeclaAbajo = spriteTeclaAbajo.getSprite().getColor();
    colorTeclaAbajo.a = 0;
    spriteTeclaAbajo.setColorDeseado(colorTeclaAbajo);
    spriteTeclaAbajo.actualizarColorInmediatamente();
}

void MenuPrincipal::cambiarPosicionRelativa()
{
    for(int i=0;i<selectores.size();i++)
    {
        selectores[i].setPosicionRelativa(i-seleccionActual);
    }

    for(int i=0;i<fondos.size();i++)
    {
        fondos[i].setPosicionRelativa(i-seleccionActual);
    }

    calcularPosicionDeseadaSpriteTeclaSeleccionar();
}

void MenuPrincipal::calcularPosicionDeseadaSpriteTeclaSeleccionar()
{
    sf::Vector2f posicionDeseada = spriteTeclaSeleccionar.getPosicionDeseada();
    posicionDeseada.x = selectores[seleccionActual].getSprite().getPosition().x - selectores[seleccionActual].getSprite().getTextureRect().size.x/2.f - spriteTeclaSeleccionar.getSprite().getTextureRect().size.x/2.f - DISTANCIA_PIXELES_SPRITE_TECLA_Y_SELECCION;
    spriteTeclaSeleccionar.setPosicionDeseada(posicionDeseada);
}

void MenuPrincipal::cambiarSpritesTeclas(Control c)
{
    switch(c)
    {
        case Control::TECLADO_IZQUIERDA:
            spriteTeclaArriba.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/tecla-arriba-w.png"));
            spriteTeclaAbajo.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/tecla-abajo-s.png"));
            spriteTeclaSeleccionar.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/tecla-seleccionar-shift.png"));
            break;
        
        case Control::MANDO0:
        case Control::MANDO1:
        case Control::MANDO2:
        case Control::MANDO3:
        case Control::MANDO4:
        case Control::MANDO5:
        case Control::MANDO6:
        case Control::MANDO7:
            spriteTeclaArriba.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/boton-arriba-d-pad.png"));
            spriteTeclaAbajo.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/boton-abajo-d-pad.png"));
            spriteTeclaSeleccionar.getSprite().setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/boton-seleccionar.png"));
            break;
    }

    // Se recalcula el TextureRect de cada Sprite para evitar que se entrecorte la textura
    // si la nueva tiene un tamaño distinto a la antigua
    spriteTeclaArriba.getSprite().setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaArriba.getSprite().getTexture().getSize())));
    spriteTeclaAbajo.getSprite().setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaAbajo.getSprite().getTexture().getSize())));
    spriteTeclaSeleccionar.getSprite().setTextureRect(sf::IntRect({0,0},static_cast<sf::Vector2i>(spriteTeclaSeleccionar.getSprite().getTexture().getSize())));


    // Se establece el origen de los sprites de las teclas o botones en el centro
    spriteTeclaArriba.getSprite().setOrigin({static_cast<float>(spriteTeclaArriba.getSprite().getTextureRect().size.x)/2.f,static_cast<float>(spriteTeclaArriba.getSprite().getTextureRect().size.y)/2.f});
    spriteTeclaAbajo.getSprite().setOrigin({static_cast<float>(spriteTeclaAbajo.getSprite().getTextureRect().size.x)/2.f,static_cast<float>(spriteTeclaAbajo.getSprite().getTextureRect().size.y)/2.f});
    spriteTeclaSeleccionar.getSprite().setOrigin({static_cast<float>(spriteTeclaSeleccionar.getSprite().getTextureRect().size.x)/2.f,static_cast<float>(spriteTeclaSeleccionar.getSprite().getTextureRect().size.y)/2.f});

    // Se establece cuál es la posición deseada para los sprites de las teclas o botones
    float factorDiferenciaPosicionYSpritesArribaAbajo = 1.7f;
    spriteTeclaArriba.setPosicionDeseada({POSICION_X_SELECTOR_MENU_PRINCIPAL, POSICION_INICIAL_Y_SELECTOR_MENU_PRINCIPAL - factorDiferenciaPosicionYSpritesArribaAbajo*DIFERENCIA_POSICION_Y_SELECTOR_MENU_PRINCIPAL*std::sqrt(1.f - DIFERENCIA_ESCALA_SELECTOR_MENU_PRINCIPAL)});
    spriteTeclaAbajo.setPosicionDeseada({POSICION_X_SELECTOR_MENU_PRINCIPAL, POSICION_INICIAL_Y_SELECTOR_MENU_PRINCIPAL + factorDiferenciaPosicionYSpritesArribaAbajo*DIFERENCIA_POSICION_Y_SELECTOR_MENU_PRINCIPAL*std::sqrt(1.f - DIFERENCIA_ESCALA_SELECTOR_MENU_PRINCIPAL)});
    spriteTeclaSeleccionar.setPosicionDeseada({0.f, POSICION_INICIAL_Y_SELECTOR_MENU_PRINCIPAL});

    // Se calcula la posición en la que debería estar
    // el sprite de la tecla o botón para seleccionar
    calcularPosicionDeseadaSpriteTeclaSeleccionar();

    // Se mueven los sprites de las teclas o botones a su lugar correspondiente
    spriteTeclaArriba.actualizarPosicionInmediatamente();
    spriteTeclaAbajo.actualizarPosicionInmediatamente();
    spriteTeclaSeleccionar.actualizarPosicionInmediatamente();
}

TipoSelectorMenuPrincipal MenuPrincipal::comenzar(){

    ClienteDiscord::unicaInstancia()->actualizarRichPresence("En el menú principal", "Eligiendo un modo de juego...");

    resetear();

    ReproductorDeMusica::unicaInstancia()->reproducir("musica/menu-principal.ogg");

    sf::RenderWindow * ventana = VentanaPrincipal::unicaInstancia();

    std::list<std::shared_ptr<Animacion>> animaciones;

    animaciones.push_back(cartelTitulo);

    selectorPulsado = false;

    seleccionActual = 0;

    // Esto cuenta hace cuántos frames se pulsó el selector en caso de que
    // selectorPulsado esté a true
    int contadorSelectorPulsado = 0;

    while(!(selectorPulsado && rectanguloNegro.getFillColor().a == 255)){
        // Se prepara un reloj para ver cuánto tiempo pasa entre fotogramas
        sf::Clock reloj;

        if(selectorPulsado)
            contadorSelectorPulsado++;
        
        // Se ejecutan cosillas de Discord en cada frame
        discordpp::RunCallbacks();

        while(const std::optional evento = ventana->pollEvent()){
            if(evento->is<sf::Event::Closed>()){
                exit(EXIT_SUCCESS);
            } else if(!selectorPulsado) {
                InfoEvento infoEvento = GestorDeControles::unicaInstancia()->comprobarEvento(evento);
                if(infoEvento.realizada && infoEvento.jugador == Jugador::JUGADOR1){
                    if (infoEvento.accion == Accion::ESCAPE)
                    {
                        exit(EXIT_SUCCESS);
                    }
                    else if(infoEvento.accion == Accion::ARRIBA && seleccionActual > 0)
                    {
                        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-principal/cambiar-seleccion.ogg");
                        seleccionActual--;
                        spriteTeclaArriba.getSprite().move({0.f,-static_cast<float>(REBOTE_SPRITES_TECLAS_MENU_PRINCIPAL)});
                        cambiarPosicionRelativa();
                    }
                    else if (infoEvento.accion == Accion::ABAJO && seleccionActual < selectores.size()-1)
                    {
                        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-principal/cambiar-seleccion.ogg");
                        seleccionActual++;
                        spriteTeclaAbajo.getSprite().move({0.f,static_cast<float>(REBOTE_SPRITES_TECLAS_MENU_PRINCIPAL)});
                        cambiarPosicionRelativa();
                    }
                    else if (infoEvento.accion == Accion::ATACAR)
                    {
                        std::list<std::shared_ptr<Animacion>> nuevasAnimaciones;
                        
                        selectorPulsado = selectores[seleccionActual].seleccionar(nuevasAnimaciones);

                        animaciones.splice(animaciones.end(),nuevasAnimaciones);

                        if(selectorPulsado)
                        {
                            spriteTeclaSeleccionar.getSprite().move({static_cast<float>(REBOTE_SPRITES_TECLAS_MENU_PRINCIPAL),0.f});

                            ReproductorDeMusica::unicaInstancia()->detener();
                            ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-principal/seleccionar.ogg");
                        }
                    }
                }
            }
        }

        std::list<std::shared_ptr<Animacion>> nuevasAnimaciones;

        for(std::shared_ptr<Animacion> &a : animaciones){
            a->actualizar(nuevasAnimaciones);
        }

        // Si se ha cambiado de control, hay que cambiar los sprites
        if(controlUtilizadoParaLosSprites != GestorDeControles::unicaInstancia()->obtenerControlUsadoPorJugador(Jugador::JUGADOR1))
        {
            controlUtilizadoParaLosSprites = GestorDeControles::unicaInstancia()->obtenerControlUsadoPorJugador(Jugador::JUGADOR1);
            cambiarSpritesTeclas(controlUtilizadoParaLosSprites);
        }

        // Se cambia la opacidad del rectángulo negro según sea necesario
        if(rectanguloNegro.getFillColor().a > 0 && (!selectorPulsado || (selectorPulsado && contadorSelectorPulsado < FRAMES_ESPERA_SALIR_MENU)))
            rectanguloNegro.setFillColor(sf::Color(rectanguloNegro.getFillColor().r, rectanguloNegro.getFillColor().g, rectanguloNegro.getFillColor().b, rectanguloNegro.getFillColor().a-5));
        
        else if (selectorPulsado && contadorSelectorPulsado >= FRAMES_ESPERA_SALIR_MENU && rectanguloNegro.getFillColor().a < 255)
            rectanguloNegro.setFillColor(sf::Color(rectanguloNegro.getFillColor().r, rectanguloNegro.getFillColor().g, rectanguloNegro.getFillColor().b, rectanguloNegro.getFillColor().a+5));

        // Se mueven los fondos de dientes de sierra
        dientesSierraArriba.actualizar(nuevasAnimaciones);
        dientesSierraAbajo.actualizar(nuevasAnimaciones);

        for(std::shared_ptr<Animacion> &nA : nuevasAnimaciones){
            animaciones.push_back(nA);
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

        for(SelectorMenuPrincipal& selector : selectores)
        {
            selector.actualizar();
        }

        for(FondoMenuPrincipal& fondo : fondos)
        {
            fondo.actualizar();
        }

        // Se actualiza el color deseado del sprite de la tecla o botón
        // para moverse hacia abajo si es necesario
        if(spriteTeclaAbajo.getColorDeseado().a > 0 && seleccionActual == selectores.size()-1)
        {
            sf::Color nuevoColor = spriteTeclaAbajo.getColorDeseado();
            nuevoColor.a = 0;
            spriteTeclaAbajo.setColorDeseado(nuevoColor);
        }
        else if (spriteTeclaAbajo.getColorDeseado().a < 255 && seleccionActual < selectores.size()-1)
        {
            sf::Color nuevoColor = spriteTeclaAbajo.getColorDeseado();
            nuevoColor.a = 255;
            spriteTeclaAbajo.setColorDeseado(nuevoColor);
        }
        
        // Lo mismo de antes, pero para el sprite de la tecla o botón
        // para moverse hacia arriba
        if(spriteTeclaArriba.getColorDeseado().a > 0 && seleccionActual == 0)
        {
            sf::Color nuevoColor = spriteTeclaArriba.getColorDeseado();
            nuevoColor.a = 0;
            spriteTeclaArriba.setColorDeseado(nuevoColor);
        }
        else if (spriteTeclaArriba.getColorDeseado().a < 255 && seleccionActual > 0)
        {
            sf::Color nuevoColor = spriteTeclaArriba.getColorDeseado();
            nuevoColor.a = 255;
            spriteTeclaArriba.setColorDeseado(nuevoColor);
        }

        // Se actualizan los atributos de los sprites que indican qué teclas
        // o botones hay que pulsar
        spriteTeclaAbajo.actualizar();
        spriteTeclaArriba.actualizar();
        spriteTeclaSeleccionar.actualizar();

        ventana->clear(sf::Color::Black);

        for(FondoMenuPrincipal& fondo : fondos)
        {
            ventana->draw(fondo);
        }

        ventana->draw(dientesSierraArriba);
        ventana->draw(dientesSierraAbajo);

        for(SelectorMenuPrincipal& selector : selectores)
        {
            ventana->draw(selector);
        }

        ventana->draw(spriteTeclaArriba);
        ventana->draw(spriteTeclaAbajo);
        ventana->draw(spriteTeclaSeleccionar);

        for(std::shared_ptr<Animacion> &a : animaciones){
            ventana->draw(*a);
        }

        ventana->draw(rectanguloNegro);
        
        ventana->display();

        sf::sleep(sf::seconds(1.f / Configuracion::unicaInstancia()->getFPS()) - reloj.reset());
    }

    return static_cast<TipoSelectorMenuPrincipal>(seleccionActual);
}

