#include "MenuPausa.hpp"

#include "VentanaPrincipal.hpp"
#include "Constantes.hpp"
#include "ClienteDiscord.hpp"
#include "GestorDeControles.hpp"
#include "Configuracion.hpp"
#include "SpriteDeseado.hpp"
#include "Utilidades.hpp"
#include "ReproductorDeMusica.hpp"
#include <SFML/Graphics.hpp>
#include <string>

// La instancia es nula al principio
MenuPausa * MenuPausa::menuPausa = nullptr;

MenuPausa * MenuPausa::unicaInstancia()
{
    if(menuPausa == nullptr)
        menuPausa = new MenuPausa();
    return menuPausa;
}

MenuPausa::~MenuPausa()
{
    if(menuPausa != nullptr) delete menuPausa;
}

MenuPausa::MenuPausa() :
fondoCuadriculado(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/eleccion-personaje/fondo-cuadricula.png"),Direccion::ARRIBA_IZQUIERDA,VELOCIDAD_FONDO_CUADRICULADO_SELECCION_PERSONAJE)
{

}

void MenuPausa::comenzar()
{
    ReproductorDeMusica::unicaInstancia()->pausarCancionCombate();

    fondoCuadriculado.resetear();

    // Aquí se guarda la ventana principal para tenerla más a mano
    sf::RenderWindow* ventana(VentanaPrincipal::unicaInstancia());

    // En esta textura se almacena el contenido de la ventana antes
    // de pausar
    sf::Texture texturaVentana(ventana->getSize());
    texturaVentana.update(*ventana);

    // Se obtiene una imagen de la textura
    sf::Image imagenVentana(texturaVentana.copyToImage());

    // Se escala la imagen para que cada píxel del dibujo sea
    // de tamaño 1x1 píxeles en la imagen
    sf::Image nuevaImagenVentana(util::reescalarImagenA1por1pixel(imagenVentana,VentanaPrincipal::getZoom()));

    // Se actualiza la textura con la imagen reescalada
    bool resizeCorrecto = texturaVentana.resize(nuevaImagenVentana.getSize());
    if(!resizeCorrecto)
    {
        Bitacora::unicaInstancia()->escribir("Emilio: mire señor Juan, he intentado cambiar el tamaño de esta textura para el menú de pausa, pero nada, no hay manera.");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: ¿cómo que no hay manera?");
        Bitacora::unicaInstancia()->escribir("Emilio: que sí, que sí, mire. La textura es de tamaño "+std::to_string(texturaVentana.getSize().x)+"x"+std::to_string(texturaVentana.getSize().y)+", pero si la intento cambiar para que sea de tamaño "+std::to_string(nuevaImagenVentana.getSize().x)+"x"+std::to_string(nuevaImagenVentana.getSize().y)+", no hay manera.");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: qué follón... se suspende la junta.");
        exit(EXIT_FAILURE);
    }

    texturaVentana.update(nuevaImagenVentana);

    // En base a la textura recién creada, se construye un sprite,
    // el cual se mostrará en el fondo
    SpriteDeseado spriteFondo(texturaVentana,FACTOR_APROXIMACION_ESTADO_ANTERIOR_MENU_PAUSA);

    // Se establecen los valores deseados para el sprite del estado
    // anterior a la pausa
    spriteFondo.setColorDeseado(COLOR_ESTADO_ANTERIOR_MENU_PAUSA);
    spriteFondo.setEscalaDeseada({ESCALA_ESTADO_ANTERIOR_MENU_PAUSA,ESCALA_ESTADO_ANTERIOR_MENU_PAUSA});
    spriteFondo.setPosicionDeseada({static_cast<float>(VENTANA_ANCHURA)/2.f,static_cast<float>(VENTANA_ALTURA)/2.f});

    // El origen del sprite estará en el centro
    spriteFondo.getSprite().setOrigin({static_cast<float>(VENTANA_ANCHURA)/2.f,static_cast<float>(VENTANA_ALTURA)/2.f});

    // Se coloca el sprite en el centro
    spriteFondo.actualizarPosicionInmediatamente();

    // En este flag se indica cuándo se debe salir del menú de pausa
    bool saliendo = false;

    while(!(saliendo && util::floatsIguales(spriteFondo.getSprite().getScale().x,1.f))){
        // Se prepara un reloj para ver cuánto tiempo pasa entre fotogramas
        sf::Clock reloj;
        
        // Se ejecutan cosillas de Discord en cada frame
        discordpp::RunCallbacks();

        while(const std::optional evento = ventana->pollEvent()){
            if(evento->is<sf::Event::Closed>()){
                exit(EXIT_SUCCESS);
            }
            else
            {
                InfoEvento infoEvento = GestorDeControles::unicaInstancia()->comprobarEvento(evento);
                if(infoEvento.realizada && infoEvento.jugador == Jugador::JUGADOR1){
                    if (infoEvento.accion == Accion::ESCAPE)
                    {
                        saliendo = true;
                        spriteFondo.setEscalaDeseada({1.f,1.f});
                        spriteFondo.setColorDeseado(sf::Color::White);
                    }
                }
            }
        }

        spriteFondo.actualizar();
        
        std::list<std::shared_ptr<Animacion>> nuevasAnimaciones;
        fondoCuadriculado.actualizar(nuevasAnimaciones);

        ventana->clear();

        ventana->draw(fondoCuadriculado);
        ventana->draw(spriteFondo);

        ventana->display();

        sf::sleep(sf::seconds(1.f / Configuracion::unicaInstancia()->getFPS()) - reloj.reset());
    }

    ReproductorDeMusica::unicaInstancia()->reanudarCancionCombate();
}