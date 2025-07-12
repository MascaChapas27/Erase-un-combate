#include "ContenedorDeEfectos.hpp"
#include "ContenedorDeRecursos.hpp"
#include "Bitacora.hpp"
#include "AnimacionPorFrames.hpp"
#include "AnimacionConGravedad.hpp"
#include "AnimacionAgrandable.hpp"
#include "AnimacionDesvaneciente.hpp"
#include "Utilidades.hpp"
#include <SFML/Audio.hpp>
#include <filesystem>
#include "yaml-cpp/yaml.h"

ContenedorDeEfectos *ContenedorDeEfectos::contenedorDeEfectos = nullptr;

ContenedorDeEfectos *ContenedorDeEfectos::unicaInstancia()
{
    if (contenedorDeEfectos == nullptr)
        contenedorDeEfectos = new ContenedorDeEfectos();
    return contenedorDeEfectos;
}

ContenedorDeEfectos::~ContenedorDeEfectos()
{
    if (contenedorDeEfectos != nullptr)
        delete contenedorDeEfectos;
}

std::shared_ptr<Animacion> ContenedorDeEfectos::obtenerEfecto(std::string nombre)
{

    if (animaciones.count(nombre) == 0)
    {
        Bitacora::unicaInstancia()->escribir("ERROR: se ha pedido el efecto " + nombre + ", pero no existe");
        exit(EXIT_FAILURE);
    }

    // Es importante copiar la animación al trabajar con punteros
    return animaciones.at(nombre)->clonar();
}

