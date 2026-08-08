#pragma once

#include "Animacion.hpp"
#include "AnimacionFondoBucleLineal.hpp"
#include <memory>
#include <vector>
#include "SelectorMenuPrincipal.hpp"
#include "FondoMenuPrincipal.hpp"
#include "SpriteDeseado.hpp"

/*
    Esta clase representa el menú principal en el que puedes elegir el modo
    de juego que quieres jugar o entrar en opciones para cambiar cosicas.
*/
class MenuPrincipal {
    private:
        // El constructor es privado porque la clase es Singleton
        MenuPrincipal();

        ~MenuPrincipal();

        // La única instancia
        static MenuPrincipal * menuPrincipal;

        // Aquí se almacenan los selectores asociados a cada
        // sitio al que se puede acceder desde el menú principal
        std::vector<SelectorMenuPrincipal> selectores;

        // Aquí se almacenan los fondos asociados a cada selector
        std::vector<FondoMenuPrincipal> fondos;

        // El índice de la selección actual
        unsigned int seleccionActual;

        // El cartel con el título del juego
        std::shared_ptr<Animacion> cartelTitulo;

        // Los sprites que indican qué teclas o botones hay que pulsar para
        // avanzar al selector de arriba, al selector de abajo, o para seleccionar
        // el selector actual, respectivamente
        SpriteDeseado spriteTeclaArriba;
        SpriteDeseado spriteTeclaAbajo;
        SpriteDeseado spriteTeclaSeleccionar;

        // Este es el control para el que se están mostrando los sprites que indican
        // qué teclas o botones hay que pulsar. Si el control es un teclado, se mostrarán
        // teclas, y si es un mando, se mostrarán botones. Este atributo permite saber si
        // ha habido un cambio en los controles, pudiendo cambiar los sprites. Por ejemplo,
        // si controlUtilizadoParaLosSprites es Control::TECLADO_IZQUIERDA y entonces el
        // jugador 1 empieza a usar un mando, es necesario dejar de mostrar indicaciones
        // de teclas y empezar a mostrar indicaciones de botones
        Control controlUtilizadoParaLosSprites;

        // El efecto que hay a la izquierda que es como dientes de sierra oscuros
        AnimacionFondoBucleLineal dientesSierraArriba;
        AnimacionFondoBucleLineal dientesSierraAbajo;

        // Si está a true, significa que se ha pulsado un selector y es necesario salir
        // lo antes posible del menú principal
        bool selectorPulsado;

        // Rectángulo que tapa el menú principal cuando se selecciona una opción
        sf::RectangleShape rectanguloNegro;

        // Se devuelven los botones y carteles y demás a su posición original
        void resetear();

        // Se avisa a cada selector de su nueva posición relativa cuando se
        // cambia la selección
        void cambiarPosicionRelativa();

        // Se calcula la posición en la que debería estar el sprite de la tecla
        // o el botón que indica cómo realizar una selección. Se llama cada vez
        // que se resetea el menú o cada vez que se cambia de selección
        void calcularPosicionDeseadaSpriteTeclaSeleccionar();

        // Cambia los sprites que indican las teclas o botones que hay que
        // pulsar
        void cambiarSpritesTeclas(Control c);

    public:

        // Comienza el funcionamiento del menú principal, que toma el control
        // de la ventana hasta que no se selecciona otra cosa
        TipoSelectorMenuPrincipal comenzar();

        // NUNCA SE COPIA UN SINGLETON
        MenuPrincipal(MenuPrincipal &otro) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const MenuPrincipal &) = delete;

        // Devuelve la única instancia
        static MenuPrincipal * unicaInstancia();
};