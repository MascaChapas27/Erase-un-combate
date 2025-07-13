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

            ingredientes.rutaTextura = "sprites/efectos/" + nombreEfecto + ".png";

            ingredientes.posicion = {0.f,0.f};

            ingredientes.numRectangulos = fichero["rectangulos"].size();

            // Una referencia a la textura para ahorrar espacio al calcular el origen
            sf::Texture& textura = ContenedorDeTexturas::unicaInstancia()->obtener(ingredientes.rutaTextura);

            ingredientes.origen = {(textura.getSize().x/ingredientes.numRectangulos)/2.f,textura.getSize().y/2.f};

            ingredientes.tipoBucle = util::stringATipoBucle(fichero["bucle"].as<std::string>());

            ingredientes.rutaTextura = "sprites/efectos/" + nombreEfecto + ".png";

            Bitacora::unicaInstancia()->escribir("Juan Cuesta: Tipo de bucle " + fichero["bucle"].as<std::string>() + ", y " + std::to_string(ingredientes.numRectangulos) + " rectángulos.");

            // Se establece el tipo de bucle
            ingredientes.tipoBucle = util::stringATipoBucle(fichero["bucle"].as<std::string>());

            // Se itera por cada rectángulo
            for(size_t i = 0; i < fichero["rectangulos"].size(); i++)
            {
                // Se crea un vector de hitboxes vacío primero
                ingredientes.hitboxes[i].clear();

                // Se itera por cada hitbox del rectángulo
                for(size_t j = 0; j < fichero["rectangulos"][i].size(); j++)
                {
                    ingredientes.hitboxes[i].emplace_back(sf::IntRect({fichero["rectangulos"][i][j]["posX"].as<int>(),fichero["rectangulos"][i][j]["posY"].as<int>()},{fichero["rectangulos"][i][j]["ancho"].as<int>(),fichero["rectangulos"][i][j]["alto"].as<int>()}),fichero["rectangulos"][i][j]["ataque"].as<int>(),false);
                }
            }

            assert(ingredientes.numRectangulos == ingredientes.hitboxes.size());

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
        else if (tipoAnimacion == "Gravedad")
        {
            Hitbox hitbox(sf::IntRect({-1, -1}, {-1, -1}), 0, false);
            bool hitboxValida = false;

            if(fichero["Hitbox"]){
                hitbox = Hitbox(sf::IntRect({fichero["Hitbox"]["posX"].as<int>(),fichero["Hitbox"]["posY"].as<int>()}, {fichero["Hitbox"]["ancho"].as<int>(),fichero["Hitbox"]["alto"].as<int>()}), fichero["Hitbox"]["ataque"].as<int>(), false);
                hitboxValida = true;
            }

            // Esto es feísimo y tendría que mirármelo
            anim = std::make_shared<AnimacionConGravedad>(textura, sf::Vector2f(0, 0), sf::Vector2f(0, 0), 0, rutaSonido);
            if (hitboxValida)
                ((AnimacionConGravedad *)anim.get())->setHitbox(hitbox);
        }
        else if (tipoAnimacion == "Agrandable")
        {
            rutaSonido = "sonidos/efectos/" + nombreEfecto + ".ogg";

            int framesEspera = fichero["framesEspera"].as<int>();

            anim = std::make_shared<AnimacionAgrandable>(framesEspera, textura, rutaSonido);
        }
        else if (tipoAnimacion == "Desvaneciente")
        {   
            float escalado = fichero["escala"].as<float>();

            anim = std::make_shared<AnimacionDesvaneciente>(textura, escalado);
        }

        animaciones.insert(std::pair<std::string, std::shared_ptr<Animacion>>(nombreEfecto, anim));

        Bitacora::unicaInstancia()->escribir("Juan Cuesta: se terminó de cargar la animación para el efecto " + nombreEfecto + ".\n");
    }
}
