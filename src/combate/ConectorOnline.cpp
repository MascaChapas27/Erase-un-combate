#include "ConectorOnline.hpp"
#include "Constantes.hpp"
#include "Enums.hpp"
#include "Utilidades.hpp"
#include "Bitacora.hpp"
#include <bitset>

ConectorOnline::ConectorOnline(sf::IpAddress direccionIP, bool lider) : lider(lider), direccionIP(direccionIP) {

    Bitacora::unicaInstancia()->escribir("Juan Cuesta: Emilio, abre el puerto " + std::to_string(NUMERO_PUERTO) + ", es hora de aprovechar la conexión de alta velocidad recién instalada.");

    if (socket.bind(NUMERO_PUERTO) != sf::Socket::Status::Done){
        Bitacora::unicaInstancia()->escribir("Emilio: Oye señor Juan, ¿no estará usando este puerto para algo ya?");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Imposible, si yo los puertos ni los he tocado desde que vino el instalador");
        Bitacora::unicaInstancia()->escribir("Emilio: Pues me da a mí que hay alguien que no ha tardado en engancharse. Vamos, que el puerto " + std::to_string(NUMERO_PUERTO) + " ya está en uso.");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Qué follón... se suspende la junta.");
        exit(EXIT_FAILURE);
    }

    Bitacora::unicaInstancia()->escribir("Emilio: Puerto listo. A ver si el otro está listo allá donde esté...");

    if (lider)
    {
        sf::Packet paquete;
        unsigned short puerto = NUMERO_PUERTO;
        if(socket.receive(paquete, this->direccionIP, puerto) != sf::Socket::Status::Done){
            Bitacora::unicaInstancia()->escribir("Emilio: Pues parece que no.");
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: ¿cómo que no? ¿Lo has puesto bien?");
            Bitacora::unicaInstancia()->escribir("Emilio: Vamos, digo yo que sí. No sé qué ha pasado, pero esto ha petado.");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        sf::Packet paquete;
        unsigned short puerto = NUMERO_PUERTO;

        if(socket.send(paquete, direccionIP, puerto) != sf::Socket::Status::Done){
            Bitacora::unicaInstancia()->escribir("Emilio: Pues parece que no.");
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: ¿Cómo que no? ¿Lo has puesto bien?");
            Bitacora::unicaInstancia()->escribir("Emilio: Vamos, digo yo que sí. No sé qué ha pasado, pero esto ha petado.");
            exit(EXIT_FAILURE);
        }
    }
}

bool ConectorOnline::isLider(){
    return lider;
}

AccionesOnline ConectorOnline::enviarRecibirAcciones(std::unordered_set<Accion> accionesRealizadas, std::unordered_set<Accion> accionesDetenidas){
    
    uint8_t accionesRealizadasLocalBits = 0;
    uint8_t accionesDetenidasLocalBits = 0;

    for(const Accion& a : accionesRealizadas){
        accionesRealizadasLocalBits |= util::accionABit(a);
    }

    for(const Accion& a : accionesDetenidas){
        accionesDetenidasLocalBits |= util::accionABit(a);
    }
    
    // Se crea un paquete con las acciones realizadas y las acciones detenidas
    sf::Packet paqueteEnviado;
    paqueteEnviado << accionesRealizadasLocalBits << accionesDetenidasLocalBits;

    // Se crea un paquete para recibir datos
    sf::Packet paqueteRecibido;

    unsigned short puerto = NUMERO_PUERTO;

    if (lider)
    {
        // El lider es el que envía primero
        sf::Socket::Status estado;

        do {
            estado = socket.send(paqueteEnviado, direccionIP.value(), puerto);
        } while (estado == sf::Socket::Status::Partial);

        if(estado == sf::Socket::Status::Error){
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: Pero bueno, esto es indignante. ¿Pago una conexión de alta velocidad y no puedo enviar datos?");
            Bitacora::unicaInstancia()->escribir("Emilio: Si esto del Internet es una estafa, a saber cuánto le habrán cobrado por el trastajo este.");
            exit(EXIT_FAILURE);
        }

        if (socket.receive(paqueteRecibido, direccionIP, puerto) != sf::Socket::Status::Done)
        {
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: Emilio, ¿qué pasa ahora...?");
            Bitacora::unicaInstancia()->escribir("Emilio: Pues que estaba aquí recibiendo datos, y de repente ha petado esto.");
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: ¿Cómo que \"ha petado\"?");
            Bitacora::unicaInstancia()->escribir("Emilio: Mire, yo que usted devolvía esto y me compraba un aparato de estos de gimnasia pasiva, que se acerca el veranito y hay que coger buen cuerpo.");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // El invitado envía después
        if (socket.receive(paqueteRecibido, direccionIP, puerto) != sf::Socket::Status::Done)
        {
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: Emilio, ¿qué pasa ahora...?");
            Bitacora::unicaInstancia()->escribir("Emilio: Pues que estaba aquí recibiendo datos, y de repente ha petado esto.");
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: ¿Cómo que \"ha petado\"?");
            Bitacora::unicaInstancia()->escribir("Emilio: Mire, yo que usted devolvía esto y me compraba un aparato de estos de gimnasia pasiva, que se acerca el veranito y hay que coger buen cuerpo.");
            exit(EXIT_FAILURE);
        }

        sf::Socket::Status estado;

        do {
            estado = socket.send(paqueteEnviado, direccionIP.value(), puerto);
        } while (estado == sf::Socket::Status::Partial);

        if(estado == sf::Socket::Status::Error){
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: Pero bueno, esto es indignante. ¿Pago una conexión de alta velocidad y no puedo enviar datos?");
            Bitacora::unicaInstancia()->escribir("Emilio: Si esto del Internet es una estafa, a saber cuánto le habrán cobrado por el trastajo este.");
            exit(EXIT_FAILURE);
        }
    }

    uint8_t accionesRealizadasRemotoBits;
    uint8_t accionesDetenidasRemotoBits;

    paqueteRecibido >> accionesRealizadasRemotoBits >> accionesDetenidasRemotoBits;

    std::unordered_set<Accion> accionesRealizadasRemoto;

    if(accionesRealizadasRemotoBits & BIT_ABAJO){
        accionesRealizadasRemoto.insert(Accion::ABAJO);
    }

    if(accionesRealizadasRemotoBits & BIT_ARRIBA){
        accionesRealizadasRemoto.insert(Accion::ARRIBA);
    }

    if(accionesRealizadasRemotoBits & BIT_IZQUIERDA){
        accionesRealizadasRemoto.insert(Accion::IZQUIERDA);
    }

    if(accionesRealizadasRemotoBits & BIT_DERECHA){
        accionesRealizadasRemoto.insert(Accion::DERECHA);
    }

    if(accionesRealizadasRemotoBits & BIT_ATAQUE){
        accionesRealizadasRemoto.insert(Accion::ATACAR);
    }

    std::unordered_set<Accion> accionesDetenidasRemoto;

    if(accionesDetenidasRemotoBits & BIT_ABAJO){
        accionesDetenidasRemoto.insert(Accion::ABAJO);
    }

    if(accionesDetenidasRemotoBits & BIT_ARRIBA){
        accionesDetenidasRemoto.insert(Accion::ARRIBA);
    }

    if(accionesDetenidasRemotoBits & BIT_IZQUIERDA){
        accionesDetenidasRemoto.insert(Accion::IZQUIERDA);
    }

    if(accionesDetenidasRemotoBits & BIT_DERECHA){
        accionesDetenidasRemoto.insert(Accion::DERECHA);
    }

    if(accionesDetenidasRemotoBits & BIT_ATAQUE){
        accionesDetenidasRemoto.insert(Accion::ATACAR);
    }

    AccionesOnline acciones;

    acciones.accionesRealizadasLocal = accionesRealizadas;
    acciones.accionesDetenidasLocal = accionesDetenidas;
    acciones.accionesRealizadasRemoto = accionesRealizadasRemoto;
    acciones.accionesDetenidasRemoto = accionesDetenidasRemoto;

    return acciones;
}