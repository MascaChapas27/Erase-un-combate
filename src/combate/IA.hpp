#pragma once

#include <SFML/Graphics.hpp>
#include "Personaje.hpp"

/*
    Esta clase define la IA que usan los bots
    del juego para poder pegarte y que te duela
    mucho. En vez de luchar contra un ser humano,
    puedes luchar contra un jugador controlado
    por esta clase, lo cual mola bastante, pero hay
    que hacerlo bien para que no quede raro
*/
class IA
{
    private:
        // Una referencia al personaje que está manejando la IA
        Personaje& personajeIA;

        // Una referencia al personaje que está manejando el jugador
        Personaje& personajeJugador;

        // Calcula la distancia en el eje X entre los dos personajes
        float calcularDistanciaX();

        // Calcula la distancia en el eje Y entre los dos personajes
        float calcularDistanciaY();

        // Detiene todas las acciones que está realizando
        // el personaje controlado por IA (es como si un
        // jugador soltara las teclas)
        void detenerAcciones();

    public:
        // La IA se construye usando a los dos personajes
        IA(Personaje& personajeIA,Personaje& personajeJugador);

        // En cada frame del combate se debe llamar a esta función.
        // Se encarga de examinar la situación y de realizar acciones
        // según sea necesario
        void actualizarIA();
};