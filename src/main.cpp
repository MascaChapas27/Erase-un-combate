#include "ContenedorDePersonajes.hpp"
#include "ContenedorDeEfectos.hpp"
#include "MenuPrincipal.hpp"
#include "Combate.hpp"

#include "yaml-cpp/yaml.h"
#include <iostream>

int main(int argc, char* argv[]){

    YAML::Node config = YAML::LoadFile("config.yaml");

    YAML::Node infoPersonaje = config["infoPersonaje"];

    YAML::Node estados = config["estados"];

    std::cout << "Personaje " << infoPersonaje["nombre"].as<std::string>() << ", con " << infoPersonaje["vida"].as<int>() << " puntos de vida, velocidad de " << infoPersonaje["velocidad"].as<int>() << " y fuerza de salto de " << infoPersonaje["salto"].as<int>() << "\n";

    for(YAML::const_iterator itEstado = estados.begin(); itEstado != estados.end(); ++itEstado)
    {
        std::cout << "Encontrado estado [" << itEstado->first.as<std::string>() << "]\n";
        for(YAML::const_iterator itAtributoEstado = itEstado->second.begin(); itAtributoEstado != itEstado->second.end(); ++itAtributoEstado)
        {
            if(itAtributoEstado->first.as<std::string>() == "bucle")
            {
                std::cout << "El bucle es de tipo [" << itAtributoEstado->second.as<std::string>() << "]\n";
            }
            else if(itAtributoEstado->first.as<std::string>() == "rectangulos")
            {
                std::cout << "Extrayendo información sobre los " << itAtributoEstado->second.size() << " rectángulos...\n";
                for(size_t rectanguloActual = 0; rectanguloActual < itAtributoEstado->second.size(); rectanguloActual++)
                {
                    std::cout << "Rectángulo " << rectanguloActual << ", posición (" << itAtributoEstado->second[rectanguloActual]["posX"].as<int>() << "," << itAtributoEstado->second[rectanguloActual]["posY"].as<int>() << "), tamaño " << itAtributoEstado->second[rectanguloActual]["ancho"].as<int>() << "x" << itAtributoEstado->second[rectanguloActual]["alto"].as<int>() << ", ataque " << itAtributoEstado->second[rectanguloActual]["ataque"].as<int>() << "\n";
                }
            }
            else if(itAtributoEstado->first.as<std::string>() == "frames")
            {
                std::cout << "Encontrados " << itAtributoEstado->second.size() << " frames: ";
                for(size_t frameActual = 0; frameActual < itAtributoEstado->second.size(); frameActual++)
                {
                    std::cout << itAtributoEstado->second[frameActual] << ", ";
                }
            }
            else if(itAtributoEstado->first.as<std::string>() == "sonido")
            {
                s
            }
        }
        std::cout << "\n";
    }

    return 0;

    // Los personajes y efectos son recursos que se tienen que cargar desde un principio.
    // Los recursos más sencillos (efectos de sonido, fuentes, texturas) se pueden cargar al vuelo según se
    // necesite
    ContenedorDePersonajes::unicaInstancia()->cargarTodosLosPersonajes();

    ContenedorDeEfectos::unicaInstancia()->cargarTodosLosEfectos();

    while(true){

        Seleccion seleccion = MenuPrincipal::unicaInstancia()->comenzar();

        if(seleccion == Seleccion::BATALLA_VS){
            Combate combate("juan-cuesta-sin-casco", "juan-cuesta-sin-casco", "fachada-erase-unos-estatutos");
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