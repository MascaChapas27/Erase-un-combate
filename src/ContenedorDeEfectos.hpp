#pragma once

#include "Constantes.hpp"
#include "Enums.hpp"
#include "Animacion.hpp"
#include <string>
#include <map>

/*
    Clase que se encarga de contener efectos, como efectos de golpe cuando te pegan o
    carátulas de películas voladoras (voladoras las carátulas, las películas no)
*/
class ContenedorDeEfectos
{
    private:
        ContenedorDeEfectos(){}
        ~ContenedorDeEfectos();

        static ContenedorDeEfectos * contenedorDeEfectos;

        std::map<std::string, std::shared_ptr<Animacion>> animaciones;

    public:

        // Devuelve una copia del efecto pedido. El nombre es el nombre del fichero sin extensión
        // y sin incluir directorios. Por ejemplo, "bloqueado" para el círculo azul que aparece al
        // bloquear un ataque
        std::shared_ptr<Animacion> obtenerEfecto(std::string nombre);

        // Carga todos los efectos, sacados de varios ficheros en la misma carpeta
        void cargarTodosLosEfectos();

        // NUNCA SE COPIA UN SINGLETON
        ContenedorDeEfectos(ContenedorDeEfectos &otro) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const ContenedorDeEfectos &) = delete;

        // Devuelve la única instancia
        static ContenedorDeEfectos * unicaInstancia();

};


