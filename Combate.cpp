#include "Combate.hpp"
#include "VentanaPrincipal.hpp"
#include "GestorDeControles.hpp"
#include "ContenedorDePersonajes.hpp"
#include "ContenedorDeEfectos.hpp"
#include "ReproductorDeMusica.hpp"
#include "Utilidades.hpp"
#include <omp.h>
#include <iostream>
#include <list>

// Al usar initializer lists o como se diga en españolo me evito que se creen los
// personajes usando el constructor vacío para nada (porque se queja el g++ más que nada)
Combate::Combate(std::string nombrePersonajeJ1, std::string nombrePersonajeJ2, std::string nombreEscenario, sf::IpAddress direccionIP, bool lider) :
    personajeJugador1(ContenedorDePersonajes::unicaInstancia()->obtenerPersonaje(nombrePersonajeJ1)),
    personajeJugador2(ContenedorDePersonajes::unicaInstancia()->obtenerPersonaje(nombrePersonajeJ2)),
    GUIJugador1(personajeJugador1,true), GUIJugador2(personajeJugador2,false),
    escenario(ContenedorDeTexturas::unicaInstanciaTexturas()->obtener("sprites/escenarios/"+nombreEscenario+"/fondo.png"),
              ContenedorDeTexturas::unicaInstanciaTexturas()->obtener("sprites/escenarios/"+nombreEscenario+"/frente.png"),
              ContenedorDeTexturas::unicaInstanciaTexturas()->obtener("sprites/escenarios/"+nombreEscenario+"/suelo.png")), lider(lider),
    cartelTodoListo(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("cartel-todo-listo")),
    cartelAPelear(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("cartel-a-pelear")){

    rectanguloOscuro.setPosition({0,0});
    rectanguloOscuro.setSize(sf::Vector2f(VENTANA_ANCHURA,VENTANA_ALTURA));
    rectanguloOscuro.setOutlineThickness(0);
    rectanguloOscuro.setFillColor(sf::Color::Black);

    if(lider){
        if(listener.listen(NUMERO_PUERTO) != sf::Socket::Status::Done){
            exit(1);
        }
        
        if(listener.accept(socket) != sf::Socket::Status::Done){
            exit(1);
        }
    } else if (direccionIP != sf::IpAddress(0,0,0,0)){
        if(socket.connect(direccionIP,NUMERO_PUERTO) != sf::Socket::Status::Done){
            exit(1);
        }
    }

    personajeJugador1.setPosicion(VENTANA_ANCHURA/3,ALTURA_SUELO);
    personajeJugador2.setPosicion(2*VENTANA_ANCHURA/3,ALTURA_SUELO);

    cartelTodoListo->setPosicion(POSICION_CARTELES);
    cartelAPelear->setPosicion(POSICION_CARTELES);
}

void Combate::resetear(){
    // Los carteles se resetean
    cartelTodoListo->resetear();
    cartelAPelear->resetear();

    // Los personajes vuelven a su posición
    personajeJugador1.setPosicion(VENTANA_ANCHURA/3,ALTURA_SUELO);
    personajeJugador2.setPosicion(2*VENTANA_ANCHURA/3,ALTURA_SUELO);

    // Los personajes se curan y se vacía su medidor se súper
    personajeJugador1.setPuntosDeVida(MAX_PUNTOS_DE_VIDA);
    personajeJugador1.setMedidorSuper(0);
    personajeJugador2.setPuntosDeVida(MAX_PUNTOS_DE_VIDA);
    personajeJugador2.setMedidorSuper(0);

    // Las GUIs se preparan para la siguiente ronda
    GUIJugador1.restablecerVida();
    GUIJugador2.restablecerVida();

    // Los personajes pasan al estado quieto
    personajeJugador1.cambiarEstado(EstadoPersonaje::QUIETO);
    personajeJugador2.cambiarEstado(EstadoPersonaje::QUIETO);

    // Se coloca el escenario en el centro
    escenario.resetear();
}

