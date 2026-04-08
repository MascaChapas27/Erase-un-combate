#include "FondoPersonaje.hpp"
#include "Constantes.hpp"
#include "Utilidades.hpp"
#include "ContenedorDeRecursos.hpp"

#include <algorithm>

FondoPersonaje::FondoPersonaje(const sf::Texture& texturaFondo, const std::string& nombrePersonaje, Jugador jugador, int posicionRelativa) :
spriteFondo(texturaFondo), nombrePersonaje(nombrePersonaje), posicionRelativa(posicionRelativa), jugador(jugador), seleccionado(false)
{
    if(jugador == Jugador::JUGADOR2)
    {
        spriteFondo.setScale({-1.f,1.f});
        spriteFondo.setOrigin({static_cast<float>(spriteFondo.getTextureRect().size.x),0.f});
    }

    if (!shader.loadFromFile("shaders/blendColor.frag", sf::Shader::Type::Fragment))
    {
        Bitacora::unicaInstancia()->escribir("ERROR: no se pudo cargar el shader");
        exit(EXIT_FAILURE);
    }

    shader.setUniform("amount", 255.f);
    
    resetear(posicionRelativa);
}

void FondoPersonaje::resetear(int posicionRelativa)
{
    quitarSeleccion();
    
    this->posicionRelativa = posicionRelativa;

    // Se pone el fondo en la posición correcta
    float posicionX = jugador == Jugador::JUGADOR1 ?
                      POSICION_X_FONDO_PERSONAJE_J1 :
                      POSICION_X_FONDO_PERSONAJE_J2;
    
    spriteFondo.setPosition({posicionX + posicionRelativa*DIFERENCIA_POSICION_X_FONDO_PERSONAJE, POSICION_Y_FONDO_PERSONAJE});

    // Se pone el color correcto para el fondo
    sf::Color colorFondo;
    if(posicionRelativa == 0)
        colorFondo = COLOR_FONDO_PERSONAJE_POSICION_RELATIVA_0;
    else
        colorFondo = sf::Color::Transparent;

    spriteFondo.setColor(colorFondo);
}

void FondoPersonaje::actualizar()
{
    sf::Vector2f posicionDeseada;
    sf::Color colorDeseado;

    posicionDeseada.x = jugador == Jugador::JUGADOR1 ?
                        POSICION_X_FONDO_PERSONAJE_J1 :
                        POSICION_X_FONDO_PERSONAJE_J2;
    posicionDeseada.x += posicionRelativa*DIFERENCIA_POSICION_X_FONDO_PERSONAJE;
    posicionDeseada.y = POSICION_Y_FONDO_PERSONAJE;

    if(posicionRelativa == 0)
        colorDeseado = COLOR_FONDO_PERSONAJE_POSICION_RELATIVA_0;
    else
        colorDeseado = sf::Color::Transparent;

    spriteFondo.setPosition(util::aproximarVector2f(spriteFondo.getPosition(),posicionDeseada,0.8f));
    spriteFondo.setColor(util::aproximarColor(spriteFondo.getColor(),colorDeseado,0.8f));
}

void FondoPersonaje::seleccionar()
{
    // Se actualiza el color y la posición directamente
    if(jugador == Jugador::JUGADOR1)
        spriteFondo.setPosition({POSICION_X_FONDO_PERSONAJE_J1,POSICION_Y_FONDO_PERSONAJE});
    else
        spriteFondo.setPosition({POSICION_X_FONDO_PERSONAJE_J2,POSICION_Y_FONDO_PERSONAJE});

    spriteFondo.setColor(COLOR_FONDO_PERSONAJE_POSICION_RELATIVA_0);

    // El shader hace que el sprite se vuelva blanco
    seleccionado = true;
}

void FondoPersonaje::quitarSeleccion()
{
    seleccionado = false;
}

void FondoPersonaje::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(seleccionado) states.shader = &shader;
    target.draw(spriteFondo,states);
}