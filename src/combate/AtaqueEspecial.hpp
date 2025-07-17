#pragma once

#include "Enums.hpp"
#include <vector>
#include <unordered_set>

/*
    Esta clase se encarga de registrar si un jugador está pulsando
    con suficiente rapidez la combinación de teclas necesaria para
    hacer el ataque especial
*/
class AtaqueEspecial {
    private:

        // Vector de acciones que hay que hacer para soltar el ataque
        std::vector<Accion> acciones;

        // Índice que dice cuál es la acción que se está realizando actualmente
        size_t accionActual;

        // Contador que va bajando según pasa el tiempo para que tengas que
        // hacer la combinación de teclas rápido
        int cooldown;

        // Cuando se pulsa una tecla es necesario no pulsar nada después para que
        // la combinación deteclas funcione bien
        bool esperandoPausa;
    
    public:
        
        // Construye una acción en base a las acciones que hay que hacer
        AtaqueEspecial(const std::vector<Accion> &acciones);

        // Comprueba cómo va el ataque y devuelve true cuando ya esté listo
        bool actualizar(std::unordered_set<Accion> &acciones);
        
        // Resetea el ataque especial para volver al principio
        void resetear();
};

