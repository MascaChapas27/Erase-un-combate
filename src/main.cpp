#include "ContenedorDePersonajes.hpp"
#include "ContenedorDeEfectos.hpp"
#include "MenuPrincipal.hpp"
#include "Combate.hpp"

#include "yaml-cpp/yaml.h"
#include <iostream>

int main(int argc, char* argv[]){

    // Los personajes y efectos son recursos que se tienen que cargar desde un principio.
    // Los recursos más sencillos (efectos de sonido, fuentes, texturas) se pueden cargar al vuelo según se
    // necesite
    ContenedorDePersonajes::unicaInstancia()->cargarTodosLosPersonajes();

    ContenedorDeEfectos::unicaInstancia()->cargarTodosLosEfectos();

    while(true){

        Seleccion seleccion = MenuPrincipal::unicaInstancia()->comenzar();

        if(seleccion == Seleccion::BATALLA_VS){
            Combate combate("juan-cuesta-sin-casco", "andres-guerra", "fachada-erase-unos-estatutos");
            combate.comenzar();
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