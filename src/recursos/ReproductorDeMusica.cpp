#include "ReproductorDeMusica.hpp"
#include "Bitacora.hpp"
#include "Utilidades.hpp"
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

ReproductorDeMusica::ReproductorDeMusica() : volumenActual(VOLUMEN_MAXIMO_MUSICA), tonoActual(TONO_MUSICA_NORMAL)
{
    if(!std::filesystem::is_directory("musica/combate"))
    {
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Oye Emilio... he notado que el directorio musica/combate es más liviano de lo habitial, ¿no crees?");
        Bitacora::unicaInstancia()->escribir("Emilio: Y tanto liviano, directamente no existe");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Esto es el acabose. Pongo una carpeta comunitaria para que la gente deje su música y así me lo pagan. No se puede tener talante...");
        Bitacora::unicaInstancia()->escribir("Emilio: Pues nada, a pelear sin música. Anda que... Vaya mierda de vecinos...");
    }
    else if(!std::filesystem::is_directory("musica/combate-pausa"))
    {
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Oye Emilio... he notado que el directorio musica/combate-pausa es más liviano de lo habitual, ¿no crees?");
        Bitacora::unicaInstancia()->escribir("Emilio: Y tanto liviano, directamente no existe");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Esto es el acabose. Pongo una carpeta comunitaria para que la gente deje su música de pausa y así me lo pagan. No se puede tener talante...");
        Bitacora::unicaInstancia()->escribir("Emilio: Pues nada, a pelear sin música. Anda que... Vaya mierda de vecinos...");
    }
    else
    {
        for(const std::filesystem::directory_entry & entrada : std::filesystem::directory_iterator("musica/combate"))
        {

            // Aquí se almacena la ruta de la canción en formato string
            std::string rutaCancion(entrada.path().string());

            // Es necesario comprobar que los cuatro últimos caracteres del string son ".ogg"
            std::size_t posicionPunto = rutaCancion.size()-4;

            if(rutaCancion.substr(posicionPunto,4) == ".ogg")
            {
                // Se cambia la barra invertida que pone Windows por una barra normal
                // y corriente estilo Linux (y supongo que Unix también)
                while(rutaCancion.find('\\') != std::string::npos)
                {
                    rutaCancion[rutaCancion.find('\\')] = '/';
                }

                cancionesCombate.push_back(rutaCancion);
            }
        }
    }
}

void ReproductorDeMusica::reproducir(std::string cancion, bool bucle, sf::Time progreso)
{
    if(!canciones.count(cancion) && !canciones[cancion].openFromFile(cancion)){
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Emilio, vamos a animar un poco el ambiente, reproduce la canción " + cancion);
        Bitacora::unicaInstancia()->escribir("Emilio: Ah no, esa se la llevó mi padre y ya no la he vuelto a ver");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Pero... ¿para qué le das a tu padre un bien común de esta, nuestra comunidad?");
        Bitacora::unicaInstancia()->escribir("Emilio: Es que se puso muy pesado, y con tal de que me dejara un rato en paz...");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Voy a encontrar a Mariano inmediatamente. Se suspende la junta.");
        Bitacora::unicaInstancia()->escribir("Emilio: Pero no se enfade con él, señor Juan. No es maldad, es que está demente ya.");
        exit(EXIT_FAILURE);
    }

    canciones[cancion].setLooping(bucle);
    canciones[cancion].setVolume(volumenActual);
    canciones[cancion].setPitch(tonoActual);
    canciones[cancion].setPlayingOffset(progreso);
    canciones[cancion].play();

    cancionActual = cancion;
}

void ReproductorDeMusica::reproducirCancionCombate(){

    // Si no hay canciones de combate, no se reproduce nada
    if(cancionesCombate.empty()) return;

    int indiceCancion = std::rand() % (cancionesCombate.size());

    reproducir(cancionesCombate[indiceCancion]);
}

void ReproductorDeMusica::pausarCancionCombate()
{
    // Se obtiene el progreso de la canción actual
    pausar();
    sf::Time progresoCancionActual = canciones[cancionActual].getPlayingOffset();

    // Se encuentra la ruta de la canción de pausa
    std::filesystem::path rutaCancionActual(cancionActual);
    std::vector<std::string> nombreCancionActualSeparado(util::separarString(rutaCancionActual.string(),'/'));
    std::string nombreNuevaCancion = "musica/combate-pausa/"+nombreCancionActualSeparado[nombreCancionActualSeparado.size()-1];

    // Se detiene la canción antes de reproducir la siguiente
    detener();

    // Se reproduce la canción de pausa
    reproducir(nombreNuevaCancion,true,progresoCancionActual);
}

void ReproductorDeMusica::reanudarCancionCombate()
{
    // Se obtiene el progreso de la canción actual
    pausar();
    sf::Time progresoCancionActual = canciones[cancionActual].getPlayingOffset();

    // Se encuentra la ruta de la canción de combate
    std::vector nombreCancionActualSeparado(util::separarString(cancionActual,'/'));
    std::string nombreNuevaCancion = "musica/combate/"+nombreCancionActualSeparado[nombreCancionActualSeparado.size()-1];

    // Se detiene la canción antes de reproducir la siguiente
    detener();

    // Se reproduce la canción de pausa
    reproducir(nombreNuevaCancion,true,progresoCancionActual);
}

void ReproductorDeMusica::detener()
{
    canciones[cancionActual].stop();
    cancionActual = "";
}

void ReproductorDeMusica::pausar(){
    canciones[cancionActual].pause();
}

float ReproductorDeMusica::getVolumen()
{
    return canciones[cancionActual].getVolume();
}

bool ReproductorDeMusica::estaReproduciendo()
{
    return canciones[cancionActual].getStatus() == sf::Music::Status::Playing;
}

std::string ReproductorDeMusica::getCancionActual(){
    return cancionActual;
}

void ReproductorDeMusica::setVolumen(float nuevoVolumen)
{
    volumenActual = (nuevoVolumen > VOLUMEN_MAXIMO_MUSICA ? VOLUMEN_MAXIMO_MUSICA : nuevoVolumen < 0 ? 0 : nuevoVolumen);
    if(cancionActual != "") canciones[cancionActual].setVolume(volumenActual);
}

float ReproductorDeMusica::getTono()
{
    return this->tonoActual;
}

void ReproductorDeMusica::setTono(float nuevoTono)
{
    tonoActual = nuevoTono;
    if(cancionActual != "") canciones[cancionActual].setPitch(tonoActual);
}