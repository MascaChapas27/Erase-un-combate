#include "ContenedorDePersonajes.hpp"
#include "ContenedorDeRecursos.hpp"
#include "Bitacora.hpp"
#include "Utilidades.hpp"
#include "Enums.hpp"
#include "IndicacionesSobreAnimacion.hpp"
#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>

ContenedorDePersonajes *ContenedorDePersonajes::contenedorDePersonajes = nullptr;

ContenedorDePersonajes *ContenedorDePersonajes::unicaInstancia()
{
    if (contenedorDePersonajes == nullptr)
        contenedorDePersonajes = new ContenedorDePersonajes();
    return contenedorDePersonajes;
}

ContenedorDePersonajes::~ContenedorDePersonajes()
{
    if (contenedorDePersonajes != nullptr)
        delete contenedorDePersonajes;
}

Personaje ContenedorDePersonajes::obtenerPersonaje(std::string nombre)
{
    // Se obtiene el personaje y se devuelve por valor, copiándolo todo
    Personaje nuevo(personajes.at(nombre));

    return nuevo;
}

void ContenedorDePersonajes::cargarTodosLosPersonajes()
{
    Bitacora::unicaInstancia()->escribir("Juan Cuesta: Siguiente punto del día: registro de personajes en el listado oficial. Emilio, apunta, voy a dictarte los personajes en orden descendente de aparición.");

    // Abrimos cada fichero del directorio
    for (const std::filesystem::directory_entry &entrada : std::filesystem::directory_iterator("ficheros/personajes"))
    {
        // En esta variable se van a poner datos del fichero línea a línea
        std::string linea;

        // En esta variable se van a meter elementos separados en base a un string dado
        std::vector<std::string> elementosSeparados;

        // En esta variable se guarda el nombre del personaje actual
        std::string nombrePersonaje;

        // En esta variable se guarda el nombre del estado actual
        std::string nombreEstado;

        // En esta variable se guarda el nombre del tipo de bucle usado para la animación actual
        std::string nombreBucle;

        // En esta variable se guarda el número de rectángulos de la animación actual
        int numeroRectangulos;
        
        // Se abre el fichero con información del personaje actual
        std::ifstream fichero(entrada.path());

        // Aprovechando que tenemos la ruta del fichero podemos sacar el nombre del personaje
        nombrePersonaje = entrada.path().stem().string();

        // Se prepara el vector de acciones para el ataque especial
        std::vector<Accion> accionesAtaqueEspecial;

        if (!fichero.is_open())
        {
            Bitacora::unicaInstancia()->escribir("Emilio: ... pero señor Juan, es para hoy.");
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: Es que creo que he perdido el fichero ficheros/personajes.txt...");
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: Qué follon... se suspende la junta.");
            exit(EXIT_FAILURE);
        }
        else
        {
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: registrando personaje " + nombrePersonaje + ".");
        }

        // En este mapa se van a guardar las animaciones según el estado
        std::map<EstadoPersonaje, std::shared_ptr<AnimacionPorFrames>> animaciones;

        std::getline(fichero, linea);

        while (linea != SECUENCIA_FIN_FICHERO)
        {
            // Se obtiene el nombre del estado
            nombreEstado = util::separarString(linea, ':')[1];

            Bitacora::unicaInstancia()->escribir("Juan Cuesta: estado " + nombreEstado + ".");

            // Se obtiene el tipo de bucle
            std::getline(fichero, linea);
            nombreBucle = util::separarString(linea, ':')[1];

            // Se salta una línea en blanco y se empiezan a sacar rectángulos
            numeroRectangulos = 0;
            std::getline(fichero, linea);
            std::getline(fichero, linea);

            // En esta variable se guarda el mapa que mapea números de rectángulo a hitboxes
            std::map<int, std::list<Hitbox>> hitboxes;

            while (util::separarString(linea, ':')[0] == "Rectangulo")
            {
                // Lista de hitboxes para este rectángulo
                std::list<Hitbox> listaHitboxes;

                // Se salta la línea que dice "Hitboxes" y empezamos a contar hitboxes
                std::getline(fichero, linea);
                std::getline(fichero, linea);

                while (linea != "")
                {
                    // Se sacan los enteros (pero son strings)
                    std::vector<std::string> enterosPeroSonStrings = util::separarString(linea, ',');

                    // Se sacan los enteros de verdad
                    std::vector<int> enteros;
                    for (std::string string : enterosPeroSonStrings)
                    {
                        enteros.push_back(std::stoi(string));
                    }

                    // Se crea la hitbox en base a muchas cosas
                    Hitbox h(sf::IntRect({enteros[0], enteros[1]}, {enteros[2], enteros[3]}), enteros[4], nombreEstado == "ataque-agachado" && enteros[4] > 0);

                    listaHitboxes.push_back(h);

                    std::getline(fichero, linea);
                }

                hitboxes[numeroRectangulos] = listaHitboxes;

                std::getline(fichero, linea);
                numeroRectangulos++;
            }

            if (nombreEstado == "ataque-agachado")
                Bitacora::unicaInstancia()->escribir("Juan Cuesta: las hitboxes con daño de este estado serán de ataque bajo");

            // Ahora sacamos la correspondencia de frames y rectángulos
            std::getline(fichero, linea);

            int contadorFrame = 0;

            // En esta variable se guarda el mapa que mapea números de frame a números de rectángulo
            std::map<int, int> frameARectangulo;

            for (std::string rectanguloString : util::separarString(linea, ','))
            {
                frameARectangulo[contadorFrame] = std::stoi(rectanguloString);

                contadorFrame++;
            }

            // Nos saltamos dos líneas, y ahora leemos información sobre los sonidos
            std::getline(fichero, linea);
            std::getline(fichero, linea);

            std::string rutaSonido;

            bool repetirSonido = false;

            std::set<int> framesConSonido;

            std::map<int, sf::Vector2f> framesConMovimiento;

            std::map<int, IndicacionesSobreAnimacion> framesConAnimaciones;

            if (util::separarString(linea, ':')[1] != "sin sonido")
            {

                repetirSonido = util::separarString(linea, ':')[1] == "repetir";

                rutaSonido = "sonidos/personajes/" + nombrePersonaje + "/" + nombreEstado + ".wav";

                // Avanzamos de línea para conseguir la lista de frames
                std::getline(fichero, linea);

                for (std::string s : util::separarString(linea, ','))
                {
                    framesConSonido.insert(std::stoi(s));
                }
            }

            // Nos saltamos dos líneas para comprobar los movimientos
            std::getline(fichero, linea);
            std::getline(fichero, linea);

            // Nos saltamos la línea que dice "Movimiento:"
            std::getline(fichero, linea);

            while (linea != "")
            {
                elementosSeparados = util::separarString(linea, ',');

                framesConMovimiento[std::stoi(elementosSeparados[0])] = sf::Vector2f(std::stof(elementosSeparados[1]), std::stof(elementosSeparados[2]));

                std::getline(fichero, linea);
            }

            // Saltamos una línea para poder extraer las animaciones
            std::getline(fichero, linea);

            // Saltamos la línea que dice "Efectos:"
            std::getline(fichero, linea);

            while (linea != "" && linea != SECUENCIA_DELIMITADORA_FICHERO)
            {
                elementosSeparados = util::separarString(linea, ',');

                IndicacionesSobreAnimacion indicaciones;

                indicaciones.necesitaVoltearse = false;

                indicaciones.rutaAnimacion = elementosSeparados[1];
                indicaciones.posicionInicial = sf::Vector2f(std::stof(elementosSeparados[2]), std::stof(elementosSeparados[3]));

                if (elementosSeparados.size() == 6)
                {
                    indicaciones.velocidadInicial = sf::Vector2f(std::stof(elementosSeparados[4]), std::stof(elementosSeparados[5]));
                }

                framesConAnimaciones[std::stoi(elementosSeparados[0])] = indicaciones;

                std::getline(fichero, linea);
            }

            // Se salta una línea, puede que hayamos llegado al final de esta acción
            // o puede que hayamos llegado a una línea en blanco porque después están
            // las acciones para el ataque especial
            std::getline(fichero, linea);
            
            // Si el estado es ataque especial, se leen las acciones
            if(nombreEstado == "ataque-especial"){
                if(util::separarString(linea,':')[0] == "Acciones"){
                    for(const std::string &s : util::separarString(util::separarString(linea,':')[1],',')){
                        accionesAtaqueEspecial.push_back(util::stringAAccion(s));
                    }
                    std::getline(fichero, linea);
                    std::getline(fichero, linea);
                } else {
                    Bitacora::unicaInstancia()->escribir("Emilio: Señor Juan, no me pregunte por qué, pero este ataque especial no tiene teclas asociadas.");
                    Bitacora::unicaInstancia()->escribir("Juan Cuesta: Esto es inaudito, vamos. Se suspende la junta.");
                    exit(EXIT_FAILURE);
                }
            }

            std::shared_ptr<AnimacionPorFrames> anim = std::make_shared<AnimacionPorFrames>(0, 0, PERSONAJE_PLANTILLA_ORIGEN.x, PERSONAJE_PLANTILLA_ORIGEN.y, numeroRectangulos,
                                                                                            ContenedorDeTexturas::unicaInstancia()->obtener("sprites/personajes/" + nombrePersonaje + "/" + nombreEstado + ".png"),
                                                                                            util::stringATipoBucle(nombreBucle), 0, hitboxes, frameARectangulo, framesConSonido, framesConMovimiento,
                                                                                            framesConAnimaciones, rutaSonido, repetirSonido);

            animaciones.insert(std::pair<EstadoPersonaje, std::shared_ptr<AnimacionPorFrames>>(util::stringAEstadoPersonaje(nombreEstado), anim));

            Bitacora::unicaInstancia()->escribir("Juan Cuesta: se terminó de cargar la animación para el estado " + nombreEstado + ".\n");
        }

        personajes.insert(std::pair<std::string, Personaje>(nombrePersonaje, Personaje(animaciones, nombrePersonaje, accionesAtaqueEspecial)));

        Bitacora::unicaInstancia()->escribir("Juan Cuesta: concluye la inserción del personaje " + nombrePersonaje + ".\n");
    }
}