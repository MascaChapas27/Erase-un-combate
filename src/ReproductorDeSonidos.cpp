#include "ReproductorDeSonidos.hpp"
#include "ContenedorDeRecursos.hpp"
#include "Bitacora.hpp"
#include <filesystem>

ReproductorDeSonidos *ReproductorDeSonidos::reproductorDeSonidos = nullptr;

ReproductorDeSonidos *ReproductorDeSonidos::unicaInstancia()
{
    if (reproductorDeSonidos == nullptr)
        reproductorDeSonidos = new ReproductorDeSonidos();
    return reproductorDeSonidos;
}

ReproductorDeSonidos::ReproductorDeSonidos()
{
    volumenActual = VOLUMEN_MAXIMO_SONIDOS;
}

ReproductorDeSonidos::~ReproductorDeSonidos()
{
    if (reproductorDeSonidos != nullptr)
        delete reproductorDeSonidos;
}

void ReproductorDeSonidos::reproducir(std::string sonido)
{
    // Primero se crea el sonido si no existe
    if (!sonidos.count(sonido))
    {
        sf::Sound nuevoSonido(ContenedorDeSonidos::unicaInstancia()->obtener(sonido));
        sonidos.insert(std::pair<std::string, sf::Sound>(sonido, nuevoSonido));
    }

    // Se reproduce el sonido requerido
    sonidos.at(sonido).setVolume(volumenActual);
    sonidos.at(sonido).play();
}

float ReproductorDeSonidos::getVolumen()
{
    return volumenActual;
}

void ReproductorDeSonidos::setVolumen(float nuevoVolumen)
{
    volumenActual = (nuevoVolumen > VOLUMEN_MAXIMO_MUSICA ? VOLUMEN_MAXIMO_MUSICA :
                     nuevoVolumen < 0 ? 0 :
                     nuevoVolumen);
}

bool ReproductorDeSonidos::estaReproduciendo(std::string sonido){
    return sonidos.at(sonido).getStatus() == sf::SoundSource::Status::Playing;
}