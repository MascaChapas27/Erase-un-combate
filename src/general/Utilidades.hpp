#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Enums.hpp"
#include <string>
#include <sstream>
#include <vector>

/*
    Este namespace contiene funciones útiles
*/

namespace util{

    // Dado un string de entrada y un carácter separador, corta el string original en substrings y
    // los devuelve en forma de vector (sin incluir el carácter separador)
    std::vector<std::string> separarString(std::string string, char separador);

    // Dado un string, devuelve el tipo de bucle correspondiente
    TipoBucle stringATipoBucle(std::string string);

    // Dado un string, devuelve el estado de personaje correspondiente
    EstadoPersonaje stringAEstadoPersonaje(std::string string);

    // Dado un string, devuelve la acción correspondiente
    Accion stringAAccion(const std::string &string);

    // Dados dos rectángulos, calcula el centro del subrectángulo que resulta de la intersección
    // de ambos rectángulos (en SFML 3 está la función findIntersection que te encuentra el rectángulo
    // de la intersección pero ya que hice esto pues lo voy a seguir utilizando porque le he cogido cariño)
    sf::Vector2f centroDeInterseccion(sf::IntRect r1, sf::IntRect r2);

    // Devuelve un número real en el rango [0,1)
    double realAleatorio();

    // Dada una acción, devuelve el bit correspondiente
    uint8_t accionABit(Accion accion);

    // Dados dos colores, devuelve una aproximación del primero al segundo, multiplicando el primero por
    // el factorPrimero y multiplicando el segundo por (1-factorPrimero), y sumando los dos resultados.
    // En caso de que la diferencia entre alguno de los elementos (red, green, blue o alpha) sea 0, se cambia
    // en 1 para que no se queda atascada para siempre
    sf::Color aproximarColor(const sf::Color& primerColor, const sf::Color& segundoColor, double factorPrimero);

    // Dados dos floats, devuelve una aproximación del primero al segundo, multiplicando el primero por
    // el factorPrimero y multiplicando el segundo por (1-factorPrimero), y sumando los dos resultados
    float aproximarFloat(const float& primerFloat, const float& segundoFloat, double factorPrimero);

    // En base al estado en el que se encuentra un personaje, devuelve cuánta prioridad tiene a la hora
    // de ser dibujado. Si un personaje tiene más prioridad que otro, significa que se dibujará encima,
    // para que el personaje con menos prioridad esté más al fondo. Las prioridades son las siguientes:
    // Prioridad 1 (baja): el personaje está tumbado o sufriendo un ataque.
    // Prioridad 2 (media): el personaje está quieto o andando.
    // Prioridad 3 (alta): el personaje está atacando, saltando o celebrando.
    int getPrioridadDibujo(EstadoPersonaje estado);
}

