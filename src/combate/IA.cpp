#include "IA.hpp"
#include "Personaje.hpp"
#include "Constantes.hpp"
#include <cmath>

IA::IA(Personaje& personajeIA, Personaje& personajeJugador) : 
personajeIA(personajeIA), personajeJugador(personajeJugador)
{

}

float IA::calcularDistanciaX()
{
    return personajeJugador.getPosicion().x - personajeIA.getPosicion().x;
}

float IA::calcularDistanciaY()
{
    return personajeJugador.getPosicion().y - personajeIA.getPosicion().y;
}

void IA::detenerAcciones()
{
    personajeIA.detenerAccion(Accion::ARRIBA);
    personajeIA.detenerAccion(Accion::ABAJO);
    personajeIA.detenerAccion(Accion::IZQUIERDA);
    personajeIA.detenerAccion(Accion::DERECHA);
    personajeIA.detenerAccion(Accion::ATACAR);
}

void IA::actualizarIA()
{
    // Se dejan de realizar todas las acciones
    detenerAcciones();

    // Se calcula la distancia en el eje X entre los dos personajes
    float distanciaX = calcularDistanciaX();

    // También se calcula la distancia en el eje Y entre los dos personajes
    float distanciaY = calcularDistanciaY();

    // Si el enemigo está cerca, le pego
    if (std::abs(distanciaX) <= RANGO_ATAQUE_IA)
    {
        personajeIA.realizarAccion(Accion::ATACAR);
    }
    // Si no, se mueve el personaje hacia el enemigo
    else if (distanciaX > 0)
    {
        personajeIA.realizarAccion(Accion::DERECHA);
    }
    else
    {
        personajeIA.realizarAccion(Accion::IZQUIERDA);
    }

    // Además, si el enemigo está por encima del personaje,
    // este salta
    if(distanciaY < 0)
    {
        personajeIA.realizarAccion(Accion::ARRIBA);
    }
}