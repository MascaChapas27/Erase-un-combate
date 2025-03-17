#ifndef __VENTANA_PRINCIPAL_HPP__
#define __VENTANA_PRINCIPAL_HPP__

#include <SFML/Graphics.hpp>

/*
    Clase "Singleton" para tener una ventana global. En vez de ser un Singleton de la propia clase, lo es
    de una ventana sf::RenderWindow, que es un poco más raro pero bueno, funciona bien que es lo que importa
*/
class VentanaPrincipal {
    private:
        // Constructor por defecto vacío
        VentanaPrincipal();

        ~VentanaPrincipal();

        // Única instancia
        static sf::RenderWindow * ventanaPrincipal;

        // Zoom aplicado
        static int zoom;

        // Indica si está permitido que la ventana vibre
        static bool vibracionActivada;

        // Potencia de vibración
        static int potenciaVibracion;

        // Contador que, al llegar a cero, indica que se puede actualizar la vibración otra vez
        static int contadorVibracion;

    public:
        // Devuelve la única instancia
        static sf::RenderWindow * unicaInstancia();

        // Aumenta el zoom en 100% con respecto al zoom actual (de 100% a 200%, luego 300%...)
        static void aumentarZoom();

        // Disminuye el zoom en 100% con respecto al zoom actual (de 300% a 200%, luego 100% y ya no baja más)
        static void disminuirZoom();

        // Establece el zoom actual en el indicado (debe ser 1 o mayor)
        static void setZoom(int zoom);

        // Devuelve un número del 1 al n representando el aumento con respecto al tamaño inicial
        static int getZoom();

        // Dada una potencia de vibración, la pantalla se pone a vibrar hacia arriba y hacia abajo
        static void vibrar(int potenciaVibracion);

        // Actualiza la ventana si tiene que vibrar
        static void actualizar();

        // NUNCA SE COPIA UN SINGLETON
        VentanaPrincipal(VentanaPrincipal &otra) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const VentanaPrincipal &) = delete;
};

#endif // __VENTANA_PRINCIPAL_HPP__
