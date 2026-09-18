#pragma once

#include "Constantes.hpp"
#include "Enums.hpp"
#include "Personaje.hpp"
#include <string>
#include <unordered_map>

/*
    Clase que se encarga de contener personajes preparados para pelear
*/
class ContenedorDePersonajes
{
    private:
        ContenedorDePersonajes(){}
        ~ContenedorDePersonajes();

        static ContenedorDePersonajes * contenedorDePersonajes;

        std::unordered_map<std::string, Personaje> personajes;

    public:

        // Devuelve una copia del personaje pedido
        Personaje obtenerPersonaje(std::string nombre);

        // Devuelve un vector con los nombres de todos los personajes
        std::vector<std::string> obtenerNombresPersonajes();

        // Carga todos los personajes, sacados del mismo fichero
        void cargarTodosLosPersonajes();

        // NUNCA SE COPIA UN SINGLETON
        ContenedorDePersonajes(ContenedorDePersonajes &otro) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const ContenedorDePersonajes &) = delete;

        // Devuelve la única instancia
        static ContenedorDePersonajes * unicaInstancia();
};