void Combate::actualizarFramePreparandoSuper(std::list<std::shared_ptr<Animacion>> &efectos){
    
    // Sacamos la ventana principal
    sf::RenderWindow * ventana = VentanaPrincipal::unicaInstancia();

    // Se actualizan solo los personajes preparando súper
    if(personajeJugador1.getEstado() == EstadoPersonaje::PREPARANDO_SUPER){
        std::list<std::shared_ptr<Animacion>> nuevosEfectos;

        personajeJugador1.actualizar(personajeJugador2.getPosicion(),nuevosEfectos);

        for(const std::shared_ptr<Animacion> &efecto : nuevosEfectos){
            efectos.push_back(efecto);
        }
    }

    if(personajeJugador2.getEstado() == EstadoPersonaje::PREPARANDO_SUPER){
        std::list<std::shared_ptr<Animacion>> nuevosEfectos;

        personajeJugador2.actualizar(personajeJugador1.getPosicion(),nuevosEfectos);

        for(const std::shared_ptr<Animacion> &efecto : nuevosEfectos){
            efectos.push_back(efecto);
        }
    }
    
    // No se actualiza el escenario para dar el efecto de que se ha parado el tiempo

    VentanaPrincipal::actualizar();
    GUIJugador1.actualizar();
    GUIJugador2.actualizar();

    ventana->clear(sf::Color(100,100,120));

    // Se dibuja todo como de costumbre (menos los personajes que están preparando súper)
    escenario.dibujarFondo(*ventana,sf::RenderStates::Default);

    if(personajeJugador1.getEstado() != EstadoPersonaje::PREPARANDO_SUPER)
        ventana->draw(personajeJugador1);
    if(personajeJugador2.getEstado() != EstadoPersonaje::PREPARANDO_SUPER)
        ventana->draw(personajeJugador2);
    
    for(std::list<std::shared_ptr<Animacion>>::iterator iter = efectos.begin(); iter != efectos.end(); iter++){
        ventana->draw(**iter);
    }

    // Se dibuja un rectángulo oscuro encima
    sf::RectangleShape rectanguloOscuro(sf::Vector2f(VENTANA_ANCHURA,VENTANA_ALTURA));
    rectanguloOscuro.setPosition({0,0});
    rectanguloOscuro.setFillColor(sf::Color(0,0,0,100));
    ventana->draw(rectanguloOscuro);

    // Se dibujan los personajes preparando súper
    if(personajeJugador1.getEstado() == EstadoPersonaje::PREPARANDO_SUPER)
        ventana->draw(personajeJugador1);
    if(personajeJugador2.getEstado() == EstadoPersonaje::PREPARANDO_SUPER)
        ventana->draw(personajeJugador2);

    escenario.dibujarFrente(*ventana,sf::RenderStates::Default);

    ventana->draw(GUIJugador1);
    ventana->draw(GUIJugador2);

    ventana->display();
}

void Combate::recibirEntradaOffline(){

    sf::RenderWindow * ventana = VentanaPrincipal::unicaInstancia();

    while(const std::optional evento = ventana->pollEvent()){
        if(evento->is<sf::Event::Closed>()){
            ventana->close();
            exit(EXIT_SUCCESS);
        } else {
            std::pair<Jugador,Accion> par = GestorDeControles::unicaInstancia()->comprobarEvento(evento);

            Personaje& personajeElegido = par.first == Jugador::JUGADOR1 ? personajeJugador1 : personajeJugador2;

            if((dynamic_cast<AnimacionAgrandable*>(cartelAPelear.get()))->haTerminadoDeAgrandarse()){
                if(evento->is<sf::Event::KeyPressed>() || evento->is<sf::Event::JoystickButtonPressed>() || (evento->is<sf::Event::JoystickMoved>() && std::abs(evento->getIf<sf::Event::JoystickMoved>()->position) > UMBRAL_JOYSTICK)){
                    personajeElegido.realizarAccion(par.second);
                } else if(evento->is<sf::Event::KeyReleased>() || evento->is<sf::Event::JoystickButtonReleased>() || (evento->is<sf::Event::JoystickMoved>() && std::abs(evento->getIf<sf::Event::JoystickMoved>()->position) < UMBRAL_JOYSTICK)){
                    personajeElegido.detenerAccion(par.second);
                }
            }
        }
    }
}

