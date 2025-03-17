#ifndef __UTILIDADES_HPP__
#define __UTILIDADES_HPP__

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
}

#endif