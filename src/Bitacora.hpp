#pragma once

#include <fstream>
#include <string>

/*
    Esta clase gestiona una bitácora que informa de cosas
    importantes que pasan en la ejecución del programa
*/

class Bitacora
{
private:
    // El propio fichero de bitácora
    std::ofstream ficheroBitacora;

    Bitacora();
    ~Bitacora();

    // Única instancia
    static Bitacora *bitacora;

public:
    // Se escriben cosas en el fichero de bitácora terminando con un fin de línea
    void escribir(std::string cosillas);

    // Se comprueba si la tarjeta gráfica tiene lo que hay que tener
    void comprobarGrafica();

    // NUNCA SE COPIA UN SINGLETON
    Bitacora(Bitacora &otra) = delete;

    // NUNCA SE ASIGNA UN SINGLETON
    void operator=(const Bitacora &) = delete;

    // Devuelve la única instancia de la bitácora
    static Bitacora *unicaInstancia();
};


