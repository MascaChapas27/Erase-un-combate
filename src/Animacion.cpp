#include "Animacion.hpp"
#include "Constantes.hpp"

Animacion::Animacion(sf::Texture &textura) : sprite(textura) {

}

void Animacion::setPosicion(float x, float y){
    sprite.setPosition({x,y});
}

void Animacion::setPosicion(sf::Vector2f posicion){
    setPosicion(posicion.x,posicion.y);
}

void Animacion::setRotacion(double angulo){
    sprite.setRotation(sf::degrees(angulo));
}

void Animacion::setColor(sf::Color color){
    sprite.setColor(color);
}

void Animacion::setEscala(const sf::Vector2f& escala)
{
    sprite.setScale(escala);
}

sf::Vector2f Animacion::getPosicion(){
    return sprite.getPosition();
}

sf::Vector2f Animacion::getPosicionEsqSupIzq() const{
    return sf::Vector2f(sprite.getPosition().x-sprite.getOrigin().x,sprite.getPosition().y-sprite.getOrigin().y);
}

void Animacion::mover(float x, float y){
    sprite.move({x,y});
}

void Animacion::mover(sf::Vector2f diferencia){
    mover(diferencia.x,diferencia.y);
}

void Animacion::chocar(Hitbox hitbox){
    // Solo las animaciones con gravedad necesitan saber que chocan
}

void Animacion::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(sprite,states);
}
