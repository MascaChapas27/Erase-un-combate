#include "AnimacionDesvaneciente.hpp"

AnimacionDesvaneciente::AnimacionDesvaneciente(sf::Texture &textura, float escalado) :
    Animacion(textura), escalado(escalado) {

    sprite.setOrigin(sf::Vector2f(sprite.getTextureRect().size/2));
}

void AnimacionDesvaneciente::actualizar(std::list<std::shared_ptr<Animacion>> &nuevasAnimaciones){
    sprite.setScale(sprite.getScale()*0.95f + sf::Vector2f(escalado,escalado)*0.05f);
    sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, sprite.getColor().a*0.95f + 0*0.05f));
}

void AnimacionDesvaneciente::resetear(){
    sprite.setScale(sf::Vector2f(1.f,1.f));
    sprite.setColor(sf::Color::White);
}

void AnimacionDesvaneciente::voltear(){
    sprite.setScale(sf::Vector2f(sprite.getScale().x*-1,sprite.getScale().y));
}

std::vector<Hitbox> AnimacionDesvaneciente::getHitboxes(){
    return std::vector<Hitbox>();
}

bool AnimacionDesvaneciente::haTerminado(){
    return sprite.getColor().a == 0;
}

std::shared_ptr<Animacion> AnimacionDesvaneciente::clonar(){
    return std::make_shared<AnimacionDesvaneciente>(*this);
}