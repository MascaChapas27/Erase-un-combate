#include "AnimacionAgrandable.hpp"
#include "Constantes.hpp"
#include "ReproductorDeSonidos.hpp"
#include <iostream>

AnimacionAgrandable::AnimacionAgrandable(int framesEspera, sf::Texture& textura, std::string rutaSonido) : 
    Animacion(textura), escalaActual(0.0f), seEstaAgrandando(true), framesEspera(framesEspera), contadorEspera(0) {
    
    sprite.setOrigin({(float)sprite.getTextureRect().size.x / 2, (float)sprite.getTextureRect().size.y / 2});
    sprite.setScale({0.f,0.f});

    this->rutaSonido = rutaSonido;
}

void AnimacionAgrandable::actualizar(std::list<std::shared_ptr<Animacion>> &nuevasAnimaciones){

    // Este objeto tiene tres posibles estados:

    // 1: Agrandándose. El sprite crece en tamaño hasta obtener
    // el 100% del tamaño original
    if(seEstaAgrandando){

        // Adicionalmente, si es la primera vez que se actualiza, se
        // reproduce el efecto de sonido asociado
        if(std::abs(escalaActual - 0.0f) < UMBRAL_FLOAT)
            ReproductorDeSonidos::unicaInstancia()->reproducir(rutaSonido);

        escalaActual = escalaActual*0.7f + 1.0f*0.3f;

        if(std::abs(escalaActual - 1.0f) < UMBRAL_FLOAT) {
            // Se ha hecho grande del todo
            escalaActual = 1.0f;
            seEstaAgrandando = false;
        }
        sprite.setScale({escalaActual,escalaActual});

    }
    
    // 2: Manteniéndose en un tamaño fijo. El sprite ha crecido hasta
    // conseguir una escala de 1 (tamaño original) y se mantiene así
    // durante unos frames
    else if (contadorEspera < framesEspera){
        contadorEspera++;
    }

    // 3: Disminuyendo. El sprite ha estado el tiempo que tenía que estar
    // sin cambiar de tamaño y ahora le toca hacerse más pequeño
    else if (framesEspera != -1){
        escalaActual-=(1/(escalaActual*30.f));
        if(escalaActual <= 0.0f) escalaActual = 0.0f;
        sprite.setScale({escalaActual,escalaActual});
    }
}

void AnimacionAgrandable::resetear(){
    sprite.setScale({0.f,0.f});
    escalaActual = 0;
    seEstaAgrandando = true;
    contadorEspera = 0;
}

void AnimacionAgrandable::voltear(){
    // El sprite se escala de forma que el eje X esté invertido. Esto no
    // significa que la escala sea 1 (eso sería con setScale()), sino que
    // la escala actual de multiplica por lo que se indica
    sprite.scale({-1,1});
}

std::list<Hitbox> AnimacionAgrandable::getHitboxes(){
    return std::list<Hitbox>();
}

bool AnimacionAgrandable::haTerminado(){
    if(framesEspera == -1) return !seEstaAgrandando && !ReproductorDeSonidos::unicaInstancia()->estaReproduciendo(rutaSonido);
    else return !seEstaAgrandando && escalaActual == 0;
}

bool AnimacionAgrandable::haTerminadoDeAgrandarse(){
    return !seEstaAgrandando;
}

std::shared_ptr<Animacion> AnimacionAgrandable::clonar(){
    return std::make_shared<AnimacionAgrandable>(*this);
}

void AnimacionAgrandable::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(sprite,states);
}