void Combate::recibirEntradaOnline(){

    sf::RenderWindow * ventana = VentanaPrincipal::unicaInstancia();

    // Estas son máscaras de bits donde se ponen las acciones que hacemos o dejamos
    // de hacer. Será lo que se le mande al otro jugador por la red
    uint8_t accionesRealizadas = 0;
    uint8_t accionesDetenidas = 0;

    while(const std::optional evento = ventana->pollEvent()){
        if(evento->is<sf::Event::Closed>()){
            ventana->close();
            exit(EXIT_SUCCESS);
        } else {
            std::pair<Jugador,Accion> par = GestorDeControles::unicaInstancia()->comprobarEvento(evento);

            // No merece la pena prestarle atención: se han usado las teclas del otro jugador
            if((par.first == Jugador::JUGADOR1 && !lider) || (par.first == Jugador::JUGADOR2 && lider)){
                continue;
            }

            Personaje& personajeElegido = par.first == Jugador::JUGADOR1 ? personajeJugador1 : personajeJugador2;

            if((dynamic_cast<AnimacionAgrandable*>(cartelAPelear.get()))->haTerminadoDeAgrandarse()){
                if(evento->is<sf::Event::KeyPressed>() || evento->is<sf::Event::JoystickButtonPressed>() || (evento->is<sf::Event::JoystickMoved>() && std::abs(evento->getIf<sf::Event::JoystickMoved>()->position) > UMBRAL_JOYSTICK)){
                    personajeElegido.realizarAccion(par.second);
                    accionesRealizadas |= util::accionABit(par.second);
                } else if(evento->is<sf::Event::KeyReleased>() || evento->is<sf::Event::JoystickButtonReleased>() || (evento->is<sf::Event::JoystickMoved>() && std::abs(evento->getIf<sf::Event::JoystickMoved>()->position) < UMBRAL_JOYSTICK)){
                    personajeElegido.detenerAccion(par.second);
                    accionesDetenidas |= util::accionABit(par.second);
                }
            }
        }
    }

    // Se crea un paquete con las acciones realizadas y las acciones detenidas
    sf::Packet paqueteEnviado;
    paqueteEnviado << accionesRealizadas << accionesDetenidas;

    // Se crea un paquete para recibir datos
    sf::Packet paqueteRecibido;

    if(lider){
        // El lider es el que envía primero
        while(socket.send(paqueteEnviado) != sf::Socket::Status::Done);
        if(socket.receive(paqueteRecibido) != sf::Socket::Status::Done){
            exit(1);
        }
    } else {
        // El invitado envía después
        if(socket.receive(paqueteRecibido) != sf::Socket::Status::Done){
            exit(1);
        }
        while(socket.send(paqueteEnviado) != sf::Socket::Status::Done);
    }

    paqueteRecibido >> accionesRealizadas >> accionesDetenidas;

    Personaje& personajeElegido = lider ? personajeJugador2 : personajeJugador1;

    // Ahora se comprueba qué venía en el paquete

    // Acciones realizadas
    if(accionesRealizadas & BIT_ARRIBA){
        personajeElegido.realizarAccion(Accion::ARRIBA);
    }

    if(accionesRealizadas & BIT_ABAJO){
        personajeElegido.realizarAccion(Accion::ABAJO);
    }

    if(accionesRealizadas & BIT_IZQUIERDA){
        personajeElegido.realizarAccion(Accion::IZQUIERDA);
    }

    if(accionesRealizadas & BIT_DERECHA){
        personajeElegido.realizarAccion(Accion::DERECHA);
    }

    if(accionesRealizadas & BIT_ATAQUE){
        personajeElegido.realizarAccion(Accion::ATACAR);
    }

    // Acciones detenidas
    if(accionesDetenidas & BIT_ARRIBA){
        personajeElegido.detenerAccion(Accion::ARRIBA);
    }

    if(accionesDetenidas & BIT_ABAJO){
        personajeElegido.detenerAccion(Accion::ABAJO);
    }

    if(accionesDetenidas & BIT_IZQUIERDA){
        personajeElegido.detenerAccion(Accion::IZQUIERDA);
    }

    if(accionesDetenidas & BIT_DERECHA){
        personajeElegido.detenerAccion(Accion::DERECHA);
    }

    if(accionesDetenidas & BIT_ATAQUE){
        personajeElegido.detenerAccion(Accion::ATACAR);
    }
}

