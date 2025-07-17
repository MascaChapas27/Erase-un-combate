#include "ContenedorDePersonajes.hpp"
#include "ContenedorDeRecursos.hpp"
#include "Bitacora.hpp"
#include "Utilidades.hpp"
#include "Enums.hpp"
#include "IndicacionesSobreAnimacion.hpp"
#include <SFML/Audio.hpp>
#include <filesystem>
#include "yaml-cpp/yaml.h"

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
        // En este mapa se van a guardar las animaciones según el estado
        std::map<EstadoPersonaje, std::shared_ptr<AnimacionPorFrames>> animaciones;

        // Las acciones que hay que hacer en orden para poder desencadenar el ataque especial
        std::vector<Accion> accionesAtaqueEspecial;
        
        // Se abre el fichero con información del personaje actual
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Abriendo fichero " + entrada.path().generic_string());
        YAML::Node fichero = YAML::LoadFile(entrada.path().generic_string());

        // Aprovechando que tenemos la ruta del fichero podemos sacar el nombre del personaje
        std::string nombrePersonaje = entrada.path().stem().string();

        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Extrayendo información del personaje " + nombrePersonaje);

        // Se consigue el nodo con la información del personaje y el nodo con los estados
        YAML::Node infoPersonaje = fichero["infoPersonaje"];

        YAML::Node estados = fichero["estados"];

        // Se saca la info del propio personaje (no de sus estados)
        int vidaPersonaje = infoPersonaje["vida"].as<int>();

        float velocidadPersonaje = infoPersonaje["velocidad"].as<float>();

        float fuerzaSaltoPersonaje = infoPersonaje["salto"].as<float>();
        
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Personaje con " + std::to_string(vidaPersonaje) + " puntos de vida, velocidad de " + std::to_string(velocidadPersonaje) + " y fuerza de salto de " + std::to_string(fuerzaSaltoPersonaje));

        // Ahora se saca la info de los estados del personaje
        for(YAML::const_iterator itEstado = estados.begin(); itEstado != estados.end(); ++itEstado)
        {
            // Se saca el nombre de este estado
            std::string nombreEstado = itEstado->first.as<std::string>();

            // Se preparan los ingredientes
            IngredientesAnimacionPorFrames ingredientes;

            // Los personajes sí tienen sombra
            if(nombreEstado == "golpeado-subiendo" || nombreEstado == "golpeado-bajando" || nombreEstado == "tumbado")
            {
                ingredientes.tipoSombra = TipoSombra::LARGA;
            }
            else
            {
                ingredientes.tipoSombra = TipoSombra::NORMAL;
            }
            

            ingredientes.posicion = {0.f, 0.f};

            ingredientes.origen = {PERSONAJE_PLANTILLA_ORIGEN.x, PERSONAJE_PLANTILLA_ORIGEN.y};

            ingredientes.rutaTextura = "sprites/personajes/" + nombrePersonaje + "/" + nombreEstado + ".png";

            Bitacora::unicaInstancia()->escribir("Juan Cuesta: Encontrado estado \"" + nombreEstado + "\".");

            // Dentro de este estado se sacan datos
            for(YAML::const_iterator itAtributoEstado = itEstado->second.begin(); itAtributoEstado != itEstado->second.end(); ++itAtributoEstado)
            {
                // Se saca la info del tipo de bucle
                if(itAtributoEstado->first.as<std::string>() == "bucle")
                {
                    std::string nombreBucle = itAtributoEstado->second.as<std::string>();

                    ingredientes.tipoBucle = util::stringATipoBucle(nombreBucle);

                    Bitacora::unicaInstancia()->escribir("Juan Cuesta: El bucle es de tipo \"" + nombreBucle + "\".");
                }
                
                // Se saca la info de los rectángulos de la animación de este estado
                else if(itAtributoEstado->first.as<std::string>() == "rectangulos")
                {
                    Bitacora::unicaInstancia()->escribir("Juan Cuesta: Extrayendo información sobre los " + std::to_string(itAtributoEstado->second.size()) + " rectángulos...");

                    ingredientes.numRectangulos = itAtributoEstado->second.size();

                    // Se itera por cada rectángulo
                    for(size_t rectanguloActual = 0; rectanguloActual < itAtributoEstado->second.size(); rectanguloActual++)
                    {
                        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Rectángulo " + std::to_string(rectanguloActual));
                        
                        // Se crea un vector de hitboxes vacío primero
                        ingredientes.hitboxes[rectanguloActual].clear();
                        
                        // Por cada rectángulo, se itera por sus hitboxes
                        for(size_t hitboxActual = 0; hitboxActual < itAtributoEstado->second[rectanguloActual].size(); hitboxActual++)
                        {
                            sf::Vector2i posicionHitbox({itAtributoEstado->second[rectanguloActual][hitboxActual]["posX"].as<int>(),itAtributoEstado->second[rectanguloActual][hitboxActual]["posY"].as<int>()});
                            sf::Vector2i tamanoHitbox({itAtributoEstado->second[rectanguloActual][hitboxActual]["ancho"].as<int>(),itAtributoEstado->second[rectanguloActual][hitboxActual]["alto"].as<int>()});
                            int ataqueHitbox = itAtributoEstado->second[rectanguloActual][hitboxActual]["ataque"].as<int>();
                            bool ataqueBajo = nombreEstado == "ataque-agachado";

                            ingredientes.hitboxes[rectanguloActual].emplace_back(sf::IntRect(posicionHitbox,tamanoHitbox),ataqueHitbox,ataqueBajo);

                            Bitacora::unicaInstancia()->escribir("Juan Cuesta: Hitbox " + std::to_string(hitboxActual) + ". Posición (" + std::to_string(posicionHitbox.x) + "," + std::to_string(posicionHitbox.y) + "), tamaño " + std::to_string(tamanoHitbox.x) + "x" + std::to_string(tamanoHitbox.y) + ", ataque " + std::to_string(ataqueHitbox));
                        }
                    }
                }

                // Se saca la info de en qué fotograma va cada rectángulo
                else if(itAtributoEstado->first.as<std::string>() == "fotogramas")
                {
                    Bitacora::unicaInstancia()->escribir("Juan Cuesta: Encontrados " + std::to_string(itAtributoEstado->second.size()) + " fotogramas.");

                    for(size_t frameActual = 0; frameActual < itAtributoEstado->second.size(); frameActual++)
                    {
                        ingredientes.rectanguloCorrespondiente[frameActual] = itAtributoEstado->second[frameActual].as<int>();
                    }
                }

                // Se saca la info del sonido
                else if(itAtributoEstado->first.as<std::string>() == "sonido")
                {
                    ingredientes.rutaSonido = "sonidos/personajes/" + nombrePersonaje + "/" + nombreEstado + ".ogg";

                    ingredientes.repetirSonido = itAtributoEstado->second["tipo"].as<std::string>() == "Repetir";
                    
                    if(itAtributoEstado->second["tipo"].as<std::string>() == "No repetir")
                    {
                        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Este estado tiene un sonido que no se debe repetir.");
                    }
                    else if(itAtributoEstado->second["tipo"].as<std::string>() == "Repetir")
                    {
                        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Este estado tiene un sonido que se debe repetir.");
                    }

                    for(size_t i = 0; i < itAtributoEstado->second["fotogramas"].size(); i++)
                    {
                        ingredientes.framesConSonido.insert(itAtributoEstado->second["fotogramas"][i].as<int>());
                    }
                }
                else if(itAtributoEstado->first.as<std::string>() == "efectos")
                {
                    Bitacora::unicaInstancia()->escribir("Juan Cuesta: Encontrados " + std::to_string(itAtributoEstado->second.size()) + " efectos");

                    for(size_t i = 0; i < itAtributoEstado->second.size(); i++)
                    {
                        std::string nombreEfecto = itAtributoEstado->second[i]["efecto"].as<std::string>();
                        int fotogramaEfecto = itAtributoEstado->second[i]["fotograma"].as<int>();
                        sf::Vector2f posicionEfecto({itAtributoEstado->second[i]["posX"].as<float>(), itAtributoEstado->second[i]["posY"].as<float>()});
                        sf::Vector2f velocidadInicialEfecto({itAtributoEstado->second[i]["velX"].as<float>(), itAtributoEstado->second[i]["velY"].as<float>()});

                        IndicacionesSobreAnimacion indicacionesAnimacion;
                        indicacionesAnimacion.necesitaVoltearse = false;
                        indicacionesAnimacion.posicionInicial = posicionEfecto;
                        indicacionesAnimacion.rutaAnimacion = nombreEfecto;
                        indicacionesAnimacion.velocidadInicial = velocidadInicialEfecto;

                        ingredientes.framesConAnimaciones[fotogramaEfecto] = indicacionesAnimacion;

                        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Efecto número " + std::to_string(i) + " de nombre \"" + nombreEfecto + "\" desencadenado en fotograma " + std::to_string(fotogramaEfecto) + ". Posición (" + std::to_string(posicionEfecto.x) + "," + std::to_string(posicionEfecto.y) + ") y velocidad inicial (" + std::to_string(velocidadInicialEfecto.x) + "," + std::to_string(velocidadInicialEfecto.y) + ").");
                    }
                }
                else if(itAtributoEstado->first.as<std::string>() == "movimientos")
                {
                    Bitacora::unicaInstancia()->escribir("Juan Cuesta: Encontrados " + std::to_string(itAtributoEstado->second.size()) + " movimientos.");

                    for(size_t i = 0; i < itAtributoEstado->second.size(); i++)
                    {
                        int fotogramaMovimiento = itAtributoEstado->second[i]["fotograma"].as<int>();
                        sf::Vector2f velocidadMovimiento = {itAtributoEstado->second[i]["velX"].as<float>(),itAtributoEstado->second[i]["velY"].as<float>()};

                        ingredientes.framesConMovimiento[fotogramaMovimiento] = velocidadMovimiento;

                        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Movimiento número " + std::to_string(i) + " desencadenado en fotograma " + std::to_string(fotogramaMovimiento) + ". Movimiento de (" + std::to_string(velocidadMovimiento.x) + "," + std::to_string(velocidadMovimiento.y) + ").");
                    }
                }
                else if(itAtributoEstado->first.as<std::string>() == "estiramientos")
                {
                    Bitacora::unicaInstancia()->escribir("Juan Cuesta: Encontrados " + std::to_string(itAtributoEstado->second.size()) + " estiramientos.");

                    for(size_t i = 0; i < itAtributoEstado->second.size(); i++)
                    {
                        int fotogramaEstiramiento = itAtributoEstado->second[i]["fotograma"].as<int>();
                        sf::Vector2f escalaEstiramiento = {itAtributoEstado->second[i]["escalaX"].as<float>(),itAtributoEstado->second[i]["escalaY"].as<float>()};

                        ingredientes.framesConEstiramientos[fotogramaEstiramiento] = escalaEstiramiento;

                        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Estiramiento número " + std::to_string(i) + " desencadenado en fotograma " + std::to_string(fotogramaEstiramiento) + ". Estiramiento de (" + std::to_string(escalaEstiramiento.x) + "," + std::to_string(escalaEstiramiento.y) + ").");
                    }
                }
                else if(itEstado->first.as<std::string>() == "ataque-especial" && itAtributoEstado->first.as<std::string>() == "acciones")
                {
                    Bitacora::unicaInstancia()->escribir("Juan Cuesta: Las acciones a llevar a cabo para realizar este ataque especial son:");

                    for(size_t i = 0; i < itAtributoEstado->second.size(); i++)
                    {
                        accionesAtaqueEspecial.push_back(util::stringAAccion(itAtributoEstado->second[i].as<std::string>()));
                        Bitacora::unicaInstancia()->escribir("Juan Cuesta: " + itAtributoEstado->second[i].as<std::string>());
                    }
                }
            }

            std::shared_ptr<AnimacionPorFrames> anim = std::make_shared<AnimacionPorFrames>(ingredientes);

            animaciones.insert(std::pair<EstadoPersonaje, std::shared_ptr<AnimacionPorFrames>>(util::stringAEstadoPersonaje(nombreEstado), anim));
            
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: Se terminó de cargar la animación para el estado " + nombreEstado + ".\n");
        }

        personajes.insert(std::pair<std::string, Personaje>(nombrePersonaje, Personaje(animaciones, nombrePersonaje, vidaPersonaje, velocidadPersonaje, fuerzaSaltoPersonaje, accionesAtaqueEspecial)));

        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Concluye la inserción del personaje " + nombrePersonaje + ".\n");
    }
}