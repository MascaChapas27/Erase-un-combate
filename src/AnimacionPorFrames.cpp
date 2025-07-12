#include "AnimacionPorFrames.hpp"
#include "AnimacionConGravedad.hpp"
#include "ReproductorDeSonidos.hpp"
#include "Constantes.hpp"
#include "ContenedorDeEfectos.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>

AnimacionPorFrames::AnimacionPorFrames(IngredientesAnimacionPorFrames &ingredientes) :
    Animacion(ingredientes.textura) {

    sprite.setTextureRect(sf::IntRect({0,0}, {static_cast<int>(ingredientes.textura.getSize().x/ingredientes.numRectangulos), static_cast<int>(ingredientes.textura.getSize().y)}));
    sprite.setOrigin(ingredientes.origen);
    sprite.setPosition(ingredientes.posicion);

    this->tipoBucle = ingredientes.tipoBucle;
    this->hitboxes = ingredientes.hitboxes;
    this->rectanguloCorrespondiente = ingredientes.rectanguloCorrespondiente;
    this->rutaSonido = ingredientes.rutaSonido;
    this->framesConSonido = ingredientes.framesConSonido;
    this->framesConMovimiento = ingredientes.framesConMovimiento;
    this->framesConAnimaciones = ingredientes.framesConAnimaciones;
    this->repetirSonido = ingredientes.repetirSonido;

    sonidoYaReproducido = false;

    resetear();
}

void AnimacionPorFrames::actualizar(std::list<std::shared_ptr<Animacion>> &nuevasAnimaciones) {

    if(tipoBucle == TipoBucle::NORMAL){
        if(!primerFrame) frameActual++;
        if(frameActual >= (int)rectanguloCorrespondiente.size()) {
            frameActual = 0;
            if(!repetirSonido) sonidoYaReproducido = true;
        }
    } else if (tipoBucle == TipoBucle::SIN_BUCLE){
        if(frameActual < (int)rectanguloCorrespondiente.size()-1)
            if(!primerFrame) frameActual++;
    }

    // Ya seguro que no es el primer frame
    primerFrame = false;

    // Se reproduce el sonido si es necesario
    if(!sonidoYaReproducido && framesConSonido.count(frameActual)){
        ReproductorDeSonidos::unicaInstancia()->reproducir(rutaSonido);
    }

    // Se actualizan las nuevas animaciones si es necesario
    if(framesConAnimaciones.count(frameActual)){

        std::shared_ptr<Animacion> anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto(framesConAnimaciones[frameActual].rutaAnimacion);

        sf::Vector2f posicionAnimacion(getPosicionEsqSupIzq());

        posicionAnimacion.x+=framesConAnimaciones[frameActual].posicionInicial.x;
        posicionAnimacion.y+=framesConAnimaciones[frameActual].posicionInicial.y;

        anim->setPosicion(posicionAnimacion);

        if(std::string(typeid(*anim).name()) == "AnimacionConGravedad"){
            ((AnimacionConGravedad*)(anim.get()))->setVelocidad(framesConAnimaciones[frameActual].velocidadInicial);
        }

        if(framesConAnimaciones.at(frameActual).necesitaVoltearse){
            // En este caso hay que usar la posición actual del sprite, al contrario
            // que las hitboxes, que usan una posición relativa al sprite que luego se
            // actualiza. Aquí, la posición es relativa a la ventana, por lo que hay que
            // tener en cuenta dónde está ahora mismo el sprite en la ventana
            int puntoCentral = sprite.getPosition().x;
            posicionAnimacion.x = posicionAnimacion.x - (posicionAnimacion.x-puntoCentral)*2;
            anim->setPosicion(posicionAnimacion);

            anim->voltear();
        }

        nuevasAnimaciones.push_back(anim);
    }

    sprite.setTextureRect(sf::IntRect({rectanguloCorrespondiente[frameActual]*sprite.getTextureRect().size.x,0}, {sprite.getTextureRect().size.x,sprite.getTextureRect().size.y}));
}

void AnimacionPorFrames::voltear(){
    // Al escalar el eje X por -1 se le da la vuelta muy guay todo pero no es suficiente
    sprite.scale({-1,1});

    // También hay que voltear las hitboxes
    for(auto const &[entero, listaHitboxes] : hitboxes){
        std::vector<Hitbox> nuevaLista;

        for(Hitbox hitbox : listaHitboxes){

            // Esto se explicaría mejor con un dibujito pero bueno
            int puntoCentral = sprite.getTextureRect().size.x/2;

            int derecha = hitbox.getRectangulo().position.x+hitbox.getRectangulo().size.x;

            int nuevaIzquierda = derecha - (derecha-puntoCentral)*2;

            Hitbox nuevaHitbox(sf::IntRect({nuevaIzquierda,hitbox.getRectangulo().position.y},{hitbox.getRectangulo().size.x,hitbox.getRectangulo().size.y}),hitbox.getFuerzaAtaque(),hitbox.esAtaqueBajo());

            nuevaLista.push_back(nuevaHitbox);
        }

        hitboxes[entero] = nuevaLista;
    }

    // También hay que voltear los movimientos
    for(auto &par : framesConMovimiento){
        par.second.x = -par.second.x;
    }

    // También hay que voltear las animaciones adicionales
    for(auto &par : framesConAnimaciones){
        par.second.necesitaVoltearse = !par.second.necesitaVoltearse;
    }
}

int AnimacionPorFrames::getNumeroRectangulo() {
    return rectanguloCorrespondiente[frameActual];
}

int AnimacionPorFrames::getNumeroFrame(){
    return frameActual;
}

void AnimacionPorFrames::setTipoBucle(TipoBucle tipoBucle){
    this->tipoBucle = tipoBucle;
}

sf::Vector2f AnimacionPorFrames::getMovimientoFrameActual(){
    return framesConMovimiento[frameActual];
}

bool AnimacionPorFrames::haTerminado(){
    return tipoBucle == TipoBucle::SIN_BUCLE && frameActual == (int)rectanguloCorrespondiente.size()-1;
}

void AnimacionPorFrames::resetear(){
    frameActual = 0;
    sonidoYaReproducido = false;
    primerFrame = true;
}

std::shared_ptr<Animacion> AnimacionPorFrames::clonar(){
    return std::make_shared<AnimacionPorFrames>(*this);
}

std::vector<Hitbox> AnimacionPorFrames::getHitboxes(){
    return hitboxes[rectanguloCorrespondiente[frameActual]];
}

void AnimacionPorFrames::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(sprite,states);

    if(DEBUG){
        for(Hitbox h : hitboxes.at(rectanguloCorrespondiente.at(frameActual))){
            sf::RectangleShape rectanguloHitbox(sf::Vector2f(h.getRectangulo().size.x,h.getRectangulo().size.y));
            rectanguloHitbox.setPosition({(float)h.getRectangulo().position.x,(float)h.getRectangulo().position.y});
            rectanguloHitbox.move(getPosicionEsqSupIzq());
            rectanguloHitbox.setFillColor(sf::Color::Transparent);
            rectanguloHitbox.setOutlineThickness(1);
            rectanguloHitbox.setOutlineColor(h.getFuerzaAtaque() > 0 ? HITBOX_ATAQUE_COLOR : HITBOX_NO_ATAQUE_COLOR);
            target.draw(rectanguloHitbox,states);
        }
    }
}
