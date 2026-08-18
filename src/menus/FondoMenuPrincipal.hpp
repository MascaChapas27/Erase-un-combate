#pragma once

#include "Animacion.hpp"
#include "SpriteDeseado.hpp"
#include "Enums.hpp"

/*
    Esta es una clase auxiliar para los fondos del menú principal. Cada
    fondo está asociado a un selector
*/
class FondoMenuPrincipal : public sf::Drawable {
    private:
        // El sprite asociado al fondo
        SpriteDeseado spriteFondo;

        // El tipo de selector al que está asociado este fondo
        TipoSelectorMenuPrincipal tipoSelector;
        
        // La posición relativa a la que se encuentra este fondo (explicación
        // completa en el código de la clase SelectorMenuPrincipal)
        int posicionRelativa;
    
    public:
        // Se crea el fondo inicializando el SpriteDeseado y especificando qué
        // tipo de selector es (si es para el menú de opciones, para el modo
        // historia, etc.)
        FondoMenuPrincipal(sf::Texture& texturaFondo, float factorPrimerValor, TipoSelectorMenuPrincipal tipoSelector);

        // Se reestablece el fondo a su estado inicial
        void resetear();

        // Se actualiza la posición y la transparencia del fondo
        void actualizar();

        // Permite dibujar el fondo
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        // Establece el valor de la posición relativa y actualiza la posición
        // y el color deseados del SpriteDeseado
        void setPosicionRelativa(int posicionRelativa);

        // Devuelve una referencia al sprite del fondo
        sf::Sprite& getSprite();
};