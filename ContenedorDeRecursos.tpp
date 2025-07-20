#include <assert.h>
#include "Bitacora.hpp"

// Las variables est�ticas para implementar Singleton se inicializan nulas
template <class Recurso, class Identificador>
ContenedorDeTexturas * ContenedorDeRecursos<Recurso,Identificador>::contenedorDeTexturas = nullptr;

template <class Recurso, class Identificador>
ContenedorDeSonidos * ContenedorDeRecursos<Recurso,Identificador>::contenedorDeSonidos = nullptr;

template <class Recurso, class Identificador>
ContenedorDeFuentes * ContenedorDeRecursos<Recurso,Identificador>::contenedorDeFuentes = nullptr;

template <class Recurso, class Identificador>
void ContenedorDeRecursos<Recurso,Identificador>::cargar(Identificador id, const std::string& rutaFichero){
    // Los punteros �nicos son mejores que los normales porque se borran solos cuando
    // se salen del scope este como le llaman (cuando se acaba la funcion o el if{} ya me entiendes)
    std::unique_ptr<Recurso> recurso(new Recurso());

    // Se intenta cargar el recurso. Si no es posible, se escribe en la bit�cora y se va todo a la mi3rda
    if(!recurso->loadFromFile(rutaFichero)){
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Emilio, �has podido cargar el fichero " + rutaFichero + " como te ped�?");
        Bitacora::unicaInstancia()->escribir("Emilio: Lo siento se�or Juan pero no est� por aqu�, y yo de porter�a para fuera cobro un suplemento.");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Qu� follon... Se suspende la junta.");
        exit(EXIT_FAILURE);
    }

    // Si todo ha ido bien, creamos un par (Identificador,std::unique_ptr<Recurso>) y lo metemos en el mapa
    // Es importante utilizar std::move() para que el recurso pase a los dominios del mapa y deje
    // de existir en estos dominios, esa es la gracia de los unique_ptr
    mapaDeRecursos.insert(std::make_pair(id,std::move(recurso)));
}

template <class Recurso, class Identificador>
Recurso& ContenedorDeRecursos<Recurso,Identificador>::obtener(Identificador id){
    // Primero encontramos el elemento en el mapa (auto significa que
    // el tipo se sabe por el contexto y no hace falta ponerlo)
    auto encontrado = mapaDeRecursos.find(id);

    // Una vez se tiene el elemento (es un par (Identificador,std::unique_ptr<Recurso>)), se saca
    // el recurso y se devuelve. Como el segundo elemento es un puntero, se
    // usa el asterisco para decir que vamos a devolver el propio objeto (en realidad
    // devolvemos una referencia, el objeto real est� en memoria din�mica)
    return *(encontrado -> second);
}

template <class Recurso, class Identificador>
ContenedorDeTexturas * ContenedorDeRecursos<Recurso,Identificador>::unicaInstanciaTexturas()
{
    if(contenedorDeTexturas == nullptr)
        contenedorDeTexturas = new ContenedorDeTexturas;
    return contenedorDeTexturas;
}

template <class Recurso, class Identificador>
ContenedorDeSonidos * ContenedorDeRecursos<Recurso,Identificador>::unicaInstanciaSonidos()
{
    if(contenedorDeSonidos == nullptr)
        contenedorDeSonidos = new ContenedorDeSonidos;
    return contenedorDeSonidos;
}

template <class Recurso, class Identificador>
ContenedorDeFuentes * ContenedorDeRecursos<Recurso,Identificador>::unicaInstanciaFuentes()
{
    if(contenedorDeFuentes == nullptr)
        contenedorDeFuentes = new ContenedorDeFuentes;
    return contenedorDeFuentes;
}

template <class Recurso, class Identificador>
void ContenedorDeRecursos<Recurso,Identificador>::cargarTodasLasTexturas()
{
    //load(TextureID::GabrielaTextbox,"sprites/textbox/gabrielaTextbox.png");
}

template <class Recurso, class Identificador>
void ContenedorDeRecursos<Recurso,Identificador>::cargarTodosLosSonidos()
{
    //load(SoundID::GabrielaSpeaking,"sounds/speaking/gabrielaSpeaking.wav");
}

template <class Recurso, class Identificador>
void ContenedorDeRecursos<Recurso,Identificador>::cargarTodasLasFuentes()
{
    //load(FontID::GabrielaFont,"fonts/gabriela.ttf");
}
