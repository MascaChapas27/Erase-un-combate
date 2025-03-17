#ifndef __REPRODUCTOR_DE_SONIDOS_HPP__
#define __REPRODUCTOR_DE_SONIDOS_HPP__

#include <SFML/Audio.hpp>
#include "Constantes.hpp"
#include "Enums.hpp"
#include <string>
#include <map>
#include <vector>

/*
    Clase que se encarga de reproducir efectos de sonido para que el
    resto de clases no se tengan que preocupar de eso
*/
class ReproductorDeSonidos
{
    private:

        // Constructor privado para que sea más Singleton
        ReproductorDeSonidos();

        ~ReproductorDeSonidos();

        // Única instancia
        static ReproductorDeSonidos * reproductorDeSonidos;

        // Mapa donde se relaciona cada ruta de fichero de sonido con su sonido
        std::map<std::string,sf::Sound> sonidos;

        // Volumen actual para los efectos de sonido (del 0 al 100)
        float volumenActual;

    public:

        // Reproduce el sonido dado su identificador (ruta del fichero relativa al directorio donde
        // se ejecuta el programa)
        void reproducir(std::string sonido);

        // Detiene por completo la reproducción de la canción
        void detener();

        // Devuelve el volumen actual para efectos de sonido
        float getVolumen();

        // Establece el nuevo volumen para los efectos de sonido
        void setVolumen(float nuevoVolumen);

        // Devuelve true si el efecto de sonido se está reproduciendo
        bool estaReproduciendo(std::string sonido);

        // NUNCA SE COPIA UN SINGLETON
        ReproductorDeSonidos(ReproductorDeSonidos &otro) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const ReproductorDeSonidos &) = delete;

        // Devuelve la única instancia
        static ReproductorDeSonidos * unicaInstancia();

};

#endif // __REPRODUCTOR_DE_SONIDOS_HPP__
