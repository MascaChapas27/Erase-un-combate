#pragma once

#include <SFML/Audio.hpp>
#include "Constantes.hpp"
#include "Enums.hpp"
#include <string>
#include <unordered_map>
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
    static ReproductorDeMusica *reproductorDeMusica;

    // Ruta de la canción que se está reproduciendo actualmente
    std::string cancionActual;

    // Mapa donde se guardan las canciones según se van reproduciendo
    // para volver a reproducirlas después y así tenerlas disponibles
    std::unordered_map<std::string, sf::Music> canciones;

    // Todas las canciones de combate que hay
    std::vector<std::string> cancionesCombate;

    // Volumen actual de la música (del 0 al 100)
    float volumenActual;

    // Tono actual de la música. Por defecto es 1, pero se puede
    // poner a un valor mayor que 1 para que la música suene más
    // agudo, o a un valor menor que 1 para que suene más grave
    float tonoActual;

public:
    // Reproduce desde el principio (o desde un offset dado) la canción dado su
    // identificador (ruta del fichero relativa al directorio donde se ejecuta
    // el programa)
    void reproducir(const std::string& cancion, bool bucle = true, const sf::Time& progreso = sf::seconds(0.f));

    // Reanuda una canción que había sido pausada anteriormente. Si la canción no
    // está en pausa, ocurre un error
    void reanudar(const std::string& cancion);

    // Reproduce aleatoriamente una canción de combate
    void reproducirCancionCombate();

    // Detiene la canción de combate actual y reproduce
    // la canción de pausa correspondiente
    void pausarCancionCombate();

    // Detiene la versión de pausa de la canción de combate
    // actual y sigue reproduciendo la versión normal
    void reanudarCancionCombate();

    // Detiene por completo la reproducción de la canción actual
    void detener();

    // Pausa la reproducción de la canción actual. Si se reproduce de nuevo, seguirá
    // por donde estaba
    void pausar();

    // Devuelve el offset (el tiempo que ha pasado desde el inicio) de la canción que
    // se está reproduciendo actualmente
    sf::Time getOffsetCancionActual();

    // Devuelve el volumen de reproducción de música
    float getVolumen();

    // Establece el nuevo volumen para la canción actual y las siguientes
    void setVolumen(float nuevoVolumen);

    // Devuelve el tono de reproducción de música
    float getTono();

    // Establece el nuevo tono para la canción actual y las siguientes
    void setTono(float nuevoTono);

    // Dice si se está reproduciendo una canción o no
    bool estaReproduciendo();

    // Devuelve la ruta de la canción que se está reproduciendo actualmente
    std::string getCancionActual();

    // NUNCA SE COPIA UN SINGLETON
    ReproductorDeMusica(ReproductorDeMusica &otro) = delete;

    // NUNCA SE ASIGNA UN SINGLETON
    void operator=(const ReproductorDeMusica &) = delete;

    // Devuelve la única instancia
    static ReproductorDeMusica *unicaInstancia();
};
