#include "AnimacionParticulaLineal.hpp"
#include "Constantes.hpp"
#include "Utilidades.hpp"
#include <cmath>

AnimacionParticulaLineal::AnimacionParticulaLineal(sf::Texture &textura) : 
    Animacion(textura)
{
    sprite.setOrigin({(float)sprite.getTextureRect().size.x / 2, (float)sprite.getTextureRect().size.y / 2});
}

AnimacionParticulaLineal::AnimacionParticulaLineal(sf::Texture &textura, sf::Vector2f posicionInicial, sf::Vector2f posicionFinal) : 
    Animacion(textura), posicionInicial(posicionInicial), posicionFinal(posicionFinal)
{
    sprite.setOrigin({(float)sprite.getTextureRect().size.x / 2, (float)sprite.getTextureRect().size.y / 2});
    sprite.setPosition(posicionInicial);
}

AnimacionParticulaLineal::AnimacionParticulaLineal(sf::Texture &textura, sf::Vector2f posicionInicial, float angulo, float longitud) : 
    Animacion(textura), posicionInicial(posicionInicial)
{
    sprite.setOrigin({(float)sprite.getTextureRect().size.x / 2, (float)sprite.getTextureRect().size.y / 2});
    
    setPosicionFinal(angulo,longitud);
}

void AnimacionParticulaLineal::setPosicionInicial(sf::Vector2f posicionInicial)
{
    sprite.setPosition(posicionInicial);
    this->posicionInicial = posicionInicial;
}

void AnimacionParticulaLineal::setPosicionFinal(sf::Vector2f posicionFinal)
{
    this->posicionFinal = posicionFinal;
}

void AnimacionParticulaLineal::setPosicionFinal(float angulo, float longitud)
{
    this->posicionFinal.x = posicionInicial.x + (longitud * std::cos(angulo));
    this->posicionFinal.y = posicionInicial.y + (longitud * std::sin(angulo));
}

void AnimacionParticulaLineal::actualizar(std::list<std::shared_ptr<Animacion>> &nuevasAnimaciones)
{
    sf::Vector2f posicionSiguiente(util::aproximarFloat(sprite.getPosition().x,posicionFinal.x,0.7f),util::aproximarFloat(sprite.getPosition().y,posicionFinal.y,0.7f));
    sprite.setPosition(posicionSiguiente);

    if((1 - util::distanciaEuclidiana(posicionSiguiente,posicionFinal) / util::distanciaEuclidiana(posicionInicial,posicionFinal)) > PROPORCION_RECORRIDO_MENGUAR_PARTICULA_LINEAL)
        sprite.scale(sf::Vector2f(0.8f,0.8f));
}

void AnimacionParticulaLineal::resetear(){
    sprite.setPosition(posicionInicial);
    sprite.setScale(sf::Vector2f(1.f,1.f));
}

void AnimacionParticulaLineal::voltear(){
    // El sprite se escala de forma que el eje X esté invertido. Esto no
    // significa que la escala sea 1 (eso sería con setScale()), sino que
    // la escala actual de multiplica por lo que se indica
    sprite.scale({-1,1});
    posicionFinal.y = -posicionFinal.y;
}

std::vector<Hitbox> AnimacionParticulaLineal::getHitboxes(){
    return std::vector<Hitbox>();
}

bool AnimacionParticulaLineal::haTerminado()
{
    return sprite.getScale().x < UMBRAL_FLOAT && sprite.getScale().y < UMBRAL_FLOAT;
}

std::shared_ptr<Animacion> AnimacionParticulaLineal::clonar(){
    return std::make_shared<AnimacionParticulaLineal>(*this);
}

void AnimacionParticulaLineal::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(sprite,states);
}