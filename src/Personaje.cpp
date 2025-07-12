#include "Personaje.hpp"
#include "Enums.hpp"
#include "Constantes.hpp"
#include "Utilidades.hpp"
#include "ContenedorDeEfectos.hpp"
#include "ContenedorDePersonajes.hpp"
#include "AnimacionConGravedad.hpp"
#include "VentanaPrincipal.hpp"
#include "Bitacora.hpp"
#include "ReproductorDeSonidos.hpp"
#include <iostream>

Personaje::Personaje(std::map<EstadoPersonaje,std::shared_ptr<AnimacionPorFrames>> animaciones, std::string nombre, std::vector<Accion> accionesAtaqueEspecial) :
    ataqueEspecial(accionesAtaqueEspecial)
{
    puntosDeVida = MAX_PUNTOS_DE_VIDA;
    medidorSuper = 0;
    velocidad.y = 0;
    velocidad.x = 0;
    escalaSprite = {1.f,1.f};
    contadorTumbado = 0;
    contadorBlanco = 0;
    contadorEsquiveSuper = 0;
    this->nombre = nombre;
    this->animaciones = animaciones;
    estado = EstadoPersonaje::QUIETO;
    shader = std::make_shared<sf::Shader>();
    if(!shader->loadFromFile("shaders/blendColor.frag",sf::Shader::Type::Fragment)){
        Bitacora::unicaInstancia()->escribir("ERROR: no se pudo cargar el shader");
        exit(EXIT_FAILURE);
    }
}

Personaje Personaje::clonar() {

    Personaje nuevo(*this);

    // El shader se hace de nuevo
    nuevo.shader = std::make_shared<sf::Shader>();
    if(!nuevo.shader->loadFromFile("shaders/blendColor.frag",sf::Shader::Type::Fragment)){
        Bitacora::unicaInstancia()->escribir("ERROR: no se pudo cargar el shader");
        exit(EXIT_FAILURE);
    }

    // Se limpian las animaciones porque se van a hacer de nuevo
    nuevo.animaciones.clear();

    for(std::pair<EstadoPersonaje,std::shared_ptr<AnimacionPorFrames>> par : this->animaciones){
        nuevo.animaciones.insert({par.first,std::dynamic_pointer_cast<AnimacionPorFrames>(par.second->clonar())});
    }

    return nuevo;
}

void Personaje::realizarAccion(Accion accion){
    accionesRealizadas[accion] = true;
}

void Personaje::detenerAccion(Accion accion){
    accionesRealizadas[accion] = false;
}

int Personaje::getPuntosDeVida(){
    return puntosDeVida;
}

void Personaje::setPuntosDeVida(int puntosDeVida){
    this->puntosDeVida = puntosDeVida;
}

void Personaje::setMedidorSuper(int medidorSuper){
    this->medidorSuper = medidorSuper;
}

void Personaje::setPosicion(float x, float y){
    animaciones.at(estado)->setPosicion(x,y);
}

void Personaje::setPosicion(sf::Vector2f posicion){
    animaciones.at(estado)->setPosicion(posicion);
}

sf::Vector2f Personaje::getPosicion(){
    return animaciones.at(estado)->getPosicion();
}

bool Personaje::isMirandoDerecha(){
    return (escalaSprite.x > 0);
}

void Personaje::voltear()
{
    escalaSprite.x *= -1;
    for(auto &[estado, anim] : animaciones){
        anim->voltear();
    }
}

EstadoPersonaje Personaje::getEstado(){
    return estado;
}

void Personaje::mover(float offsetX, float offsetY){
    animaciones.at(estado)->mover(offsetX,offsetY);
}

void Personaje::setJugador(Jugador jugador){
    this->jugador = jugador;
}

Jugador Personaje::getJugador(){
    return this->jugador;
}

std::shared_ptr<AnimacionPorFrames> Personaje::getAnimacionSegunEstado(EstadoPersonaje estado){
    return animaciones.at(estado);
}

void Personaje::setAnimaciones(const std::map<EstadoPersonaje,std::shared_ptr<AnimacionPorFrames>> &animaciones){
    this->animaciones = animaciones;
}

void Personaje::cambiarEstado(EstadoPersonaje estadoNuevo){

    animaciones.at(estadoNuevo)->setPosicion(animaciones.at(estado)->getPosicion());
    animaciones.at(estadoNuevo)->resetear();
    estado = estadoNuevo;
}

void Personaje::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(*animaciones.at(estado),states);
    if(contadorBlanco > 0) {
        states.shader = shader.get();
        target.draw(*animaciones.at(estado),states);
    }
}

void Personaje::moverseIzquierda(){
    // La velocidad disminuye un poco pero sin pasarse
    velocidad.x-=VELOCIDAD_ANDAR_AUMENTO;
    if(velocidad.x <= -VELOCIDAD_ANDAR_MAX)
        velocidad.x = -VELOCIDAD_ANDAR_MAX;
}

void Personaje::moverseDerecha(){
    // La velocidad aumenta un poco pero sin pasarse
    velocidad.x+=VELOCIDAD_ANDAR_AUMENTO;
    if(velocidad.x >= VELOCIDAD_ANDAR_MAX)
        velocidad.x = VELOCIDAD_ANDAR_MAX;
}

void Personaje::pararMovimiento(){
    // Si la velocidad es tan baja que se podría decir que es cero, termina
    // directamente para no entrar en un bucle
    if(std::abs(velocidad.x) < UMBRAL_FLOAT) return;

    // Si la velocidad es positiva se baja, si no se sube
    bool positivo = velocidad.x > 0;
    if(positivo){
        velocidad.x-=VELOCIDAD_ANDAR_AUMENTO;
    } else {
        velocidad.x+=VELOCIDAD_ANDAR_AUMENTO;
    }

    // Si la velocidad se ha pasado de 0, se pone a 0 y el personaje ya se para del todo
    if((positivo && velocidad.x < 0) ||
       (!positivo && velocidad.x > 0)) velocidad.x = 0;
}

void Personaje::levantarPolvo(std::list<std::shared_ptr<Animacion>> &efectosInsertados){
    std::shared_ptr<Animacion> polvo(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("polvo"));

    std::shared_ptr<Animacion> polvoVolteado(polvo->clonar());

    polvoVolteado->voltear();

    polvoVolteado->setPosicion(this->getPosicion().x-OFFSET_POLVO, this->getPosicion().y);

    polvo->setPosicion(this->getPosicion().x+OFFSET_POLVO, this->getPosicion().y);

    efectosInsertados.push_back(polvoVolteado);
    efectosInsertados.push_back(polvo);
}