void Combate::actualizarPersonajesEfectosGuisEscenarioVentana(std::list<std::shared_ptr<Animacion>> &efectos, std::list<std::shared_ptr<Animacion>> &nuevosEfectos){
    // Se preparan otra lista para meter nuevos efectos (dos listas porque se quiere paralelizar la actualización
    // de los personajes, así que si digo de meter cosas en la misma lista se nos va a la mi3rda todo)
    std::list<std::shared_ptr<Animacion>> nuevosEfectosAux;

    // Aquí la ejecución se bifurca en dos hilos que se ejecutarán a la vez o a lo mejor no, quién sabe. El caso
    // es que así tarda menos digo yo, ley de Amdahl ley de Moore ley de Gustaffson transistores power wall ya
    // me entiendes
    #pragma omp parallel num_threads(2)
    {
        #pragma omp sections
        {
            #pragma omp section
            personajeJugador1.actualizar(personajeJugador2.getPosicion(),nuevosEfectos);

            #pragma omp section
            personajeJugador2.actualizar(personajeJugador1.getPosicion(),nuevosEfectosAux);
        }
    }

    // Se añaden los efectos del jugador 2 a los del jugador 1 y así tenemos solo una lista
    for(std::shared_ptr<Animacion>& anim : nuevosEfectosAux){
        nuevosEfectos.push_back(anim);
    }

    for(auto iter = efectos.begin(); iter != efectos.end();){
        if((*iter)->haTerminado()){
            iter = efectos.erase(iter);
        } else {
            (*iter)->actualizar(nuevosEfectos);
            iter++;
        }
    }

    GUIJugador1.actualizar();
    GUIJugador2.actualizar();

    escenario.actualizar(personajeJugador1,personajeJugador2,efectos);

    VentanaPrincipal::actualizar();
}

void Combate::actualizarFrameNormal(std::list<std::shared_ptr<Animacion>> &efectos){

    sf::RenderWindow * ventana = VentanaPrincipal::unicaInstancia();

    // PRIMER PASO: RECIBIR ENTRADA DEL TECLADO O DE LOS MANDOS
    // En este paso se actualizan los valores booleanos de los personajes, que
    // indican si una acción está siendo realizada o no. Se comprueba si se han terminado
    // de mostrar los dos carteles del principio del combate y entonces se puede comenzar a
    // jugar (en realidad el segundo cartel, el de "¡A pelear!" deja de bloquear el movimiento
    // de los personajes cuando se agranda a su máximo tamaño)
    if(!cartelTodoListo->haTerminado()){
        cartelTodoListo->actualizar(efectos);
    } else if(!cartelAPelear->haTerminado()){
        cartelAPelear->actualizar(efectos);
    }

    if(socket.getRemoteAddress() != sf::IpAddress(0,0,0,0))
        recibirEntradaOnline();
    else
        recibirEntradaOffline();

    // SEGUNDO PASO: ACTUALIZAR PERSONAJES, EFECTOS, GUIS, ESCENARIO Y VENTANA

    std::list<std::shared_ptr<Animacion>> nuevosEfectosA;
    std::list<std::shared_ptr<Animacion>> nuevosEfectosB;

    actualizarPersonajesEfectosGuisEscenarioVentana(efectos, nuevosEfectosA);

    // TERCER PASO: COMPROBAR COLISIONES

    // Copias de la lista de efectos para poder meter las hitboxes del enemigo para cada uno
    // de los dos personajes
    std::list<std::shared_ptr<Animacion>> efectosA(efectos);
    std::list<std::shared_ptr<Animacion>> efectosB(efectos);

    // Se mete la hitbox del otro jugador para cada lista auxiliar de efectos. Esto es un poco follón pero de esta forma me
    // aseguro que se puedan comprobar las colisiones en paralelo sin que haya problemas. Se crea una copia de la animación actual
    // de cada jugador (empiezo a pensar que paralelizar esto va a ser más porculero que hacerlo secuencial)

    efectosA.push_back(personajeJugador2.getAnimacionSegunEstado(personajeJugador2.getEstado())->clonar());
    efectosB.push_back(personajeJugador1.getAnimacionSegunEstado(personajeJugador1.getEstado())->clonar());

    #pragma omp parallel num_threads(2)
    {
        #pragma omp single
        {
            #pragma omp task
            personajeJugador1.comprobarColisiones(efectosA,nuevosEfectosA);
    
            #pragma omp task
            personajeJugador2.comprobarColisiones(efectosB,nuevosEfectosB);
        }
    }
        

    // Se añaden los efectos del jugador 2 a los del jugador 1 y así tenemos solo una lista
    for(std::shared_ptr<Animacion>& anim : nuevosEfectosB){
        nuevosEfectosA.push_back(anim);
    }

    for(auto iter = nuevosEfectosA.begin(); iter != nuevosEfectosA.end();iter++){
        efectos.push_back(*iter);
    }

    // CUARTO PASO: DIBUJAR EL ESCENARIO, LOS PERSONAJES Y LAS ANIMACIONES

    ventana->clear(sf::Color(100,100,120));
    escenario.dibujarFondo(*ventana,sf::RenderStates::Default);
    ventana->draw(personajeJugador1);
    ventana->draw(personajeJugador2);

    for(auto iter = efectos.begin(); iter != efectos.end(); iter++){
        ventana->draw(**iter);
    }

    escenario.dibujarFrente(*ventana,sf::RenderStates::Default);

    ventana->draw(GUIJugador1);
    ventana->draw(GUIJugador2);

    if(!cartelTodoListo->haTerminado()){
        ventana->draw(*cartelTodoListo);
    } else if (!cartelAPelear->haTerminado()){
        ventana->draw(*cartelAPelear);
    }

    // Lo último que se dibuja es el rectángulo que cubre el combate
    ventana->draw(rectanguloOscuro);
    
    ventana->display();
}

