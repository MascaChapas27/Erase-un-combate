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

        // Canción para el menú principal y el menú de selección de personajes
        std::string cancionMenu;

        // Canción para el menú de ajustes y el tutorial
        std::string cancionAjustes;

        // Canción que suena entre rondas
        std::string cancionFinRonda;

        // Volumen actual de la música (del 0 al 100)
        float volumenActual;

    public:

        // Reproduce la canción dado su identificador (ruta del fichero relativa al directorio donde
        // se ejecuta el programa)
        void reproducir(std::string cancion, bool bucle = true);

        // Reproduce aleatoriamente una canción de combate
        void reproducirCancionCombate();

        // Reproduce la canción del menú principal
        void reproducirCancionMenu();

        // Reproduce la canción del menú de ajustes
        void reproducirCancionAjustes();

        // Reproduce la canción que suena entre rondas
        void reproducirCancionFinRonda();

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
