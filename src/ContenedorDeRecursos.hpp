#ifndef __CONTENEDOR_DE_RECURSOS_HPP__
#define __CONTENEDOR_DE_RECURSOS_HPP__

#include "Enums.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <map>

/*
    El Contenedor de Recursos es una clase que se encarga de guardar recursos. En vez
    de tenerlos por ahí desperdigados, se tienen en esta clase guardados y centralizados
    para que no se pierdan. Como es una clase genérica se puede usar para texturas,
    sonidos y fuentes.
*/

// Primero declaramos la clase para que quede bien claro
template <class Recurso, class Identificador>
class ContenedorDeRecursos;

// Ahora ponemos typedefs para no tener que escribir la biblia cada dos por tres
typedef ContenedorDeRecursos<sf::Texture,std::string> ContenedorDeTexturas;
typedef ContenedorDeRecursos<sf::SoundBuffer,std::string> ContenedorDeSonidos;
typedef ContenedorDeRecursos<sf::Font,std::string> ContenedorDeFuentes;

// Y ahora podemos usar los typedefs
template <class Recurso, class Identificador>
class ContenedorDeRecursos
{
    protected:
        ContenedorDeRecursos(){}
        ~ContenedorDeRecursos(){}

        static ContenedorDeTexturas * contenedorDeTexturas;
        static ContenedorDeSonidos * contenedorDeSonidos;
        static ContenedorDeFuentes * contenedorDeFuentes;

    private:
        // Un mapa que mapea identificadores a su respectivo recurso
        std::map<Identificador, std::unique_ptr<Recurso>> mapaDeRecursos;

    public:
        // Carga un recurso dado su identificador y la ruta del fichero
        void cargar(Identificador id, const std::string& rutaFichero);

        // Devuelve un recurso dado su identificador
        Recurso& obtener(Identificador id);

        // NUNCA SE CLONA UN SINGLETON
        ContenedorDeRecursos(ContenedorDeRecursos &otro) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const ContenedorDeRecursos &) = delete;

        // Métodos Singleton para obtener la única instancia
        static ContenedorDeRecursos<Recurso,Identificador> * unicaInstancia();

};

// Al usar templates no se puede incluir ContenedorDeRecursos.hpp en un hipotético fichero
// ContenedorDeRecursos.cpp, así que no queda otra, tenemos que hacerlo al revés. También hay
// que cambiar la extensión del .cpp a cualquier otra cosa (por ejemplo, .tpp) para que no se
// compile, ya que lo normal es que los .cpp contengan la cabecera encima, pero como este es
// al revés pues no se compila y se deja aquí debajo que está muy bien puesto. Además la cosa se
// lía aún más y también he tenido que poner un cpp, así que tenemos un hpp, tpp y cpp dios santo

#include "ContenedorDeRecursos.tpp"

#endif // __CONTENEDOR_DE_RECURSOS_HPP__
