#include "SelectorPersonaje.hpp"
#include "Constantes.hpp"
#include "Utilidades.hpp"
#include "ContenedorDeRecursos.hpp"
#include "Animacion.hpp"
#include "ContenedorDeEfectos.hpp"

#include <algorithm>
#include <list>

SelectorPersonaje::SelectorPersonaje(const sf::Texture& texturaSelector, const std::string& nombrePersonaje, Jugador jugador, int posicionRelativa) :
spriteSelector(texturaSelector), spriteNombrePersonaje(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/personajes/"+nombrePersonaje+"/nombre.png")),
bordeCuadrado(sf::Vector2f(spriteSelector.getTextureRect().size)), nombrePersonaje(nombrePersonaje), posicionRelativa(posicionRelativa), jugador(jugador)
{
    // Se pone el origen de los dos sprites en el centro
    spriteSelector.setOrigin(sf::Vector2f(spriteSelector.getTextureRect().size/2));
    spriteNombrePersonaje.setOrigin(sf::Vector2f(spriteNombrePersonaje.getTextureRect().size/2));

    if(jugador == Jugador::JUGADOR1)
    {
        spriteNombrePersonaje.setPosition({POSICION_X_SELECTOR_PERSONAJE_J1,POSICION_Y_NOMBRE_SELECTOR_PERSONAJE});
    }
    else
    {
        // Los selectores del jugador 2 están dados la vuelta
        spriteSelector.setScale({-1.f,1.f});

        spriteNombrePersonaje.setPosition({POSICION_X_SELECTOR_PERSONAJE_J2,POSICION_Y_NOMBRE_SELECTOR_PERSONAJE});
    }

    bordeCuadrado.setOrigin(bordeCuadrado.getSize()/2.f);
    bordeCuadrado.setOutlineColor(sf::Color::White);
    bordeCuadrado.setOutlineThickness(1.f);
    bordeCuadrado.setFillColor(sf::Color::Transparent);

    resetear(posicionRelativa);
}

void SelectorPersonaje::resetear(int posicionRelativa)
{
    this->posicionRelativa = posicionRelativa;

    // Se pone la escala correcta para el sprite
    float escalaDeseadaSprite = 1.f - std::abs(posicionRelativa)*DIFERENCIA_ESCALA_SELECTOR_PERSONAJE;
    if(escalaDeseadaSprite < 0.f) escalaDeseadaSprite = 0.f;

    // Se pone el selector en las posiciones correctas
    float posicionX = jugador == Jugador::JUGADOR1 ?
                      POSICION_X_SELECTOR_PERSONAJE_J1 :
                      POSICION_X_SELECTOR_PERSONAJE_J2;

    spriteSelector.setPosition({posicionX + posicionRelativa*DIFERENCIA_POSICION_X_SELECTOR_PERSONAJE*escalaDeseadaSprite, POSICION_Y_SELECTOR_PERSONAJE});

    // Se pone el color correcto para el selector
    sf::Color colorSelector = COLOR_SELECTOR_PERSONAJE_POSICION_RELATIVA_0;
    colorSelector.a = std::clamp(colorSelector.a-std::abs(posicionRelativa)*DIFERENCIA_TRANSPARENCIA_SELECTOR_PERSONAJE,0,255);
    spriteSelector.setColor(colorSelector);

    if(jugador == Jugador::JUGADOR1)
        spriteSelector.setScale({escalaDeseadaSprite,escalaDeseadaSprite});
    else
        spriteSelector.setScale({-escalaDeseadaSprite,escalaDeseadaSprite});

    bordeCuadrado.setPosition(spriteSelector.getPosition());
    bordeCuadrado.setScale(spriteSelector.getScale());
    bordeCuadrado.setOutlineColor(spriteSelector.getColor());
}

void SelectorPersonaje::actualizar()
{
    sf::Vector2f posicionDeseadaSprite;
    sf::Color colorDeseadoSprite = COLOR_SELECTOR_PERSONAJE_POSICION_RELATIVA_0;
    sf::Vector2f posicionDeseadaFondo;
    sf::Color colorDeseadoFondo;

    float escalaDeseadaSprite = 1.f - std::abs(posicionRelativa)*DIFERENCIA_ESCALA_SELECTOR_PERSONAJE;
    if(escalaDeseadaSprite < 0.f) escalaDeseadaSprite = 0.f;

    posicionDeseadaSprite.x = jugador == Jugador::JUGADOR1 ?
                              POSICION_X_SELECTOR_PERSONAJE_J1 :
                              POSICION_X_SELECTOR_PERSONAJE_J2;
    posicionDeseadaSprite.x += posicionRelativa*DIFERENCIA_POSICION_X_SELECTOR_PERSONAJE*escalaDeseadaSprite;
    posicionDeseadaSprite.y = POSICION_Y_SELECTOR_PERSONAJE;

    colorDeseadoSprite.a = std::clamp(colorDeseadoSprite.a-std::abs(posicionRelativa)*DIFERENCIA_TRANSPARENCIA_SELECTOR_PERSONAJE,0,255);

    spriteSelector.setPosition(util::aproximarVector2f(spriteSelector.getPosition(),posicionDeseadaSprite,0.8f));
    spriteSelector.setColor(util::aproximarColor(spriteSelector.getColor(),colorDeseadoSprite,0.8f));

    if(jugador == Jugador::JUGADOR1)
        spriteSelector.setScale(util::aproximarVector2f(spriteSelector.getScale(),{escalaDeseadaSprite,escalaDeseadaSprite},0.8f));
    else
        spriteSelector.setScale(util::aproximarVector2f(spriteSelector.getScale(),{-escalaDeseadaSprite,escalaDeseadaSprite},0.8f));

    bordeCuadrado.setPosition(spriteSelector.getPosition());
    bordeCuadrado.setScale(spriteSelector.getScale());
    bordeCuadrado.setOutlineColor(spriteSelector.getColor());
}

void SelectorPersonaje::ajustarPosicion()
{
    sf::Vector2f posicionDeseadaSprite;
    sf::Color colorDeseadoSprite = COLOR_SELECTOR_PERSONAJE_POSICION_RELATIVA_0;
    sf::Vector2f posicionDeseadaFondo;
    sf::Color colorDeseadoFondo;

    float escalaDeseadaSprite = 1.f - std::abs(posicionRelativa)*DIFERENCIA_ESCALA_SELECTOR_PERSONAJE;
    if(escalaDeseadaSprite < 0.f) escalaDeseadaSprite = 0.f;

    posicionDeseadaSprite.x = jugador == Jugador::JUGADOR1 ?
                              POSICION_X_SELECTOR_PERSONAJE_J1 :
                              POSICION_X_SELECTOR_PERSONAJE_J2;
    posicionDeseadaSprite.x += posicionRelativa*DIFERENCIA_POSICION_X_SELECTOR_PERSONAJE*escalaDeseadaSprite;
    posicionDeseadaSprite.y = POSICION_Y_SELECTOR_PERSONAJE;

    colorDeseadoSprite.a = std::clamp(colorDeseadoSprite.a-std::abs(posicionRelativa)*DIFERENCIA_TRANSPARENCIA_SELECTOR_PERSONAJE,0,255);

    spriteSelector.setPosition(posicionDeseadaSprite);
    spriteSelector.setColor(colorDeseadoSprite);

    if(jugador == Jugador::JUGADOR1)
        spriteSelector.setScale({escalaDeseadaSprite,escalaDeseadaSprite});
    else
        spriteSelector.setScale({-escalaDeseadaSprite,escalaDeseadaSprite});

    bordeCuadrado.setPosition(spriteSelector.getPosition());
    bordeCuadrado.setScale(spriteSelector.getScale());
    bordeCuadrado.setOutlineColor(spriteSelector.getColor());
}

bool SelectorPersonaje::seleccionar(std::list<std::shared_ptr<Animacion>>& nuevasAnimaciones)
{
    // Si el selector aún se está moviendo (y, por tanto, aún tiene un
    // tamaño más pequeño que el que debería), no se puede seleccionar
    if(std::abs(spriteSelector.getScale().x) < 0.95f || std::abs(spriteSelector.getScale().y) < 0.95f)
        return false;

    std::shared_ptr<Animacion> anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("menu-seleccion-personaje-destello-selector");

    sf::Vector2f posicionEfecto = spriteSelector.getPosition();

    anim->setPosicion(posicionEfecto);
    nuevasAnimaciones.push_back(anim);

    spriteSelector.setScale(spriteSelector.getScale()/2.f);

    return true;
}

void SelectorPersonaje::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(spriteSelector,states);
    target.draw(bordeCuadrado,states);

    if(posicionRelativa == 0)
        target.draw(spriteNombrePersonaje,states);
}