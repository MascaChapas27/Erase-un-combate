#include "SpriteDeseado.hpp"
#include "Utilidades.hpp"

SpriteDeseado::SpriteDeseado(const sf::Texture& textura, float factorPrimerValor) :
sprite(textura),
factorPrimerValor(factorPrimerValor),
posicionDeseada(sprite.getPosition()),
escalaDeseada(sprite.getScale()),
colorDeseado(sprite.getColor())
{

}


sf::Sprite& SpriteDeseado::getSprite()
{
    return sprite;
}


void SpriteDeseado::actualizar()
{
    // Primero, se actualiza la posición
    sf::Vector2f posicionActual = sprite.getPosition();
    sf::Vector2f posicionActualizada = util::aproximarVector2f(posicionActual,posicionDeseada,factorPrimerValor);
    sprite.setPosition(posicionActualizada);

    // Luego, se actualiza la escala
    sf::Vector2f escalaActual = sprite.getScale();
    sf::Vector2f escalaActualizada = util::aproximarVector2f(escalaActual,escalaDeseada,factorPrimerValor);
    sprite.setScale(escalaActualizada);

    // Finalmente, se actualiza el color
    sf::Color colorActual = sprite.getColor();
    sf::Color colorActualizado = util::aproximarColor(colorActual,colorDeseado,factorPrimerValor);
    sprite.setColor(colorActualizado);
}


void SpriteDeseado::actualizarPosicionInmediatamente()
{
    sprite.setPosition(posicionDeseada);
}


void SpriteDeseado::actualizarEscalaInmediatamente()
{
    sprite.setScale(escalaDeseada);
}


void SpriteDeseado::actualizarColorInmediatamente()
{
    sprite.setColor(colorDeseado);
}

void SpriteDeseado::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite,states);
}


void SpriteDeseado::setPosicionDeseada(const sf::Vector2f& posicionDeseada)
{
    this->posicionDeseada = posicionDeseada;
}


void SpriteDeseado::setEscalaDeseada(const sf::Vector2f& escalaDeseada)
{
    this->escalaDeseada = escalaDeseada;
}


void SpriteDeseado::setColorDeseado(const sf::Color& colorDeseado)
{
    this->colorDeseado = colorDeseado;
}


sf::Vector2f SpriteDeseado::getPosicionDeseada()
{
    return posicionDeseada;
}


sf::Vector2f SpriteDeseado::getEscalaDeseada()
{
    return escalaDeseada;
}


sf::Color SpriteDeseado::getColorDeseado()
{
    return colorDeseado;
}