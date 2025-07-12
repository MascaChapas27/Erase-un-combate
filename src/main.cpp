#include "ContenedorDePersonajes.hpp"
#include "ContenedorDeEfectos.hpp"
#include "MenuPrincipal.hpp"
#include "Combate.hpp"

#include "yaml-cpp/yaml.h"
#include <iostream>

int main(int argc, char* argv[]){

    YAML::Node config = YAML::LoadFile("ficheros/personajes/juan-cuesta-sin-casco.yaml");

    YAML::Node infoPersonaje = config["infoPersonaje"];

    YAML::Node estados = config["estados"];

    std::cout << "Personaje " << infoPersonaje["nombre"].as<std::string>() << ", con " << infoPersonaje["vida"].as<int>() << " puntos de vida, velocidad de " << infoPersonaje["velocidad"].as<float>() << " y fuerza de salto de " << infoPersonaje["salto"].as<float>() << "\n";

    for(YAML::const_iterator itEstado = estados.begin(); itEstado != estados.end(); ++itEstado)
    {
        std::cout << "Encontrado estado \"" << itEstado->first.as<std::string>() << "\"\n";
        for(YAML::const_iterator itAtributoEstado = itEstado->second.begin(); itAtributoEstado != itEstado->second.end(); ++itAtributoEstado)
        {
            if(itAtributoEstado->first.as<std::string>() == "bucle")
            {
                std::cout << "El bucle es de tipo \"" << itAtributoEstado->second.as<std::string>() << "\"\n";
            }
            else if(itAtributoEstado->first.as<std::string>() == "rectangulos")
            {
                std::cout << "Extrayendo información sobre los " << itAtributoEstado->second.size() << " rectángulos...\n";
                for(size_t rectanguloActual = 0; rectanguloActual < itAtributoEstado->second.size(); rectanguloActual++)
                {
                    std::cout << "Rectángulo " << rectanguloActual << ".\n";
                    
                    for(size_t hitboxActual = 0; hitboxActual < itAtributoEstado->second[rectanguloActual].size(); hitboxActual++)
                    {
                        std::cout << "Hitbox " << hitboxActual << ". Posición (" << itAtributoEstado->second[rectanguloActual][hitboxActual]["posX"].as<int>() << "," << itAtributoEstado->second[rectanguloActual][hitboxActual]["posY"].as<int>() << "), tamaño " << itAtributoEstado->second[rectanguloActual][hitboxActual]["ancho"].as<int>() << "x" << itAtributoEstado->second[rectanguloActual][hitboxActual]["alto"].as<int>() << ", ataque " << itAtributoEstado->second[rectanguloActual][hitboxActual]["ataque"].as<int>() << "\n";
                    }
                }
            }
            else if(itAtributoEstado->first.as<std::string>() == "fotogramas")
            {
                std::cout << "Encontrados " << itAtributoEstado->second.size() << " fotogramas: ";
                for(size_t frameActual = 0; frameActual < itAtributoEstado->second.size(); frameActual++)
                {
                    std::cout << itAtributoEstado->second[frameActual];

                    if(frameActual+1 == itAtributoEstado->second.size()) std::cout << "\n";
                    else std::cout << ", ";
                }
            }
            else if(itAtributoEstado->first.as<std::string>() == "sonido")
            {
                if(itAtributoEstado->second["tipo"].as<std::string>() == "No repetir")
                {
                    std::cout << "Este estado tiene un sonido que no se debe repetir.";
                }
                else if(itAtributoEstado->second["tipo"].as<std::string>() == "Repetir")
                {
                    std::cout << "Este estado tiene un sonido que se debe repetir.";
                }

                std::cout << " Los fotogramas son: ";

                for(size_t i = 0; i < itAtributoEstado->second["fotogramas"].size(); i++)
                {
                    std::cout << itAtributoEstado->second["fotogramas"][i];

                    if(i+1 == itAtributoEstado->second["fotogramas"].size()) std::cout << "\n";
                    else std::cout << ", ";
                }
                
                std::cout << "\n";
            }
            else if(itAtributoEstado->first.as<std::string>() == "efectos")
            {
                std::cout << "Encontrados " << itAtributoEstado->second.size() << " efectos\n";

                for(size_t i = 0; i < itAtributoEstado->second.size(); i++)
                {
                    std::cout << "Efecto número " << i << " de nombre \"" << itAtributoEstado->second[i]["efecto"].as<std::string>() << "\"desencadenado en fotograma " << itAtributoEstado->second[i]["fotograma"].as<int>() << ". Posición (" << itAtributoEstado->second[i]["posX"].as<int>() << "," << itAtributoEstado->second[i]["posY"].as<int>() << ") y velocidad inicial (" << itAtributoEstado->second[i]["velX"].as<float>() << "," << itAtributoEstado->second[i]["velY"].as<float>() << ").\n";
                }
            }
            else if(itAtributoEstado->first.as<std::string>() == "movimiento")
            {
                std::cout << "Encontrados " << itAtributoEstado->second.size() << " movimientos\n";

                for(size_t i = 0; i < itAtributoEstado->second.size(); i++)
                {
                    std::cout << "Movimiento número " << i << " desencadenado en fotograma " << itAtributoEstado->second[i]["fotograma"].as<int>() << ". Movimiento de (" << itAtributoEstado->second[i]["velX"].as<float>() << "," << itAtributoEstado->second[i]["velY"].as<float>() << "\n";
                }
            }
            else if(itEstado->first.as<std::string>() == "ataque-especial" && itAtributoEstado->first.as<std::string>() == "acciones")
            {
                std::cout << "Las acciones a llevar a cabo para realizar este ataque especial son: ";

                for(size_t i = 0; i < itAtributoEstado->second.size(); i++)
                {
                    std::cout << itAtributoEstado->second[i].as<std::string>();

                    if(i+1 == itAtributoEstado->second.size()) std::cout << "\n";
                    else std::cout << ", ";
                }

                std::cout << "\n";
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