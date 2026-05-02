#include "AnimacionPorFotogramas.hpp"
#include "AnimacionConGravedad.hpp"
#include "ReproductorDeSonidos.hpp"
#include "Constantes.hpp"
#include "ContenedorDeEfectos.hpp"
#include "Configuracion.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>
#include "ContenedorDeRecursos.hpp"

AnimacionPorFotogramas::AnimacionPorFotogramas(IngredientesAnimacionPorFotogramas &ingredientes) :
Animacion(ContenedorDeTexturas::unicaInstancia()->obtener(ingredientes.rutaTextura))
{
    sprite.setTextureRect(sf::IntRect({0,0}, {static_cast<int>(sprite.getTextureRect().size.x/ingredientes.numRectangulos), static_cast<int>(sprite.getTextureRect().size.y)}));
    sprite.setOrigin(ingredientes.origen);
    sprite.setPosition(ingredientes.posicion);

    this->tipoBucle = ingredientes.tipoBucle;
    this->hitboxes = ingredientes.hitboxes;
    this->fotogramaAGrupoDeColision = ingredientes.fotogramaAGrupoDeColision;
    for(int i=0;i<ingredientes.numGruposDeColision;i++){
        this->grupoDeColisionActivo[i] = true;
    }
    this->rectanguloCorrespondiente = ingredientes.rectanguloCorrespondiente;
    this->rutaSonido = ingredientes.rutaSonido;
    this->fotogramasConSonido = ingredientes.fotogramasConSonido;
    this->fotogramasConMovimiento = ingredientes.fotogramasConMovimiento;
    this->fotogramasConEstiramientos = ingredientes.fotogramasConEstiramientos;
    this->fotogramasConAnimaciones = ingredientes.fotogramasConAnimaciones;
    this->repetirSonido = ingredientes.repetirSonido;
    this->tipoSombra = ingredientes.tipoSombra;

    sonidoYaReproducido = false;

    resetear();
}

