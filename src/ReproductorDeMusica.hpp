#ifndef __REPRODUCTOR_DE_MUSICA_HPP__
#define __REPRODUCTOR_DE_MUSICA_HPP__

#include <SFML/Audio.hpp>
#include "Constantes.hpp"
#include "Enums.hpp"
#include <string>
#include <map>
#include <vector>

/*
    Clase que se encarga de reproducir música para que el
    resto de clases no se tengan que preocupar de eso
*/
class ReproductorDeMusica
{
    private:

        // Constructor privado para que sea más Singleton
        ReproductorDeMusica();

        ~ReproductorDeMusica();

        // Única instancia
        static ReproductorDeMusica * reproductorDeMusica;

        // Música que se está reproduciendo actualmente
        sf::Music musicaActual;

        // Todas las canciones de combate que hay
        std::vector<std::string> cancionesCombate;

        // Volumen actual de la música (del 0 al 100)
        float volumenActual;

    public:

        // Reproduce la canción dado su identificador (ruta del fichero relativa al directorio donde
        // se ejecuta el programa)
        void reproducir(std::string cancion, bool bucle = true);

        // Reproduce aleatoriamente una canción de combate
        void reproducirCancionCombate();

        // Detiene por completo la reproducción de la canción
        void detener();

        // Devuelve el volumen de la canción actual
        float getVolumen();

        // Establece el nuevo volumen para la canción actual y las siguientes
        void setVolumen(float nuevoVolumen);

        // Dice si se está reproduciendo una canción o no
        bool estaReproduciendo();

        // NUNCA SE COPIA UN SINGLETON
        ReproductorDeMusica(ReproductorDeMusica &otro) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const ReproductorDeMusica &) = delete;

        // Devuelve la única instancia
        static ReproductorDeMusica * unicaInstancia();

};

#endif // __REPRODUCTOR_DE_MUSICA_HPP__
