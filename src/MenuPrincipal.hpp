#ifndef __MENU_PRINCIPAL_HPP__
#define __MENU_PRINCIPAL_HPP__

#include "Animacion.hpp"
#include <memory>

/*
    Enumerado con las posibles selecciones
*/
enum class Seleccion{
    MODO_HISTORIA, BATALLA_VS, OPCIONES
};

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

        // La selección actual
        Seleccion seleccionActual;

        // El cartel con el título del juego
        std::shared_ptr<Animacion> cartelTitulo;

        // El efecto que hay a la izquierda que es como dientes de sierra oscuros
        sf::Sprite dientesSierraArriba;
        sf::Sprite dientesSierraAbajo;

        // Las posibles selecciones del menú principal
        sf::Sprite selectorModoHistoria;
        sf::Sprite selectorBatallaVS;
        sf::Sprite selectorOpciones;

        // Las capturas correspondientes con cada selección
        sf::Sprite capturaModoHistoria;
        sf::Sprite capturaBatallaVS;
        sf::Sprite capturaOpciones;

    public:

        // Comienza el funcionamiento del menú principal, que toma el control
        // de la ventana hasta que no se selecciona otra cosa
        void comenzar();

        // NUNCA SE COPIA UN SINGLETON
        MenuPrincipal(MenuPrincipal &otro) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const MenuPrincipal &) = delete;

        // Devuelve la única instancia
        static MenuPrincipal * unicaInstancia();
};

#endif