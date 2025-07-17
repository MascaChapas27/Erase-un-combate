#include "AnimacionPorFrames.hpp"
#include "AnimacionConGravedad.hpp"
#include "ReproductorDeSonidos.hpp"
#include "Constantes.hpp"
#include "ContenedorDeEfectos.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>
#include "ContenedorDeRecursos.hpp"

AnimacionPorFrames::AnimacionPorFrames(IngredientesAnimacionPorFrames &ingredientes) :
    Animacion(ContenedorDeTexturas::unicaInstancia()->obtener(ingredientes.rutaTextura)) {

    sprite.setTextureRect(sf::IntRect({0,0}, {static_cast<int>(sprite.getTextureRect().size.x/ingredientes.numRectangulos), static_cast<int>(sprite.getTextureRect().size.y)}));
    sprite.setOrigin(ingredientes.origen);
    sprite.setPosition(ingredientes.posicion);

    this->tipoBucle = ingredientes.tipoBucle;
    this->hitboxes = ingredientes.hitboxes;
    this->rectanguloCorrespondiente = ingredientes.rectanguloCorrespondiente;
    this->rutaSonido = ingredientes.rutaSonido;
    this->framesConSonido = ingredientes.framesConSonido;
    this->framesConMovimiento = ingredientes.framesConMovimiento;
    this->framesConEstiramientos = ingredientes.framesConEstiramientos;
    this->framesConAnimaciones = ingredientes.framesConAnimaciones;
    this->repetirSonido = ingredientes.repetirSonido;
    this->tipoSombra = ingredientes.tipoSombra;

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

    if(framesConEstiramientos.count(frameActual)){
        sprite.scale(framesConEstiramientos[frameActual]);
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

sf::Vector2f AnimacionPorFrames::getEstiramientoFrameActual(){
    if(framesConEstiramientos.count(frameActual)) return framesConEstiramientos[frameActual];
    else return sf::Vector2f{1.f,1.f};
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
    // Cuando un rectángulo tiene una hitbox con daño, solo el primer frame de ese rectángulo hará
    // realmente daño. Esto hace que si un rectángulo cn hitbox dañina dura varios frames, solo el
    // primero sea el que cause daño, mientras que los siguientes son para que la animación se vea
    // más bonita y dé tiempo a verla, y así no pase demasiado rápido
    if(frameActual > 0 && rectanguloCorrespondiente[frameActual] == rectanguloCorrespondiente[frameActual-1])
    {
        std::vector<Hitbox> nuevasHitboxes = hitboxes[rectanguloCorrespondiente[frameActual]];

        for(Hitbox& h : nuevasHitboxes){
            if(h.getFuerzaAtaque() != 0){
                h.setFuerzaAtaque(0);
            }
        }

        return nuevasHitboxes;
    }
    else
    {
        return hitboxes[rectanguloCorrespondiente[frameActual]];
    }
}

void AnimacionPorFrames::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(tipoSombra != TipoSombra::SIN_SOMBRA)
    {
        // La sombra tendrá una anchura mayor y una sombra
        // más oscura cuanto más cerca esté el objeto del suelo
        float anchuraSombra = sprite.getTextureRect().size.x/(tipoSombra == TipoSombra::LARGA ? 1.5f : 3.f);
        float alturaSombra = ALTURA_SOMBRA;
        sf::Color colorSombra = sf::Color::Black;

        if(sprite.getPosition().y < ALTURA_SUELO/2)
        {
            anchuraSombra = 0;
            alturaSombra = 0;
            colorSombra = sf::Color::Transparent;
        }
        else
        {
            anchuraSombra *= (sprite.getPosition().y - ALTURA_SUELO/2) / (ALTURA_SUELO/2);
            alturaSombra *= (sprite.getPosition().y - ALTURA_SUELO/2) / (ALTURA_SUELO/2);

            // En el caso de la transparencia del color se divide entre (ALTURA_SUELO/2) para
            // que el rango de transparencia esté entre 0 y 128 más menos para que la sombra
            // sea más transparente y no sea completamente opaca
            colorSombra.a *= (sprite.getPosition().y - ALTURA_SUELO/2) / (ALTURA_SUELO);
        }

        anchuraSombra *= (sprite.getPosition().y/ALTURA_SUELO);

        sf::CircleShape sombra(1.f);
        sombra.setOrigin({1.f,1.f});
        sombra.setScale({anchuraSombra/2.f,alturaSombra/2.f});
        sombra.setFillColor(colorSombra);
        sombra.setPosition({sprite.getPosition().x,ALTURA_SUELO});

        target.draw(sombra,states);
    }

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
