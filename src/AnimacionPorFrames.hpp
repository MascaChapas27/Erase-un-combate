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
    Esta clase define animaciones para personajes y ataques formadas por varios frames
    Cada animación puede tener asociado cualquier número de hitboxes
*/
class AnimacionPorFrames : public Animacion
{
private:
    // Tipo de repetición en bucle
    TipoBucle tipoBucle;

    // Si tipoBucle es PING_PONG, indica si la animación va hacia delante o hacia atrás
    bool pingPongHaciaDelante;

    // Si tipoBucle no es SIN_BUCLE, indica cuántas veces se repetirá la animación (0 es infinito)
    int numRepeticionesTotal;

    // Si numRepeticionesTotal no es 0, indica cuántas veces se ha repetido la animación de momento
    int numRepeticionesActual;

    // Correspondencia entre rectángulo y hitboxes
    std::map<int, std::list<Hitbox>> hitboxes;

    // Correspondencia entre frame y rectángulo (por ejemplo, frame 0 rectángulo 0, o frame 5 rectángulo 2)
    std::map<int, int> rectanguloCorrespondiente;

    // Indica qué frames deben hacer sonar el sonido de la animación
    std::set<int> framesConSonido;

    // Indica qué frames tienen movimiento
    std::map<int,sf::Vector2f> framesConMovimiento;

    // Indica qué frames hacen aparecer animaciones adicionales
    std::map<int,IndicacionesSobreAnimacion> framesConAnimaciones;

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

    // Constructor de la hostia que podría ser más corto pero nose
    AnimacionPorFrames(float posicionX, float posicionY, float origenX, float origenY, int numRectangulos, sf::Texture &textura, TipoBucle tipoBucle, int numRepeticionesTotal, std::map<int,std::list<Hitbox>> hitboxes, std::map<int,int> rectanguloCorrespondiente, std::set<int> framesConSonido, std::map<int,sf::Vector2f> framesConMovimiento, std::map<int,IndicacionesSobreAnimacion> framesConAnimaciones, std::string rutaSonido, bool repetirSonido);

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
    virtual void resetear();

    // Devuelve un clon de la animación
    virtual std::shared_ptr<Animacion> clonar();

    // Devuelve una lista con las hitboxes del frame actual
    virtual std::list<Hitbox> getHitboxes();

    // Las clases que heredan de sf::Drawable deben implementar draw
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