void Personaje::actualizar(sf::Vector2f posicionEnemigo, std::list<std::shared_ptr<Animacion>> &efectosInsertados){

    // Se comprueba el ataque especial con los botones pulsados
    std::unordered_set<Accion> acciones;
    for(const std::pair<Accion,bool> par : accionesRealizadas){
        if(par.second){
            acciones.insert(par.first);
        }
    }

    // Se comprueba si se han pulsado los botones necesarios para hacer el ataque especial
    bool realizarAtaqueEspecial = ataqueEspecial.actualizar(acciones);

    // Según el estado, se hace una cosa u otra
    switch(estado){
    case EstadoPersonaje::TOCANDO_SUELO:
        pararMovimiento();
        if(animaciones.at(estado)->haTerminado())
            cambiarEstado(EstadoPersonaje::QUIETO);
        
        // No se pone break porque el estado TOCANDO_SUELO es como si fuera el estado QUIETO
        // pero con un poco más de paripé para hacerlo más realista todo
        break;

    case EstadoPersonaje::QUIETO:

        pararMovimiento();

        if(realizarAtaqueEspecial){
            ataqueEspecial.resetear();
            cambiarEstado(EstadoPersonaje::ATAQUE_ESPECIAL);
        }
        else if(accionesRealizadas[Accion::ARRIBA])
        {
            velocidad.y = VELOCIDAD_SALTO;
            accionesRealizadas[Accion::ARRIBA] = false;
            levantarPolvo(efectosInsertados);
            escalaSprite.x = (escalaSprite.x > 0) ? 0.8 : -0.8;
            escalaSprite.y = 1.2;
            cambiarEstado(EstadoPersonaje::SALTANDO_SUBIENDO);
        }
        else if (accionesRealizadas[Accion::ABAJO])
        {
            escalaSprite.x = (escalaSprite.x > 0) ? 1.05 : -1.05;
            escalaSprite.y = 0.95;
            cambiarEstado(EstadoPersonaje::AGACHADO);
        }
        else if (accionesRealizadas[Accion::DERECHA])
        {
            velocidad.x+=VELOCIDAD_ANDAR_AUMENTO;
            if(velocidad.x >= VELOCIDAD_ANDAR_MAX)
                velocidad.x = VELOCIDAD_ANDAR_MAX;

            if((escalaSprite.x > 0))
                cambiarEstado(EstadoPersonaje::ANDANDO_ACERCANDOSE);
            else
                cambiarEstado(EstadoPersonaje::ANDANDO_ALEJANDOSE);

        }
        else if (accionesRealizadas[Accion::IZQUIERDA])
        {
            velocidad.x-=VELOCIDAD_ANDAR_AUMENTO;
            if(velocidad.x <= -VELOCIDAD_ANDAR_MAX)
                velocidad.x = -VELOCIDAD_ANDAR_MAX;

            if(!(escalaSprite.x > 0))
                cambiarEstado(EstadoPersonaje::ANDANDO_ACERCANDOSE);
            else
                cambiarEstado(EstadoPersonaje::ANDANDO_ALEJANDOSE);
        }
        else if (accionesRealizadas[Accion::ATACAR]){
            
            if(medidorSuper == MAX_MEDIDOR_SUPER){
                contadorBlanco = 255;
                medidorSuper = 0;
                cambiarEstado(EstadoPersonaje::PREPARANDO_SUPER);
            } else {
                cambiarEstado(EstadoPersonaje::ATAQUE_NORMAL_1);
            }
        }

        break;

    case EstadoPersonaje::ANDANDO_ACERCANDOSE:

        if(realizarAtaqueEspecial){
            ataqueEspecial.resetear();
            cambiarEstado(EstadoPersonaje::ATAQUE_ESPECIAL);
        } else if (accionesRealizadas[Accion::ATACAR]){
            cambiarEstado(EstadoPersonaje::ATAQUE_NORMAL_1);
        } else if(accionesRealizadas[Accion::ARRIBA]){
            velocidad.y = VELOCIDAD_SALTO;
            accionesRealizadas[Accion::ARRIBA] = false;
            levantarPolvo(efectosInsertados);
            escalaSprite.x = (escalaSprite.x > 0) ? 0.8 : -0.8;
            escalaSprite.y = 1.2;
            cambiarEstado(EstadoPersonaje::SALTANDO_SUBIENDO);
        } else if(accionesRealizadas[Accion::DERECHA]){
            moverseDerecha();
            if(!(escalaSprite.x > 0)){
                cambiarEstado(EstadoPersonaje::ANDANDO_ALEJANDOSE);
            }
        } else if(accionesRealizadas[Accion::IZQUIERDA]){
            moverseIzquierda();
            if((escalaSprite.x > 0)){
                cambiarEstado(EstadoPersonaje::ANDANDO_ALEJANDOSE);
            }
        } else {
            pararMovimiento();
            cambiarEstado(EstadoPersonaje::QUIETO);
        }

        break;
    case EstadoPersonaje::ANDANDO_ALEJANDOSE:

        contadorEsquiveSuper++;

        if(realizarAtaqueEspecial){
            ataqueEspecial.resetear();
            cambiarEstado(EstadoPersonaje::ATAQUE_ESPECIAL);
        } else if (accionesRealizadas[Accion::ATACAR]){
            contadorEsquiveSuper=0;
            cambiarEstado(EstadoPersonaje::ATAQUE_NORMAL_1);
        } else if(accionesRealizadas[Accion::ARRIBA]){
            velocidad.y = VELOCIDAD_SALTO;
            accionesRealizadas[Accion::ARRIBA] = false;
            levantarPolvo(efectosInsertados);
            contadorEsquiveSuper=0;
            escalaSprite.x = (escalaSprite.x > 0) ? 0.8 : -0.8;
            escalaSprite.y = 1.2;
            cambiarEstado(EstadoPersonaje::SALTANDO_SUBIENDO);
        } else if(accionesRealizadas[Accion::DERECHA]){
            moverseDerecha();
            if((escalaSprite.x > 0)){
                contadorEsquiveSuper=0;
                cambiarEstado(EstadoPersonaje::ANDANDO_ACERCANDOSE);
            }
        } else if(accionesRealizadas[Accion::IZQUIERDA]){
            moverseIzquierda();
            if(!(escalaSprite.x > 0)){
                contadorEsquiveSuper=0;
                cambiarEstado(EstadoPersonaje::ANDANDO_ACERCANDOSE);
            }
        } else {
            pararMovimiento();
            contadorEsquiveSuper=0;
            cambiarEstado(EstadoPersonaje::QUIETO);
        }

        break;

    case EstadoPersonaje::SALTANDO_SUBIENDO:
        velocidad.y+=GRAVEDAD;

        if(accionesRealizadas[Accion::ATACAR]) {
            detenerAccion(Accion::ATACAR);
            cambiarEstado(EstadoPersonaje::ATAQUE_AEREO);
            break;
        }
        
        if(accionesRealizadas[Accion::DERECHA]) moverseDerecha();
        else if (accionesRealizadas[Accion::IZQUIERDA]) moverseIzquierda();

        if(velocidad.y > 0)
            cambiarEstado(EstadoPersonaje::SALTANDO_BAJANDO);
        break;

    case EstadoPersonaje::SALTANDO_BAJANDO:
        velocidad.y+=GRAVEDAD;
        escalaSprite.x+=((escalaSprite.x > 0) ? -0.01 : 0.01);
        escalaSprite.y+=0.01;

        if(accionesRealizadas[Accion::ATACAR]) {
            detenerAccion(Accion::ATACAR);
            cambiarEstado(EstadoPersonaje::ATAQUE_AEREO);
            break;
        }

        if(accionesRealizadas[Accion::DERECHA]) moverseDerecha();
        else if (accionesRealizadas[Accion::IZQUIERDA]) moverseIzquierda();

        if(animaciones.at(estado)->getPosicion().y > ALTURA_SUELO){
            animaciones.at(estado)->setPosicion(animaciones.at(estado)->getPosicion().x,ALTURA_SUELO);
            velocidad.y = 0;
            levantarPolvo(efectosInsertados);

            escalaSprite.x = (escalaSprite.x > 0) ? 1.1 : -1.1;
            escalaSprite.y = 0.9;
            cambiarEstado(EstadoPersonaje::TOCANDO_SUELO);
        }
        break;
    
    case EstadoPersonaje::ATAQUE_AEREO:
        velocidad.y+=GRAVEDAD;

        if(accionesRealizadas[Accion::DERECHA]) moverseDerecha();
        else if (accionesRealizadas[Accion::IZQUIERDA]) moverseIzquierda();

        if(animaciones.at(estado)->getPosicion().y > ALTURA_SUELO){
            animaciones.at(estado)->setPosicion(animaciones.at(estado)->getPosicion().x,ALTURA_SUELO);
            velocidad.y = 0;
            levantarPolvo(efectosInsertados);

            escalaSprite.x = (escalaSprite.x > 0) ? 1.1 : -1.1;
            escalaSprite.y = 0.9;
            cambiarEstado(EstadoPersonaje::TOCANDO_SUELO);
        } else if (animaciones.at(estado)->haTerminado()){
            if(velocidad.y < 0)
                cambiarEstado(EstadoPersonaje::SALTANDO_SUBIENDO);
            else
                cambiarEstado(EstadoPersonaje::SALTANDO_BAJANDO);
        }
        break;

    case EstadoPersonaje::ATAQUE_NORMAL_1:

        pararMovimiento();

        if(animaciones.at(estado)->haTerminado()){
            if(accionesRealizadas[Accion::ATACAR])
                cambiarEstado(EstadoPersonaje::ATAQUE_NORMAL_2);
            else
                cambiarEstado(EstadoPersonaje::QUIETO);
        }
        break;
    case EstadoPersonaje::ATAQUE_NORMAL_2:

        pararMovimiento();

        if(animaciones.at(estado)->haTerminado()){
            if(accionesRealizadas[Accion::ATACAR])
                cambiarEstado(EstadoPersonaje::ATAQUE_NORMAL_3);
            else
                cambiarEstado(EstadoPersonaje::QUIETO);
        }
        break;
    case EstadoPersonaje::ATAQUE_NORMAL_3:

        pararMovimiento();

        if(animaciones.at(estado)->haTerminado()){
            cambiarEstado(EstadoPersonaje::QUIETO);
            accionesRealizadas[Accion::ATACAR] = false;
        }
        break;
    
    case EstadoPersonaje::ATAQUE_ESPECIAL:

        pararMovimiento();

        if(animaciones.at(estado)->haTerminado()){
            cambiarEstado(EstadoPersonaje::QUIETO);
        }
        break;
    
    case EstadoPersonaje::GOLPEADO_PEQUE:
    case EstadoPersonaje::GOLPEADO_MEDIO:

        if(getPosicion().y < ALTURA_SUELO) {
            // Le ha pillado en el aire
            velocidad.y+=GRAVEDAD;

            if(animaciones.at(estado)->haTerminado()){
                cambiarEstado(velocidad.y < 0 ? EstadoPersonaje::SALTANDO_SUBIENDO : EstadoPersonaje::SALTANDO_BAJANDO);
            }

        } else if (velocidad.y != 0){
            // Ha caído
            velocidad.y = 0;
            animaciones.at(estado)->setPosicion(animaciones.at(estado)->getPosicion().x,ALTURA_SUELO);
            levantarPolvo(efectosInsertados);

            //cambiarEstado(EstadoPersonaje::TOCANDO_SUELO);
        } else {
            // Le ha pillado en el suelo
            pararMovimiento();

            if(animaciones.at(estado)->haTerminado()){
                cambiarEstado(EstadoPersonaje::QUIETO);
            }
        }

        break;
    
    case EstadoPersonaje::GOLPEADO_SUBIENDO:
        velocidad.y+=GRAVEDAD;

        if(velocidad.y > 0)
            cambiarEstado(EstadoPersonaje::GOLPEADO_BAJANDO);
        break;
    
    case EstadoPersonaje::GOLPEADO_BAJANDO:
        velocidad.y+=GRAVEDAD;

        if(animaciones.at(estado)->getPosicion().y > ALTURA_SUELO){
            animaciones.at(estado)->setPosicion(animaciones.at(estado)->getPosicion().x,ALTURA_SUELO);
            levantarPolvo(efectosInsertados);
            velocidad.x *= 0.5;

            if(velocidad.y < VELOCIDAD_PASAR_A_TIRADO){
                velocidad.y = 0;
                
                escalaSprite.x = (escalaSprite.x > 0) ? 1.1 : -1.1;
                escalaSprite.y = 0.9;

                cambiarEstado(EstadoPersonaje::TUMBADO);
            } else {
                velocidad.y *= -0.5;
                escalaSprite.x = (escalaSprite.x > 0) ? 1.1 : -1.1;
                escalaSprite.y = 0.9;
                cambiarEstado(EstadoPersonaje::GOLPEADO_SUBIENDO);
            }
        }
        break;

    case EstadoPersonaje::TUMBADO:
        {
            velocidad.x *= 0.9;

            // Los personajes derrotados no se pueden levantar
            if(puntosDeVida == 0) break;
            
            // El contador siempre sube para que la gente no se quede tirada
            contadorTumbado++;

            // Se pone a true si el jugador se ha movido en este frame para
            // levantarse antes
            bool movido = false;

            // También se puede subir pulsando botones
            if(accionesRealizadas[Accion::ABAJO]){
                contadorTumbado++;
                accionesRealizadas[Accion::ABAJO] = false;
                movido = true;
            }

            if(accionesRealizadas[Accion::ARRIBA]){
                contadorTumbado++;
                accionesRealizadas[Accion::ARRIBA] = false;
                movido = true;
            }

            if(accionesRealizadas[Accion::IZQUIERDA]){
                contadorTumbado++;
                accionesRealizadas[Accion::IZQUIERDA] = false;
                movido = true;
            }

            if(accionesRealizadas[Accion::DERECHA]){
                contadorTumbado++;
                accionesRealizadas[Accion::DERECHA] = false;
                movido = true;
            }

            if(accionesRealizadas[Accion::ATACAR]){
                contadorTumbado++;
                accionesRealizadas[Accion::ATACAR] = false;
                movido = true;
            }

            // Cuanto más dañado esté el personaje más le cuesta levantarse
            if(contadorTumbado >= (MAX_CONTADOR_TUMBADO+(MAX_PUNTOS_DE_VIDA-puntosDeVida))){
                contadorTumbado = 0;
                cambiarEstado(EstadoPersonaje::LEVANTANDOSE);
            } else if (movido){
                velocidad.x+=((velocidad.x > 0 ? -0.5 : 0.5) * util::realAleatorio());
            }
        }
        break;
    
    case EstadoPersonaje::LEVANTANDOSE:
        pararMovimiento();
        
        if(animaciones.at(estado)->haTerminado())
            cambiarEstado(EstadoPersonaje::QUIETO);

        break;

    case EstadoPersonaje::BLOQUEANDO:
        if(velocidad.y == 0 && animaciones.at(estado)->getPosicion().y == ALTURA_SUELO) {
            pararMovimiento();
            if(accionesRealizadas[Accion::ATACAR]){
                cambiarEstado(EstadoPersonaje::ATAQUE_NORMAL_1);
            } else if(animaciones.at(estado)->haTerminado()){
                if((accionesRealizadas[Accion::DERECHA] && !(escalaSprite.x > 0)) ||
                   (accionesRealizadas[Accion::IZQUIERDA] && (escalaSprite.x > 0))){
                    cambiarEstado(EstadoPersonaje::ANDANDO_ALEJANDOSE);
                } else {
                    cambiarEstado(EstadoPersonaje::QUIETO);
                }
            }
        } else {
            velocidad.y+=GRAVEDAD;
            if(accionesRealizadas[Accion::ATACAR]){
                cambiarEstado(EstadoPersonaje::ATAQUE_AEREO);
            } else if(animaciones.at(estado)->haTerminado()){
                cambiarEstado(velocidad.y >= 0 ? EstadoPersonaje::SALTANDO_BAJANDO : EstadoPersonaje::SALTANDO_SUBIENDO);
            } else if (animaciones.at(estado)->getPosicion().y >= ALTURA_SUELO){
                animaciones.at(estado)->setPosicion(animaciones.at(estado)->getPosicion().x,ALTURA_SUELO);
                velocidad.y = 0;
                levantarPolvo(efectosInsertados);

                escalaSprite.x = (escalaSprite.x > 0) ? 1.1 : -1.1;
            escalaSprite.y = 0.9;
                cambiarEstado(EstadoPersonaje::TOCANDO_SUELO);
            }
        }
        break;

    case EstadoPersonaje::AGACHADO:
        pararMovimiento();

        if(realizarAtaqueEspecial){
            ataqueEspecial.resetear();
            cambiarEstado(EstadoPersonaje::ATAQUE_ESPECIAL);
        }
        else if(!accionesRealizadas[Accion::ABAJO]){
            escalaSprite.x = (escalaSprite.x > 0) ? 0.95 : -0.95;
            escalaSprite.y = 1.05;
            cambiarEstado(EstadoPersonaje::QUIETO);
        } else if (accionesRealizadas[Accion::ATACAR]){
            cambiarEstado(EstadoPersonaje::ATAQUE_AGACHADO);
            accionesRealizadas[Accion::ATACAR] = false;
        }
        break;

    case EstadoPersonaje::ATAQUE_AGACHADO:
        pararMovimiento();
        
        if(animaciones.at(estado)->haTerminado()){
            cambiarEstado(EstadoPersonaje::AGACHADO);
        }
        break;
    
    case EstadoPersonaje::PREPARANDO_SUPER:
        pararMovimiento();

        if(animaciones.at(estado)->haTerminado()){
            cambiarEstado(EstadoPersonaje::ATAQUE_SUPER);
        }
        break;
    
    case EstadoPersonaje::ATAQUE_SUPER:
        pararMovimiento();

        if(animaciones.at(estado)->haTerminado()){
            contadorBlanco = 255;
            cambiarEstado(EstadoPersonaje::QUIETO);
        }
        break;
    
    case EstadoPersonaje::CELEBRANDO:
        pararMovimiento();

        break;

    case EstadoPersonaje::ESQUIVE_SUPER:
        pararMovimiento();

        if(animaciones.at(estado)->haTerminado()){
            cambiarEstado(EstadoPersonaje::QUIETO);
        }

        break;
    }

    // Se comprueba si el enemigo está a la derecha o a la izquierda y se voltea el
    // sprite según sea necesario. Si el personaje está atacando o recibiendo un golpe
    // (incluyendo bloqueo), está demasiado ocupado como para ir volteándose
    switch(estado){
        case EstadoPersonaje::ATAQUE_NORMAL_1:
        case EstadoPersonaje::ATAQUE_NORMAL_2:
        case EstadoPersonaje::ATAQUE_NORMAL_3:
        case EstadoPersonaje::ATAQUE_ESPECIAL:
        case EstadoPersonaje::ATAQUE_AEREO:
        case EstadoPersonaje::ATAQUE_AGACHADO:
        case EstadoPersonaje::ATAQUE_SUPER:
        case EstadoPersonaje::GOLPEADO_BAJANDO:
        case EstadoPersonaje::GOLPEADO_SUBIENDO:
        case EstadoPersonaje::TUMBADO:
        case EstadoPersonaje::BLOQUEANDO:
        case EstadoPersonaje::ESQUIVE_SUPER:
        case EstadoPersonaje::CELEBRANDO:
        case EstadoPersonaje::LEVANTANDOSE:
        case EstadoPersonaje::PREPARANDO_SUPER:
        case EstadoPersonaje::SALTANDO_BAJANDO:
        case EstadoPersonaje::SALTANDO_SUBIENDO:
            break;
        
        case EstadoPersonaje::AGACHADO:
        case EstadoPersonaje::ANDANDO_ACERCANDOSE:
        case EstadoPersonaje::ANDANDO_ALEJANDOSE:
        case EstadoPersonaje::QUIETO:
        case EstadoPersonaje::TOCANDO_SUELO:
        case EstadoPersonaje::GOLPEADO_PEQUE:
        case EstadoPersonaje::GOLPEADO_MEDIO:
            if((animaciones.at(estado)->getPosicion().x < posicionEnemigo.x && !(escalaSprite.x > 0)) ||
                (animaciones.at(estado)->getPosicion().x > posicionEnemigo.x && (escalaSprite.x > 0))){
                    this->voltear();
                }
            break;
    }

    // Se actualiza la animación por frames del estado
    animaciones.at(estado)->actualizar(efectosInsertados);

    // Se actualiza la escala del personaje
    animaciones[estado]->setEscala(escalaSprite);

    // Se devuelve la escala del sprite a su valor original
    escalaSprite.x = util::aproximarFloat(escalaSprite.x,(escalaSprite.x > 0) ? 1.f : -1.f, 0.8);
    escalaSprite.y = util::aproximarFloat(escalaSprite.y,1.f,0.8);

    // Se consulta el movimiento de la animación por frames
    sf::Vector2f movimiento;
    movimiento = animaciones.at(estado)->getMovimientoFrameActual();
    velocidad.x+=movimiento.x;
    velocidad.y+=movimiento.y;
    
    // Una vez se hace todo, se aumenta la velocidad según se vea
    animaciones.at(estado)->mover(velocidad.x,velocidad.y);

    // Si el personaje se sale por la derecha, no dejar que pase
    if(animaciones.at(estado)->getPosicion().x > VENTANA_ANCHURA-1){
        animaciones.at(estado)->setPosicion(VENTANA_ANCHURA-1,animaciones.at(estado)->getPosicion().y);
        if(estado == EstadoPersonaje::GOLPEADO_BAJANDO || estado == EstadoPersonaje::GOLPEADO_SUBIENDO || estado == EstadoPersonaje::TUMBADO) velocidad.x *= -1;
        else velocidad.x = 0;
    } else if (animaciones.at(estado)->getPosicion().x < 0){
        animaciones.at(estado)->setPosicion(0,animaciones.at(estado)->getPosicion().y);
        if(estado == EstadoPersonaje::GOLPEADO_BAJANDO || estado == EstadoPersonaje::GOLPEADO_SUBIENDO || estado == EstadoPersonaje::TUMBADO) velocidad.x *= -1;
        else velocidad.x = 0;
    }

    shader->setUniform("amount",contadorBlanco/255.f);

    if(contadorBlanco > 0) {
        contadorBlanco-=3;
        if(contadorBlanco < 0) contadorBlanco = 0;
    }
}

