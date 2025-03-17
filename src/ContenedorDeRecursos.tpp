#include "Bitacora.hpp"

template <class Recurso, class Identificador>
void ContenedorDeRecursos<Recurso,Identificador>::cargar(Identificador id, const std::string& rutaFichero){
    // Los punteros únicos son mejores que los normales porque se borran solos cuando
    // se salen del scope este como le llaman (cuando se acaba la funcion o el if{} ya me entiendes)
    std::unique_ptr<Recurso> recurso = std::make_unique<Recurso>();

    // Se intenta cargar el recurso. Si no es posible, se escribe en la bitácora y se va todo a la mi3rda
    if(!recurso->loadFromFile(rutaFichero)){
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Emilio, ¿has podido cargar el fichero " + rutaFichero + " como te pedí?");
        Bitacora::unicaInstancia()->escribir("Emilio: Lo siento señor Juan pero no está por aquí, y yo de portería para fuera cobro un suplemento.");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Qué follon... Se suspende la junta.");
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

    if(encontrado == mapaDeRecursos.end()){
        cargar(id,id);
        return obtener(id);
    }

    // Una vez se tiene el elemento (es un par (Identificador,std::unique_ptr<Recurso>)), se saca
    // el recurso y se devuelve. Como el segundo elemento es un puntero, se
    // usa el asterisco para decir que vamos a devolver el propio objeto (en realidad
    // devolvemos una referencia, el objeto real está en memoria dinámica)
    return *(encontrado -> second);
}