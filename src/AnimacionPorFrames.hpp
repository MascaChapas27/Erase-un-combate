#pragma once

#include "Enums.hpp"
#include "Hitbox.hpp"
#include "Animacion.hpp"
#include "IndicacionesSobreAnimacion.hpp"
#include <SFML/Graphics.hpp>
#include <list>
#include <set>
#include <map>

/*
    Esta estructura de datos es para pasar la info al constructor de forma más bonita
    y corta en vez de pasarle 1894178 parámetros que sería un coñazo
*/
struct IngredientesAnimacionPorFrames{
    // Posición de la animación
    sf::Vector2f posicion = {0.f, 0.f};
    
    // Origen a partir del cual se harán las operaciones estas de escalado,
    // traslación y rotación y tal y cual
    sf::Vector2f origen = {0.f, 0.f};
    
    // Número de rectángulos distintos que tiene la animación
    int numRectangulos = 0;
    
    // Textura de la animación
    sf::Texture textura;
    
    // Tipo de repetición en bucle
    TipoBucle tipoBucle = TipoBucle::NORMAL;
    
    // Correspondencia entre rectángulo y hitboxes
    std::unordered_map<int,std::vector<Hitbox>> hitboxes;

    // Correspondencia entre frame y rectángulo (por ejemplo, frame 0 rectángulo 0, o frame 5 rectángulo 2)
    std::unordered_map<int, int> rectanguloCorrespondiente;

    // Indica qué frames deben hacer sonar el sonido de la animación
    std::unordered_set<int> framesConSonido;

    // Indica qué frames tienen movimiento
    std::unordered_map<int,sf::Vector2f> framesConMovimiento;

    // Indica qué frames hacen aparecer animaciones adicionales
    std::unordered_map<int,IndicacionesSobreAnimacion> framesConAnimaciones;

    // Ruta del sonido a reproducir
    std::string rutaSonido = "";
    
    // Indica si el sonido se debe repetir cada vez que vuelva a salir el frame o si una vez se
    // termine el bucle no hay que repetirlo
    bool repetirSonido = false;
};

/*
    Esta clase define animaciones para personajes y ataques formadas por varios frames
    Cada animación puede tener asociado cualquier número de hitboxes
*/
class AnimacionPorFrames : public Animacion
{
private:
    // Tipo de repetición en bucle
    TipoBucle tipoBucle;

    // Correspondencia entre rectángulo y hitboxes
    std::unordered_map<int, std::vector<Hitbox>> hitboxes;

    // Correspondencia entre frame y rectángulo (por ejemplo, frame 0 rectángulo 0, o frame 5 rectángulo 2)
    std::unordered_map<int, int> rectanguloCorrespondiente;

    // Indica qué frames deben hacer sonar el sonido de la animación
    std::unordered_set<int> framesConSonido;

    // Indica qué frames tienen movimiento
    std::unordered_map<int,sf::Vector2f> framesConMovimiento;

    // Indica qué frames hacen aparecer animaciones adicionales
    std::unordered_map<int,IndicacionesSobreAnimacion> framesConAnimaciones;

    // Indica si el sonido se debe repetir cada vez que vuelva a salir el frame o si una vez se
    // termine el bucle no hay que repetirlo
    bool repetirSonido;

    // Si repetirSonido está a false, indica si el sonido ya se ha reproducido durante todo un bucle,
    // por lo que habría que resetear la animación para que volviera a sonar
    bool sonidoYaReproducido;

    // Número de frame actual
    int frameActual;

    // Indica si es el primer frame, para no ir al siguiente (si no, nunca se mostraría el primer frame al actualizarlo inmediatamente)
    bool primerFrame;

public:

    // Constructor que se pone a cocinar una animación por frames en base a unos ingredientes
    AnimacionPorFrames(IngredientesAnimacionPorFrames &ingredientes);

    // Actualiza la animación (avanza un frame), reproduciendo el sonido si es necesario. En nuevasAnimaciones
    // se insertan las nuevas animaciones que serán introducidas. Puede que se deba mover la animación, por lo que se 
    virtual void actualizar(std::list<std::shared_ptr<Animacion>> &nuevasAnimaciones);

    // Le da la vuelta al sprite
    virtual void voltear();

    // Devuelve qué rectángulo se está mostrando actualmente
    int getNumeroRectangulo();

    // Devuelve qué frame se está mostrando actualmente
    int getNumeroFrame();

    // Cambia el tipo de bucle para la animación
    void setTipoBucle(TipoBucle tipoBucle);

    // Devuelve el movimiento asociado con el frame actual
    sf::Vector2f getMovimientoFrameActual();

    // Devuelve si ha terminado la animación (si el tipo de bucle es "SIN_BUCLE")
    bool haTerminado();

    // Resetea la animación al primer frame y el primer rectángulo
    void resetear();

    // Devuelve un clon de la animación
    std::shared_ptr<Animacion> clonar();

    // Devuelve un vector con las hitboxes del frame actual
    std::vector<Hitbox> getHitboxes();

    // Las clases que heredan de sf::Drawable deben implementar draw
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

