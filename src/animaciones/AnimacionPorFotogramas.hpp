#pragma once

#include "Enums.hpp"
#include "Hitbox.hpp"
#include "Animacion.hpp"
#include "IndicacionesSobreAnimacion.hpp"
#include <SFML/Graphics.hpp>
#include <list>
#include <set>
#include <unordered_map>

/*
    Esta estructura de datos es para pasar la info al constructor de forma más bonita
    y corta en vez de pasarle 1894178 parámetros que sería un coñazo
*/
struct IngredientesAnimacionPorFotogramas
{
    // Posición de la animación
    sf::Vector2f posicion = {0.f, 0.f};

    // Origen a partir del cual se harán las operaciones estas de escalado,
    // traslación y rotación y tal y cual
    sf::Vector2f origen = {0.f, 0.f};

    // Número de rectángulos distintos que tiene la animación
    int numRectangulos = 0;

    // Ruta de la textura de la animación
    std::string rutaTextura = "";

    // Tipo de repetición en bucle
    TipoBucle tipoBucle = TipoBucle::NORMAL;

    // Correspondencia entre rectángulo y hitboxes
    std::unordered_map<int, std::vector<Hitbox>> hitboxes;

    // Dado un fotograma, indica el grupo de colisión correspondiente
    std::unordered_map<int, int> fotogramaAGrupoDeColision;

    // Indica cuántos grupos de colisión hay en total (desde el grupo 0 hasta
    // el grupo numGruposDeColision-1)
    int numGruposDeColision;

    // Correspondencia entre fotograma y rectángulo (por ejemplo, fotograma 0 rectángulo 0, o fotograma 5 rectángulo 2)
    std::unordered_map<int, int> rectanguloCorrespondiente;

    // Indica qué fotogramas deben hacer sonar el sonido de la animación
    std::unordered_set<int> fotogramasConSonido;

    // Indica qué fotogramas tienen movimiento
    std::unordered_map<int, sf::Vector2f> fotogramasConMovimiento;

    // Indica qué fotogramas tienen estiramientos
    std::unordered_map<int, sf::Vector2f> fotogramasConEstiramientos;

    // Indica qué fotogramas hacen aparecer animaciones adicionales
    std::unordered_map<int, IndicacionesSobreAnimacion> fotogramasConAnimaciones;

    // Ruta del sonido a reproducir
    std::string rutaSonido = "";

    // Indica si el sonido se debe repetir cada vez que vuelva a salir el fotograma o si una vez se
    // termine el bucle no hay que repetirlo
    bool repetirSonido = false;

    // Indica el tipo de sombra para la animación
    TipoSombra tipoSombra = TipoSombra::SIN_SOMBRA;
};

/*
    Esta clase define animaciones para personajes y ataques formadas por varios fotogramas
    Cada animación puede tener asociado cualquier número de hitboxes
*/
class AnimacionPorFotogramas : public Animacion
{
private:
    // Tipo de repetición en bucle
    TipoBucle tipoBucle;

    // Correspondencia entre rectángulo y hitboxes
    std::unordered_map<int, std::vector<Hitbox>> hitboxes;

    // Dado un fotograma, indica el grupo de colisión correspondiente
    std::unordered_map<int, int> fotogramaAGrupoDeColision;

    // Dado un grupo de colisión, indica si está activo o no
    std::unordered_map<int,bool> grupoDeColisionActivo;

    // Correspondencia entre fotograma y rectángulo (por ejemplo, fotograma 0 rectángulo 0, o fotograma 5 rectángulo 2)
    std::unordered_map<int, int> rectanguloCorrespondiente;

    // Indica qué fotogramas deben hacer sonar el sonido de la animación
    std::unordered_set<int> fotogramasConSonido;

    // Indica qué fotogramas tienen movimiento
    std::unordered_map<int, sf::Vector2f> fotogramasConMovimiento;

    // Indica qué fotogramas hacen que la animación se estire
    std::unordered_map<int, sf::Vector2f> fotogramasConEstiramientos;

    // Indica qué fotogramas hacen aparecer animaciones adicionales
    std::unordered_map<int, IndicacionesSobreAnimacion> fotogramasConAnimaciones;

    // Indica si el sonido se debe repetir cada vez que vuelva a salir el fotograma o si una vez se
    // termine el bucle no hay que repetirlo
    bool repetirSonido;

    // Si repetirSonido está a false, indica si el sonido ya se ha reproducido durante todo un bucle,
    // por lo que habría que resetear la animación para que volviera a sonar
    bool sonidoYaReproducido;

    // Número de fotograma actual
    int fotogramaActual;

    // Indica si es el primer fotograma, para no ir al siguiente (si no, nunca se mostraría el primer fotograma al actualizarlo inmediatamente)
    bool primerFotograma;

    // Indica el tipo de sombra para esta animación
    TipoSombra tipoSombra;

public:
    // Constructor que se pone a cocinar una animación por fotogramas en base a unos ingredientes
    AnimacionPorFotogramas(IngredientesAnimacionPorFotogramas &ingredientes);

    // Actualiza la animación (avanza un fotograma), reproduciendo el sonido si es necesario. En nuevasAnimaciones
    // se insertan las nuevas animaciones que serán introducidas. Puede que se deba mover la animación, por lo que se
    virtual void actualizar(std::list<std::shared_ptr<Animacion>> &nuevasAnimaciones);

    // Le da la vuelta al sprite
    virtual void voltear();

    // Devuelve qué rectángulo se está mostrando actualmente
    int getNumeroRectangulo();

    // Devuelve qué fotograma se está mostrando actualmente
    int getNumeroFotograma();

    // Devuelve la escala a la que se debería estirar el sprite
    // en el fotograma actual
    sf::Vector2f getEstiramientoFotogramaActual();

    // Cambia el tipo de bucle para la animación
    void setTipoBucle(TipoBucle tipoBucle);

    // Devuelve el movimiento asociado con el fotograma actual
    sf::Vector2f getMovimientoFotogramaActual();

    // Devuelve si ha terminado la animación (si el tipo de bucle es "SIN_BUCLE")
    bool haTerminado();

    // Resetea la animación al primer fotograma y el primer rectángulo
    void resetear();

    // Indica a la animación que acaba de chocar
    void chocar();

    // Devuelve un clon de la animación
    std::shared_ptr<Animacion> clonar();

    // Devuelve un vector con las hitboxes del fotograma actual
    std::vector<Hitbox> getHitboxes();

    // Las clases que heredan de sf::Drawable deben implementar draw
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};
