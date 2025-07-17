#pragma once

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

        // Ruta de la canción que se está reproduciendo actualmente
        std::string cancionActual;

        // Mapa donde se guardan las canciones según se van reproduciendo
        // para volver a reproducirlas después y así tenerlas disponibles
        std::unordered_map<std::string,sf::Music> canciones;

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

        // Detiene por completo la reproducción de la canción actual
        void detener();

        // Pausa la reproducción de la canción actual. Si se reproduce de nuevo, seguirá
        // por donde estaba
        void pausar();

        // Devuelve el volumen de la canción actual
        float getVolumen();

        // Establece el nuevo volumen para la canción actual y las siguientes
        void setVolumen(float nuevoVolumen);

        // Dice si se está reproduciendo una canción o no
        bool estaReproduciendo();

        // Devuelve la ruta de la canción que se está reproduciendo actualmente
        std::string getCancionActual();

        // NUNCA SE COPIA UN SINGLETON
        ReproductorDeMusica(ReproductorDeMusica &otro) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const ReproductorDeMusica &) = delete;

        // Devuelve la única instancia
        static ReproductorDeMusica * unicaInstancia();

};


