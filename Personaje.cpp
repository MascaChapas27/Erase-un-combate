#include "Personaje.hpp"
#include "Enums.hpp"
#include "Constantes.hpp"
#include "Utilidades.hpp"
#include "ContenedorDeEfectos.hpp"
#include "ContenedorDePersonajes.hpp"
#include "AnimacionConGravedad.hpp"
#include "VentanaPrincipal.hpp"
#include "Bitacora.hpp"
#include <iostream>

Personaje::Personaje(std::map<EstadoPersonaje,std::shared_ptr<AnimacionPorFrames>> animaciones, std::string nombre){
    puntosDeVida = MAX_PUNTOS_DE_VIDA;
    medidorSuper = 0;
    velY = 0;
    velX = 0;
    contadorTumbado = 0;
    contadorBlanco = 0;
    contadorEsquiveSuper = 0;
    this->nombre = nombre;
    this->animaciones = animaciones;
    estado = EstadoPersonaje::QUIETO;
    mirandoDerecha = true;
    shader = std::shared_ptr<sf::Shader>(new sf::Shader());
    if(!shader->loadFromFile("shaders/blendColor.frag",sf::Shader::Type::Fragment)){
        Bitacora::unicaInstancia()->escribir("ERROR: no se pudo cargar el shader");
        exit(EXIT_FAILURE);
    }
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

EstadoPersonaje Personaje::getEstado(){
    return estado;
}

void Personaje::mover(float offsetX, float offsetY){
    animaciones.at(estado)->mover(offsetX,offsetY);
}

void Personaje::setJugador(Jugador jugador){
    this->jugador = jugador;
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
    velX-=VELOCIDAD_ANDAR_AUMENTO;
    if(velX <= -VELOCIDAD_ANDAR_MAX)
        velX = -VELOCIDAD_ANDAR_MAX;
}

void Personaje::moverseDerecha(){
    // La velocidad aumenta un poco pero sin pasarse
    velX+=VELOCIDAD_ANDAR_AUMENTO;
    if(velX >= VELOCIDAD_ANDAR_MAX)
        velX = VELOCIDAD_ANDAR_MAX;
}

void Personaje::pararMovimiento(){
    // Si la velocidad es tan baja que se podría decir que es cero, termina
    // directamente para no entrar en un bucle
    if(std::abs(velX) < UMBRAL_FLOAT) return;

    // Si la velocidad es positiva se baja, si no se sube
    bool positivo = velX > 0;
    if(positivo){
        velX-=VELOCIDAD_ANDAR_AUMENTO;
    } else {
        velX+=VELOCIDAD_ANDAR_AUMENTO;
    }

    // Si la velocidad se ha pasado de 0, se pone a 0 y el personaje ya se para del todo
    if((positivo && velX < 0) ||
       (!positivo && velX > 0)) velX = 0;
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

        if(accionesRealizadas[Accion::ARRIBA])
        {
            velY = VELOCIDAD_SALTO;
            accionesRealizadas[Accion::ARRIBA] = false;
            levantarPolvo(efectosInsertados);
            cambiarEstado(EstadoPersonaje::SALTANDO_SUBIENDO);
        }
        else if (accionesRealizadas[Accion::ABAJO])
        {
            cambiarEstado(EstadoPersonaje::AGACHADO);
        }
        else if (accionesRealizadas[Accion::DERECHA])
        {
            velX+=VELOCIDAD_ANDAR_AUMENTO;
            if(velX >= VELOCIDAD_ANDAR_MAX)
                velX = VELOCIDAD_ANDAR_MAX;

            if(mirandoDerecha)
                cambiarEstado(EstadoPersonaje::ANDANDO_ACERCANDOSE);
            else
                cambiarEstado(EstadoPersonaje::ANDANDO_ALEJANDOSE);

        }
        else if (accionesRealizadas[Accion::IZQUIERDA])
        {
            velX-=VELOCIDAD_ANDAR_AUMENTO;
            if(velX <= -VELOCIDAD_ANDAR_MAX)
                velX = -VELOCIDAD_ANDAR_MAX;

            if(!mirandoDerecha)
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

        if (accionesRealizadas[Accion::ATACAR]){
            cambiarEstado(EstadoPersonaje::ATAQUE_NORMAL_1);
        } else if(accionesRealizadas[Accion::ARRIBA]){
            velY = VELOCIDAD_SALTO;
            accionesRealizadas[Accion::ARRIBA] = false;
            levantarPolvo(efectosInsertados);
            cambiarEstado(EstadoPersonaje::SALTANDO_SUBIENDO);
        } else if(accionesRealizadas[Accion::DERECHA]){
            moverseDerecha();
            if(!mirandoDerecha){
                cambiarEstado(EstadoPersonaje::ANDANDO_ALEJANDOSE);
            }
        } else if(accionesRealizadas[Accion::IZQUIERDA]){
            moverseIzquierda();
            if(mirandoDerecha){
                cambiarEstado(EstadoPersonaje::ANDANDO_ALEJANDOSE);
            }
        } else {
            pararMovimiento();
            cambiarEstado(EstadoPersonaje::QUIETO);
        }

        break;
    case EstadoPersonaje::ANDANDO_ALEJANDOSE:

        contadorEsquiveSuper++;

        if (accionesRealizadas[Accion::ATACAR]){
            contadorEsquiveSuper=0;
            cambiarEstado(EstadoPersonaje::ATAQUE_NORMAL_1);
        } else if(accionesRealizadas[Accion::ARRIBA]){
            velY = VELOCIDAD_SALTO;
            accionesRealizadas[Accion::ARRIBA] = false;
            levantarPolvo(efectosInsertados);
            contadorEsquiveSuper=0;
            cambiarEstado(EstadoPersonaje::SALTANDO_SUBIENDO);
        } else if(accionesRealizadas[Accion::DERECHA]){
            moverseDerecha();
            if(mirandoDerecha){
                contadorEsquiveSuper=0;
                cambiarEstado(EstadoPersonaje::ANDANDO_ACERCANDOSE);
            }
        } else if(accionesRealizadas[Accion::IZQUIERDA]){
            moverseIzquierda();
            if(!mirandoDerecha){
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
        velY+=GRAVEDAD;

        if(accionesRealizadas[Accion::ATACAR]) {
            detenerAccion(Accion::ATACAR);
            cambiarEstado(EstadoPersonaje::ATAQUE_AEREO);
            break;
        }
        
        if(accionesRealizadas[Accion::DERECHA]) moverseDerecha();
        else if (accionesRealizadas[Accion::IZQUIERDA]) moverseIzquierda();

        if(velY > 0)
            cambiarEstado(EstadoPersonaje::SALTANDO_BAJANDO);
        break;

    case EstadoPersonaje::SALTANDO_BAJANDO:
        velY+=GRAVEDAD;

        if(accionesRealizadas[Accion::ATACAR]) {
            detenerAccion(Accion::ATACAR);
            cambiarEstado(EstadoPersonaje::ATAQUE_AEREO);
            break;
        }

        if(accionesRealizadas[Accion::DERECHA]) moverseDerecha();
        else if (accionesRealizadas[Accion::IZQUIERDA]) moverseIzquierda();

        if(animaciones.at(estado)->getPosicion().y > ALTURA_SUELO){
            animaciones.at(estado)->setPosicion(animaciones.at(estado)->getPosicion().x,ALTURA_SUELO);
            velY = 0;
            levantarPolvo(efectosInsertados);

            cambiarEstado(EstadoPersonaje::TOCANDO_SUELO);
        }
        break;
    
    case EstadoPersonaje::ATAQUE_AEREO:
        velY+=GRAVEDAD;

        if(accionesRealizadas[Accion::DERECHA]) moverseDerecha();
        else if (accionesRealizadas[Accion::IZQUIERDA]) moverseIzquierda();

        if(animaciones.at(estado)->getPosicion().y > ALTURA_SUELO){
            animaciones.at(estado)->setPosicion(animaciones.at(estado)->getPosicion().x,ALTURA_SUELO);
            velY = 0;
            levantarPolvo(efectosInsertados);

            cambiarEstado(EstadoPersonaje::TOCANDO_SUELO);
        } else if (animaciones.at(estado)->haTerminado()){
            if(velY < 0)
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
    
    case EstadoPersonaje::GOLPEADO_PEQUE:
    case EstadoPersonaje::GOLPEADO_MEDIO:

        if(getPosicion().y < ALTURA_SUELO) {
            // Le ha pillado en el aire
            velY+=GRAVEDAD;

            if(animaciones.at(estado)->haTerminado()){
                cambiarEstado(velY < 0 ? EstadoPersonaje::SALTANDO_SUBIENDO : EstadoPersonaje::SALTANDO_BAJANDO);
            }

        } else if (velY != 0){
            // Ha caído
            velY = 0;
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
        velY+=GRAVEDAD;

        if(velY > 0)
            cambiarEstado(EstadoPersonaje::GOLPEADO_BAJANDO);
        break;
    
    case EstadoPersonaje::GOLPEADO_BAJANDO:
        velY+=GRAVEDAD;

        if(animaciones.at(estado)->getPosicion().y > ALTURA_SUELO){
            animaciones.at(estado)->setPosicion(animaciones.at(estado)->getPosicion().x,ALTURA_SUELO);
            levantarPolvo(efectosInsertados);
            velX *= 0.5;

            if(velY < VELOCIDAD_PASAR_A_TIRADO){
                velY = 0;

                cambiarEstado(EstadoPersonaje::TUMBADO);
            } else {
                velY *= -0.5;

                cambiarEstado(EstadoPersonaje::GOLPEADO_SUBIENDO);
            }
        }
        break;

    case EstadoPersonaje::TUMBADO:
        {
            velX *= 0.9;

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
                velX+=((velX > 0 ? -0.5 : 0.5) * util::realAleatorio());
            }
        }
        break;
    
    case EstadoPersonaje::LEVANTANDOSE:
        pararMovimiento();
        
        if(animaciones.at(estado)->haTerminado())
            cambiarEstado(EstadoPersonaje::QUIETO);

        break;

    case EstadoPersonaje::BLOQUEANDO:
        if(velY == 0 && animaciones.at(estado)->getPosicion().y == ALTURA_SUELO) {
            pararMovimiento();
            if(accionesRealizadas[Accion::ATACAR]){
                cambiarEstado(EstadoPersonaje::ATAQUE_NORMAL_1);
            } else if(animaciones.at(estado)->haTerminado()){
                cambiarEstado(EstadoPersonaje::QUIETO);
            }
        } else {
            velY+=GRAVEDAD;
            if(accionesRealizadas[Accion::ATACAR]){
                cambiarEstado(EstadoPersonaje::ATAQUE_AEREO);
            } else if(animaciones.at(estado)->haTerminado()){
                cambiarEstado(velY >= 0 ? EstadoPersonaje::SALTANDO_BAJANDO : EstadoPersonaje::SALTANDO_SUBIENDO);
            } else if (animaciones.at(estado)->getPosicion().y >= ALTURA_SUELO){
                animaciones.at(estado)->setPosicion(animaciones.at(estado)->getPosicion().x,ALTURA_SUELO);
                velY = 0;
                levantarPolvo(efectosInsertados);

                cambiarEstado(EstadoPersonaje::TOCANDO_SUELO);
            }
        }
        break;

    case EstadoPersonaje::AGACHADO:
        pararMovimiento();

        if(!accionesRealizadas[Accion::ABAJO]){
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
        case EstadoPersonaje::ATAQUE_AEREO:
        case EstadoPersonaje::ATAQUE_AGACHADO:
        case EstadoPersonaje::ATAQUE_SUPER:
        case EstadoPersonaje::GOLPEADO_BAJANDO:
        case EstadoPersonaje::GOLPEADO_SUBIENDO:
        case EstadoPersonaje::TUMBADO:
        case EstadoPersonaje::GOLPEADO_PEQUE:
        case EstadoPersonaje::GOLPEADO_MEDIO:
        case EstadoPersonaje::BLOQUEANDO:
            break;
        
        default:
            if((animaciones.at(estado)->getPosicion().x < posicionEnemigo.x && !mirandoDerecha) ||
                (animaciones.at(estado)->getPosicion().x > posicionEnemigo.x && mirandoDerecha)){
                    mirandoDerecha = !mirandoDerecha;
                    for(auto &[estado, anim] : animaciones){
                        anim->voltear();
                    }
                }
            break;
    }

    // Se actualiza la animación por frames del estado
    animaciones.at(estado)->actualizar(efectosInsertados);

    // Se consulta el movimiento de la animación por frames
    sf::Vector2f movimiento;
    movimiento = animaciones.at(estado)->getMovimientoFrameActual();
    velX+=movimiento.x;
    velY+=movimiento.y;
    
    // Una vez se hace todo, se aumenta la velocidad según se vea
    animaciones.at(estado)->mover(velX,velY);

    // Si el personaje se sale por la derecha, no dejar que pase
    if(animaciones.at(estado)->getPosicion().x > VENTANA_ANCHURA-1){
        animaciones.at(estado)->setPosicion(VENTANA_ANCHURA-1,animaciones.at(estado)->getPosicion().y);
        if(estado == EstadoPersonaje::GOLPEADO_BAJANDO || estado == EstadoPersonaje::GOLPEADO_SUBIENDO || estado == EstadoPersonaje::TUMBADO) velX *= -1;
        else velX = 0;
    } else if (animaciones.at(estado)->getPosicion().x < 0){
        animaciones.at(estado)->setPosicion(0,animaciones.at(estado)->getPosicion().y);
        if(estado == EstadoPersonaje::GOLPEADO_BAJANDO || estado == EstadoPersonaje::GOLPEADO_SUBIENDO || estado == EstadoPersonaje::TUMBADO) velX *= -1;
        else velX = 0;
    }

    shader->setUniform("amount",contadorBlanco/255.f);

    if(contadorBlanco > 0) {
        contadorBlanco-=3;
        if(contadorBlanco < 0) contadorBlanco = 0;
    }
}

void Personaje::comprobarColisiones(const std::list<std::shared_ptr<Animacion>> &animaciones, std::list<std::shared_ptr<Animacion>> &efectosInsertados){

    // Se sacan las hitboxes de la animación del estado actual
    std::list<Hitbox> hitboxes = this->animaciones.at(estado)->getHitboxes();

    // Si la animación del estado actual no tiene hitboxes en este momento, no hace falta comprobar nada
    if(hitboxes.size() == 0) return;

    // Si el personaje está en un estado en el que no puede ser golpeado, no hace falta comprobar nada
    switch(estado){
        case EstadoPersonaje::GOLPEADO_MEDIO:
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
                this->animaciones.at(estado)->mover(mirandoDerecha ? -0.25 : 0.25, 0);
                break;
            case EstadoPersonaje::ATAQUE_SUPER:
                break;
            default:
                this->animaciones.at(estado)->mover(mirandoDerecha ? -0.5 : 0.5, 0);
                break;
        }

        // Además, si nos estamos moviendo hacia el enemigo, la velocidad en el eje X se anula
        switch(estado){
            case EstadoPersonaje::SALTANDO_SUBIENDO:
            case EstadoPersonaje::SALTANDO_BAJANDO:
                // Si estamos saltando y el enemigo también (su hurtbox elegida está por encima
                // del suelo), y además estamos saltando hacia él, es como que nos chocamos y nos
                // dejamos de mover
                if(((mirandoDerecha && velX>0) || (!mirandoDerecha && velX<0)) && colisionVoladora){
                    velX = 0;
                }
                break;
            case EstadoPersonaje::ATAQUE_SUPER:
                // Si estamos haciendo el súper ataque de aquí no nos mueve ni Dios
                break;
            default:
                // En cualquier otro caso, dejamos de movernos
                if(((mirandoDerecha && velX>0) || (!mirandoDerecha && velX<0))){
                    velX = 0;
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
            velX = mirandoDerecha ? -IMPULSO_X_GOLPE_GRANDE : IMPULSO_X_GOLPE_GRANDE;
            velY = IMPULSO_Y_GOLPE_GRANDE;

            cambiarEstado(EstadoPersonaje::GOLPEADO_SUBIENDO);
        }
        
    } else {

        // Si no es súper, ahora sí que depende de cómo esté el personaje, pues
        // los ataques medios y pequeños se pueden esquivar
        switch(estado){

            // En situaciones en las que no se puede golpear otra vez (si el personaje está en
            // alguno de estos estados, se debería haber detenido la ejecución de esta función antes)
            case EstadoPersonaje::GOLPEADO_MEDIO:
            case EstadoPersonaje::GOLPEADO_BAJANDO:
            case EstadoPersonaje::GOLPEADO_SUBIENDO:
            case EstadoPersonaje::PREPARANDO_SUPER:
            case EstadoPersonaje::ESQUIVE_SUPER:
            case EstadoPersonaje::TUMBADO:
            case EstadoPersonaje::LEVANTANDOSE:
            case EstadoPersonaje::CELEBRANDO:
                break;

            // Atacando o sin moverse al revés del enemigo (todo te pega)
            case EstadoPersonaje::GOLPEADO_PEQUE:
            case EstadoPersonaje::QUIETO:
            case EstadoPersonaje::ANDANDO_ACERCANDOSE:
            case EstadoPersonaje::ATAQUE_NORMAL_1:
            case EstadoPersonaje::ATAQUE_NORMAL_2:
            case EstadoPersonaje::ATAQUE_NORMAL_3:
            case EstadoPersonaje::ATAQUE_AGACHADO:
            case EstadoPersonaje::TOCANDO_SUELO:
            case EstadoPersonaje::ATAQUE_AEREO:

            // Caso especial en el que puedes bloquear en el aire
            case EstadoPersonaje::SALTANDO_SUBIENDO:
            case EstadoPersonaje::SALTANDO_BAJANDO:
                if((estado == EstadoPersonaje::SALTANDO_SUBIENDO || estado == EstadoPersonaje::SALTANDO_BAJANDO) &&
                   ((mirandoDerecha && accionesRealizadas[Accion::IZQUIERDA]) || (!mirandoDerecha && accionesRealizadas[Accion::DERECHA]))){
                    // Los gotos son el diablo pero no se me ocurre nada mejor jejej
                    goto bloqueoAereo;
                }

                if(hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_PEQUE){
                    velX = mirandoDerecha ? -IMPULSO_GOLPE_PEQUE : IMPULSO_GOLPE_PEQUE;
                    cambiarEstado(EstadoPersonaje::GOLPEADO_PEQUE);
                } else if (hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_MEDIO){
                    velX = mirandoDerecha ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
                    if(hitboxElegidaEnemigo.esAtaqueBajo() && puntosDeVida < MAX_PUNTOS_DE_VIDA/2){
                        velX/=2;
                        velY = IMPULSO_GOLPE_BAJO_MEDIO;
                        cambiarEstado(EstadoPersonaje::GOLPEADO_SUBIENDO);
                    } else {
                        cambiarEstado(EstadoPersonaje::GOLPEADO_MEDIO);
                    }
                } else {
                    velX = mirandoDerecha ? -IMPULSO_X_GOLPE_GRANDE : IMPULSO_X_GOLPE_GRANDE;
                    velY = IMPULSO_Y_GOLPE_GRANDE;

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
                        velX = mirandoDerecha ? -IMPULSO_GOLPE_PEQUE : IMPULSO_GOLPE_PEQUE;
                        cambiarEstado(EstadoPersonaje::GOLPEADO_PEQUE);
                    } else if (hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_MEDIO){
                        if(puntosDeVida < MAX_PUNTOS_DE_VIDA/2){
                            velX = mirandoDerecha ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
                            velX/=2;
                            velY = IMPULSO_GOLPE_BAJO_MEDIO;
                            cambiarEstado(EstadoPersonaje::GOLPEADO_SUBIENDO);
                        } else {
                            velX = mirandoDerecha ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
                            cambiarEstado(EstadoPersonaje::GOLPEADO_MEDIO);
                        }
                    }
                } else {
                    if(hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_PEQUE){
                        velX = mirandoDerecha ? -IMPULSO_GOLPE_PEQUE : IMPULSO_GOLPE_PEQUE;
                        cambiarEstado(EstadoPersonaje::BLOQUEANDO);
                    } else if (hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_MEDIO){
                        velX = mirandoDerecha ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
                        cambiarEstado(EstadoPersonaje::BLOQUEANDO);
                    }
                }

                break;
            
            // Agachado (los ataques no bajos te pegan, los bajos los bloqueas)
            case EstadoPersonaje::AGACHADO:
                if(!hitboxElegidaEnemigo.esAtaqueBajo()){
                    if(hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_PEQUE){
                        velX = mirandoDerecha ? -IMPULSO_GOLPE_PEQUE : IMPULSO_GOLPE_PEQUE;
                        cambiarEstado(EstadoPersonaje::GOLPEADO_PEQUE);
                    } else if (hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_MEDIO){
                        velX = mirandoDerecha ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
                        cambiarEstado(EstadoPersonaje::GOLPEADO_MEDIO);
                    }
                } else {
                    if(hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_PEQUE){
                        velX = mirandoDerecha ? -IMPULSO_GOLPE_PEQUE : IMPULSO_GOLPE_PEQUE;
                        cambiarEstado(EstadoPersonaje::BLOQUEANDO);
                    } else if (hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_MEDIO){
                        velX = mirandoDerecha ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
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
        }
    }

    // Ahora, se añaden efectos según haya salido la cosa

    // Primero, se va a calcular el punto exacto en el que aparecerá el efecto
    
    sf::Vector2f posicionMedia = util::centroDeInterseccion(hitboxElegidaEnemigo.getRectangulo(),hitboxElegidaPropia.getRectangulo());
    
    std::shared_ptr<Animacion> anim;
    
    // Una vez se sabe dónde se va a colocar, se comprueba cómo está el personaje ahora mismo
    if(estado == EstadoPersonaje::BLOQUEANDO){
        anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("bloqueado");
    } else if (hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_PEQUE){
        anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("golpeado-peque");

        for(int i(0);i < NUM_PARTICULAS_GOLPEADO_PEQUE;++i){
            int j(1+rand()%TIPO_PEQUE_CUANTAS_PARTICULAS);

            auto particula = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("particula-golpeado-peque-"+std::to_string(j));
            ((AnimacionConGravedad*)(particula.get()))->setPosicion(posicionMedia);
            ((AnimacionConGravedad*)(particula.get()))->setVelocidad(sf::Vector2f((mirandoDerecha ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_PEQUE,-1 * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_PEQUE));
            ((AnimacionConGravedad*)(particula.get()))->setVelocidadGiro((rand()%2==0 ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_GIRO_PART);

            efectosInsertados.push_back(particula);
        }

    } else if (hitboxElegidaEnemigo.getFuerzaAtaque() <= MAX_ATAQUE_MEDIO){

        VentanaPrincipal::vibrar(VIBRACION_ATAQUE_MEDIO);

        anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("golpeado-medio");

        for(int i(0);i < NUM_PARTICULAS_GOLPEADO_MEDIO;++i){
            int j(1+rand()%TIPO_MEDIO_CUANTAS_PARTICULAS);

            auto particula = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("particula-golpeado-medio-"+std::to_string(j));
            ((AnimacionConGravedad*)(particula.get()))->setPosicion(posicionMedia);
            ((AnimacionConGravedad*)(particula.get()))->setVelocidad(sf::Vector2f((mirandoDerecha ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_MEDIA,-1 * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_MEDIA));
            ((AnimacionConGravedad*)(particula.get()))->setVelocidadGiro((rand()%2==0 ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_GIRO_PART);

            efectosInsertados.push_back(particula);
        }

    } else if(estado != EstadoPersonaje::ESQUIVE_SUPER){

        VentanaPrincipal::vibrar(VIBRACION_ATAQUE_SUPER);

        anim = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("golpeado-grande");

        for(int i(0);i < NUM_PARTICULAS_GOLPEADO_GRANDE;++i){
            int j(1+rand()%TIPO_GRANDE_CUANTAS_PARTICULAS);

            auto particula = ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("particula-golpeado-grande-"+std::to_string(j));
            ((AnimacionConGravedad*)(particula.get()))->setPosicion(posicionMedia);
            ((AnimacionConGravedad*)(particula.get()))->setVelocidad(sf::Vector2f((mirandoDerecha ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_GRANDE,-1 * util::realAleatorio()*MAX_VELOCIDAD_PARTICULA_GRANDE));
            ((AnimacionConGravedad*)(particula.get()))->setVelocidadGiro((rand()%2==0 ? -1 : 1) * util::realAleatorio()*MAX_VELOCIDAD_GIRO_PART);

            efectosInsertados.push_back(particula);
        }

    } else {
        // En este último caso se ha hecho un esquive súper, por lo que no hay puntos de vida que quitar ni nada más
        // que hacer en esta función
        return;
    }

    // Finalmente, se quitan puntos de vida según se vea
    if(estado == EstadoPersonaje::BLOQUEANDO){
        // Al bloquear, los ataques pequeños pasan a no hacer nada de daño, mientras que
        // los ataques medios hacen la mitad de daño (se supone que los ataques súper son
        // inesquivables, por lo que no hay que tenerlos en cuenta)
        if(hitboxElegidaEnemigo.getFuerzaAtaque() > MAX_ATAQUE_PEQUE){
            puntosDeVida-=(hitboxElegidaEnemigo.getFuerzaAtaque()/2);
        }

        // Los ataques bloqueados aumentan el medidor de súper al doble
        medidorSuper+=(hitboxElegidaEnemigo.getFuerzaAtaque()*2);
    } else {
        puntosDeVida-=hitboxElegidaEnemigo.getFuerzaAtaque();
        medidorSuper+=hitboxElegidaEnemigo.getFuerzaAtaque();
    }

    // Los personajes derrotados se tiran al suelo
    if(puntosDeVida <= 0) {
        puntosDeVida = 0;
        if(estado != EstadoPersonaje::GOLPEADO_SUBIENDO){
            velX = mirandoDerecha ? -IMPULSO_GOLPE_MEDIO : IMPULSO_GOLPE_MEDIO;
            velX/=2;
            velY = IMPULSO_GOLPE_BAJO_MEDIO;
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
