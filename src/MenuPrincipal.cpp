#include "MenuPrincipal.hpp"
#include "ContenedorDeEfectos.hpp"
#include "ContenedorDeRecursos.hpp"
#include "Constantes.hpp"
#include "VentanaPrincipal.hpp"

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

MenuPrincipal::MenuPrincipal() : cartelTitulo(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("titulo")),
                                 dientesSierraArriba(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/dientes-sierra.png")),
                                 dientesSierraAbajo(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/dientes-sierra.png")){
    cartelTitulo->setPosicion(POSICION_TITULO);
    dientesSierraAbajo.setPosition({0,-58});
}

void MenuPrincipal::comenzar(){

    sf::RenderWindow * ventana = VentanaPrincipal::unicaInstancia();

    std::list<std::shared_ptr<Animacion>> animaciones;

    animaciones.push_back(cartelTitulo);

    while(true){
        // Se prepara un reloj para ver cuánto tiempo pasa entre frames
        sf::Clock reloj;

        while(const std::optional evento = ventana->pollEvent()){
            if(evento->is<sf::Event::Closed>()){
                exit(EXIT_SUCCESS);
            }
        }

        std::list<std::shared_ptr<Animacion>> nuevasAnimaciones;

        for(std::shared_ptr<Animacion> &a : animaciones){
            a->actualizar(nuevasAnimaciones);
        }

        dientesSierraArriba.move({0,-0.5});

        if(dientesSierraArriba.getPosition().y <= -58) dientesSierraArriba.setPosition({0,0});

        dientesSierraAbajo.move({0,0.5});

        if(dientesSierraAbajo.getPosition().y >= 0) dientesSierraAbajo.setPosition({0,-58});

        for(std::shared_ptr<Animacion> &nA : nuevasAnimaciones){
            animaciones.push_back(nA);
        }

        ventana->clear(sf::Color(160,160,160));

        ventana->draw(dientesSierraArriba);
        ventana->draw(dientesSierraAbajo);

        for(std::shared_ptr<Animacion> &a : animaciones){
            ventana->draw(*a);
        }
        
        ventana->display();

        sf::sleep(sf::seconds(1.f / NUMERO_FPS) - reloj.reset());
    }
}