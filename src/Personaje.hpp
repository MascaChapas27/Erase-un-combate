#pragma once

#include "AnimacionPorFrames.hpp"
#include "Enums.hpp"
#include "AtaqueEspecial.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include <list>
#include <memory>

/*
    Esta clase define las cualidades de un personaje genérico, pudiendo ser
    utilizada por todos los posibles personajes al ser estos lo suficientemente
    parecidos como para compartir clase
*/
class Personaje : public sf::Drawable {

    // La clase Personaje es completamente ajena a la existencia de la clase GUIPersonaje,
    // pero la segunda pueda acceder con total libertad a los miembros privados de la primera
    friend class GUIPersonaje;

    private:

        // Cantidad actual de puntos de vida
        int puntosDeVida;

        // Cantidad máxima de puntos de vida
        int maxPuntosDeVida;

        // Cantidad actual del medidor de súper
        int medidorSuper;

        // Velocidad máxima del personaje en el eje X
        float velocidadMaxima;

        // Fuerza de salto del personaje (aunque en el fichero YAML
        // se especifique un número positivo, fuerzaSalto será negativo
        // ya que en SFML el eje Y va al revés)
        float fuerzaSalto;

        // Nombre del personaje
        std::string nombre;

        // Velocidad en los ejes X e Y
        sf::Vector2f velocidad;

        // Escala para el sprite del personaje para así dar la
        // sensación de que se estira y se comprime (o al menos eso me
        // ha enseñado el vídeo ese de los 12 principios de la animación)
        sf::Vector2f escalaSprite;

        // Va aumentando si te mueves estando tumbado y cuando llega a MAX_CONTADOR_TUMBADO
        // tu personaje se levanta y el contador vuelve a cero
        int contadorTumbado;

        // Sirve para que el personaje se vuelva blanco al transformarse para su ataque súper,
        // siendo su máximo 255 (completamente blanco) y su mínimo 0 (normal)
        int contadorBlanco;

        // Cuenta cuántos frames lleva el personaje andando alejándose del enemigo
        int contadorEsquiveSuper;

        // Jugador al que está asociado el personaje
        Jugador jugador;

        // Estado en el que se encuentra el personaje
        EstadoPersonaje estado;

        // Shader que hace que el personaje se ponga blanco al preparar el ataque súper
        std::shared_ptr<sf::Shader> shader;

        // Animación del personaje según el estado (siempre son animaciones por frames pero es bueno
        // usar punteros compartidos con las animaciones)
        std::map<EstadoPersonaje,std::shared_ptr<AnimacionPorFrames>> animaciones;

        // Indica qué acciones están siendo realizadas
        std::map<Accion,bool> accionesRealizadas;

        // Sirve para comprobar si el jugador ha pulsado las teclas necesarias para hacer el ataque especial
        AtaqueEspecial ataqueEspecial;

        // Función auxiliar para moverse un poco a la derecha
        void moverseDerecha();

        // Función auxiliar para moverse un poco a la izquierda
        void moverseIzquierda();

        // Función auxiliar para disminuir la velocidad un poco hacia 0
        void pararMovimiento();

        // Función auxiliar para mostrar dos nubes de polvo a los pies del personaje
        void levantarPolvo(std::list<std::shared_ptr<Animacion>> &efectosInsertados);

    public:

        // Construye el personaje desde cero
        Personaje(std::map<EstadoPersonaje,std::shared_ptr<AnimacionPorFrames>> animaciones, std::string nombre, int maxPuntosDeVida, float velocidadMaxima, float fuerzaSalto, std::vector<Accion> accionesAtaqueEspecial);

        // Indica que se está pulsando un botón
        void realizarAccion(Accion accion);

        // Indica que se ha soltado un botón
        void detenerAccion(Accion accion);

        // Realiza un clon del personaje para que las animaciones no se rayen
        Personaje clonar();

        // Actualiza la posición del personaje y demás según los botones que están pulsados
        virtual void actualizar(sf::Vector2f posicionEnemigo, std::list<std::shared_ptr<Animacion>> &efectosInsertados);

        // Comprueba si la hitbox del personaje ha colisionado con algún ataque enemigo y
        // reacciona de forma adecuada
        virtual void comprobarColisiones(const std::list<std::shared_ptr<Animacion>> &animaciones, std::list<std::shared_ptr<Animacion>> &efectosInsertados);

        // Devuelve los puntos de vida actuales
        int getPuntosDeVida();

        // Devuelve la cantidad máxima de puntos de vida del personaje
        int getMaxPuntosDeVida();

        // Reestablece los puntos de vida del personaje a su máximo valor
        void curarAlMaximo();

        // Establece la cantidad de puntos de vida
        void setPuntosDeVida(int puntosDeVida);

        // Establece la carga del medidor de super
        void setMedidorSuper(int medidorSuper);

        // Establece la posición del personaje
        void setPosicion(float x, float y);
        void setPosicion(sf::Vector2f posicion);

        // Devuelve la posición del personaje
        sf::Vector2f getPosicion();

        // Devuelve true si está mirando hacia la derecha
        bool isMirandoDerecha();

        // Voltea al jugador para que mire al otro lado (al principio estará
        // mirando a la derecha)
        void voltear();

        // Mueve al personaje según lo indicado (en realidad mueve la animación del estado
        // actual pero el resultado es el mismo)
        void mover(float offsetX, float offsetY);

        // Devuelve el estado actual del personaje
        EstadoPersonaje getEstado();

        // Establece el jugador asociado
        void setJugador(Jugador jugador);

        // Devuelve el jugador que está controlando al personaje
        Jugador getJugador();

        // Devuelve la animación de un estado
        std::shared_ptr<AnimacionPorFrames> getAnimacionSegunEstado(EstadoPersonaje estado);

        // Establece el mapa de animaciones
        void setAnimaciones(const std::map<EstadoPersonaje,std::shared_ptr<AnimacionPorFrames>> &animaciones);

        // Cambia el estado
        void cambiarEstado(EstadoPersonaje estadoNuevo);

        // Las clases que heredan de sf::Drawable deben implementar draw
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};


