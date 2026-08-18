#include "InfoCombo.hpp"
#include "Constantes.hpp"
#include "ContenedorDeRecursos.hpp"
#include "ReproductorDeSonidos.hpp"
#include "Utilidades.hpp"
#include <assert.h>

InfoCombo::InfoCombo(Jugador jugador) :
    jugador(jugador), numeroGolpes(0), ataqueCausado(0),
    comboActivo(true), fotogramasRestantes(FRAMES_RESTANTES_COMBO),
    fotogramasDesdeQueTermino(0),
    spriteTexto(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/combos/j" + std::string(jugador == Jugador::JUGADOR1 ? "1" : "2") + "-texto.png"))
{
    if(jugador == Jugador::JUGADOR1)
    {
        spriteTexto.setPosition({0.f-MARGEN_HORIZONTAL_INFO_COMBO-spriteTexto.getTextureRect().size.x,POSICION_Y_INFO_COMBO});
    }
    else if(jugador == Jugador::JUGADOR2)
    {
        spriteTexto.setPosition({VENTANA_ANCHURA+MARGEN_HORIZONTAL_INFO_COMBO,POSICION_Y_INFO_COMBO});
    }

    posicionDeseada = spriteTexto.getPosition();
}

Jugador InfoCombo::getJugador()
{
    return jugador;
}

void InfoCombo::continuarCombo(int ataque)
{
    // Para continuar un combo, este debe estar activo
    assert(comboActivo);

    // Se incrementa en 1 el número de golpes y se suma
    // el ataque causado ahora al ataque total causado
    numeroGolpes++;
    ataqueCausado += ataque;

    // Se borran los sprites de los numeritos para el daño
    // y los golpes para meter sprites nuevos
    spritesNumeroGolpes.clear();
    spritesNumeroPuntosDeDano.clear();

    // Se meten los sprites de los numeritos nuevos para
    // los golpes del combo
    int auxNumeroGolpes = numeroGolpes;
    do
    {
        int digitoActual = auxNumeroGolpes%10;

        sf::Sprite spriteDigitoActual(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/combos/numeros-j" + std::string(jugador == Jugador::JUGADOR1 ? "1" : "2") + "-claro.png"));
        
        sf::Vector2i tamanoDigito = spriteDigitoActual.getTextureRect().size;

        spriteDigitoActual.setTextureRect(sf::IntRect({(tamanoDigito.x/10)*digitoActual,0},{tamanoDigito.x/10,tamanoDigito.y}));

        spritesNumeroGolpes.push_back(spriteDigitoActual);

        auxNumeroGolpes/=10;
    }
    while (auxNumeroGolpes != 0);

    // Se meten los sprites de los numeritos nuevos para
    // el ataque causado durante el combo
    int auxAtaqueCausado = ataqueCausado;
    do
    {
        int digitoActual = auxAtaqueCausado%10;

        sf::Sprite spriteDigitoActual(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/combos/numeros-j" + std::string(jugador == Jugador::JUGADOR1 ? "1" : "2") + "-oscuro.png"));
        
        sf::Vector2i tamanoDigito = spriteDigitoActual.getTextureRect().size;

        spriteDigitoActual.setTextureRect(sf::IntRect({(tamanoDigito.x/10)*digitoActual,0},{tamanoDigito.x/10,tamanoDigito.y}));

        spritesNumeroPuntosDeDano.push_back(spriteDigitoActual);

        auxAtaqueCausado/=10;
    }
    while (auxAtaqueCausado != 0);

    // Los fotogramas restantes para que se termine el
    // combo se resetean
    fotogramasRestantes = FRAMES_RESTANTES_COMBO;

    if(jugador == Jugador::JUGADOR1 && numeroGolpes == MIN_GOLPES_MOSTRAR_COMBO)
    {
        // La posición de la info del combo para el jugador 1 es más sencilla de
        // poner y nunca cambia
        posicionDeseada = {MARGEN_HORIZONTAL_INFO_COMBO,POSICION_Y_INFO_COMBO};
    }
    else if(jugador == Jugador::JUGADOR2 && numeroGolpes >= MIN_GOLPES_MOSTRAR_COMBO)
    {
        // En el caso del jugador 2 la posición cambia según los números que haya
        // a la derecha
        posicionDeseada.x = VENTANA_ANCHURA - MARGEN_HORIZONTAL_INFO_COMBO -
                            spriteTexto.getTextureRect().size.x -
                            std::max(spritesNumeroGolpes.size(),spritesNumeroPuntosDeDano.size())*ANCHURA_DIGITO_COMBO;
    }

    // Cada vez que se continúa el combo, el texto y los números rebotan
    if(numeroGolpes > MIN_GOLPES_MOSTRAR_COMBO)
    {
        spriteTexto.move(MOVIMIENTO_INFO_COMBO_CONTINUAR);

        for(sf::Sprite &sprite : spritesNumeroGolpes)
        {
            sprite.move(MOVIMIENTO_INFO_COMBO_CONTINUAR);
        }

        for(sf::Sprite &sprite : spritesNumeroPuntosDeDano)
        {
            sprite.move(MOVIMIENTO_INFO_COMBO_CONTINUAR);
        }
    }

    // Se reproduce un sonidito cada vez que se continúa el combo (si se ha
    // llegado al mínimo de golpes para que se considere un combo de verdad)
    if(numeroGolpes >= MIN_GOLPES_MOSTRAR_COMBO)
    {
        float tono = (numeroGolpes-MIN_GOLPES_MOSTRAR_COMBO)*0.05f+1.f;

        if(tono > MAX_TONO_SONIDO_CONTINUAR_COMBO)
        {
            tono = MAX_TONO_SONIDO_CONTINUAR_COMBO;
        }

        if(jugador == Jugador::JUGADOR1)
            ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/combos/continuar-combo-j1.ogg",tono);
        else if(jugador == Jugador::JUGADOR2)
            ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/combos/continuar-combo-j2.ogg",tono);
    }
}

void InfoCombo::terminarCombo()
{
    if(comboActivo && numeroGolpes >= MIN_GOLPES_MOSTRAR_COMBO)
    {
        if(jugador == Jugador::JUGADOR1)
            ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/combos/terminar-combo-j1.ogg");
        else if(jugador == Jugador::JUGADOR2)
            ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/combos/terminar-combo-j2.ogg");

        posicionDeseada.y = spriteTexto.getPosition().y+spriteTexto.getTextureRect().size.y;
        
        spriteTexto.setColor(COLOR_COMBO_TERMINADO);

        for(sf::Sprite &sprite : spritesNumeroGolpes)
        {
            sprite.setColor(COLOR_COMBO_TERMINADO);
        }

        for(sf::Sprite &sprite : spritesNumeroPuntosDeDano)
        {
            sprite.setColor(COLOR_COMBO_TERMINADO);
        }
    }

    comboActivo = false;
}

void InfoCombo::actualizar()
{
    // Se actualiza la posición del sprite del texto
    spriteTexto.setPosition({util::aproximarFloat(spriteTexto.getPosition().x,posicionDeseada.x,0.8),
                             util::aproximarFloat(spriteTexto.getPosition().y,posicionDeseada.y,0.8)});
            
    for(int i = 0; i < spritesNumeroGolpes.size(); i++)
    {
        sf::Vector2f nuevaPosicion = spriteTexto.getPosition();

        nuevaPosicion.x += spriteTexto.getTextureRect().size.x;
        nuevaPosicion.x += (spritesNumeroGolpes[i].getTextureRect().size.x) * (spritesNumeroGolpes.size()-i-1);
        nuevaPosicion.y += ALTURA_RELATIVA_NUMEROS_GOLPES_COMBO;

        spritesNumeroGolpes[i].setPosition(nuevaPosicion);
    }

    for(int i = 0; i < spritesNumeroPuntosDeDano.size(); i++)
    {
        sf::Vector2f nuevaPosicion = spriteTexto.getPosition();

        nuevaPosicion.x += spriteTexto.getTextureRect().size.x;
        nuevaPosicion.x += (spritesNumeroPuntosDeDano[i].getTextureRect().size.x) * (spritesNumeroPuntosDeDano.size()-i-1);
        nuevaPosicion.y += ALTURA_RELATIVA_NUMEROS_ATAQUE_COMBO;

        spritesNumeroPuntosDeDano[i].setPosition(nuevaPosicion);
    }

    if(comboActivo)
    {
        fotogramasRestantes--;
        if(fotogramasRestantes <= 0) terminarCombo();
    }
    else
    {
        fotogramasDesdeQueTermino++;

        // Una vez se llega a la mitad del tiempo en el que se muestra
        // la info del combo tras terminar este, la info se empieza a 
        // desvanecer (se vuelve transparente)
        if(fotogramasDesdeQueTermino > FRAMES_MOSTRAR_COMBO_TERMINADO/2)
        {
            sf::Color nuevoColor = spriteTexto.getColor();
            nuevoColor.a = 255*(1.f-((fotogramasDesdeQueTermino-FRAMES_MOSTRAR_COMBO_TERMINADO/2.f)/(FRAMES_MOSTRAR_COMBO_TERMINADO/2.f)));
            spriteTexto.setColor(nuevoColor);

            for(sf::Sprite &sprite : spritesNumeroGolpes)
            {
                sprite.setColor(nuevoColor);
            }

            for(sf::Sprite &sprite : spritesNumeroPuntosDeDano)
            {
                sprite.setColor(nuevoColor);
            }
        }
    }
}

bool InfoCombo::sigueActivo()
{
    return comboActivo;
}

bool InfoCombo::haTerminado()
{
    return !comboActivo && fotogramasDesdeQueTermino == FRAMES_MOSTRAR_COMBO_TERMINADO;
}

void InfoCombo::draw(sf::RenderTarget& target, sf::RenderStates states) const {

    // Si no se llega al mínimo número de golpes para mostrar
    // el combo, no se dibuja
    if(numeroGolpes < MIN_GOLPES_MOSTRAR_COMBO)
        return;

    target.draw(spriteTexto,states);

    for(const sf::Sprite& sprite : spritesNumeroGolpes)
    {
        target.draw(sprite,states);
    }

    for(const sf::Sprite& sprite : spritesNumeroPuntosDeDano)
    {
        target.draw(sprite,states);
    }
}
