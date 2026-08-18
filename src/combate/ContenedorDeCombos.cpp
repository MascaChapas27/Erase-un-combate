#include "ContenedorDeCombos.hpp"
#include "Constantes.hpp"
#include "VentanaPrincipal.hpp"

ContenedorDeCombos *ContenedorDeCombos::contenedorDeCombos = nullptr;

ContenedorDeCombos *ContenedorDeCombos::unicaInstancia()
{
    if (contenedorDeCombos == nullptr)
        contenedorDeCombos = new ContenedorDeCombos();
    return contenedorDeCombos;
}

ContenedorDeCombos::~ContenedorDeCombos()
{
    if (contenedorDeCombos != nullptr)
        delete contenedorDeCombos;
}

void ContenedorDeCombos::actualizar()
{
    // CUIDADO: en este for NO se actualiza automáticamente el iterador. Esto pasa
    // porque hay que eliminar los InfoCombo que ya han terminado, así que hay que
    // actualizar el iterador a mano para evitar que se joda todo
    for(std::vector<InfoCombo>::iterator it = combos.begin(); it != combos.end();)
    {
        if(it->haTerminado())
        {
            it = combos.erase(it);
        }
        else
        {
            it->actualizar();
            it++;
        }
    }
}

void ContenedorDeCombos::informar(Jugador jugador, bool continuaCombo, int ataque)
{
    bool comboEncontrado = false;

    for(InfoCombo& infoCombo : combos)
    {
        if(infoCombo.getJugador() == jugador)
        {
            if(continuaCombo && infoCombo.sigueActivo())
            {
                infoCombo.continuarCombo(ataque);
                comboEncontrado = true;
            }
            else if(!continuaCombo && infoCombo.sigueActivo())
            {
                infoCombo.terminarCombo();
                comboEncontrado = true;
            }
        }
    }

    // Comienza un nuevo combo
    if(continuaCombo && !comboEncontrado)
    {
        InfoCombo nuevoCombo(jugador);
        nuevoCombo.continuarCombo(ataque);
        combos.push_back(nuevoCombo);
    }
}

void ContenedorDeCombos::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(const InfoCombo& infoCombo : combos)
    {
        target.draw(infoCombo,states);
    }
}
