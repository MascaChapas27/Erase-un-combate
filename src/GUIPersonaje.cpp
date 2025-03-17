#include "GUIPersonaje.hpp"
#include "Constantes.hpp"
#include "ContenedorDeRecursos.hpp"

GUIPersonaje::GUIPersonaje(Personaje &personaje, bool parteIzquierda) : personaje(personaje), parteIzquierda(parteIzquierda),
                                                                        spriteNombre(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/personajes/" + personaje.nombre + "/nombre.png")),
                                                                        spritePortrait(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/personajes/" + personaje.nombre + "/portrait.png")),
                                                                        vibracion(0), contadorVibracion(CONTADOR_VIBRACION_MAX),
                                                                        spritePrincipalBase(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/base-izquierda-0.png")),
                                                                        spritePrincipalFrente(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/frente-izquierda.png")),
                                                                        rondasGanadas(0)
{

    rectanguloVidaReal.setSize(TAMANO_BARRA_VIDA);
    rectanguloVidaReal.setFillColor(COLOR_BARRA_VIDA_REAL);

    rectanguloVidaAtrasada.setSize(TAMANO_BARRA_VIDA);
    rectanguloVidaAtrasada.setFillColor(COLOR_BARRA_VIDA_ATRASADA);

    rectanguloSuper.setSize(sf::Vector2f(0, TAMANO_BARRA_SUPER.y));
    rectanguloSuper.setFillColor(COLOR_BARRA_SUPER);

    contadorVidaAtrasado = MAX_PUNTOS_DE_VIDA;
    retrasoContadorVidaAtrasado = 0;

    spritePortrait.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/personajes/" + personaje.nombre + "/portrait.png"));

    spriteNombre.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/personajes/" + personaje.nombre + "/nombre.png"));

    if (parteIzquierda)
    {
        rectanguloVidaReal.setPosition(POSICION_GUI_IZQUIERDA + POSICION_BARRA_VIDA_IZQUIERDA);

        rectanguloVidaAtrasada.setPosition(POSICION_GUI_IZQUIERDA + POSICION_BARRA_VIDA_IZQUIERDA);

        rectanguloSuper.setPosition(POSICION_GUI_IZQUIERDA + POSICION_BARRA_SUPER_IZQUIERDA);

        spritePortrait.setPosition(POSICION_GUI_IZQUIERDA + POSICION_PORTRAIT_IZQUIERDA);

        spriteNombre.setPosition(POSICION_GUI_IZQUIERDA + POSICION_NOMBRE_IZQUIERDA);

        spritePrincipalBase.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/base-izquierda-0.png"));
        spritePrincipalFrente.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/frente-izquierda.png"));
        spritePrincipalBase.setPosition(POSICION_GUI_IZQUIERDA);
        spritePrincipalFrente.setPosition(POSICION_GUI_IZQUIERDA);
    }
    else
    {
        sf::Vector2f posicionBarraVida(POSICION_GUI_IZQUIERDA + POSICION_BARRA_VIDA_IZQUIERDA);
        posicionBarraVida.x = VENTANA_ANCHURA - posicionBarraVida.x - rectanguloVidaReal.getSize().x;
        rectanguloVidaReal.setPosition(posicionBarraVida);
        rectanguloVidaAtrasada.setPosition(posicionBarraVida);

        sf::Vector2f posicionBarraSuper(POSICION_GUI_IZQUIERDA + POSICION_BARRA_SUPER_IZQUIERDA);
        posicionBarraSuper.x = VENTANA_ANCHURA - posicionBarraSuper.x - rectanguloSuper.getSize().x;
        rectanguloSuper.setPosition(posicionBarraSuper);

        // Al tener que darle la vuelta el origen pasa a ser la esquina superior derecha en
        // vez de la esquina superior izquierda así que hay que cambiar un poco esto
        sf::Vector2f posicionPortrait(POSICION_GUI_IZQUIERDA + POSICION_PORTRAIT_IZQUIERDA);
        posicionPortrait.x = VENTANA_ANCHURA - posicionPortrait.x;
        spritePortrait.setScale({-1.f, 1.f});
        spritePortrait.setPosition(posicionPortrait);

        sf::Vector2f posicionNombre(POSICION_GUI_IZQUIERDA + POSICION_NOMBRE_IZQUIERDA);
        posicionNombre.x = VENTANA_ANCHURA - posicionNombre.x - spriteNombre.getTextureRect().size.x;
        spriteNombre.setPosition(posicionNombre);

        spritePrincipalBase.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/base-derecha-0.png"));
        spritePrincipalFrente.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/frente-derecha.png"));

        sf::Vector2f posicionGUI(POSICION_GUI_IZQUIERDA);
        posicionGUI.x = VENTANA_ANCHURA - posicionGUI.x - spritePrincipalBase.getTextureRect().size.x + 2; // 2 píxeles de la sombra que no cuentan
        spritePrincipalBase.setPosition(posicionGUI);
        spritePrincipalFrente.setPosition(posicionGUI);
    }
}

void GUIPersonaje::actualizar()
{

    // Se alterna la vibración lentamente
    if (--contadorVibracion <= 0)
    {
        if (vibracion > 0)
        {
            vibracion = -vibracion;
        }
        else if (vibracion < 0)
        {
            vibracion = -vibracion;
            vibracion--;
        }
        contadorVibracion = CONTADOR_VIBRACION_MAX;
    }

    // Se actualiza el valor de la vibración si se recibe un golpe
    if (abs(vibracion) < contadorVidaAtrasado - personaje.puntosDeVida)
    {
        vibracion = contadorVidaAtrasado - personaje.puntosDeVida;
    }

    // Se cambia el color de la base de la GUI según la vibración
    int colorVibracion = 255 - std::abs(vibracion) * 10;
    if (colorVibracion < 0)
        colorVibracion = 0;
    spritePrincipalBase.setColor(sf::Color(255, colorVibracion, colorVibracion));

    if (retrasoContadorVidaAtrasado == 0)
    {
        if (personaje.puntosDeVida > contadorVidaAtrasado)
        {
            contadorVidaAtrasado++;
            retrasoContadorVidaAtrasado = MAX_RETRASO_MEDIDOR_VIDA_ATRASADO;
        }
        else if (personaje.puntosDeVida < contadorVidaAtrasado)
        {
            contadorVidaAtrasado--;
            retrasoContadorVidaAtrasado = MAX_RETRASO_MEDIDOR_VIDA_ATRASADO;
        }
    }
    else
    {
        retrasoContadorVidaAtrasado--;
    }

    rectanguloVidaReal.setSize(sf::Vector2f(((float)personaje.puntosDeVida / MAX_PUNTOS_DE_VIDA) * TAMANO_BARRA_VIDA.x, rectanguloVidaReal.getSize().y));
    rectanguloVidaAtrasada.setSize(sf::Vector2f(((float)contadorVidaAtrasado / MAX_PUNTOS_DE_VIDA) * TAMANO_BARRA_VIDA.x, rectanguloVidaAtrasada.getSize().y));

    rectanguloSuper.setSize(sf::Vector2f(((float)personaje.medidorSuper / MAX_MEDIDOR_SUPER) * TAMANO_BARRA_SUPER.x, rectanguloSuper.getSize().y));

    if (!parteIzquierda)
    {
        sf::Vector2f posicionBarraVidaReal(POSICION_GUI_IZQUIERDA + POSICION_BARRA_VIDA_IZQUIERDA);
        posicionBarraVidaReal.x = VENTANA_ANCHURA - posicionBarraVidaReal.x - rectanguloVidaReal.getSize().x;
        rectanguloVidaReal.setPosition(posicionBarraVidaReal);

        sf::Vector2f posicionBarraVidaAtrasada(POSICION_GUI_IZQUIERDA + POSICION_BARRA_VIDA_IZQUIERDA);
        posicionBarraVidaAtrasada.x = VENTANA_ANCHURA - posicionBarraVidaAtrasada.x - rectanguloVidaAtrasada.getSize().x;
        rectanguloVidaAtrasada.setPosition(posicionBarraVidaAtrasada);

        sf::Vector2f posicionBarraSuper(POSICION_GUI_IZQUIERDA + POSICION_BARRA_SUPER_IZQUIERDA);
        posicionBarraSuper.x = VENTANA_ANCHURA - posicionBarraSuper.x - rectanguloSuper.getSize().x;
        rectanguloSuper.setPosition(posicionBarraSuper);
    }
}

void GUIPersonaje::ganarRonda()
{
    rondasGanadas++;
    if (parteIzquierda)
        spritePrincipalBase.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/base-izquierda-" + std::to_string(rondasGanadas) + ".png"));
    else
        spritePrincipalBase.setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/base-derecha-" + std::to_string(rondasGanadas) + ".png"));
}

int GUIPersonaje::getRondasGanadas()
{
    return rondasGanadas;
}

void GUIPersonaje::restablecerVida()
{
    vibracion = 0;
    contadorVibracion = CONTADOR_VIBRACION_MAX;

    contadorVidaAtrasado = MAX_PUNTOS_DE_VIDA;
    retrasoContadorVidaAtrasado = 0;
}

void GUIPersonaje::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Transform mover;
    mover.translate({0.f, (float)vibracion});
    states.transform *= mover;

    target.draw(spritePrincipalBase, states);
    target.draw(rectanguloVidaAtrasada, states);
    target.draw(rectanguloVidaReal, states);
    target.draw(rectanguloSuper, states);
    target.draw(spriteNombre, states);
    target.draw(spritePortrait, states);
    target.draw(spritePrincipalFrente, states);
}