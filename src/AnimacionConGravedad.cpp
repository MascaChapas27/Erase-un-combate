#include "AnimacionConGravedad.hpp"
#include "Constantes.hpp"

AnimacionConGravedad::AnimacionConGravedad(sf::Texture &textura, sf::Vector2f posicion, sf::Vector2f velocidad, double velocidadGiro, std::string rutaSonido) : Animacion(textura)
{
    // El rectángulo será el de la izquierda en un principio
    sf::IntRect rectangulo = sprite.getTextureRect();
    rectangulo.size.x /= 2;
    sprite.setTextureRect(rectangulo);

    this->velocidad = velocidad;
    this->velocidadGiro = velocidadGiro;

    sprite.setOrigin({(float)rectangulo.size.x / 2, (float)rectangulo.size.y / 2});

    sprite.setPosition(posicion);

    this->rutaSonido = rutaSonido;

    resetear();
}

void AnimacionConGravedad::actualizar(std::list<std::shared_ptr<Animacion>> &nuevasAnimaciones)
{
    sprite.move(velocidad);
    sprite.rotate(sf::degrees(velocidadGiro));

    velocidad.y += GRAVEDAD;

    if (sprite.getPosition().y+std::min(sprite.getTextureRect().size.y,sprite.getTextureRect().size.x)/2.0 > VENTANA_ALTURA)
    {
        if(!haChocado) {
            haChocado = true;
            sf::IntRect rectangulo = sprite.getTextureRect();
            rectangulo.position.x = rectangulo.size.x;
            sprite.setTextureRect(rectangulo);
        }

        velocidad.y /= -2;
        if(velocidad.y > -1) velocidad.y = 0;

        velocidadGiro = velocidad.x*2;

        velocidad.x /= 1.2;
        
        sprite.setPosition({sprite.getPosition().x,VENTANA_ALTURA-std::min(sprite.getTextureRect().size.y,sprite.getTextureRect().size.x)/2.f});

    } else if (sprite.getPosition().x+std::min(sprite.getTextureRect().size.y,sprite.getTextureRect().size.x)/2.0 > VENTANA_ANCHURA)
    {
        if(!haChocado) {
            haChocado = true;
            sf::IntRect rectangulo = sprite.getTextureRect();
            rectangulo.position.x = rectangulo.size.x;
            sprite.setTextureRect(rectangulo);
        }

        if(velocidad.x > 0) velocidad.x/=-1.3;
        
        sprite.setPosition({VENTANA_ANCHURA-std::min(sprite.getTextureRect().size.y,sprite.getTextureRect().size.x)/2.f,sprite.getPosition().y});

    } else if (sprite.getPosition().x-std::min(sprite.getTextureRect().size.y,sprite.getTextureRect().size.x)/2.0 < 0)
    {
        if(!haChocado) {
            haChocado = true;
            sf::IntRect rectangulo = sprite.getTextureRect();
            rectangulo.position.x = rectangulo.size.x;
            sprite.setTextureRect(rectangulo);
        }

        if(velocidad.x < 0) velocidad.x/=-1.3;
        
        sprite.setPosition({std::min(sprite.getTextureRect().size.y,sprite.getTextureRect().size.x)/2.f,sprite.getPosition().y});
    }

    if(velocidad.y == 0){
        contadorParpadeo++;
    }

    if(contadorParpadeo >= MAX_CONTADOR_PARPADEO){
        sprite.setColor(sf::Color::Transparent);
    } else {
        uint8_t transparencia(((MAX_CONTADOR_PARPADEO-contadorParpadeo)/(double)MAX_CONTADOR_PARPADEO)*255);
        sprite.setColor(sf::Color(255,255,255,transparencia));
    }
}

void AnimacionConGravedad::resetear()
{
    haChocado = false;
    contadorParpadeo = 0;
}

std::list<Hitbox> AnimacionConGravedad::getHitboxes(){
    if(!haChocado){
        std::list<Hitbox> lista = std::list<Hitbox>();
        if(hitbox.has_value())
            lista.push_back(hitbox.value());
        return lista;
    }
    else return std::list<Hitbox>();
}

bool AnimacionConGravedad::haTerminado(){
    return contadorParpadeo > MAX_CONTADOR_PARPADEO;
}

void AnimacionConGravedad::voltear(){
    // Al escalar el eje X por -1 se le da la vuelta muy guay todo pero no es suficiente
    sprite.scale({-1,1});

    // También hay que voltear la hitbox
    int puntoCentral = sprite.getTextureRect().size.x/2;

    if(hitbox.has_value()){
        int derecha = hitbox.value().getRectangulo().position.x+hitbox.value().getRectangulo().size.x;

        int nuevaIzquierda = derecha - (derecha-puntoCentral)*2;

        Hitbox nuevaHitbox(sf::IntRect({nuevaIzquierda,hitbox.value().getRectangulo().position.y},{hitbox.value().getRectangulo().size.x,hitbox.value().getRectangulo().size.y}),hitbox.value().getFuerzaAtaque(),hitbox.value().esAtaqueBajo());

        hitbox = nuevaHitbox;
    }
    

    // También hay que voltear la velocidad
    velocidad.x = -velocidad.x;
}

void AnimacionConGravedad::setVelocidad(sf::Vector2f velocidad){
    this->velocidad = velocidad;
}

void AnimacionConGravedad::setVelocidadGiro(const double &velocidadGiro){
    this->velocidadGiro = velocidadGiro;
}

void AnimacionConGravedad::setHitbox(Hitbox hitbox){
    this->hitbox = hitbox;
}

std::shared_ptr<Animacion> AnimacionConGravedad::clonar(){
    return std::make_shared<AnimacionConGravedad>(*this);
}

void AnimacionConGravedad::chocar(Hitbox hitbox){
    hitbox.getRectangulo();
}

void AnimacionConGravedad::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    if(contadorParpadeo%(DURACION_PARPADEO*2) < DURACION_PARPADEO) target.draw(sprite,states);

    if(DEBUG && hitbox.has_value()){
        sf::RectangleShape rectanguloHitbox(sf::Vector2f(hitbox.value().getRectangulo().size.x,hitbox.value().getRectangulo().size.y));
        rectanguloHitbox.setPosition({(float)hitbox.value().getRectangulo().position.x,(float)hitbox.value().getRectangulo().position.y});
        rectanguloHitbox.move(getPosicionEsqSupIzq());
        rectanguloHitbox.setFillColor(sf::Color::Transparent);
        rectanguloHitbox.setOutlineThickness(1);
        rectanguloHitbox.setOutlineColor(hitbox.value().getFuerzaAtaque() > 0 ? HITBOX_ATAQUE_COLOR : HITBOX_NO_ATAQUE_COLOR);
        target.draw(rectanguloHitbox,states);
    }
}