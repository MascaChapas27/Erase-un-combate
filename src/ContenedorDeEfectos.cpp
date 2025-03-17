#include "ContenedorDeEfectos.hpp"
#include "ContenedorDeRecursos.hpp"
#include "Bitacora.hpp"
#include "AnimacionPorFrames.hpp"
#include "AnimacionConGravedad.hpp"
#include "AnimacionAgrandable.hpp"
#include "Utilidades.hpp"
#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>

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
        std::ifstream fichero(entrada.path());

        // Aprovechando que tenemos la ruta del fichero podemos sacar el nombre del efecto
        nombreEfecto = entrada.path().stem().string();

        // En esta variable se guarda una referencia a la textura de este efecto
        sf::Texture &textura = ContenedorDeTexturas::unicaInstancia()->obtener("sprites/efectos/" + nombreEfecto + ".png");

        if (!fichero.is_open())
        {
            Bitacora::unicaInstancia()->escribir("Emilio: ... pero señor Juan, es para hoy.");
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: Creo... creo que he extraviado el fichero ficheros/efectos.txt...");
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: Qué follón... se suspende la junta.");
            exit(EXIT_FAILURE);
        }
        else
        {
            // Bitacora::unicaInstancia()->escribir("Emilio: Listo señor Juan.");
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: registrando efecto " + nombreEfecto + ".");
        }

        std::getline(fichero, linea);

        // Se obtiene el tipo de animación
        tipoAnimacion = util::separarString(linea, ':')[1];

        // Bitacora::unicaInstancia()->escribir("Juan Cuesta: animación de tipo " + tipoAnimacion + ".");

        std::shared_ptr<Animacion> anim;

        if (tipoAnimacion == "frames")
        {

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

                // Bitacora::unicaInstancia()->escribir("Juan Cuesta: rectángulo " + std::to_string(numeroRectangulos) + ".");

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
                    Hitbox h(sf::IntRect({enteros[0], enteros[1]}, {enteros[2], enteros[3]}), enteros[4], false);

                    listaHitboxes.push_back(h);

                    std::getline(fichero, linea);
                }

                hitboxes[numeroRectangulos] = listaHitboxes;

                std::getline(fichero, linea);
                numeroRectangulos++;
            }

            // Bitacora::unicaInstancia()->escribir("Juan Cuesta: finalmente, se apuntan los frames.");

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

            // Bitacora::unicaInstancia()->escribir("Juan Cuesta: número de frames: " + std::to_string(contadorFrame));

            // Nos saltamos dos líneas, y ahora puede haber información sobre los sonidos o no
            std::getline(fichero, linea);
            std::getline(fichero, linea);

            bool repetirSonido = false;

            if (util::separarString(linea, ':')[0] == "Sonido")
            {

                repetirSonido = util::separarString(linea, ':')[1] == "repetir";

                rutaSonido = "sonidos/efectos/" + nombreEfecto + ".wav";

                // Avanzamos de línea para conseguir la lista de frames
                std::getline(fichero, linea);

                std::set<int> framesConSonido;

                for (std::string s : util::separarString(linea, ','))
                {
                    framesConSonido.insert(std::stoi(s));
                }
            }

            anim = std::make_shared<AnimacionPorFrames>(0, 0, textura.getSize().x / numeroRectangulos / 2, textura.getSize().y / 2, numeroRectangulos, textura,
                                                        util::stringATipoBucle(nombreBucle), 0, hitboxes, frameARectangulo, std::set<int>(), std::map<int, sf::Vector2f>(),
                                                        std::map<int, IndicacionesSobreAnimacion>(), rutaSonido, repetirSonido);
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

            rutaSonido = "sonidos/efectos/" + nombreEfecto + ".wav";

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

        animaciones.insert(std::pair<std::string, std::shared_ptr<Animacion>>(nombreEfecto, anim));

        Bitacora::unicaInstancia()->escribir("Juan Cuesta: se terminó de cargar la animación para el efecto " + nombreEfecto + ".\n");
    }
}