void ContenedorDeEfectos::cargarTodosLosEfectos()
{
    Bitacora::unicaInstancia()->escribir("Juan Cuesta: Bien, sigamos... siguiente punto del día: registro de efectos especiales. Emilio, apunta, voy a dictarte los efectos en orden descendente de aparición.");

    // En esta variable se van a poner datos del fichero línea a línea
    std::string linea;

    // En esta variable se van a meter elementos separados en base a un string dado
    std::vector<std::string> elementosSeparados;

    // En esta variable se guarda el nombre del efecto
    std::string nombreEfecto;

    // En esta variable se guarda el tipo de animación del efecto
    std::string tipoAnimacion;

    // En esta variable se guarda el nombre del tipo de bucle usado para la animación actual
    std::string nombreBucle;

    // En esta variable se guarda el número de rectángulos de la animación actual
    int numeroRectangulos;

    // En esta variable se guarda la ruta del sonido que se reproducirá en cada animación
    std::string rutaSonido;

    // Abrimos cada fichero del directorio
    for (const auto &entrada : std::filesystem::directory_iterator("ficheros/efectos"))
    {
        // Se abre el fichero con información del personaje actual
        YAML::Node fichero = YAML::LoadFile(entrada.path().generic_string());

        // Aprovechando que tenemos la ruta del fichero podemos sacar el nombre del efecto
        nombreEfecto = entrada.path().stem().string();

        // En esta variable se guarda una referencia a la textura de este efecto
        sf::Texture &textura = ContenedorDeTexturas::unicaInstancia()->obtener("sprites/efectos/" + nombreEfecto + ".png");

        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Registrando efecto " + nombreEfecto + ".");

        // Se obtiene el tipo de animación
        tipoAnimacion = fichero["tipo"].as<std::string>();

        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Animación de tipo " + tipoAnimacion + ".");

        std::shared_ptr<Animacion> anim;

        if (tipoAnimacion == "Fotogramas")
        {
            // Estos serán los ingredientes para hacer la animación
            IngredientesAnimacionPorFrames ingredientes;

            ingredientes.posicion = {0.f,0.f};

            ingredientes.tipoBucle = util::stringATipoBucle(fichero["bucle"].as<std::string>());

            ingredientes.numRectangulos = fichero["rectangulos"].size();

            // Se itera por cada rectángulo
            for(size_t i = 0; i < fichero["rectangulos"].size(); i++)
            {
                // Se itera por cada hitbox del rectángulo
                for(size_t j = 0; j < fichero["rectangulos"][i].size(); j++)
                {
                    ingredientes.hitboxes[i].emplace_back(sf::IntRect({fichero["rectangulos"][i][j]["posX"].as<int>(),fichero["rectangulos"][i][j]["posY"].as<int>()},{fichero["rectangulos"][i][j]["ancho"].as<int>(),fichero["rectangulos"][i][j]["alto"].as<int>()}),fichero["rectangulos"][i][j]["ataque"].as<int>(),false);
                }
            }

            // Ahora se saca la correspondencia de fotogramas y rectángulos
            for(size_t i = 0; i < fichero["fotogramas"].size(); i++)
            {
                ingredientes.rectanguloCorrespondiente[i] = fichero["fotogramas"][i].as<int>();
            }

            if(fichero["sonido"]){
                ingredientes.rutaSonido = "sonidos/efectos/" + nombreEfecto + ".ogg";

                ingredientes.repetirSonido = fichero["sonido"]["tipo"].as<std::string>() == "Repetir";
                
                if(fichero["sonido"]["tipo"].as<std::string>() == "No repetir")
                {
                    Bitacora::unicaInstancia()->escribir("Juan Cuesta: Este efecto tiene un sonido que no se debe repetir.");
                }
                else if(fichero["sonido"]["tipo"].as<std::string>() == "Repetir")
                {
                    Bitacora::unicaInstancia()->escribir("Juan Cuesta: Este efecto tiene un sonido que se debe repetir.");
                }

                for(size_t i = 0; i < fichero["sonido"]["fotogramas"].size(); i++)
                {
                    ingredientes.framesConSonido.insert(fichero["sonido"]["fotogramas"][i].as<int>());
                }
            }

            anim = std::make_shared<AnimacionPorFrames>(ingredientes);
        }
        else if (tipoAnimacion == "gravedad")
        {

            // Se salta una línea en blanco y se saca la hitbox
            std::getline(fichero, linea);
            std::getline(fichero, linea);

            Hitbox hitbox(sf::IntRect({-1, -1}, {-1, -1}), 0, false);
            bool hitboxValida = false;

            auto vectorAux = util::separarString(linea, ':');

            if (vectorAux[0] == "Hitbox")
            {

                if (vectorAux.size() > 1)
                {
                    // Se sacan los enteros (pero son strings)
                    std::vector<std::string> enterosPeroSonStrings(util::separarString(vectorAux[1], ','));

                    // Se sacan los enteros de verdad
                    std::vector<int> enteros;
                    for (std::string string : enterosPeroSonStrings)
                    {
                        enteros.push_back(std::stoi(string));
                    }

                    // Se crea la hitbox en base a muchas cosas
                    hitbox = Hitbox(sf::IntRect({enteros[0], enteros[1]}, {enteros[2], enteros[3]}), enteros[4], false);
                    hitboxValida = true;
                }
            }
            else
            {
                Bitacora::unicaInstancia()->escribir("Juan Cuesta: Oye Emilio... esto de que el fichero " + entrada.path().string() + " tenga una línea de hitbox que no comienza por \"Hitbox\"... ¿Tú lo ves normal?");
                Bitacora::unicaInstancia()->escribir("Emilio: Bueno, normal... es que después de vivir con mi padre ya no hay nada que me sorprenda.");
                Bitacora::unicaInstancia()->escribir("Juan Cuesta: Pues no, no es normal. Qué follón... se suspende la junta.");
                exit(EXIT_FAILURE);
            }

            // Esto es feísimo y tendría que mirármelo
            anim = std::make_shared<AnimacionConGravedad>(textura, sf::Vector2f(0, 0), sf::Vector2f(0, 0), 0, rutaSonido);
            if (hitboxValida)
                ((AnimacionConGravedad *)anim.get())->setHitbox(hitbox);
        }
        else if (tipoAnimacion == "agrandable")
        {
            // Se salta una línea en blanco y se saca el número de frames de espera
            std::getline(fichero, linea);
            std::getline(fichero, linea);

            rutaSonido = "sonidos/efectos/" + nombreEfecto + ".ogg";

            int framesEspera;

            auto vectorAux = util::separarString(linea, ':');

            if (vectorAux[0] == "FramesEspera")
            {
                framesEspera = std::stoi(vectorAux[1]);
            }
            else
            {
                Bitacora::unicaInstancia()->escribir("Emilio: mire señor Juan, mire lo que me he encontrado.");
                Bitacora::unicaInstancia()->escribir("Juan Cuesta: pero si ahí debería poner \"FramesEspera\"... ¿qué clase de chapuza es esta?");
                Bitacora::unicaInstancia()->escribir("Emilio: anda que... quien sea que hiciera el fichero " + entrada.path().string() + " se ha quedado a gusto.");
                exit(EXIT_FAILURE);
            }

            anim = std::make_shared<AnimacionAgrandable>(framesEspera, textura, rutaSonido);
        }
        else if (tipoAnimacion == "desvaneciente")
        {
            // Se salta una línea en blanco y se saca la escala nueva
            std::getline(fichero, linea);
            std::getline(fichero, linea);
            
            float escalado;

            std::vector<std::string> lineaSeparada(util::separarString(linea,':'));

            if(lineaSeparada[0] != "Escala"){
                Bitacora::unicaInstancia()->escribir("Juan Cuesta: esto es inaudito. En el fichero " + entrada.path().string() + " han escrito \"" + lineaSeparada[0] + "\" en vez de \"Escala\"... es imposible continuar en esas condiciones. Se suspende la junta.");
                exit(EXIT_FAILURE);
            } else {
                escalado = std::stof(lineaSeparada[1]);
            }

            anim = std::make_shared<AnimacionDesvaneciente>(textura,escalado);
        }

        animaciones.insert(std::pair<std::string, std::shared_ptr<Animacion>>(nombreEfecto, anim));

        Bitacora::unicaInstancia()->escribir("Juan Cuesta: se terminó de cargar la animación para el efecto " + nombreEfecto + ".\n");
    }
}
