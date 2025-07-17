#include "Bitacora.hpp"
#include "Constantes.hpp"
#include <iostream>

Bitacora * Bitacora::bitacora = nullptr;

Bitacora * Bitacora::unicaInstancia()
{
    if(bitacora==nullptr)
        bitacora = new Bitacora();
    return bitacora;
}

Bitacora::Bitacora(){
    // Abre la bitácora para añadir información al final (append, como se dice en inglés)
    ficheroBitacora.open(BITACORA_RUTA,std::ios_base::app);

    // Se registra el día de hoy en la bitácora para situarnos
    time_t tt;
    struct tm* ti;
    time(&tt);
    ti = localtime(&tt);

    ficheroBitacora << "----------------------------------------------" << "\n";
    ficheroBitacora << "Juan Cuesta: Queridos convecinos, gracias por asistir a esta junta. Hoy estamos a: " << asctime(ti) << "\n" << "\n";

    comprobarGrafica();
}

Bitacora::~Bitacora(){
    ficheroBitacora.close();
    if(bitacora != nullptr)
        delete bitacora;
}

void Bitacora::escribir(std::string cosillas)
{
    ficheroBitacora << cosillas << "\n";
    ficheroBitacora.flush();
    if(DEBUG)
        std::cerr << cosillas << "\n";
}

void Bitacora::comprobarGrafica(){
    ficheroBitacora << "Juan Cuesta: Primer punto del día: comprobación rutinaria de la tarjeta gráfica." << "\n";
    ficheroBitacora << "Emilio: Señor Juan, he estado mirando y la tarjeta gráfica soporta texturas de hasta " << sf::Texture::getMaximumSize() << " píxeles." << "\n";
    if(sf::Texture::getMaximumSize() < MAXIMA_LONGITUD_TEXTURA){
        ficheroBitacora << "Juan Cuesta: ¡¿Cómo?! Pero bueno, esto es... no doy crédito..." << "\n";
        ficheroBitacora << "Paloma: Ay Juan, ¡tranquilízate, hombre ya! ¡Recuerda la úlcera!" << "\n";
        ficheroBitacora << "Juan Cuesta: Si es que no puede ser... este juego tiene texturas de hasta " << MAXIMA_LONGITUD_TEXTURA << " píxeles, esta tarjeta gráfica no va a poder aguantarlo..." << "\n";
        ficheroBitacora << "Concha: ¡Váyase señor Cuesta! ¡¡VÁYASE!!" << "\n";
        ficheroBitacora << "Marisa: La has cagao macho, esto no tira ni de puta coña." << "\n";
        ficheroBitacora << "Emilio: A ver, las ultras, que se tranquilicen." << "\n" << "\n";
    } else {
        ficheroBitacora << "Juan Cuesta: Perfecto, una buena cifra. Prosigamos." << "\n";
    }
}