void Combate::actualizarFrameCelebracion(std::list<std::shared_ptr<Animacion>> &efectos, int &contadorCelebracion, Personaje &ganador){
    
    sf::RenderWindow * ventana = VentanaPrincipal::unicaInstancia();

    // PRIMER PASO: solo se recibe entrada si se cierra la ventana
    while(const std::optional evento = ventana->pollEvent()){
        if(evento->is<sf::Event::Closed>()){
            ventana->close();
            exit(EXIT_SUCCESS);
        }
    }

    // SEGUNDO PASO: ACTUALIZAR PERSONAJES Y EFECTOS

    std::list<std::shared_ptr<Animacion>> nuevosEfectos;

    actualizarPersonajesEfectosGuisEscenarioVentana(efectos, nuevosEfectos);

    for(std::shared_ptr<Animacion> &efecto : nuevosEfectos){
        efectos.push_back(efecto);
    }

    // TERCER PASO: no se comprueban colisiones porque se supone que ya se ha terminado esta ronda. En su lugar, se
    // comprueba si el personaje puede celebrar y se le dice que celebre

    // Si el contador de celebración aún no ha llegado a cero, se disminuye
    if(contadorCelebracion > 0){
        contadorCelebracion--;
    }

    // Si ya ha llegado a cero, se le dice al personaje que celebre y se reproduce la canción de fin de ronda
    else if (ganador.getEstado() != EstadoPersonaje::CELEBRANDO){
        ganador.cambiarEstado(EstadoPersonaje::CELEBRANDO);
        ReproductorDeMusica::unicaInstancia()->reproducirCancionFinRonda();
        if(personajeJugador1.getPuntosDeVida() > 0) GUIJugador1.ganarRonda();
        else GUIJugador2.ganarRonda();
    }
    // Si ya se le ha dicho que celebre, se oscurece el rectángulo si ha terminado de celebrar
    else if(ganador.getAnimacionSegunEstado(EstadoPersonaje::CELEBRANDO)->haTerminado() && !ReproductorDeMusica::unicaInstancia()->estaReproduciendo()) {
        sf::Color nuevoColor(rectanguloOscuro.getFillColor());
        nuevoColor.a+=5;
        rectanguloOscuro.setFillColor(nuevoColor);
    }

    // CUARTO PASO: DIBUJAR EL ESCENARIO, LOS PERSONAJES Y LAS ANIMACIONES

    ventana->clear(sf::Color(100,100,100));
    escenario.dibujarFondo(*ventana,sf::RenderStates::Default);
    ventana->draw(personajeJugador1);
    ventana->draw(personajeJugador2);

    for(auto iter = efectos.begin(); iter != efectos.end(); iter++){
        ventana->draw(**iter);
    }

    escenario.dibujarFrente(*ventana,sf::RenderStates::Default);

    ventana->draw(GUIJugador1);
    ventana->draw(GUIJugador2);

    ventana->draw(rectanguloOscuro);
    
    ventana->display();
}

