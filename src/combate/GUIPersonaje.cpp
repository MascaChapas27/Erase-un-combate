#include "GUIPersonaje.hpp"
#include "Constantes.hpp"
#include "ContenedorDeRecursos.hpp"
#include "ContenedorDeEfectos.hpp"
#include <string>
#include <iostream>

GUIPersonaje::GUIPersonaje(Personaje &personaje, bool parteIzquierda) : personaje(personaje), parteIzquierda(parteIzquierda),
                                                                        spriteNombre(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/personajes/" + personaje.nombre + "/nombre.png")),
                                                                        spritePortrait(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/personajes/" + personaje.nombre + "/portrait.png")),
                                                                        vibracion(0), contadorVibracion(CONTADOR_VIBRACION_MAX),
                                                                        spritePrincipalBase(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/base-izquierda-0.png")),
                                                                        spritePrincipalFrente(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/frente-izquierda.png")),
                                                                        rondasGanadas(0), medidorDeSuperLleno(false), personajeMirandoDerecha(personaje.isMirandoDerecha())
{
    rectanguloVidaReal.setSize(TAMANO_BARRA_VIDA);
    rectanguloVidaReal.setFillColor(COLOR_BARRA_VIDA_REAL);

    rectanguloVidaAtrasada.setSize(TAMANO_BARRA_VIDA);
    rectanguloVidaAtrasada.setFillColor(COLOR_BARRA_VIDA_ATRASADA);

    rectanguloSuper.setSize(sf::Vector2f(0, TAMANO_BARRA_SUPER.y));
    if(parteIzquierda)
        rectanguloSuper.setFillColor(COLOR_BARRA_SUPER_J1);
    else
        rectanguloSuper.setFillColor(COLOR_BARRA_SUPER_J2);

    contadorVidaAtrasado = personaje.getMaxPuntosDeVida();
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

    // Por último, se colocan los sprites del cartelito que te dice las
    // acciones que tienes que hacer para hacer el ataque especial
    if(parteIzquierda)
    {
        spritesEspecial.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/spec-inicio-izquierda.png"));
        spritesEspecial[0].setPosition(POSICION_CARTEL_ESPECIAL_GUI_IZQUIERDA);
    }
    else
    {
        spritesEspecial.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/spec-inicio-derecha.png"));
        spritesEspecial[0].setOrigin({static_cast<float>(spritesEspecial[0].getTextureRect().size.x),0.f});
        spritesEspecial[0].setPosition(POSICION_CARTEL_ESPECIAL_GUI_DERECHA);
    }

    float posicionEjeX;
    float posicionEjeY = spritesEspecial[0].getPosition().y;

    if(parteIzquierda)
        posicionEjeX = spritesEspecial[0].getPosition().x + spritesEspecial[0].getTextureRect().size.x;
    else
        posicionEjeX = spritesEspecial[0].getPosition().x - spritesEspecial[0].getTextureRect().size.x;

    int indice;
    if(parteIzquierda)
        indice = 0;
    else
        indice = personaje.getAccionesAtaqueEspecial().size()-1;

    std::cout << "Preparando GUI para el personaje de la " << (parteIzquierda ? "izquierda" : "derecha") << "\n";

    while(indice >= 0 && indice < personaje.getAccionesAtaqueEspecial().size())
    {
        Accion a = personaje.getAccionesAtaqueEspecial()[indice];

        switch(a)
        {
            case Accion::ARRIBA:
                std::cout << "ARRIBA\n";
                spritesEspecial.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/spec-arriba.png"));
                break;
            
            case Accion::ABAJO:
                std::cout << "ABAJO\n";
                spritesEspecial.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/spec-abajo.png"));
                break;
            
            case Accion::IZQUIERDA:
                std::cout << "IZQUIERDA\n";
                spritesEspecial.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/spec-izquierda.png"));
                break;
            
            case Accion::DERECHA:
                std::cout << "DERECHA\n";
                spritesEspecial.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/spec-derecha.png"));
                break;
            
            case Accion::ATACAR:
                std::cout << "ATACAR\n";
                spritesEspecial.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/spec-atacar.png"));
                break;
            
            default:
                std::string nombreAccion = a == Accion::ESCAPE ? "ESCAPE" : "NADA";
                Bitacora::unicaInstancia()->escribir("Emilio: Señor Juan, ¿puede venir un momento?");
                Bitacora::unicaInstancia()->escribir("Juan Cuesta: ¿Qué pasa, Emilio?");
                Bitacora::unicaInstancia()->escribir("Emilio: No, que estaba aquí haciendo el cartelito este que indica lo que tiene que hacer " + personaje.getNombre() + " para hacer el ataque especial, y me he encontrado con la acción " + nombreAccion + ". ¿Cómo lo ve usted?");
                Bitacora::unicaInstancia()->escribir("Juan Cuesta: Pero bueno, ¿cómo lo voy a ver? Eso es una irregularidad en toda regla, rompe por completo con las mecánicas definidas para este, nuestro juego. Cancela la fabricación del cartel, esto no puede seguir así.");
                exit(EXIT_FAILURE);
                break;
        }

        spritesEspecial.back().setPosition({posicionEjeX,posicionEjeY});
        
        if(parteIzquierda)
        {
            posicionEjeX += spritesEspecial.back().getTextureRect().size.x;
            indice++;
        }
        else
        {
            spritesEspecial.back().setOrigin({static_cast<float>(spritesEspecial.back().getTextureRect().size.x),0.f});
            posicionEjeX -= spritesEspecial.back().getTextureRect().size.x;
            indice--;
        }
    }

    if(parteIzquierda)
        spritesEspecial.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/spec-fin-izquierda.png"));
    else
    {
        spritesEspecial.emplace_back(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/spec-fin-derecha.png"));
        spritesEspecial.back().setOrigin({static_cast<float>(spritesEspecial.back().getTextureRect().size.x),0.f});
    }
    spritesEspecial.back().setPosition({posicionEjeX,posicionEjeY});
}

void GUIPersonaje::actualizar()
{
    // Si el personaje se ha volteado, la GUI también se
    // debería voltear
    if(personaje.isMirandoDerecha() != personajeMirandoDerecha)
    {
        personajeMirandoDerecha = personaje.isMirandoDerecha();
        voltear();
    }

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

    // Se comprueba si el medidor de súper se ha llenado y se
    // crea un efecto de destello
    if(!medidorDeSuperLleno && personaje.medidorSuper == MAX_MEDIDOR_SUPER)
    {
        medidorDeSuperLleno = true;
        std::shared_ptr<Animacion> destelloSuperCargada = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("super-cargada-j" + std::string(personaje.jugador == Jugador::JUGADOR1 ? "1" : "2") + "-destello");
        destelloSuperCargada->setPosicion(spritePrincipalBase.getPosition() + (personaje.jugador == Jugador::JUGADOR1 ? POSICION_DESTELLO_SUPER_CARGADA_J1 : POSICION_DESTELLO_SUPER_CARGADA_J2));
        efectos.push_back(destelloSuperCargada);
    }
    else if (medidorDeSuperLleno && personaje.medidorSuper < MAX_MEDIDOR_SUPER)
    {
        medidorDeSuperLleno = false;
    }

    rectanguloVidaReal.setSize(sf::Vector2f(((float)personaje.puntosDeVida / personaje.getMaxPuntosDeVida()) * TAMANO_BARRA_VIDA.x, rectanguloVidaReal.getSize().y));
    rectanguloVidaAtrasada.setSize(sf::Vector2f(((float)contadorVidaAtrasado / personaje.getMaxPuntosDeVida()) * TAMANO_BARRA_VIDA.x, rectanguloVidaAtrasada.getSize().y));

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

    // Se actualizan los efectos
    for(std::shared_ptr<Animacion>& efecto : efectos)
    {
        efecto->actualizar(efectos);
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

    contadorVidaAtrasado = personaje.getMaxPuntosDeVida();
    retrasoContadorVidaAtrasado = 0;
}

void GUIPersonaje::voltear()
{
    std::cout << "Volteando GUI para el personaje de la " << (parteIzquierda ? "izquierda" : "derecha") << "\n";

    for(int i=0;i<personaje.getAccionesAtaqueEspecial().size();i++)
    {
        switch(personaje.getAccionesAtaqueEspecial()[i])
        {
            case Accion::IZQUIERDA:
                std::cout << "IZQUIERDA\n";
                break;
            
            case Accion::ARRIBA:
                std::cout << "ARRIBA\n";
                break;
            
            case Accion::DERECHA:
                std::cout << "DERECHA\n";
                break;
            
            case Accion::ABAJO:
                std::cout << "ABAJO\n";
                break;
            
            case Accion::ATACAR:
                std::cout << "ATACAR\n";
                break;
        }
        if(personaje.getAccionesAtaqueEspecial()[i] == Accion::DERECHA)
        {
            if(parteIzquierda)
                spritesEspecial[i+1].setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/spec-derecha.png"));
            else
                spritesEspecial[personaje.getAccionesAtaqueEspecial().size()-i].setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/spec-derecha.png"));
        }
        else if(personaje.getAccionesAtaqueEspecial()[i] == Accion::IZQUIERDA)
        {
            if(parteIzquierda)
                spritesEspecial[i+1].setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/spec-izquierda.png"));
            else
                spritesEspecial[personaje.getAccionesAtaqueEspecial().size()-i].setTexture(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/spec-izquierda.png"));
        }
    }
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

    for(const sf::Sprite& s : spritesEspecial)
    {
        target.draw(s, states);
    }

    for(std::shared_ptr<Animacion> efecto : efectos)
    {
        target.draw(*efecto);
    }
}