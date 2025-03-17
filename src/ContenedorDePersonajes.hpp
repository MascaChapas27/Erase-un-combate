#ifndef __CONTENEDOR_DE_PERSONAJES_HPP__
#define __CONTENEDOR_DE_PERSONAJES_HPP__

#include "Constantes.hpp"
#include "Enums.hpp"
#include "Personaje.hpp"
#include <string>
#include <map>

/*
    Clase que se encarga de contener personajes preparados para pelear
*/
class ContenedorDePersonajes
{
    private:
        ContenedorDePersonajes(){}
        ~ContenedorDePersonajes();

        static ContenedorDePersonajes * contenedorDePersonajes;

        std::map<std::string, Personaje> personajes;

    public:

        // Devuelve una copia del personaje pedido
        Personaje obtenerPersonaje(std::string nombre);

        // Carga todos los personajes, sacados del mismo fichero
        void cargarTodosLosPersonajes();

        // NUNCA SE COPIA UN SINGLETON
        ContenedorDePersonajes(ContenedorDePersonajes &otro) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const ContenedorDePersonajes &) = delete;

        // Devuelve la única instancia
        static ContenedorDePersonajes * unicaInstancia();

};

#endif // __CONTENEDOR_DE_PERSONAJES_HPP__
