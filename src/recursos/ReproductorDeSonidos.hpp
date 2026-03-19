#pragma once

#include <SFML/Audio.hpp>
#include "Constantes.hpp"
#include "Enums.hpp"
#include <string>
#include <unordered_map>
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
    static ReproductorDeSonidos *reproductorDeSonidos;

    // Mapa donde se relaciona cada ruta de fichero de sonido con su sonido
    std::unordered_map<std::string, sf::Sound> sonidos;

    // Volumen actual para los efectos de sonido (del 0 al 100)
    float volumenActual;

    // Tono actual para los efectos de sonido (1 es un tono normal,
    // entre 0 y 1 son tonos graves y entre 1 e infinito son tonos
    // agudos)
    float tonoActual;

public:
    // Reproduce el sonido dado su identificador (ruta del fichero relativa al directorio donde
    // se ejecuta el programa). Se puede especificar un tono en el que se reproducirá el
    // sonido, o se puede dejar como está para que se reproduzca con el tono por defecto
    void reproducir(std::string sonido, float tono = 1.f);

    // Detiene por completo la reproducción de la canción
    void detener();

    // Devuelve el volumen actual para efectos de sonido
    float getVolumen();

    // Establece el nuevo volumen para los efectos de sonido
    void setVolumen(float nuevoVolumen);

    // Devuelve el tono de reproducción de los efectos de sonido
    float getTono();

    // Establece el nuevo tono para los efectos de sonido
    void setTono(float nuevoTono);

    // Devuelve true si el efecto de sonido se está reproduciendo
    bool estaReproduciendo(std::string sonido);

    // NUNCA SE COPIA UN SINGLETON
    ReproductorDeSonidos(ReproductorDeSonidos &otro) = delete;

    // NUNCA SE ASIGNA UN SINGLETON
    void operator=(const ReproductorDeSonidos &) = delete;

    // Devuelve la única instancia
    static ReproductorDeSonidos *unicaInstancia();
};
