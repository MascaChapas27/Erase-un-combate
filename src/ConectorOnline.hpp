#pragma once

#include "Enums.hpp"
#include <SFML/Network.hpp>
#include <optional>
#include <unordered_set>

// Me dio pereza hacer un hpp y un cpp para esto así que mira aquí lo pongo
struct AccionesOnline {
    std::unordered_set<Accion> accionesRealizadasLocal;
    std::unordered_set<Accion> accionesDetenidasLocal;
    std::unordered_set<Accion> accionesRealizadasRemoto;
    std::unordered_set<Accion> accionesDetenidasRemoto;
};

// Esta clase se encarga de conectar el juego a otro jugador para poder
// jugar si el vecino tiene wifis y te los presta
class ConectorOnline {
    private:
        // Indica si el jugador actual es el líder del combate
        bool lider;

        // Permite comunicarse con el otro ordenador
        sf::UdpSocket socket;

        // Dirección IP del otro jugador
        std::optional<sf::IpAddress> direccionIP;
    
    public:
        ConectorOnline(sf::IpAddress direccionIP, bool lider);

        // Se envían las acciones locales al rival y se reciben las acciones remotas del rival
        AccionesOnline enviarRecibirAcciones(std::unordered_set<Accion> accionesRealizadas, std::unordered_set<Accion> accionesDetenidas);

        // Indica si el jugador es el lider de la partida
        bool isLider();
};

