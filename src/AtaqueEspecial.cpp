#include "AtaqueEspecial.hpp"
#include "Constantes.hpp"

AtaqueEspecial::AtaqueEspecial(const std::vector<Accion> &acciones) : acciones(acciones), accionActual(0), cooldown(0), esperandoPausa(false){

}

bool AtaqueEspecial::actualizar(std::unordered_set<Accion> &accionesRealizadas){

    // Se baja el cooldown para que no se puedan tardar 11987231 siglos en hacer los
    // movimientos para un ataque especial. Si llega a cero, hay que empezar los movimientos
    // de nuevo
    if(cooldown > 0) {
        cooldown--;
        if(cooldown == 0){
            resetear();
        }
    }

    // Si todavía no se han terminado los movimientos para hacer el ataque...
    if(accionActual < acciones.size()){

        // Si el ataque tiene dos movimientos consecutivos iguales, es necesario hacer una
        // pausa entre ellos en la que no se haga nada
        if(accionActual > 0 && acciones[accionActual] == acciones[accionActual-1]){
            if(esperandoPausa && accionesRealizadas.empty()) {
                esperandoPausa = false;
            } else if (!esperandoPausa && accionesRealizadas.count(acciones[accionActual])){
                accionActual++;
                cooldown = COOLDOWN_ATAQUE_ESPECIAL;
                if(accionActual != acciones.size() && acciones[accionActual] == acciones[accionActual-1]){
                    esperandoPausa = true;
                }
            }
        } else {
            if((accionActual > 0 && acciones[accionActual] != Accion::ATACAR && accionesRealizadas.count(acciones[accionActual]) && !accionesRealizadas.count(acciones[accionActual-1])) ||
               ((accionActual == 0 || acciones[accionActual] == Accion::ATACAR) && accionesRealizadas.count(acciones[accionActual]))){
                accionActual++;
                cooldown = COOLDOWN_ATAQUE_ESPECIAL;
                if(accionActual != acciones.size() && acciones[accionActual] == acciones[accionActual-1]){
                    esperandoPausa = true;
                }
            }
        }
    }
    
    if(accionActual == acciones.size()){
        resetear();
        return true;
    } else {
        return false;
    }
}

void AtaqueEspecial::resetear(){
    accionActual = 0;
    cooldown = 0;
    esperandoPausa = false;
}