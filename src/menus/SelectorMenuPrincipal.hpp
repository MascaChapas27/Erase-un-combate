#pragma once

#include "Animacion.hpp"
#include "SpriteDeseado.hpp"
#include "Enums.hpp"

/*
    Esta es una clase auxiliar para guardar los selectores para
    cada uno de los sitios a los que puedes ir desde el menú principal
*/
class SelectorMenuPrincipal : public sf::Drawable {
    private:
        // El sprite asociado al selector
        SpriteDeseado spriteSelector;

        // Indica a qué selección hace referencia este selector
        TipoSelectorMenuPrincipal tipoSelector;
        
        // La posición relativa a la que se encuentra este selector.
        // Por ejemplo, supongamos que este es el primer selector del vector
        // de selectores del menú principal (posición cero). Si el jugador acaba
        // de abrir el juego, este será el selector elegido por defecto, y su
        // posición relativa es 0. Si el jugador baja dos selectores más abajo,
        // ahora la posición relativa es 2 (se encuentra dos posiciones arriba del
        // selector en el que se encuentra el jugador)
        int posicionRelativa;
    
    public:
        // Se crea el selector en base a su textura para el sprite y el tipo de
        // selección al que hace referencia
        SelectorMenuPrincipal(sf::Texture& texturaSelector, float factorPrimero, TipoSelectorMenuPrincipal tipoSelector);

        // Se reestablece el selector a su estado inicial
        void resetear();

        // Se elige el selector, así que sale un brillito y nos vamos al menú seleccionado.
        // Solo se puede elegir el selector si estamos justo encima, es decir,
        // nada de seleccionarlo a mitad de camino porque queda raro. Devuelve false
        // si estamos a mitad de camino entre un selector y otro y por lo tanto
        // no ha sido posible seleccionarlo, y true si sí que se ha seleccionado
        bool seleccionar(std::list<std::shared_ptr<Animacion>>& nuevasAnimaciones);

        // Se actualiza la posición y la transparencia del selector y su fondo
        void actualizar();

        // Permite dibujar el selector
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        // Establece el valor de la posición relativa y actualiza la posición,
        // el color y la escala deseados
        void setPosicionRelativa(int posicionRelativa);

        // Devuelve el tipo de selector
        TipoSelectorMenuPrincipal getTipoSelector()
        {
            return tipoSelector;
        }

        // Devuelve una referencia al sprite del selector
        sf::Sprite& getSprite();
};