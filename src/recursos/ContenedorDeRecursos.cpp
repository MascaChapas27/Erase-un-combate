#include <assert.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include "Bitacora.hpp"
#include "ContenedorDeRecursos.hpp"

// Las variables estáticas para implementar Singleton se inicializan nulas
template <class Recurso, class Identificador>
ContenedorDeTexturas * ContenedorDeRecursos<Recurso,Identificador>::contenedorDeTexturas = nullptr;

template <class Recurso, class Identificador>
ContenedorDeSonidos * ContenedorDeRecursos<Recurso,Identificador>::contenedorDeSonidos = nullptr;

template <class Recurso, class Identificador>
ContenedorDeFuentes * ContenedorDeRecursos<Recurso,Identificador>::contenedorDeFuentes = nullptr;

template <>
ContenedorDeTexturas * ContenedorDeTexturas::unicaInstancia()
{
    if(contenedorDeTexturas == nullptr)
        contenedorDeTexturas = new ContenedorDeTexturas();
    return contenedorDeTexturas;
}

template <>
ContenedorDeSonidos * ContenedorDeSonidos::unicaInstancia()
{
    if(contenedorDeSonidos == nullptr)
        contenedorDeSonidos = new ContenedorDeSonidos();
    return contenedorDeSonidos;
}

template <>
ContenedorDeFuentes * ContenedorDeFuentes::unicaInstancia()
{
    if(contenedorDeFuentes == nullptr)
        contenedorDeFuentes = new ContenedorDeFuentes();
    return contenedorDeFuentes;
}