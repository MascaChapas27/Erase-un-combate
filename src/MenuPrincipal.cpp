#include "MenuPrincipal.hpp"
#include "ContenedorDeEfectos.hpp"
#include "ContenedorDeRecursos.hpp"
#include "Constantes.hpp"
#include "VentanaPrincipal.hpp"
#include "GestorDeControles.hpp"
#include "ReproductorDeMusica.hpp"
#include "ReproductorDeSonidos.hpp"
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

MenuPrincipal::MenuPrincipal() : seleccionActual(Seleccion::MODO_HISTORIA),
                                 cartelTitulo(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("titulo")),
                                 dientesSierraArriba(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/dientes-sierra.png")),
                                 dientesSierraAbajo(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/dientes-sierra.png")),
                                 selectorModoHistoria(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/selector-modo-historia.png")),
                                 selectorBatallaVS(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/selector-batalla-vs.png")),
                                 selectorOpciones(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/selector-opciones.png")),
                                 capturaModoHistoria(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/captura-modo-historia.png")),
                                 capturaBatallaVS(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/captura-batalla-vs.png")),
                                 capturaOpciones(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/menu-principal/captura-opciones.png")),
                                 brilloSelector(0.f), selectorPulsado(false), rectanguloNegro({VENTANA_ANCHURA,VENTANA_ALTURA}){
    // Se coloca todo en su posición inicial
    resetear();

    shaderDestello = std::make_shared<sf::Shader>();
    if(!shaderDestello->loadFromFile("shaders/blendColor.frag",sf::Shader::Type::Fragment)){
        Bitacora::unicaInstancia()->escribir("ERROR: no se pudo cargar el shader");
        exit(EXIT_FAILURE);
    }

    rectanguloNegro.setFillColor(sf::Color::Black);
}

void MenuPrincipal::resetear() {
    cartelTitulo->setPosicion(POSICION_TITULO);
    dientesSierraArriba.setPosition({0,0});
    dientesSierraAbajo.setPosition({0,-58});
    
    selectorModoHistoria.setPosition({POSICION_X_SELECTOR_SELECCIONADO,POSICION_Y_SELECTOR_MODO_HISTORIA});
    selectorBatallaVS.setPosition({POSICION_X_SELECTOR_SIN_SELECCIONAR,POSICION_Y_SELECTOR_BATALLA_VS});
    selectorOpciones.setPosition({POSICION_X_SELECTOR_SIN_SELECCIONAR,POSICION_Y_SELECTOR_OPCIONES});

    capturaModoHistoria.setPosition(sf::Vector2f(0.f,0.f));
    capturaBatallaVS.setPosition(sf::Vector2f(0.f,10.f));
    capturaOpciones.setPosition(sf::Vector2f(0.f,20.f));

    selectorModoHistoria.setColor(COLOR_SELECTOR_SELECCIONADO);
    selectorBatallaVS.setColor(COLOR_SELECTOR_SIN_SELECCIONAR);
    selectorOpciones.setColor(COLOR_SELECTOR_SIN_SELECCIONAR);

    capturaModoHistoria.setColor(COLOR_CAPTURA_SELECCIONADA);
    capturaBatallaVS.setColor(COLOR_CAPTURA_SIN_SELECCIONAR);
    capturaOpciones.setColor(COLOR_CAPTURA_SIN_SELECCIONAR);
}

Seleccion MenuPrincipal::comenzar(){

    resetear();

    ReproductorDeMusica::unicaInstancia()->reproducir("musica/menu-principal.ogg");

    sf::RenderWindow * ventana = VentanaPrincipal::unicaInstancia();

    std::list<std::shared_ptr<Animacion>> animaciones;

    animaciones.push_back(cartelTitulo);

    selectorPulsado = false;

    seleccionActual = Seleccion::MODO_HISTORIA;

    while(true){
        // Se prepara un reloj para ver cuánto tiempo pasa entre frames
        sf::Clock reloj;

        while(const std::optional evento = ventana->pollEvent()){
            if(evento->is<sf::Event::Closed>()){
                exit(EXIT_SUCCESS);
            } else if(!selectorPulsado) {
                std::pair<Jugador,Accion> par = GestorDeControles::unicaInstancia()->comprobarEvento(evento);
                if((evento->is<sf::Event::JoystickButtonPressed>() || evento->is<sf::Event::KeyPressed>())){
                    if(par.second == Accion::ARRIBA){
                        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-principal/cambiar-seleccion.ogg");
                        switch(seleccionActual){
                            case Seleccion::MODO_HISTORIA:
                                seleccionActual = Seleccion::OPCIONES;
                                break;
                            case Seleccion::BATALLA_VS:
                                seleccionActual = Seleccion::MODO_HISTORIA;
                                break;
                            case Seleccion::OPCIONES:
                                seleccionActual = Seleccion::BATALLA_VS;
                                break;
                        }
                    } else if (par.second == Accion::ABAJO){
                        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-principal/cambiar-seleccion.ogg");
                        switch(seleccionActual){
                            case Seleccion::MODO_HISTORIA:
                                seleccionActual = Seleccion::BATALLA_VS;
                                break;
                            case Seleccion::BATALLA_VS:
                                seleccionActual = Seleccion::OPCIONES;
                                break;
                            case Seleccion::OPCIONES:
                                seleccionActual = Seleccion::MODO_HISTORIA;
                                break;
                        }
                    } else if (par.second == Accion::ATACAR){
                        ReproductorDeMusica::unicaInstancia()->detener();
                        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/menu-principal/seleccionar.ogg");
                        std::shared_ptr<Animacion> anim;
                        shaderDestello->setUniform("amount",1.f);
                        brilloSelector = 1.f;
                        selectorPulsado = true;
                        switch(seleccionActual){
                            case Seleccion::MODO_HISTORIA:
                                anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("selector-modo-historia-destello");
                                anim->setPosicion(selectorModoHistoria.getPosition() + sf::Vector2f(selectorModoHistoria.getTextureRect().size/2));
                                animaciones.push_back(anim);
                                selectorModoHistoria.setPosition({POSICION_X_SELECTOR_SIN_SELECCIONAR,POSICION_Y_SELECTOR_MODO_HISTORIA});
                                break;
                            case Seleccion::BATALLA_VS:
                                anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("selector-batalla-vs-destello");
                                anim->setPosicion(selectorBatallaVS.getPosition() + sf::Vector2f(selectorBatallaVS.getTextureRect().size/2));
                                animaciones.push_back(anim);
                                selectorBatallaVS.setPosition({POSICION_X_SELECTOR_SIN_SELECCIONAR,POSICION_Y_SELECTOR_BATALLA_VS});
                                break;
                            case Seleccion::OPCIONES:
                                anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("selector-opciones-destello");
                                anim->setPosicion(selectorOpciones.getPosition() + sf::Vector2f(selectorOpciones.getTextureRect().size/2));
                                animaciones.push_back(anim);
                                selectorOpciones.setPosition({POSICION_X_SELECTOR_SIN_SELECCIONAR,POSICION_Y_SELECTOR_OPCIONES});
                                break;
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
        if(rectanguloNegro.getFillColor().a > 0 && (!selectorPulsado || (selectorPulsado && brilloSelector >= 0.1f))){
            rectanguloNegro.setFillColor(sf::Color(rectanguloNegro.getFillColor().r, rectanguloNegro.getFillColor().g, rectanguloNegro.getFillColor().b, rectanguloNegro.getFillColor().a-5));
        } else if (selectorPulsado && brilloSelector < 0.1f && rectanguloNegro.getFillColor().a < 255){
            rectanguloNegro.setFillColor(sf::Color(rectanguloNegro.getFillColor().r, rectanguloNegro.getFillColor().g, rectanguloNegro.getFillColor().b, rectanguloNegro.getFillColor().a+5));
        } else if (selectorPulsado && rectanguloNegro.getFillColor().a == 255){
            return seleccionActual;
        }

        dientesSierraArriba.move({0,-0.2});

        if(dientesSierraArriba.getPosition().y <= -58) dientesSierraArriba.setPosition({0,0});

        dientesSierraAbajo.move({0,0.2});

        if(dientesSierraAbajo.getPosition().y >= 0) dientesSierraAbajo.setPosition({0,-58});

        for(std::shared_ptr<Animacion> &nA : nuevasAnimaciones){
            animaciones.push_back(nA);
        }

        // Se actualiza el brillo del shader
        brilloSelector = brilloSelector*0.95f;
        shaderDestello->setUniform("amount",brilloSelector);

        switch(seleccionActual){
            case Seleccion::MODO_HISTORIA:
                capturaModoHistoria.setColor(util::aproximarColor(capturaModoHistoria.getColor(),COLOR_CAPTURA_SELECCIONADA,0.8));
                capturaModoHistoria.setPosition(capturaModoHistoria.getPosition()*0.8f + sf::Vector2f(0.f,0.f)*0.2f);
                selectorModoHistoria.setColor(util::aproximarColor(selectorModoHistoria.getColor(),COLOR_SELECTOR_SELECCIONADO,0.8));
                selectorModoHistoria.setPosition(selectorModoHistoria.getPosition()*0.8f + sf::Vector2f(POSICION_X_SELECTOR_SELECCIONADO,POSICION_Y_SELECTOR_MODO_HISTORIA)*0.2f);

                capturaBatallaVS.setColor(util::aproximarColor(capturaBatallaVS.getColor(),COLOR_CAPTURA_SIN_SELECCIONAR,0.8));
                capturaBatallaVS.setPosition(capturaBatallaVS.getPosition()*0.8f + sf::Vector2f(0.f,10.f)*0.2f);
                selectorBatallaVS.setColor(util::aproximarColor(selectorBatallaVS.getColor(),COLOR_SELECTOR_SIN_SELECCIONAR,0.8));
                selectorBatallaVS.setPosition(selectorBatallaVS.getPosition()*0.8f + sf::Vector2f(POSICION_X_SELECTOR_SIN_SELECCIONAR,POSICION_Y_SELECTOR_BATALLA_VS)*0.2f);

                capturaOpciones.setColor(util::aproximarColor(capturaOpciones.getColor(),COLOR_CAPTURA_SIN_SELECCIONAR,0.8));
                capturaOpciones.setPosition(capturaOpciones.getPosition()*0.8f + sf::Vector2f(0.f,20.f)*0.2f);
                selectorOpciones.setColor(util::aproximarColor(selectorOpciones.getColor(),COLOR_SELECTOR_SIN_SELECCIONAR,0.8));
                selectorOpciones.setPosition(selectorOpciones.getPosition()*0.8f + sf::Vector2f(POSICION_X_SELECTOR_SIN_SELECCIONAR,POSICION_Y_SELECTOR_OPCIONES)*0.2f);
                break;
            case Seleccion::BATALLA_VS:
                capturaModoHistoria.setColor(util::aproximarColor(capturaModoHistoria.getColor(),COLOR_CAPTURA_SIN_SELECCIONAR,0.8));
                capturaModoHistoria.setPosition(capturaModoHistoria.getPosition()*0.8f + sf::Vector2f(0.f,-10.f)*0.2f);
                selectorModoHistoria.setColor(util::aproximarColor(selectorModoHistoria.getColor(),COLOR_SELECTOR_SIN_SELECCIONAR,0.8));
                selectorModoHistoria.setPosition(selectorModoHistoria.getPosition()*0.8f + sf::Vector2f(POSICION_X_SELECTOR_SIN_SELECCIONAR,POSICION_Y_SELECTOR_MODO_HISTORIA)*0.2f);

                capturaBatallaVS.setColor(util::aproximarColor(capturaBatallaVS.getColor(),COLOR_CAPTURA_SELECCIONADA,0.8));
                capturaBatallaVS.setPosition(capturaBatallaVS.getPosition()*0.8f + sf::Vector2f(0.f,0.f)*0.2f);
                selectorBatallaVS.setColor(util::aproximarColor(selectorBatallaVS.getColor(),COLOR_SELECTOR_SELECCIONADO,0.8));
                selectorBatallaVS.setPosition(selectorBatallaVS.getPosition()*0.8f + sf::Vector2f(POSICION_X_SELECTOR_SELECCIONADO,POSICION_Y_SELECTOR_BATALLA_VS)*0.2f);

                capturaOpciones.setColor(util::aproximarColor(capturaOpciones.getColor(),COLOR_CAPTURA_SIN_SELECCIONAR,0.8));
                capturaOpciones.setPosition(capturaOpciones.getPosition()*0.8f + sf::Vector2f(0.f,10.f)*0.2f);
                selectorOpciones.setColor(util::aproximarColor(selectorOpciones.getColor(),COLOR_SELECTOR_SIN_SELECCIONAR,0.8));
                selectorOpciones.setPosition(selectorOpciones.getPosition()*0.8f + sf::Vector2f(POSICION_X_SELECTOR_SIN_SELECCIONAR,POSICION_Y_SELECTOR_OPCIONES)*0.2f);
                break;
            case Seleccion::OPCIONES:
                capturaModoHistoria.setColor(util::aproximarColor(capturaModoHistoria.getColor(),COLOR_CAPTURA_SIN_SELECCIONAR,0.8));
                capturaModoHistoria.setPosition(capturaModoHistoria.getPosition()*0.8f + sf::Vector2f(0.f,-20.f)*0.2f);
                selectorModoHistoria.setColor(util::aproximarColor(selectorModoHistoria.getColor(),COLOR_SELECTOR_SIN_SELECCIONAR,0.8));
                selectorModoHistoria.setPosition(selectorModoHistoria.getPosition()*0.8f + sf::Vector2f(POSICION_X_SELECTOR_SIN_SELECCIONAR,POSICION_Y_SELECTOR_MODO_HISTORIA)*0.2f);

                capturaBatallaVS.setColor(util::aproximarColor(capturaBatallaVS.getColor(),COLOR_CAPTURA_SIN_SELECCIONAR,0.8));
                capturaBatallaVS.setPosition(capturaBatallaVS.getPosition()*0.8f + sf::Vector2f(0.f,-10.f)*0.2f);
                selectorBatallaVS.setColor(util::aproximarColor(selectorBatallaVS.getColor(),COLOR_SELECTOR_SIN_SELECCIONAR,0.8));
                selectorBatallaVS.setPosition(selectorBatallaVS.getPosition()*0.8f + sf::Vector2f(POSICION_X_SELECTOR_SIN_SELECCIONAR,POSICION_Y_SELECTOR_BATALLA_VS)*0.2f);

                capturaOpciones.setColor(util::aproximarColor(capturaOpciones.getColor(),COLOR_CAPTURA_SELECCIONADA,0.8));
                capturaOpciones.setPosition(capturaOpciones.getPosition()*0.8f + sf::Vector2f(0.f,0.f)*0.2f);
                selectorOpciones.setColor(util::aproximarColor(selectorOpciones.getColor(),COLOR_SELECTOR_SELECCIONADO,0.8));
                selectorOpciones.setPosition(selectorOpciones.getPosition()*0.8f + sf::Vector2f(POSICION_X_SELECTOR_SELECCIONADO,POSICION_Y_SELECTOR_OPCIONES)*0.2f);
                break;
        }

        ventana->clear(sf::Color(0,0,0));

        ventana->draw(capturaModoHistoria);
        ventana->draw(capturaBatallaVS);
        ventana->draw(capturaOpciones);

        ventana->draw(dientesSierraArriba);
        ventana->draw(dientesSierraAbajo);

        sf::RenderStates states;

        if(selectorPulsado){
            states.shader = shaderDestello.get();
        }
        
        if(seleccionActual == Seleccion::MODO_HISTORIA){
            ventana->draw(selectorModoHistoria,states);
        } else {
            ventana->draw(selectorModoHistoria);
        }
        
        if(seleccionActual == Seleccion::BATALLA_VS){
            ventana->draw(selectorBatallaVS,states);
        } else {
            ventana->draw(selectorBatallaVS);
        }

        if(seleccionActual == Seleccion::OPCIONES){
            ventana->draw(selectorOpciones,states);
        } else {
            ventana->draw(selectorOpciones);
        }

        for(std::shared_ptr<Animacion> &a : animaciones){
            ventana->draw(*a);
        }

        ventana->draw(rectanguloNegro);
        
        ventana->display();

        sf::sleep(sf::seconds(1.f / NUMERO_FPS) - reloj.reset());
    }
}