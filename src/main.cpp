#define DISCORDPP_IMPLEMENTATION
#include "ClienteDiscord.hpp"
#include "ContenedorDePersonajes.hpp"
#include "ContenedorDeEfectos.hpp"
#include "MenuPrincipal.hpp"
#include "Combate.hpp"
#include "MenuSeleccionPersonaje.hpp"

#include "yaml-cpp/yaml.h"
#include <iostream>
#include <random>

#include <unistd.h>

int main(int argc, char* argv[])
{
    ClienteDiscord::unicaInstancia()->inicializar();

    // Los personajes y efectos son recursos que se tienen que cargar desde un principio.
    // Los recursos más sencillos (efectos de sonido, fuentes, texturas) se pueden cargar al vuelo según se
    // necesite
    ContenedorDePersonajes::unicaInstancia()->cargarTodosLosPersonajes();

    ContenedorDeEfectos::unicaInstancia()->cargarTodosLosEfectos();

    // Se inicializa el generador de números pseudoaleatorios con el tiempo
    // actual como se suele hacer
    std::srand(std::time(nullptr));

    while(true){

        TipoSelectorMenuPrincipal seleccion = MenuPrincipal::unicaInstancia()->comenzar();

        if(seleccion == TipoSelectorMenuPrincipal::BATALLA_VS)
        {
            std::unordered_map<Jugador,std::string> personajesElegidos = MenuSeleccionPersonaje::unicaInstancia()->comenzarEleccionDoble();
            if(personajesElegidos.size() == 2)
            {
                Combate combate(personajesElegidos[Jugador::JUGADOR1], personajesElegidos[Jugador::JUGADOR2], "fachada-erase-unos-estatutos");
                combate.comenzar();
            }
        }
        else if (seleccion == TipoSelectorMenuPrincipal::MODO_HISTORIA)
        {
            
        }
        else if (seleccion == TipoSelectorMenuPrincipal::OPCIONES)
        {
            execl("EraseUnActualizador.exe","EraseUnActualizador.exe",nullptr);
            perror("No se pudo iniciar el actualizador");
        }
    }
    

    // if(argc == 1){
    //     Combate combate("juan-cuesta-sin-casco", "juan-cuesta-sin-casco", "fachada");
    //     combate.comenzar();
    // } else {
    //     Combate combate("juan-cuesta-sin-casco", "juan-cuesta-con-casco", "fachada", sf::IpAddress(std::atoi(argv[1]),std::atoi(argv[2]),std::atoi(argv[3]),std::atoi(argv[4])), std::stoi(argv[5]));
    //     combate.comenzar();
    // }
}