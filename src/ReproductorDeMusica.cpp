#include "ReproductorDeMusica.hpp"
#include "Bitacora.hpp"
#include <filesystem>

ReproductorDeMusica * ReproductorDeMusica::reproductorDeMusica = nullptr;

ReproductorDeMusica * ReproductorDeMusica::unicaInstancia()
{
    if(reproductorDeMusica==nullptr)
        reproductorDeMusica = new ReproductorDeMusica();
    return reproductorDeMusica;
}

ReproductorDeMusica::~ReproductorDeMusica(){
    if(reproductorDeMusica != nullptr) delete reproductorDeMusica;
}

ReproductorDeMusica::ReproductorDeMusica(){
    volumenActual = VOLUMEN_MAXIMO_MUSICA;

    if(!std::filesystem::is_directory("musica/combate")){
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Oye Emilio... he notado que el directorio musica/combate es más liviano de lo habitial, ¿no crees?");
        Bitacora::unicaInstancia()->escribir("Emilio: Y tanto liviano, directamente no existe");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Esto es el acabose. Pongo una carpeta comunitaria para que la gente deje su música y así me lo pagan. No se puede tener talante...");
        Bitacora::unicaInstancia()->escribir("Emilio: Pues nada, a pelear sin música. Anda que... Vaya mierda de vecinos...");
    } else {
        for(const std::filesystem::directory_entry & entrada : std::filesystem::directory_iterator("musica/combate")){

            // Es necesario comprobar que los cuatro últimos caracteres del string son ".wav"
            std::size_t posicionPunto = entrada.path().string().size()-4;

            if(entrada.path().string().substr(posicionPunto,4) == ".wav"){
                cancionesCombate.push_back(entrada.path().string());
            }
        }
    }
}

void ReproductorDeMusica::reproducir(std::string cancion, bool bucle)
{
    if(!musicaActual.openFromFile(cancion)){
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Emilio, vamos a animar un poco el ambiente, reproduce la canción " + cancion);
        Bitacora::unicaInstancia()->escribir("Emilio: Ah no, esa se la llevó mi padre y ya no la he vuelto a ver");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Pero... ¿para qué le das a tu padre un bien común de esta, nuestra comunidad?");
        Bitacora::unicaInstancia()->escribir("Emilio: Es que se puso muy pesado, y con tal de que me dejara un rato en paz...");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Voy a encontrar a Mariano inmediatamente. Se suspende la junta.");
        Bitacora::unicaInstancia()->escribir("Emilio: Pero no se enfade con él, señor Juan. No es maldad, es que está demente ya.");
        exit(EXIT_FAILURE);
    }

    musicaActual.setLooping(bucle);
    musicaActual.setVolume(volumenActual);
    musicaActual.play();
}

void ReproductorDeMusica::reproducirCancionCombate(){

    // Si no hay canciones de combate, no se reproduce nada
    if(cancionesCombate.empty()) return;

    int indiceCancion = rand() % (cancionesCombate.size());

    reproducir(cancionesCombate[indiceCancion]);
}

void ReproductorDeMusica::detener()
{
    musicaActual.stop();
}

float ReproductorDeMusica::getVolumen()
{
    return musicaActual.getVolume();
}

bool ReproductorDeMusica::estaReproduciendo()
{
    return musicaActual.getStatus() == sf::Music::Status::Playing;
}

void ReproductorDeMusica::setVolumen(float nuevoVolumen)
{
    volumenActual = (nuevoVolumen > VOLUMEN_MAXIMO_MUSICA ? VOLUMEN_MAXIMO_MUSICA : nuevoVolumen < 0 ? 0 : nuevoVolumen);
    musicaActual.setVolume(volumenActual);
}
