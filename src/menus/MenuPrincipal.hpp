#pragma once

#include "Animacion.hpp"
#include "AnimacionFondoBucleLineal.hpp"
#include <memory>
#include <vector>

#include "SelectorMenuPrincipal.hpp"
#include "FondoMenuPrincipal.hpp"

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