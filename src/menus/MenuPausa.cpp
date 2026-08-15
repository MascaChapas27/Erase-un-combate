#include "MenuPausa.hpp"

#include "VentanaPrincipal.hpp"
#include <SFML/Graphics.hpp>

// La instancia es nula al principio
MenuPausa * MenuPausa::menuPausa = nullptr;

MenuPausa * MenuPausa::unicaInstancia()
{
    if(menuPausa == nullptr)
        menuPausa = new MenuPausa();
    return menuPausa;
}

MenuPausa::~MenuPausa()
{
    if(menuPausa != nullptr) delete menuPausa;
}

MenuPausa::MenuPausa()
{

}

void comenzar()
{
    // En esta textura
    sf::Texture texturaVentana(VentanaPrincipal::unicaInstancia()->c)
}