void Personaje::comprobarColisiones(const std::list<std::shared_ptr<Animacion>> &animaciones, std::list<std::shared_ptr<Animacion>> &efectosInsertados){

    // Se sacan las hitboxes de la animación del estado actual
    std::vector<Hitbox> hitboxes = this->animaciones.at(estado)->getHitboxes();

    // Si la animación del estado actual no tiene hitboxes en este momento, no hace falta comprobar nada
    if(hitboxes.size() == 0) return;

    // Si el personaje está en un estado en el que no puede ser golpeado, no hace falta comprobar nada
    switch(estado){
        // Bueno este case lo voy a quitar para que se puedan hacer combos
        // case EstadoPersonaje::GOLPEADO_MEDIO:
        case EstadoPersonaje::GOLPEADO_BAJANDO:
        case EstadoPersonaje::GOLPEADO_SUBIENDO:
        case EstadoPersonaje::PREPARANDO_SUPER:
        case EstadoPersonaje::TUMBADO:
        case EstadoPersonaje::LEVANTANDOSE:
        case EstadoPersonaje::CELEBRANDO:
            return;
            break;
        
        default:
            break;
    }

    // Se eliminan las hitboxes con daño
    auto it = hitboxes.begin();
    
    while(it != hitboxes.end()){
        if(it->getFuerzaAtaque() > 0)
            it = hitboxes.erase(it);
        else it++;
    }

    // Se encuentra una colisión entre una hitbox del personaje (hurtbox como la llama la chaviza)
    // y una hitbox del enemigo (la que colisione con más daño)
    Hitbox hitboxElegidaEnemigo = Hitbox(sf::IntRect({0,0},{0,0}),0,false);
    Hitbox hitboxElegidaPropia = Hitbox(sf::IntRect({0,0},{0,0}),0,false);

    // Este booleano indica que una hurtbox del enemigo (una hitbox sin daño de ataque, es decir,
    // una hitbox que representa el cuerpo del enemigo) ha golpeado una hitbox sin daño de nuestro personaje,
    // por lo que nos hemos chocado en el enemigo y no podemos seguir andando
    bool colisionConEnemigo = false;

    // Este booleano indica que la hurtbox del enemigo está por encima del suelo, por lo que el enemigo estaba
    // saltando al colisionar
    bool colisionVoladora = false;

    for(std::shared_ptr<Animacion> anim : animaciones){
        for(Hitbox hEnemigo : anim->getHitboxes()){
            for(Hitbox hPropia : hitboxes){
                
                sf::IntRect rectEnemigo = hEnemigo.getRectangulo();
                rectEnemigo.position.x += anim->getPosicionEsqSupIzq().x;
                rectEnemigo.position.y += anim->getPosicionEsqSupIzq().y;

                sf::IntRect rectPropio = hPropia.getRectangulo();
                rectPropio.position.x += this->animaciones.at(estado)->getPosicionEsqSupIzq().x;
                rectPropio.position.y += this->animaciones.at(estado)->getPosicionEsqSupIzq().y;

                bool hayInterseccion = rectEnemigo.findIntersection(rectPropio).has_value();

                if(hayInterseccion && hEnemigo.getFuerzaAtaque() > hitboxElegidaEnemigo.getFuerzaAtaque()){
                    hitboxElegidaEnemigo = Hitbox(rectEnemigo,hEnemigo.getFuerzaAtaque(),hEnemigo.esAtaqueBajo());
                    hitboxElegidaPropia = Hitbox(rectPropio,hPropia.getFuerzaAtaque(),hPropia.esAtaqueBajo());
                } else if (hayInterseccion && hEnemigo.getFuerzaAtaque() == 0){
                    colisionConEnemigo = true;
                    colisionVoladora = (rectEnemigo.position.y + rectEnemigo.size.y) < ALTURA_SUELO;
                }
            }
        }
    }

    // Si nos hemos chocado con el cuerpo del enemigo, deshacemos el movimiento en el eje x si nos
    // estamos moviendo hacia el enemigo (si no, está bien alejarnos de él (y si saltamos, podemos saltarle
    // por encima))
    if(colisionConEnemigo){
        
        // Nos alejamos un poco del enemigo (si estamos agachados somos más difíciles de mover, y si
        // estamos haciendo el súper ataque no no no nos moverán)
        switch(estado){
            case EstadoPersonaje::AGACHADO:
                this->animaciones.at(estado)->mover((escalaSprite.x > 0) ? -0.25 : 0.25, 0);
                break;
            case EstadoPersonaje::ATAQUE_SUPER:
                break;
            default:
                this->animaciones.at(estado)->mover((escalaSprite.x > 0) ? -0.5 : 0.5, 0);
                break;
        }

        // Además, si nos estamos moviendo hacia el enemigo, la velocidad en el eje X se anula
        switch(estado){
            case EstadoPersonaje::SALTANDO_SUBIENDO:
            case EstadoPersonaje::SALTANDO_BAJANDO:
            case EstadoPersonaje::ATAQUE_AEREO:
                // Si estamos saltando y el enemigo también (su hurtbox elegida está por encima
                // del suelo), y además estamos saltando hacia él, es como que nos chocamos y nos
                // dejamos de mover
                if((((escalaSprite.x > 0) && velocidad.x>0) || (!(escalaSprite.x > 0) && velocidad.x<0)) && colisionVoladora){
                    velocidad.x = 0;
                }
                break;
            case EstadoPersonaje::ATAQUE_SUPER:
                // Si estamos haciendo el súper ataque de aquí no nos mueve ni Dios
                break;
            default:
                // En cualquier otro caso, dejamos de movernos
                if((((escalaSprite.x > 0) && velocidad.x>0) || (!(escalaSprite.x > 0) && velocidad.x<0))){
                    velocidad.x = 0;
                }
                break;
        }
    }

    // Si no hubo golpe, no hay nada más que hacer
    if(hitboxElegidaEnemigo.getFuerzaAtaque() == 0){
        return;
    }

    // Si sí que hubo golpe, depende ya de cómo le pille al personaje

    // El ataque súper es inesquivablemente mortal
    if(hitboxElegidaEnemigo.getFuerzaAtaque() > MAX_ATAQUE_MEDIO){

        // Parry épico de ulti
        if(estado == EstadoPersonaje::ANDANDO_ALEJANDOSE &&  contadorEsquiveSuper < MAX_CONTADOR_ESQUIVE_SUPER){
            cambiarEstado(EstadoPersonaje::ESQUIVE_SUPER);
        } else {
            velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_X_GOLPE_SUPER : IMPULSO_X_GOLPE_SUPER;
            velocidad.y = IMPULSO_Y_GOLPE_SUPER;

            escalaSprite.x = (escalaSprite.x > 0) ? 0.8 : -0.8;
            escalaSprite.y = 1.2;
            cambiarEstado(EstadoPersonaje::GOLPEADO_SUBIENDO);
        }
        
    } else {

        // Si no es súper, ahora sí que depende de cómo esté el personaje, pues
        // los ataques medios y pequeños se pueden esquivar
        switch(estado){

            // Atacando o sin moverse al revés del enemigo (todo te pega)
            case EstadoPersonaje::GOLPEADO_PEQUE:
            case EstadoPersonaje::QUIETO:
            case EstadoPersonaje::ANDANDO_ACERCANDOSE:
            case EstadoPersonaje::ATAQUE_NORMAL_1:
            case EstadoPersonaje::ATAQUE_NORMAL_2:
            case EstadoPersonaje::ATAQUE_NORMAL_3:
            case EstadoPersonaje::ATAQUE_ESPECIAL:
            case EstadoPersonaje::ATAQUE_AGACHADO:
            case EstadoPersonaje::TOCANDO_SUELO:
            case EstadoPersonaje::ATAQUE_AEREO:

            // Caso especial en el que puedes bloquear en el aire
            case EstadoPersonaje::SALTANDO_SUBIENDO:
            case EstadoPersonaje::SALTANDO_BAJANDO:
                if((estado == EstadoPersonaje::SALTANDO_SUBIENDO || estado == EstadoPersonaje::SALTANDO_BAJANDO) &&
                   (((escalaSprite.x > 0) && accionesRealizadas[Accion::IZQUIERDA]) || (!(escalaSprite.x > 0) && accionesRealizadas[Accion::DERECHA]))){
                    // Los gotos son el diablo pero no se me ocurre nada mejor jejej
                    goto bloqueoAereo;
                }

                if(hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_PEQUE){
                    velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_GOLPE_PEQUE : IMPULSO_GOLPE_PEQUE;
                    escalaSprite.x = (escalaSprite.x > 0) ? 0.95 : -0.95;
                        escalaSprite.y = 1.05;
                    cambiarEstado(EstadoPersonaje::GOLPEADO_PEQUE);
                } else if (hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_MEDIO){
                    velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
                    if(hitboxElegidaEnemigo.esAtaqueBajo() && puntosDeVida < MAX_PUNTOS_DE_VIDA/2){
                        velocidad.x/=2;
                        velocidad.y = IMPULSO_GOLPE_BAJO_MEDIO;
                        escalaSprite.x = (escalaSprite.x > 0) ? 0.9 : -0.9;
                        escalaSprite.y = 1.1;
                        cambiarEstado(EstadoPersonaje::GOLPEADO_SUBIENDO);
                    } else {
                        escalaSprite.x = (escalaSprite.x > 0) ? 0.9 : -0.9;
                        escalaSprite.y = 1.1;
                        cambiarEstado(EstadoPersonaje::GOLPEADO_MEDIO);
                    }
                } else {
                    velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_X_GOLPE_SUPER : IMPULSO_X_GOLPE_SUPER;
                    velocidad.y = IMPULSO_Y_GOLPE_SUPER;
                    
                    escalaSprite.x = (escalaSprite.x > 0) ? 0.9 : -0.9;
                    escalaSprite.y = 1.1;
                    cambiarEstado(EstadoPersonaje::GOLPEADO_SUBIENDO);
                }
                break;
            
            // Moviéndose al revés del enemigo o bloqueando (los ataques bajos te pegan)
            case EstadoPersonaje::ANDANDO_ALEJANDOSE:
            case EstadoPersonaje::BLOQUEANDO:
            
                bloqueoAereo:

                // Aún si estás bloqueando, los ataques bajos te dan
                if(hitboxElegidaEnemigo.esAtaqueBajo()){
                    if(hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_PEQUE){
                        velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_GOLPE_PEQUE : IMPULSO_GOLPE_PEQUE;
                        escalaSprite.x = (escalaSprite.x > 0) ? 0.95 : -0.95;
                        escalaSprite.y = 1.05;
                        cambiarEstado(EstadoPersonaje::GOLPEADO_PEQUE);
                    } else if (hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_MEDIO){
                        if(puntosDeVida < MAX_PUNTOS_DE_VIDA/2){
                            velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
                            velocidad.x/=2;
                            velocidad.y = IMPULSO_GOLPE_BAJO_MEDIO;
                            escalaSprite.x = (escalaSprite.x > 0) ? 0.9 : -0.9;
                            escalaSprite.y = 1.1;
                            cambiarEstado(EstadoPersonaje::GOLPEADO_SUBIENDO);
                        } else {
                            velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
                            escalaSprite.x = (escalaSprite.x > 0) ? 0.9 : -0.9;
                            escalaSprite.y = 1.1;
                            cambiarEstado(EstadoPersonaje::GOLPEADO_MEDIO);
                        }
                    }
                } else {
                    if(hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_PEQUE){
                        velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_GOLPE_PEQUE : IMPULSO_GOLPE_PEQUE;
                        escalaSprite.x = (escalaSprite.x > 0) ? 0.95 : -0.95;
                        escalaSprite.y = 1.05;
                        cambiarEstado(EstadoPersonaje::BLOQUEANDO);
                    } else if (hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_MEDIO){
                        velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
                        escalaSprite.x = (escalaSprite.x > 0) ? 0.95 : -0.95;
                        escalaSprite.y = 1.05;
                        cambiarEstado(EstadoPersonaje::BLOQUEANDO);
                    }
                }

                break;
            
            // Agachado (los ataques no bajos te pegan, los bajos los bloqueas)
            case EstadoPersonaje::AGACHADO:
                if(!hitboxElegidaEnemigo.esAtaqueBajo()){
                    if(hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_PEQUE){
                        velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_GOLPE_PEQUE : IMPULSO_GOLPE_PEQUE;
                        escalaSprite.x = (escalaSprite.x > 0) ? 0.95 : -0.95;
                        escalaSprite.y = 1.05;
                        cambiarEstado(EstadoPersonaje::GOLPEADO_PEQUE);
                    } else if (hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_MEDIO){
                        velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
                        escalaSprite.x = (escalaSprite.x > 0) ? 0.9 : -0.9;
                        escalaSprite.y = 1.1;
                        cambiarEstado(EstadoPersonaje::GOLPEADO_MEDIO);
                    }
                } else {
                    if(hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_PEQUE){
                        velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_GOLPE_PEQUE : IMPULSO_GOLPE_PEQUE;
                        escalaSprite.x = (escalaSprite.x > 0) ? 0.95 : -0.95;
                        escalaSprite.y = 1.05;
                        cambiarEstado(EstadoPersonaje::BLOQUEANDO);
                    } else if (hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_MEDIO){
                        velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
                        escalaSprite.x = (escalaSprite.x > 0) ? 0.95 : -0.95;
                        escalaSprite.y = 1.05;
                        cambiarEstado(EstadoPersonaje::BLOQUEANDO);
                    }
                }
                break;
            
            // Realizando el ataque súper (sufre daño pero no se mueve ni cambia de estado)
            case EstadoPersonaje::ATAQUE_SUPER:
                if(hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_PEQUE){
                    // Se le baja la vida igualmente
                } else if (hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_MEDIO){
                    // Se le baja la vida igualmente
                }
                break;
            
            // En situaciones en las que no se puede golpear otra vez (si el personaje está en
            // algun otro estado, se debería haber detenido la ejecución de esta función antes)
            default:
                break;
        }
    }

    // Ahora, se añaden efectos según haya salido la cosa

    // Primero, se va a calcular el punto exacto en el que aparecerá el efecto
    
    sf::Vector2f posicionMedia = util::centroDeInterseccion(hitboxElegidaEnemigo.getRectangulo(),hitboxElegidaPropia.getRectangulo());
    
    std::shared_ptr<Animacion> anim;

    // Ahora, se calcula la fuerza del ataque realizado por el enemigo
    int fuerzaAtaque = hitboxElegidaEnemigo.getFuerzaAtaque();

    // Si estamos bloqueando, el daño se reduce
    if(estado == EstadoPersonaje::BLOQUEANDO){
        // Si el ataque era un ataque peque, el daño es 0
        if(fuerzaAtaque <= MAX_ATAQUE_PEQUE){
            fuerzaAtaque = 0;
        }
        // Si es un ataque medio, recibimos la mitad del daño
        else if (fuerzaAtaque <= MAX_ATAQUE_MEDIO){
            fuerzaAtaque/=2;
            // Además, como estamos bloqueando, el ataque no nos puede matar
            if(fuerzaAtaque >= puntosDeVida){
                fuerzaAtaque = puntosDeVida-1;
            }
        }
        // Si es un ataque súper, el programa termina porque nunca se debería bloquear un ataque súper
        else {
            Bitacora::unicaInstancia()->escribir("Emilio: Señor Juan, ¿cómo ve usted eso de bloquear ataques súper?");
            Bitacora::unicaInstancia()->escribir("Juan: Pero qué disparetes dices, Emilio. ¿Cómo iba alguien a bloquear un ataque súper?");
            Bitacora::unicaInstancia()->escribir("Emilio: No, lo digo porque acaba de pasar. Ya usted verá qué hace.");
            Bitacora::unicaInstancia()->escribir("Juan: ¡¿Cómo dices...?!");
            exit(EXIT_FAILURE);
        }
    }
    
    // Una vez se sabe dónde ha ocurrido la colisión y cuánto daño se ha hecho, se añaden los numeritos
    if(estado == EstadoPersonaje::BLOQUEANDO){
        anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("bloqueado");
        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/personajes/" + this->nombre + "/bloqueando.ogg");
        
        for(int i=0;i < fuerzaAtaque;++i){

            auto particula = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("particula-golpeado-1");
            ((AnimacionConGravedad*)(particula.get()))->setPosicion(posicionMedia);
            ((AnimacionConGravedad*)(particula.get()))->setVelocidad(sf::Vector2f(((escalaSprite.x > 0) ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_PEQUE,-1 * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_PEQUE));
            ((AnimacionConGravedad*)(particula.get()))->setVelocidadGiro((rand()%2==0 ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_GIRO_PART);

            efectosInsertados.push_back(particula);
        }

    } else if (fuerzaAtaque <= MAX_ATAQUE_PEQUE){
        anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("golpeado-peque");
        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/personajes/" + this->nombre + "/golpeado-peque.ogg");

        for(int i=0;i < fuerzaAtaque;++i){

            auto particula = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("particula-golpeado-1");
            ((AnimacionConGravedad*)(particula.get()))->setPosicion(posicionMedia);
            ((AnimacionConGravedad*)(particula.get()))->setVelocidad(sf::Vector2f(((escalaSprite.x > 0) ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_PEQUE,-1 * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_PEQUE));
            ((AnimacionConGravedad*)(particula.get()))->setVelocidadGiro((rand()%2==0 ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_GIRO_PART);

            efectosInsertados.push_back(particula);
        }

    } else if (fuerzaAtaque <= MAX_ATAQUE_MEDIO){

        VentanaPrincipal::vibrar(VIBRACION_ATAQUE_MEDIO);

        anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("golpeado-medio");
        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/personajes/" + this->nombre + "/golpeado-medio.ogg");

        for(int i=0;i < fuerzaAtaque/2;++i){

            auto particula = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("particula-golpeado-2");
            ((AnimacionConGravedad*)(particula.get()))->setPosicion(posicionMedia);
            ((AnimacionConGravedad*)(particula.get()))->setVelocidad(sf::Vector2f(((escalaSprite.x > 0) ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_MEDIA,-1 * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_MEDIA));
            ((AnimacionConGravedad*)(particula.get()))->setVelocidadGiro((rand()%2==0 ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_GIRO_PART);

            efectosInsertados.push_back(particula);
        }

        if(fuerzaAtaque%2 == 1){
            auto particula = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("particula-golpeado-1");
            ((AnimacionConGravedad*)(particula.get()))->setPosicion(posicionMedia);
            ((AnimacionConGravedad*)(particula.get()))->setVelocidad(sf::Vector2f(((escalaSprite.x > 0) ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_PEQUE,-1 * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_PEQUE));
            ((AnimacionConGravedad*)(particula.get()))->setVelocidadGiro((rand()%2==0 ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_GIRO_PART);

            efectosInsertados.push_back(particula);
        }

    } else if(estado != EstadoPersonaje::ESQUIVE_SUPER){

        VentanaPrincipal::vibrar(VIBRACION_ATAQUE_SUPER);

        anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("golpeado-super");
        ReproductorDeSonidos::unicaInstancia()->reproducir("sonidos/personajes/" + this->nombre + "/golpeado-super.ogg");

        for(int i=0;i < fuerzaAtaque/3;++i){

            auto particula = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("particula-golpeado-3");
            ((AnimacionConGravedad*)(particula.get()))->setPosicion(posicionMedia);
            ((AnimacionConGravedad*)(particula.get()))->setVelocidad(sf::Vector2f(((escalaSprite.x > 0) ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_SUPER,-1 * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_SUPER));
            ((AnimacionConGravedad*)(particula.get()))->setVelocidadGiro((rand()%2==0 ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_GIRO_PART);

            efectosInsertados.push_back(particula);
        }

        if(fuerzaAtaque%3 == 2){
            auto particula = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("particula-golpeado-2");
            ((AnimacionConGravedad*)(particula.get()))->setPosicion(posicionMedia);
            ((AnimacionConGravedad*)(particula.get()))->setVelocidad(sf::Vector2f(((escalaSprite.x > 0) ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_MEDIA,-1 * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_MEDIA));
            ((AnimacionConGravedad*)(particula.get()))->setVelocidadGiro((rand()%2==0 ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_GIRO_PART);

            efectosInsertados.push_back(particula);
        }

        if(fuerzaAtaque%3 == 1){
            auto particula = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("particula-golpeado-1");
            ((AnimacionConGravedad*)(particula.get()))->setPosicion(posicionMedia);
            ((AnimacionConGravedad*)(particula.get()))->setVelocidad(sf::Vector2f(((escalaSprite.x > 0) ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_PEQUE,-1 * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_PEQUE));
            ((AnimacionConGravedad*)(particula.get()))->setVelocidadGiro((rand()%2==0 ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_GIRO_PART);

            efectosInsertados.push_back(particula);
        }

    } else {
        // En este último caso se ha hecho un esquive súper, por lo que no hay puntos de vida que quitar ni nada más
        // que hacer en esta función
        return;
    }

    // Después, se quitan puntos de vida correspondientes
    puntosDeVida-=fuerzaAtaque;

    // Finalmente, se sube el medidor de súper según el daño original de la hitbox
    if(estado == EstadoPersonaje::BLOQUEANDO){
        // Los ataques bloqueados aumentan el medidor de súper el doble
        medidorSuper+=(hitboxElegidaEnemigo.getFuerzaAtaque()*2);
    } else {
        medidorSuper+=hitboxElegidaEnemigo.getFuerzaAtaque();
    }

    // Los personajes derrotados se tiran al suelo
    if(puntosDeVida <= 0) {
        puntosDeVida = 0;
        if(estado != EstadoPersonaje::GOLPEADO_SUBIENDO){
            velocidad.x = (escalaSprite.x > 0) ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
            velocidad.x/=2;
            velocidad.y = IMPULSO_GOLPE_BAJO_MEDIO;
            escalaSprite.x = (escalaSprite.x > 0) ? 0.9 : -0.9;
            escalaSprite.y = 1.1;
            cambiarEstado(EstadoPersonaje::GOLPEADO_SUBIENDO);
        }
    }

    // El medidor de super no puede sobrepasar el límite
    if(medidorSuper > MAX_MEDIDOR_SUPER){
        medidorSuper = MAX_MEDIDOR_SUPER;
    }

    anim->setPosicion(posicionMedia);
    
    efectosInsertados.push_back(anim);

}