void AnimacionPorFotogramas::actualizar(std::list<std::shared_ptr<Animacion>> &nuevasAnimaciones)
{

    if(tipoBucle == TipoBucle::NORMAL){
        if(!primerFotograma) fotogramaActual++;
        if(fotogramaActual >= (int)rectanguloCorrespondiente.size()) {
            fotogramaActual = 0;
            if(!repetirSonido) sonidoYaReproducido = true;
        }
    } else if (tipoBucle == TipoBucle::SIN_BUCLE){
        if(fotogramaActual < (int)rectanguloCorrespondiente.size()-1)
            if(!primerFotograma) fotogramaActual++;
    }

    // Ya seguro que no es el primer fotograma
    primerFotograma = false;

    // Se reproduce el sonido si es necesario
    if(!sonidoYaReproducido && fotogramasConSonido.count(fotogramaActual)){
        ReproductorDeSonidos::unicaInstancia()->reproducir(rutaSonido);
    }

    // Se actualizan las nuevas animaciones si es necesario
    if(fotogramasConAnimaciones.count(fotogramaActual)){

        std::shared_ptr<Animacion> anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto(fotogramasConAnimaciones[fotogramaActual].rutaAnimacion);

        sf::Vector2f posicionAnimacion(getPosicionEsqSupIzq());

        posicionAnimacion.x+=fotogramasConAnimaciones[fotogramaActual].posicionInicial.x;
        posicionAnimacion.y+=fotogramasConAnimaciones[fotogramaActual].posicionInicial.y;

        anim->setPosicion(posicionAnimacion);

        if(std::string(typeid(*anim).name()) == "AnimacionConGravedad"){
            ((AnimacionConGravedad*)(anim.get()))->setVelocidad(fotogramasConAnimaciones[fotogramaActual].velocidadInicial);
        }

        if(fotogramasConAnimaciones.at(fotogramaActual).necesitaVoltearse){
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

    if(fotogramasConEstiramientos.count(fotogramaActual)){
        sprite.scale(fotogramasConEstiramientos[fotogramaActual]);
    }

    sprite.setTextureRect(sf::IntRect({rectanguloCorrespondiente[fotogramaActual]*sprite.getTextureRect().size.x,0}, {sprite.getTextureRect().size.x,sprite.getTextureRect().size.y}));
}

void AnimacionPorFotogramas::voltear(){
    // Al escalar el eje X por -1 se le da la vuelta muy guay todo pero no es suficiente
    sprite.scale({-1,1});

    // También hay que voltear las hitboxes
    for(auto const &[entero, listaHitboxes] : hitboxes){
        std::vector<Hitbox> nuevaLista;

        for(Hitbox hitbox : listaHitboxes){

            // Esto se explicaría mejor con un dibujito pero bueno
            int derecha = hitbox.getRectangulo().position.x+hitbox.getRectangulo().size.x;

            int nuevaIzquierda = sprite.getTextureRect().size.x - derecha;

            Hitbox nuevaHitbox(sf::IntRect({nuevaIzquierda,hitbox.getRectangulo().position.y},{hitbox.getRectangulo().size.x,hitbox.getRectangulo().size.y}),hitbox.getFuerzaAtaque(),hitbox.esAtaqueBajo());

            nuevaLista.push_back(nuevaHitbox);
        }

        hitboxes[entero] = nuevaLista;
    }

    // También hay que voltear los movimientos
    for(auto &par : fotogramasConMovimiento){
        par.second.x = -par.second.x;
    }

    // También hay que voltear las animaciones adicionales
    for(auto &par : fotogramasConAnimaciones){
        par.second.necesitaVoltearse = !par.second.necesitaVoltearse;
    }
}

int AnimacionPorFotogramas::getNumeroRectangulo() {
    return rectanguloCorrespondiente[fotogramaActual];
}

int AnimacionPorFotogramas::getNumeroFotograma(){
    return fotogramaActual;
}

sf::Vector2f AnimacionPorFotogramas::getEstiramientoFotogramaActual(){
    if(fotogramasConEstiramientos.count(fotogramaActual)) return fotogramasConEstiramientos[fotogramaActual];
    else return sf::Vector2f{1.f,1.f};
}

void AnimacionPorFotogramas::setTipoBucle(TipoBucle tipoBucle){
    this->tipoBucle = tipoBucle;
}

sf::Vector2f AnimacionPorFotogramas::getMovimientoFotogramaActual(){
    return fotogramasConMovimiento[fotogramaActual];
}

bool AnimacionPorFotogramas::haTerminado(){
    return tipoBucle == TipoBucle::SIN_BUCLE && fotogramaActual == (int)rectanguloCorrespondiente.size()-1;
}

void AnimacionPorFotogramas::resetear(){
    fotogramaActual = 0;
    sonidoYaReproducido = false;
    primerFotograma = true;

    for(int i=0;i<grupoDeColisionActivo.size();i++){
        grupoDeColisionActivo[i] = true;
    }

    sprite.setTextureRect(sf::IntRect({rectanguloCorrespondiente[fotogramaActual]*sprite.getTextureRect().size.x,0}, {sprite.getTextureRect().size.x,sprite.getTextureRect().size.y}));
}

std::shared_ptr<Animacion> AnimacionPorFotogramas::clonar(){
    return std::make_shared<AnimacionPorFotogramas>(*this);
}

void AnimacionPorFotogramas::chocar()
{
    int grupoDeColision = fotogramaAGrupoDeColision[fotogramaActual];

    grupoDeColisionActivo[grupoDeColision] = false;
}

std::vector<Hitbox> AnimacionPorFotogramas::getHitboxes(){

    if(!grupoDeColisionActivo[fotogramaAGrupoDeColision[fotogramaActual]])
    {
        std::vector<Hitbox> nuevasHitboxes = hitboxes[rectanguloCorrespondiente[fotogramaActual]];

        for(Hitbox& h : nuevasHitboxes){
            if(h.getFuerzaAtaque() != 0){
                h.setFuerzaAtaque(0);
            }
        }

        return nuevasHitboxes;
    }
    else
    {
        return hitboxes[rectanguloCorrespondiente[fotogramaActual]];
    }
}

void AnimacionPorFotogramas::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    
    // El shader usado se guarda porque la sombra no debería usarlo,
    // solo el sprite de la propia animación
    const sf::Shader *shader = states.shader;
    
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

            // En el caso de la transparencia del color se divide entre ALTURA_SUELO para
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

        states.shader = nullptr;
        target.draw(sombra,states);
    }

    states.shader = shader;
    target.draw(sprite,states);

    // El shader tampoco se usa para los rectángulos de las hitboxes del modo debug
    states.shader = nullptr;

    if(Configuracion::unicaInstancia()->isHitboxesVisibles()){
        for(Hitbox h : hitboxes.at(rectanguloCorrespondiente.at(fotogramaActual))){
            // Aquí hay que hacer un trucazo porque el borde se dibuja por fuera de la hitbox,
            // por lo que se dibuja la hitbox ligeramente más pequeña y se reposiciona para que
            // el borde forme parte de la hitbox como debe ser
            sf::RectangleShape rectanguloHitbox(sf::Vector2f(h.getRectangulo().size.x-2,h.getRectangulo().size.y-2));
            rectanguloHitbox.setPosition({(float)h.getRectangulo().position.x+1,(float)h.getRectangulo().position.y+1});
            rectanguloHitbox.move(getPosicionEsqSupIzq());
            rectanguloHitbox.setFillColor(sf::Color::Transparent);
            rectanguloHitbox.setOutlineThickness(1);
            rectanguloHitbox.setOutlineColor(h.getFuerzaAtaque() > 0 ? HITBOX_ATAQUE_COLOR : HITBOX_NO_ATAQUE_COLOR);
            target.draw(rectanguloHitbox,states);
        }
    }
}