void Combate::comenzar(){

    // En esta lista hay efectos como objetos voladores o efectos de golpe
    std::list<std::shared_ptr<Animacion>> efectos;

    // Este bucle contiene varias rondas dentro y podría ser más bonito y compacto pero prefiero
    // repetir código porque virgen santisima qué por culo sería compactarlo todo
    while(GUIJugador1.getRondasGanadas() != 2 && GUIJugador2.getRondasGanadas() != 2){
        
        // Se reproduce una canción de combate
        ReproductorDeMusica::unicaInstancia()->reproducirCancionCombate();

        // Se resetean los carteles, los personajes y el escenario
        resetear();

        // El bucle de cada ronda realiza acciones en un orden muy específico para evitar problemas

        while(personajeJugador1.getPuntosDeVida() > 0 && personajeJugador2.getPuntosDeVida() > 0){

            // Se prepara un reloj para ver cuánto tiempo pasa entre frames
            sf::Clock reloj;

            // Se aclara el rectángulo que cubre el combate
            if(rectanguloOscuro.getFillColor().a > 0){
                sf::Color nuevoColor(rectanguloOscuro.getFillColor());
                // Se le baja 10 a la transparencia hasta que sea totalmente transparente
                // (con cuidado para que el contador no se dé la vuelta)
                nuevoColor.a = nuevoColor.a < 10 ? 0 : nuevoColor.a-10;
                rectanguloOscuro.setFillColor(nuevoColor);
            }
            
            // Este es un momento especial en el que al menos uno de los personajes está preparando su súper
            // ataque, por lo que todo se pone oscuro y el tiempo se para por un momento
            if(personajeJugador1.getEstado() == EstadoPersonaje::PREPARANDO_SUPER || personajeJugador2.getEstado() == EstadoPersonaje::PREPARANDO_SUPER){
                
                actualizarFramePreparandoSuper(efectos);
                
            } else {

                actualizarFrameNormal(efectos);
            }

            // El juego se duerme hasta que dé tiempo a dibujar el siguiente frame, teniendo en cuenta
            // que se deben dibujar 60 frames por segundo y que cada frame además necesita un tiempo
            // previo de preparación para actualizar y dibujar y tal
            sf::sleep(sf::seconds(1.f/NUMERO_FPS) - reloj.reset());

        }

        // Se termina el bucle de la ronda al ser uno de los luchadores derribado. Se detiene
        // la reproducción de la canción de combate
        ReproductorDeMusica::unicaInstancia()->detener();

        // Se resetean todas las acciones para que los personajes se estén quietos
        personajeJugador1.detenerAccion(Accion::ARRIBA);
        personajeJugador1.detenerAccion(Accion::ABAJO);
        personajeJugador1.detenerAccion(Accion::IZQUIERDA);
        personajeJugador1.detenerAccion(Accion::DERECHA);
        personajeJugador1.detenerAccion(Accion::ATACAR);

        personajeJugador2.detenerAccion(Accion::ARRIBA);
        personajeJugador2.detenerAccion(Accion::ABAJO);
        personajeJugador2.detenerAccion(Accion::IZQUIERDA);
        personajeJugador2.detenerAccion(Accion::DERECHA);
        personajeJugador2.detenerAccion(Accion::ATACAR);

        // Se encuentra al jugador que ha ganado (no sé si puede haber empates)
        Personaje& ganador = personajeJugador1.getPuntosDeVida() > 0 ? personajeJugador1 : personajeJugador2;

        // Este contador disminuye en 1 cada frame y cuando llega a 0 se le indica al ganador que celebre
        int contadorCelebracion = MAX_CONTADOR_CELEBRACION;

        // El bucle termina cuando el rectángulo que cubre la pantalla se oscurezca por completo (esto ocurre después de que el personaje ganador
        // haya celebrado su victoria y se haya terminado de reproducir la canción de fin de ronda)
        while(rectanguloOscuro.getFillColor().a != 255){

            sf::Clock reloj;

            actualizarFrameCelebracion(efectos,contadorCelebracion,ganador);

            sf::sleep(sf::seconds(1.f/NUMERO_FPS) - reloj.reset());
        }
    }
}
