#include "FondoMenuPrincipal.hpp"
#include "Constantes.hpp"
#include "Utilidades.hpp"

FondoMenuPrincipal::FondoMenuPrincipal(sf::Texture& texturaFondo, float factorPrimerValor, TipoSelectorMenuPrincipal tipoSelector) :
spriteFondo(texturaFondo,factorPrimerValor), tipoSelector(tipoSelector), posicionRelativa(static_cast<int>(tipoSelector))
{
    resetear();
}

void FondoMenuPrincipal::resetear()
{
    // Se establece la posición relativa en base al tipo de selector (se asume que
    // al resetear los selectores, en el menú principal se pasará a seleccionar
    // el selector 0)
    posicionRelativa = static_cast<int>(tipoSelector);

    // Se coloca el sprite en la posición correcta
    spriteFondo.setPosicionDeseada({0.f, posicionRelativa*DIFERENCIA_POSICION_Y_FONDO_SELECTOR_MENU_PRINCIPAL});
    spriteFondo.actualizarPosicionInmediatamente();

    // Según si la posición relativa es 0 o no, el fondo tendrá un color distinto
    if(posicionRelativa == 0)
        spriteFondo.setColorDeseado(COLOR_FONDO_SELECTOR_MENU_PRINCIPAL_SELECCIONADO);
    else
        spriteFondo.setColorDeseado(COLOR_FONDO_SELECTOR_MENU_PRINCIPAL_SIN_SELECCIONAR);
    
    spriteFondo.actualizarColorInmediatamente();
}

void FondoMenuPrincipal::actualizar()
{
    spriteFondo.actualizar();
}

void FondoMenuPrincipal::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(spriteFondo,states);
}

sf::Sprite& FondoMenuPrincipal::getSprite()
{
    return spriteFondo.getSprite();
}

void FondoMenuPrincipal::setPosicionRelativa(int posicionRelativa)
{
    this->posicionRelativa = posicionRelativa;

    // Se actualiza la posición deseada
    spriteFondo.setPosicionDeseada({0.f, posicionRelativa*DIFERENCIA_POSICION_Y_FONDO_SELECTOR_MENU_PRINCIPAL});

    // Según si la posición relativa es 0 o no, el fondo tendrá un color distinto
    if(posicionRelativa == 0)
        spriteFondo.setColorDeseado(COLOR_FONDO_SELECTOR_MENU_PRINCIPAL_SELECCIONADO);
    else
        spriteFondo.setColorDeseado(COLOR_FONDO_SELECTOR_MENU_PRINCIPAL_SIN_SELECCIONAR);
}