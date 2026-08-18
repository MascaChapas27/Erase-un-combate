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

    // Dados dos puntos devuelve la distancia entre ellos
    float distanciaEuclidiana(const sf::Vector2f& punto1, const sf::Vector2f& punto2);

    // Una tabla que permite implementar la función stringATipoBucle de forma eficiente
    extern std::unordered_map<std::string,TipoBucle> tablaStringATipoBucle;

    // Dado un string, devuelve el tipo de bucle correspondiente
    TipoBucle stringATipoBucle(std::string string);

    // Una tabla que permite implementar la función stringAEstadoPersonaje de forma eficiente
    extern std::unordered_map<std::string,EstadoPersonaje> tablaStringAEstadoPersonaje;

    // Dado un string, devuelve el estado de personaje correspondiente. Esto es útil
    // al leer los ficheros YAML de cada personaje
    EstadoPersonaje stringAEstadoPersonaje(std::string string);

    // Una tabla que permite implementar la función stringAAccion de forma eficiente
    extern std::unordered_map<std::string,Accion> tablaStringAAccion;

    // Dado un string, devuelve la acción correspondiente
    Accion stringAAccion(const std::string &string);

    // Dado un elemento del enumerado Jugador, devuelve un string que indica
    // el nombre del elemento para poder mostrarlo por pantalla
    std::string jugadorAString(Jugador jugador);

    // Una tabla que permite implementar la función jugadorAString de forma eficiente
    extern std::unordered_map<Jugador,std::string> tablaJugadorAString;

    // Dados dos rectángulos, calcula el centro del subrectángulo que resulta de la intersección
    // de ambos rectángulos (en SFML 3 está la función findIntersection que te encuentra el rectángulo
    // de la intersección pero ya que hice esto pues lo voy a seguir utilizando porque le he cogido cariño)
    sf::Vector2f centroDeInterseccion(sf::IntRect r1, sf::IntRect r2);

    // Devuelve un número real en el rango [0,1)
    float realAleatorio();

    // Dada una acción, devuelve el bit correspondiente
    uint8_t accionABit(Accion accion);

    // Dados dos colores, devuelve una aproximación del primero al segundo, multiplicando el primero por
    // el factorPrimero y multiplicando el segundo por (1-factorPrimero), y sumando los dos resultados.
    // En caso de que la diferencia entre alguno de los elementos (red, green, blue o alpha) sea 0, se cambia
    // en 1 para que no se queda atascada para siempre
    sf::Color aproximarColor(const sf::Color& primerColor, const sf::Color& segundoColor, float factorPrimero);

    // Dados dos floats, devuelve una aproximación del primero al segundo, multiplicando el primero por
    // el factorPrimero y multiplicando el segundo por (1-factorPrimero), y sumando los dos resultados
    float aproximarFloat(const float& primerFloat, const float& segundoFloat, float factorPrimero);

    // Dados dos vectores de dos floats, devuelve una aproximación del primero al segundo de forma similar
    // a como lo hace aproximarFloat
    sf::Vector2f aproximarVector2f(const sf::Vector2f& primerVector, const sf::Vector2f& segundoVector, float factorPrimero);

    // En base al estado en el que se encuentra un personaje, devuelve cuánta prioridad tiene a la hora
    // de ser dibujado. Si un personaje tiene más prioridad que otro, significa que se dibujará encima,
    // para que el personaje con menos prioridad esté más al fondo. Las prioridades son las siguientes:
    // Prioridad 1 (baja): el personaje está tumbado o sufriendo un ataque.
    // Prioridad 2 (media): el personaje está quieto o andando.
    // Prioridad 3 (alta): el personaje está atacando, saltando o celebrando.
    int getPrioridadDibujo(EstadoPersonaje estado);

    // Devuelve true si los dos floats dados difieren en menos que UMBRAL_FLOAT
    bool floatsIguales(float float1, float float2);

    // Cambia la escala de una imagen en la que un "píxel" de imagen son varios
    // píxeles reales. Por ejemplo, si estamos jugando con el zoom al 3 y sacamos
    // una captura (que es lo que pasa en el menú de pausa), es necesario
    // reescalar esa captura para que en vez de tener píxeles de 3x3 los tenga
    // de 1x1. En este caso, longitudLadoPixelOriginal es 3 porque los píxeles
    // de la imagen son de 3x3 píxeles reales. Espero estar explicándome xdd
    sf::Image reescalarImagenA1por1pixel(const sf::Image& imagen, unsigned int longitudLadoPixelOriginal);
}

