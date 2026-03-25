#include "AnimacionFondoBucleLineal.hpp"
#include "Constantes.hpp"
#include <SFML/Graphics.hpp>

AnimacionFondoBucleLineal::AnimacionFondoBucleLineal(sf::Texture& texturaFondo, const Direccion direccionBucle) :
Animacion(texturaFondo), direccionBucle(direccionBucle)
{
    // La textura del fondo se repite
    texturaFondo.setRepeated(true);

    // El fondo se expande de forma que abarque todo
    // lo necesario para poder funcionar correctamente
    switch(direccionBucle)
    {
        // Arriba y abajo
        case Direccion::ABAJO:
        case Direccion::ARRIBA:
            sprite.setTextureRect(sf::IntRect({0,0},{VENTANA_ANCHURA,VENTANA_ALTURA+static_cast<int>(texturaFondo.getSize().y)}));
            break;
        
        // Izquierda y derecha
        case Direccion::IZQUIERDA:
        case Direccion::DERECHA:
            sprite.setTextureRect(sf::IntRect({0,0},{VENTANA_ANCHURA+static_cast<int>(texturaFondo.getSize().x),VENTANA_ALTURA}));
            break;

        // Direcciones diagonales
        default:
            sprite.setTextureRect(sf::IntRect({0,0},{VENTANA_ANCHURA+static_cast<int>(texturaFondo.getSize().x),VENTANA_ALTURA+static_cast<int>(texturaFondo.getSize().y)}));
            break;
    }
}

void AnimacionFondoBucleLineal::actualizar(std::list<std::shared_ptr<Animacion>>& nuevasAnimaciones)
{
    switch(direccionBucle)
    {
        case Direccion::ARRIBA_IZQUIERDA:
            // El fondo se mueve hacia la izquierda y hacia arriba hasta que se sale
            // de los límites y comienza otra vez desde el principio, pareciendo así
            // que es infinito
            sprite.move({-static_cast<float>(sprite.getTexture().getSize().x/static_cast<float>(sprite.getTexture().getSize().y))/10.f,-1/10.f});
            
            if(-sprite.getPosition().x >= sprite.getTexture().getSize().x)
            {
                sprite.move(sf::Vector2f(sprite.getTexture().getSize()));
            }
            break;
        
        default:
            break;
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

void AnimacionFondoBucleLineal::chocar(Hitbox hitbox)
{
    
}