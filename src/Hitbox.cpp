#include "Hitbox.hpp"

Hitbox::Hitbox(sf::IntRect rectangulo, int fuerzaAtaque, bool bajo){
    this->rectangulo = rectangulo;
    this->fuerzaAtaque = fuerzaAtaque;
    this->bajo = bajo;
}

sf::IntRect Hitbox::getRectangulo() const{
    return rectangulo;
}

int Hitbox::getFuerzaAtaque() const{
    return fuerzaAtaque;
}

bool Hitbox::esAtaqueBajo() const{
    return bajo;
}
