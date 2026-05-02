#include "AnimacionFondoBucleLineal.hpp"
#include "Constantes.hpp"
#include <SFML/Graphics.hpp>
#include <numbers>

AnimacionFondoBucleLineal::AnimacionFondoBucleLineal(sf::Texture& texturaFondo, const Direccion direccionBucle, const float velocidad, const sf::Vector2i tamanoFondo) :
Animacion(texturaFondo), direccionBucle(direccionBucle)
{
    // La textura del fondo se repite
    texturaFondo.setRepeated(true);

    // Este es el ancho y el alto del sprite. Se declararán
    // dentro del switch
    int ancho;
    int alto;

    // El fondo se expande de forma que abarque todo
    // lo necesario para poder funcionar correctamente
    switch(direccionBucle)
    {
        // Arriba y abajo
        case Direccion::ABAJO:
        case Direccion::ARRIBA:
            ancho = tamanoFondo.x == 0.f ? VENTANA_ANCHURA : tamanoFondo.x;
            alto = VENTANA_ALTURA+static_cast<int>(texturaFondo.getSize().y);
            sprite.setTextureRect(sf::IntRect({0,0},{ancho,alto}));
            break;
        
        // Izquierda y derecha
        case Direccion::IZQUIERDA:
        case Direccion::DERECHA:
            ancho = VENTANA_ANCHURA+static_cast<int>(texturaFondo.getSize().x);
            alto = tamanoFondo.y == 0.f ? VENTANA_ALTURA : tamanoFondo.y;
            sprite.setTextureRect(sf::IntRect({0,0},{ancho,alto}));
            break;

        // Direcciones diagonales
        default:
            ancho = VENTANA_ANCHURA+static_cast<int>(texturaFondo.getSize().x);
            alto = VENTANA_ALTURA+static_cast<int>(texturaFondo.getSize().y);
            sprite.setTextureRect(sf::IntRect({0,0},{ancho,alto}));
            break;
    }

    // El ángulo del vector que indica el movimiento a realizar
    float angulo = 0.f;

    // El número pi para que no se haga tan largo el nombre
    float pi = std::numbers::pi;

    switch(direccionBucle)
    {
        case Direccion::DERECHA:
            angulo = 0.f;
            break;

        case Direccion::ARRIBA_DERECHA:
            angulo = pi/4.f;
            break;
        
        case Direccion::ARRIBA:
            angulo = pi/2.f;
            break;

        case Direccion::ARRIBA_IZQUIERDA:
            angulo = 3.f*pi/4.f;
            break;
        
        case Direccion::IZQUIERDA:
            angulo = pi;
            break;

        case Direccion::ABAJO_IZQUIERDA:
            angulo = 5.f*pi/4.f;
            break;
        
        case Direccion::ABAJO:
            angulo = 3.f*pi/2.f;
            break;

        case Direccion::ABAJO_DERECHA:
            angulo = 7.f*pi/4.f;
            break;
        
        default:
            break;
    }

    // Ahora que se tiene el ángulo, se calcula el movimiento
    // en los ejes X e Y
    this->movimientoHorizontal = velocidad * std::cosf(angulo);
    this->movimientoVertical = -velocidad * std::sinf(angulo);
}

void AnimacionFondoBucleLineal::actualizar(std::list<std::shared_ptr<Animacion>>& nuevasAnimaciones)
{
    // Se mueve el sprite del fondo según los valores
    // que se calcularon en el constructor en su momento
    sprite.move({movimientoHorizontal,movimientoVertical});

    // Si el sprite se va muy hacia la izquierda o hacia la derecha,
    // se devuelve a su sitio
    if(direccionBucle != Direccion::ARRIBA && direccionBucle != Direccion::ABAJO)
    {
        if(-sprite.getPosition().x > static_cast<float>(sprite.getTexture().getSize().x))
        {
            sprite.move(sf::Vector2f(static_cast<float>(sprite.getTexture().getSize().x),0.f));
        }
        else if(sprite.getPosition().x > 0.f)
        {
            sprite.move(sf::Vector2f(-static_cast<float>(sprite.getTexture().getSize().x),0.f));
        }
    }

    // Si el sprite se va muy hacia arriba o hacia abajo,
    // se devuelve a su sitio
    if(direccionBucle != Direccion::IZQUIERDA && direccionBucle != Direccion::DERECHA)
    {
        if(-sprite.getPosition().y > static_cast<float>(sprite.getTexture().getSize().y))
        {
            sprite.move(sf::Vector2f(0.f,static_cast<float>(sprite.getTexture().getSize().y)));
        }
        else if(sprite.getPosition().y > 0.f)
        {
            sprite.move(sf::Vector2f(0.f,-static_cast<float>(sprite.getTexture().getSize().y)));
        }
    }
}

void AnimacionFondoBucleLineal::draw(sf::RenderTarget& r, sf::RenderStates s) const
{
    r.draw(sprite,s);
}

void AnimacionFondoBucleLineal::resetear()
{
    sprite.setPosition(sf::Vector2f(0.f,0.f));
}

void AnimacionFondoBucleLineal::voltear()
{
    // De verdad le voy a dar la vuelta a un fondo???
}

std::vector<Hitbox> AnimacionFondoBucleLineal::getHitboxes()
{
    // Pero qué hitboxes va a tener un fondo
    // por dios santo
    return std::vector<Hitbox>();
}

bool AnimacionFondoBucleLineal::haTerminado()
{
    // Es un fondo en bucle infinito así que no creo
    // que vaya a terminar pronto
    return false;
}

std::shared_ptr<Animacion> AnimacionFondoBucleLineal::clonar()
{
    return std::make_shared<AnimacionFondoBucleLineal>(*this);
}