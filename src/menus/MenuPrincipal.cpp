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

// La instancia es nula al principio
MenuPrincipal * MenuPrincipal::menuPrincipal = nullptr;

MenuPrincipal * MenuPrincipal::unicaInstancia()
{
    if(menuPrincipal == nullptr)
        menuPrincipal = new MenuPrincipal();
    return menuPrincipal;
}

MenuPrincipal::~MenuPrincipal(){
    if(menuPrincipal != nullptr) delete menuPrincipal;
}

MenuPrincipal::MenuPrincipal() : seleccionActual(0),
                                 cartelTitulo(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("titulo")),
                                 dientesSierraArriba(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/dientes-sierra.png"),Direccion::ARRIBA,DIENTES_SIERRA_MENU_PRINCIPAL_VELOCIDAD),
                                 dientesSierraAbajo(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/dientes-sierra.png"),Direccion::ABAJO,DIENTES_SIERRA_MENU_PRINCIPAL_VELOCIDAD),
                                 selectorPulsado(false), rectanguloNegro({VENTANA_ANCHURA,VENTANA_ALTURA}){
    // Se coloca todo en su posición inicial
    resetear();
    
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
}

void MenuPrincipal::resetear() {
    cartelTitulo->setPosicion(POSICION_TITULO);

    for(SelectorMenuPrincipal& selector : selectores)
    {
        selector.resetear();
    }

    for(FondoMenuPrincipal& fondo : fondos)
    {
        fondo.resetear();
    }
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
}

TipoSelectorMenuPrincipal MenuPrincipal::comenzar(){

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

        while(const std::optional evento = ventana->pollEvent()){
            if(evento->is<sf::Event::Closed>()){
                exit(EXIT_SUCCESS);
            } else if(!selectorPulsado) {
                InfoEvento infoEvento = GestorDeControles::unicaInstancia()->comprobarEvento(evento);
                if(infoEvento.realizada){
                    if(infoEvento.accion == Accion::ARRIBA && seleccionActual > 0)
                    {
                        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-principal/cambiar-seleccion.ogg");
                        seleccionActual--;
                        cambiarPosicionRelativa();
                    }
                    else if (infoEvento.accion == Accion::ABAJO && seleccionActual < selectores.size()-1)
                    {
                        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-principal/cambiar-seleccion.ogg");
                        seleccionActual++;
                        cambiarPosicionRelativa();
                    }
                    else if (infoEvento.accion == Accion::ATACAR)
                    {
                        std::list<std::shared_ptr<Animacion>> nuevasAnimaciones;
                        
                        selectorPulsado = selectores[seleccionActual].seleccionar(nuevasAnimaciones);

                        animaciones.splice(animaciones.end(),nuevasAnimaciones);

                        if(selectorPulsado)
                        {
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

        // Se cambia la opacidad del rectángulo negro según sea necesario
        if(rectanguloNegro.getFillColor().a > 0 && (!selectorPulsado || (selectorPulsado && contadorSelectorPulsado < FRAMES_ESPERA_SALIR_MENU_PRINCIPAL))){
            rectanguloNegro.setFillColor(sf::Color(rectanguloNegro.getFillColor().r, rectanguloNegro.getFillColor().g, rectanguloNegro.getFillColor().b, rectanguloNegro.getFillColor().a-5));
        } else if (selectorPulsado && contadorSelectorPulsado >= FRAMES_ESPERA_SALIR_MENU_PRINCIPAL && rectanguloNegro.getFillColor().a < 255){
            rectanguloNegro.setFillColor(sf::Color(rectanguloNegro.getFillColor().r, rectanguloNegro.getFillColor().g, rectanguloNegro.getFillColor().b, rectanguloNegro.getFillColor().a+5));
        }

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

        ventana->clear(sf::Color(0,0,0));

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

        for(std::shared_ptr<Animacion> &a : animaciones){
            ventana->draw(*a);
        }

        ventana->draw(rectanguloNegro);
        
        ventana->display();

        sf::sleep(sf::seconds(1.f / Configuracion::unicaInstancia()->getFPS()) - reloj.reset());
    }

    return static_cast<TipoSelectorMenuPrincipal>(seleccionActual);
}