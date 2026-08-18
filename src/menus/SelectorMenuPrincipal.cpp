#include "SelectorMenuPrincipal.hpp"
#include "Constantes.hpp"
#include "Utilidades.hpp"
#include "ContenedorDeEfectos.hpp"
#include <cmath>
#include <algorithm>

SelectorMenuPrincipal::SelectorMenuPrincipal(sf::Texture& texturaSelector, float factorPrimero, TipoSelectorMenuPrincipal tipoSelector) :
spriteSelector(texturaSelector, factorPrimero), tipoSelector(tipoSelector), posicionRelativa(static_cast<int>(tipoSelector))
{
    // Se pone el origen en el centro porque mola más
    spriteSelector.getSprite().setOrigin(sf::Vector2f(spriteSelector.getSprite().getTextureRect().size)/2.f);
    resetear();
}

void SelectorMenuPrincipal::resetear()
{
    // Se establece la posición relativa en base al tipo de selector (se asume que
    // al resetear los selectores, en el menú principal se pasará a seleccionar
    // el selector 0)
    posicionRelativa = static_cast<int>(tipoSelector);

    // Se pone el color correcto para el selector
    sf::Color colorSelector = COLOR_SELECTOR_PERSONAJE_POSICION_RELATIVA_0;
    colorSelector.a = std::clamp(colorSelector.a-std::abs(posicionRelativa)*DIFERENCIA_TRANSPARENCIA_SELECTOR_MENU_PRINCIPAL,0,255);
    spriteSelector.setColorDeseado(colorSelector);
    spriteSelector.actualizarColorInmediatamente();

    // Se pone la escala correcta para el selector
    float escalaDeseadaSprite = 1.f - std::abs(posicionRelativa)*DIFERENCIA_ESCALA_SELECTOR_MENU_PRINCIPAL;
    if(escalaDeseadaSprite < 0.f) escalaDeseadaSprite = 0.f;
    spriteSelector.setEscalaDeseada({escalaDeseadaSprite,escalaDeseadaSprite});
    spriteSelector.actualizarEscalaInmediatamente();

    // Se pone el selector en la posición correcta
    spriteSelector.setPosicionDeseada({POSICION_X_SELECTOR_MENU_PRINCIPAL, POSICION_INICIAL_Y_SELECTOR_MENU_PRINCIPAL + posicionRelativa*DIFERENCIA_POSICION_Y_SELECTOR_MENU_PRINCIPAL*std::sqrt(escalaDeseadaSprite)});
    spriteSelector.actualizarPosicionInmediatamente();
}

void SelectorMenuPrincipal::actualizar()
{
    spriteSelector.actualizar();
}

bool SelectorMenuPrincipal::seleccionar(std::list<std::shared_ptr<Animacion>>& nuevasAnimaciones)
{
    // Si el selector aún se está moviendo (y, por tanto, aún tiene un
    // tamaño más pequeño que el que debería), no se puede seleccionar
    if(spriteSelector.getSprite().getScale().x < UMBRAL_ESCALA_SELECCION_SELECTOR_MENU_PRINCIPAL)
        return false;

    std::shared_ptr<Animacion> anim;

    switch(tipoSelector){
        case TipoSelectorMenuPrincipal::MODO_HISTORIA:
            anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("selector-modo-historia-destello");
            break;
        case TipoSelectorMenuPrincipal::BATALLA_VS:
            anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("selector-batalla-vs-destello");
            break;
        case TipoSelectorMenuPrincipal::OPCIONES:
            anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("selector-opciones-destello");
            break;
    }
    
    sf::Vector2f posicionEfecto = spriteSelector.getSprite().getPosition();

    anim->setPosicion(posicionEfecto);
    nuevasAnimaciones.push_back(anim);

    spriteSelector.getSprite().setScale(spriteSelector.getSprite().getScale()/2.f);

    return true;
}

void SelectorMenuPrincipal::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(spriteSelector,states);
}

void SelectorMenuPrincipal::setPosicionRelativa(int posicionRelativa)
{
    this->posicionRelativa = posicionRelativa;

    // Se pone el color correcto para el selector
    sf::Color colorSelector = COLOR_SELECTOR_PERSONAJE_POSICION_RELATIVA_0;
    colorSelector.a = std::clamp(colorSelector.a-std::abs(posicionRelativa)*DIFERENCIA_TRANSPARENCIA_SELECTOR_MENU_PRINCIPAL,0,255);
    spriteSelector.setColorDeseado(colorSelector);

    // Se pone la escala correcta para el selector
    float escalaDeseadaSprite = 1.f - std::abs(posicionRelativa)*DIFERENCIA_ESCALA_SELECTOR_MENU_PRINCIPAL;
    if(escalaDeseadaSprite < 0.f) escalaDeseadaSprite = 0.f;
    spriteSelector.setEscalaDeseada({escalaDeseadaSprite,escalaDeseadaSprite});

    // Se pone el selector en la posición correcta
    spriteSelector.setPosicionDeseada({POSICION_X_SELECTOR_MENU_PRINCIPAL, POSICION_INICIAL_Y_SELECTOR_MENU_PRINCIPAL + posicionRelativa*DIFERENCIA_POSICION_Y_SELECTOR_MENU_PRINCIPAL*std::sqrt(escalaDeseadaSprite)});
}

sf::Sprite& SelectorMenuPrincipal::getSprite()
{
    return spriteSelector.getSprite();
}