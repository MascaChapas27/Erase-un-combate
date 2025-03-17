#include "Combate.hpp"
#include "ContenedorDePersonajes.hpp"
#include "ContenedorDeEfectos.hpp"
#include <SFML/Network.hpp>

int main(int argc, char* argv[]){

    // Los personajes y efectos son recursos que se tienen que cargar desde un principio.
    // Los recursos más sencillos (efectos de sonido, fuentes, texturas) se pueden cargar al vuelo según se
    // necesite
    ContenedorDePersonajes::unicaInstancia()->cargarTodosLosPersonajes();

    ContenedorDeEfectos::unicaInstancia()->cargarTodosLosEfectos();

    if(argc == 1){
        Combate combate("juan-cuesta-sin-casco", "juan-cuesta-sin-casco", "fachada");
        combate.comenzar();
    } else {
        Combate combate("juan-cuesta-sin-casco", "juan-cuesta-con-casco", "fachada", sf::IpAddress(std::atoi(argv[1]),std::atoi(argv[2]),std::atoi(argv[3]),std::atoi(argv[4])), std::stoi(argv[5]));
        combate.comenzar();
    }
}