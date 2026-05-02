#include "Combate.hpp"
#include "VentanaPrincipal.hpp"
#include "GestorDeControles.hpp"
#include "ContenedorDePersonajes.hpp"
#include "ContenedorDeEfectos.hpp"
#include "ReproductorDeMusica.hpp"
#include "Utilidades.hpp"
#include "ContenedorDeCombos.hpp"
#include "Configuracion.hpp"
#include <omp.h>
#include <iostream>
#include <list>
#include <ClienteDiscord.hpp>

// Al usar initializer lists o como se diga en españolo me evito que se creen los
// personajes usando el constructor vacío para nada (porque se queja el g++ más que nada)
Combate::Combate(std::string nombrePersonajeJ1, std::string nombrePersonajeJ2, std::string nombreEscenario, sf::IpAddress direccionIP, bool lider) :
    personajeJugador1(ContenedorDePersonajes::unicaInstancia()->obtenerPersonaje(nombrePersonajeJ1)),
    personajeJugador2(ContenedorDePersonajes::unicaInstancia()->obtenerPersonaje(nombrePersonajeJ2)),
    GUIJugador1(personajeJugador1, true), GUIJugador2(personajeJugador2, false),
    escenario(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/escenarios/" + nombreEscenario + ".png")),
    contadorHitstop(0),
    cartelTodoListo(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("cartel-todo-listo")),
    cartelAPelear(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("cartel-a-pelear")),
    cartelJugador1Gana(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("cartel-jugador-1-gana")),
    cartelJugador1Perfecto(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("cartel-jugador-1-perfecto")),
    cartelJugador2Gana(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("cartel-jugador-2-gana")),
    cartelJugador2Perfecto(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("cartel-jugador-2-perfecto")),
    cartelEmpate(ContenedorDeEfectos::unicaInstancia()->obtenerEfecto("cartel-empate")),
    temporizador(ContenedorDeTexturas::unicaInstancia()->obtener("sprites/gui/temporizador.png"),ContenedorDeFuentes::unicaInstancia()->obtener("fuentes/daniela.ttf"),FOTOGRAMAS_POR_TICK_TEMPORIZADOR,TICKS_CONTADOR_TEMPORIZADOR)
{
    personajeJugador1.setPunteroHitstop(&contadorHitstop);
    personajeJugador2.setPunteroHitstop(&contadorHitstop);

    rectanguloOscuro.setPosition({0, 0});
    rectanguloOscuro.setSize(sf::Vector2f(VENTANA_ANCHURA, VENTANA_ALTURA));
    rectanguloOscuro.setOutlineThickness(0);
    rectanguloOscuro.setFillColor(sf::Color::Black);

    if (direccionIP != sf::IpAddress(0, 0, 0, 0))
    {
        conector.emplace(direccionIP, lider);
    }

    personajeJugador1.setPosicion(VENTANA_ANCHURA / 3, ALTURA_SUELO);
    personajeJugador1.setJugador(Jugador::JUGADOR1);
    personajeJugador2.setJugador(Jugador::JUGADOR2);
    personajeJugador2.setPosicion(2 * VENTANA_ANCHURA / 3, ALTURA_SUELO);

    personajeJugador2.voltear();

    cartelTodoListo->setPosicion(POSICION_CARTELES_COMBATE);
    cartelAPelear->setPosicion(POSICION_CARTELES_COMBATE);
    cartelJugador1Gana->setPosicion(POSICION_CARTELES_COMBATE);
    cartelJugador1Perfecto->setPosicion(POSICION_CARTELES_COMBATE);
    cartelJugador2Gana->setPosicion(POSICION_CARTELES_COMBATE);
    cartelJugador2Perfecto->setPosicion(POSICION_CARTELES_COMBATE);
    cartelEmpate->setPosicion(POSICION_CARTELES_COMBATE);
}

void Combate::resetear()
{
    // Los carteles se resetean
    cartelTodoListo->resetear();
    cartelAPelear->resetear();
    cartelJugador1Gana->resetear();
    cartelJugador1Perfecto->resetear();
    cartelJugador2Gana->resetear();
    cartelJugador2Perfecto->resetear();
    cartelEmpate->resetear();

    // Los personajes vuelven a su posición
    personajeJugador1.setPosicion(VENTANA_ANCHURA / 3, ALTURA_SUELO);
    personajeJugador2.setPosicion(2 * VENTANA_ANCHURA / 3, ALTURA_SUELO);

    // Los personajes se curan y se vacía su medidor se súper
    personajeJugador1.curarAlMaximo();
    personajeJugador1.setMedidorSuper(0);
    personajeJugador2.curarAlMaximo();
    personajeJugador2.setMedidorSuper(0);

    // Las GUIs se preparan para la siguiente ronda
    GUIJugador1.restablecerVida();
    GUIJugador2.restablecerVida();

    // Los personajes pasan al estado quieto
    personajeJugador1.cambiarEstado(EstadoPersonaje::QUIETO);
    personajeJugador2.cambiarEstado(EstadoPersonaje::QUIETO);

    // Las animaciones de los personajes se resetean


    // Se coloca el escenario en el centro
    escenario.resetear();

    // El temporizador se resetea
    temporizador.resetear();
}

void Combate::actualizarFotogramaPreparandoSuper(std::list<std::shared_ptr<Animacion>> &efectos)
{

    // Sacamos la ventana principal
    sf::RenderWindow *ventana = VentanaPrincipal::unicaInstancia();

    // Se actualizan solo los personajes preparando súper
    if (personajeJugador1.getEstado() == EstadoPersonaje::PREPARANDO_SUPER)
    {
        std::list<std::shared_ptr<Animacion>> nuevosEfectos;

        personajeJugador1.actualizar(personajeJugador2.getPosicion(), nuevosEfectos);

        for (const std::shared_ptr<Animacion> &efecto : nuevosEfectos)
        {
            efectos.push_back(efecto);
        }
    }

    if (personajeJugador2.getEstado() == EstadoPersonaje::PREPARANDO_SUPER)
    {
        std::list<std::shared_ptr<Animacion>> nuevosEfectos;

        personajeJugador2.actualizar(personajeJugador1.getPosicion(), nuevosEfectos);

        for (const std::shared_ptr<Animacion> &efecto : nuevosEfectos)
        {
            efectos.push_back(efecto);
        }
    }

    // No se actualiza el escenario para dar el efecto de que se ha parado el tiempo

    VentanaPrincipal::actualizar();

    ventana->clear(sf::Color(100, 100, 120));

    // Se dibuja todo como de costumbre (menos los personajes que están preparando súper)
    ventana->draw(escenario);

    if (personajeJugador1.getEstado() != EstadoPersonaje::PREPARANDO_SUPER)
        ventana->draw(personajeJugador1);
    if (personajeJugador2.getEstado() != EstadoPersonaje::PREPARANDO_SUPER)
        ventana->draw(personajeJugador2);

    ventana->draw(GUIJugador1);
    ventana->draw(GUIJugador2);

    for (std::list<std::shared_ptr<Animacion>>::iterator iter = efectos.begin(); iter != efectos.end(); iter++)
    {
        ventana->draw(**iter);
    }

    ventana->draw(*ContenedorDeCombos::unicaInstancia());

    ventana->draw(temporizador);

    // Se dibuja un rectángulo oscuro encima
    sf::RectangleShape rectanguloOscuro(sf::Vector2f(VENTANA_ANCHURA, VENTANA_ALTURA));
    rectanguloOscuro.setPosition({0, 0});
    rectanguloOscuro.setFillColor(sf::Color(0, 0, 0, 100));
    ventana->draw(rectanguloOscuro);

    // Se dibujan los personajes preparando súper
    if (personajeJugador1.getEstado() == EstadoPersonaje::PREPARANDO_SUPER)
        ventana->draw(personajeJugador1);
    if (personajeJugador2.getEstado() == EstadoPersonaje::PREPARANDO_SUPER)
        ventana->draw(personajeJugador2);

    ventana->display();
}

void Combate::recibirEntradaPlayerVSPlayerOffline()
{

    sf::RenderWindow *ventana = VentanaPrincipal::unicaInstancia();

    while (const std::optional evento = ventana->pollEvent())
    {
        if (evento->is<sf::Event::Closed>())
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            InfoEvento infoEvento = GestorDeControles::unicaInstancia()->comprobarEvento(evento);

            // Puede ser que la acción sea inválida, hay que comprobar que el jugador no es NADIE
            if (infoEvento.jugador != Jugador::NADIE)
            {

                Personaje &personajeElegido = infoEvento.jugador == Jugador::JUGADOR1 ? personajeJugador1 : personajeJugador2;

                if ((dynamic_cast<AnimacionAgrandable *>(cartelTodoListo.get()))->haTerminado())
                {
                    if (infoEvento.realizada)
                        personajeElegido.realizarAccion(infoEvento.accion);
                    else
                        personajeElegido.detenerAccion(infoEvento.accion);
                }
            }
        }
    }
}

void Combate::recibirEntradaPlayerVSBot()
{

    sf::RenderWindow *ventana = VentanaPrincipal::unicaInstancia();

    while (const std::optional evento = ventana->pollEvent())
    {
        if (evento->is<sf::Event::Closed>())
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            InfoEvento infoEvento = GestorDeControles::unicaInstancia()->comprobarEvento(evento);

            // Si se pulsa una tecla que no es del jugador 1 da igual, se comprueba la siguiente tecla
            if (infoEvento.jugador != Jugador::JUGADOR1)
                continue;

            if ((dynamic_cast<AnimacionAgrandable *>(cartelTodoListo.get()))->haTerminado())
            {
                if (infoEvento.realizada)
                    personajeJugador1.realizarAccion(infoEvento.accion);
                else
                    personajeJugador1.detenerAccion(infoEvento.accion);
            }
        }
    }

    if ((dynamic_cast<AnimacionAgrandable *>(cartelTodoListo.get()))->haTerminado())
    {
        if (std::rand() % 2 == 0)
        {
            personajeJugador2.realizarAccion(Accion::ATACAR);
        }
        else
        {
            personajeJugador2.detenerAccion(Accion::ATACAR);
        }

        if (std::rand() % 10 == 0)
        {
            personajeJugador2.realizarAccion(Accion::ARRIBA);
        }
        else
        {
            personajeJugador2.detenerAccion(Accion::ARRIBA);
        }

        if (std::rand() % 8 == 0)
        {
            if (personajeJugador2.isMirandoDerecha())
            {
                personajeJugador2.realizarAccion(Accion::DERECHA);
                personajeJugador2.detenerAccion(Accion::IZQUIERDA);
            }
            else
            {
                personajeJugador2.realizarAccion(Accion::IZQUIERDA);
                personajeJugador2.detenerAccion(Accion::DERECHA);
            }
        }

        if (std::rand() % 8 == 0)
        {
            personajeJugador2.realizarAccion(Accion::ABAJO);
        }
        else
        {
            personajeJugador2.detenerAccion(Accion::ABAJO);
        }
    }
}

void Combate::recibirEntradaPlayerVSPlayerOnline()
{

    sf::RenderWindow *ventana = VentanaPrincipal::unicaInstancia();

    // Aquí se ponen las acciones que se hacen o se dejan de hacer
    std::unordered_set<Accion> accionesRealizadas;
    std::unordered_set<Accion> accionesDetenidas;

    while (const std::optional evento = ventana->pollEvent())
    {
        if (evento->is<sf::Event::Closed>())
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            InfoEvento infoEvento = GestorDeControles::unicaInstancia()->comprobarEvento(evento);

            // No merece la pena prestarle atención: se han usado las teclas del otro jugador
            if ((infoEvento.jugador == Jugador::JUGADOR1 && !conector->isLider()) || (infoEvento.jugador == Jugador::JUGADOR2 && conector->isLider()))
            {
                continue;
            }

            if ((dynamic_cast<AnimacionAgrandable *>(cartelTodoListo.get()))->haTerminado())
            {
                if (infoEvento.realizada)
                    accionesRealizadas.insert(infoEvento.accion);
                else
                    accionesDetenidas.insert(infoEvento.accion);
            }
        }
    }

    // Ahora que sabemos lo que ha hecho el jugador local, lo tenemos que mandar por los interneses
    // para que el otro lo sepa también, y tenemos que saber qué ha hecho el otro para poder estar
    // sincronizados y que todo vaya bien
    AccionesOnline accionesOnline(conector->enviarRecibirAcciones(accionesRealizadas, accionesDetenidas));

    Personaje &personajeLocal = conector->isLider() ? personajeJugador2 : personajeJugador1;
    Personaje &personajeRemoto = conector->isLider() ? personajeJugador1 : personajeJugador2;

    // Ahora se comprueban las acciones que debería realizar cada personaje

    for (const Accion &a : accionesOnline.accionesRealizadasLocal)
        personajeLocal.realizarAccion(a);

    for (const Accion &a : accionesOnline.accionesDetenidasLocal)
        personajeLocal.detenerAccion(a);

    for (const Accion &a : accionesOnline.accionesRealizadasRemoto)
        personajeRemoto.realizarAccion(a);

    for (const Accion &a : accionesOnline.accionesDetenidasRemoto)
        personajeRemoto.detenerAccion(a);
}

void Combate::actualizarPersonajesEfectosGuisEscenarioVentanaTemporizador(std::list<std::shared_ptr<Animacion>> &efectos, std::list<std::shared_ptr<Animacion>> &nuevosEfectos, bool actualizarTemporizador)
{
    // Se preparan otra lista para meter nuevos efectos (dos listas porque se quiere paralelizar la actualización
    // de los personajes, así que si digo de meter cosas en la misma lista se nos va a la mi3rda todo)
    std::list<std::shared_ptr<Animacion>> nuevosEfectosAux;

    // Aquí la ejecución se bifurca en dos hilos que se ejecutarán a la vez o a lo mejor no, quién sabe. El caso
    // es que así tarda menos digo yo, ley de Amdahl ley de Moore ley de Gustaffson transistores power wall ya
    // me entiendes

    sf::Vector2f posicionJugador1(personajeJugador1.getPosicion());
    sf::Vector2f posicionJugador2(personajeJugador2.getPosicion());

    #pragma omp parallel num_threads(2)
    {
        #pragma omp sections
        {
            #pragma omp section
            personajeJugador1.actualizar(posicionJugador2, nuevosEfectos);

            #pragma omp section
            personajeJugador2.actualizar(posicionJugador1, nuevosEfectosAux);
        }
    }

    // Se añaden los efectos del jugador 2 a los del jugador 1 y así tenemos solo una lista
    for (std::shared_ptr<Animacion> &anim : nuevosEfectosAux)
    {
        nuevosEfectos.push_back(anim);
    }

    for (auto iter = efectos.begin(); iter != efectos.end();)
    {
        if ((*iter)->haTerminado())
        {
            iter = efectos.erase(iter);
        }
        else
        {
            (*iter)->actualizar(nuevosEfectos);
            iter++;
        }
    }

    GUIJugador1.actualizar();
    GUIJugador2.actualizar();

    escenario.actualizar(personajeJugador1, personajeJugador2, efectos);

    VentanaPrincipal::actualizar();

    if(actualizarTemporizador)
        temporizador.actualizar();
}

void Combate::actualizarFotogramaNormal(std::list<std::shared_ptr<Animacion>> &efectos)
{
    sf::RenderWindow *ventana = VentanaPrincipal::unicaInstancia();

    // PRIMER PASO: RECIBIR ENTRADA DEL TECLADO O DE LOS MANDOS
    // En este paso se actualizan los valores booleanos de los personajes, que
    // indican si una acción está siendo realizada o no. Se comprueba si se han terminado
    // de mostrar los dos carteles del principio del combate y entonces se puede comenzar a
    // jugar (en realidad el segundo cartel, el de "¡A pelear!" deja de bloquear el movimiento
    // de los personajes cuando se agranda a su máximo tamaño)
    if (!cartelTodoListo->haTerminado())
    {
        cartelTodoListo->actualizar(efectos);
    }
    else if (!cartelAPelear->haTerminado())
    {
        cartelAPelear->actualizar(efectos);
    }

    recibirEntradaPlayerVSPlayerOffline();

    // if (conector.has_value())
    //     recibirEntradaPlayerVSPlayerOnline();
    // else
    //     recibirEntradaPlayerVSPlayerOffline();

    // SEGUNDO PASO: ACTUALIZAR PERSONAJES, EFECTOS, GUIS, ESCENARIO, VENTANA Y TEMPORIZADOR

    std::list<std::shared_ptr<Animacion>> nuevosEfectosA;
    std::list<std::shared_ptr<Animacion>> nuevosEfectosB;

    // Solo se actualiza el temporizador si ha terminado el cartelito de "¿Todo listo?", porque mientras
    // este cartel está en pantalla los personajes no se pueden mover y es un poco trampa
    actualizarPersonajesEfectosGuisEscenarioVentanaTemporizador(efectos, nuevosEfectosA, cartelTodoListo->haTerminado());

    // TERCER PASO: COMPROBAR COLISIONES

    // Copias de la lista de efectos para poder meter las hitboxes del enemigo para cada uno
    // de los dos personajes
    std::list<std::shared_ptr<Animacion>> efectosA(efectos);
    std::list<std::shared_ptr<Animacion>> efectosB(efectos);

    // Se mete la hitbox del otro jugador para cada lista auxiliar de efectos. Esto es un poco follón pero de esta forma me
    // aseguro que se puedan comprobar las colisiones en paralelo sin que haya problemas. Se crea una copia de la animación actual
    // de cada jugador (empiezo a pensar que paralelizar esto va a ser más porculero que hacerlo secuencial)

    efectosA.push_back(personajeJugador2.getAnimacionSegunEstado(personajeJugador2.getEstado()));
    efectosB.push_back(personajeJugador1.getAnimacionSegunEstado(personajeJugador1.getEstado()));

    // bool golpeadoMedioAntes = personajeJugador1.getEstado() == EstadoPersonaje::GOLPEADO_MEDIO || personajeJugador2.getEstado() == EstadoPersonaje::GOLPEADO_MEDIO;

    #pragma omp parallel num_threads(2)
    {
        #pragma omp single
        {
            #pragma omp task
            personajeJugador1.comprobarColisiones(efectosA, nuevosEfectosA);

            #pragma omp task
            personajeJugador2.comprobarColisiones(efectosB, nuevosEfectosB);
        }
    }

    // Si el contador de Hitstop es mayor que 0, se baja en 1
    if(contadorHitstop > 0){
        contadorHitstop--;
    }

    // bool golpeadoMedioDespues = personajeJugador1.getEstado() == EstadoPersonaje::GOLPEADO_MEDIO || personajeJugador2.getEstado() == EstadoPersonaje::GOLPEADO_MEDIO;

    // Se añaden los efectos del jugador 2 a los del jugador 1 y así tenemos solo una lista
    for (std::shared_ptr<Animacion> &anim : nuevosEfectosB)
    {
        nuevosEfectosA.push_back(anim);
    }

    for (auto iter = nuevosEfectosA.begin(); iter != nuevosEfectosA.end(); iter++)
    {
        efectos.push_back(*iter);
    }

    // Se actualizan los combos
    ContenedorDeCombos::unicaInstancia()->actualizar();

    // CUARTO PASO: DIBUJAR EL ESCENARIO, LOS PERSONAJES, LAS ANIMACIONES, LAS GUIS Y EL TEMPORIZADO

    ventana->clear(sf::Color(100, 100, 120));
    ventana->draw(escenario);

    // Dependiendo de qué personaje esté atacando y cuál no, se dibuja uno
    // antes de dibujar otro para que no sea siempre el jugador 2 el que tape
    // al jugador 1
    int prioridadDibujoJugador1 = util::getPrioridadDibujo(personajeJugador1.getEstado());
    int prioridadDibujoJugador2 = util::getPrioridadDibujo(personajeJugador2.getEstado());

    // Puede parecer raro que si el jugador 1 tiene más prioridad se dibuje
    // después, pero esto ocurre porque, al dibujarse después, se dibuja encima
    // del jugador 2, haciendo que el jugador 1 esté en frente. A eso se refiere
    // la prioridad más bien
    if (prioridadDibujoJugador1 > prioridadDibujoJugador2)
    {
        ventana->draw(personajeJugador2);
        ventana->draw(personajeJugador1);
    }
    else
    {
        ventana->draw(personajeJugador1);
        ventana->draw(personajeJugador2);
    }

    ventana->draw(GUIJugador1);
    ventana->draw(GUIJugador2);

    for (auto iter = efectos.begin(); iter != efectos.end(); iter++)
    {
        ventana->draw(**iter);
    }

    ventana->draw(*ContenedorDeCombos::unicaInstancia());

    ventana->draw(temporizador);

    if (!cartelTodoListo->haTerminado())
    {
        ventana->draw(*cartelTodoListo);
    }
    else if (!cartelAPelear->haTerminado())
    {
        ventana->draw(*cartelAPelear);
    }

    // Lo último que se dibuja es el rectángulo que cubre el combate
    ventana->draw(rectanguloOscuro);

    ventana->display();

    // if(!golpeadoMedioAntes && golpeadoMedioDespues)
    // {
    //     sf::sleep(sf::seconds(6.f / Configuracion::unicaInstancia()->getFPS()));
    // }
}

void Combate::actualizarFotogramaCelebracion(std::list<std::shared_ptr<Animacion>> &efectos)
{
    sf::RenderWindow *ventana = VentanaPrincipal::unicaInstancia();

    // PRIMER PASO: solo se recibe entrada si se cierra la ventana
    while (const std::optional evento = ventana->pollEvent())
    {
        if (evento->is<sf::Event::Closed>())
        {
            exit(EXIT_SUCCESS);
        }
    }

    // SEGUNDO PASO: ACTUALIZAR PERSONAJES Y EFECTOS

    std::list<std::shared_ptr<Animacion>> nuevosEfectos;

    // El temporizador no se actualiza porque el combate ya ha terminado
    actualizarPersonajesEfectosGuisEscenarioVentanaTemporizador(efectos, nuevosEfectos, false);

    // Se actualiza el cartel de personaje ganador si está celebrando
    if (personajeJugador1.getPuntosDeVida() > personajeJugador2.getPuntosDeVida() && personajeJugador1.getEstado() == EstadoPersonaje::CELEBRANDO)
    {
        if(personajeJugador1.getPuntosDeVida() == personajeJugador1.getMaxPuntosDeVida())
            cartelJugador1Perfecto->actualizar(nuevosEfectos);
        else
            cartelJugador1Gana->actualizar(nuevosEfectos);
    }
    else if (personajeJugador2.getPuntosDeVida() > personajeJugador1.getPuntosDeVida() && personajeJugador2.getEstado() == EstadoPersonaje::CELEBRANDO)
    {
        if(personajeJugador2.getPuntosDeVida() == personajeJugador2.getMaxPuntosDeVida())
            cartelJugador2Perfecto->actualizar(nuevosEfectos);
        else
            cartelJugador2Gana->actualizar(nuevosEfectos);
    }
    else if (personajeJugador1.getPuntosDeVida() == personajeJugador2.getPuntosDeVida()
            &&
                (
                (personajeJugador1.getEstado() == EstadoPersonaje::DERROTA && personajeJugador2.getEstado() == EstadoPersonaje::DERROTA)
                ||
                (personajeJugador1.getPuntosDeVida() == 0 && personajeJugador2.getPuntosDeVida() == 0 && personajeJugador1.getEstado() == EstadoPersonaje::TUMBADO && personajeJugador2.getEstado() == EstadoPersonaje::TUMBADO)
                )
            )
    {
        cartelEmpate->actualizar(nuevosEfectos);
    }

    for (std::shared_ptr<Animacion> &efecto : nuevosEfectos)
    {
        efectos.push_back(efecto);
    }

    // TERCER PASO: COMPROBAR COLISIONES

    // Copias de la lista de efectos para poder meter las hitboxes del enemigo para cada uno
    // de los dos personajes
    std::list<std::shared_ptr<Animacion>> efectosA(efectos);
    std::list<std::shared_ptr<Animacion>> efectosB(efectos);

    // Se mete la hitbox del otro jugador para cada lista auxiliar de efectos. Esto es un poco follón pero de esta forma me
    // aseguro que se puedan comprobar las colisiones en paralelo sin que haya problemas. Se crea una copia de la animación actual
    // de cada jugador (empiezo a pensar que paralelizar esto va a ser más porculero que hacerlo secuencial)

    efectosA.push_back(personajeJugador2.getAnimacionSegunEstado(personajeJugador2.getEstado()));
    efectosB.push_back(personajeJugador1.getAnimacionSegunEstado(personajeJugador1.getEstado()));

    // Aquí se meten los nuevos efectos que se generen al actualizar los personajes
    std::list<std::shared_ptr<Animacion>> nuevosEfectosA;
    std::list<std::shared_ptr<Animacion>> nuevosEfectosB;

    // bool golpeadoMedioAntes = personajeJugador1.getEstado() == EstadoPersonaje::GOLPEADO_MEDIO || personajeJugador2.getEstado() == EstadoPersonaje::GOLPEADO_MEDIO;

    #pragma omp parallel num_threads(2)
    {
        #pragma omp single
        {
            #pragma omp task
            personajeJugador1.comprobarColisiones(efectosA, nuevosEfectosA);

            #pragma omp task
            personajeJugador2.comprobarColisiones(efectosB, nuevosEfectosB);
        }
    }

    // Si el contador de Hitstop es mayor que 0, se baja en 1
    if(contadorHitstop > 0){
        contadorHitstop--;
    }

    // bool golpeadoMedioDespues = personajeJugador1.getEstado() == EstadoPersonaje::GOLPEADO_MEDIO || personajeJugador2.getEstado() == EstadoPersonaje::GOLPEADO_MEDIO;

    // Se añaden los efectos del jugador 2 a los del jugador 1 y así tenemos solo una lista
    for (std::shared_ptr<Animacion> &anim : nuevosEfectosB)
    {
        nuevosEfectosA.push_back(anim);
    }

    for (auto iter = nuevosEfectosA.begin(); iter != nuevosEfectosA.end(); iter++)
    {
        efectos.push_back(*iter);
    }

    // CUARTO PASO: Se comprueba si ha habido empate, o si el jugador que ha ganado está celebrando
    if (personajeJugador1.getPuntosDeVida() == personajeJugador2.getPuntosDeVida())
    {
        if (cartelEmpate->haTerminado()
            &&
                ((personajeJugador1.getEstado() == EstadoPersonaje::DERROTA && personajeJugador1.getAnimacionSegunEstado(personajeJugador1.getEstado())->haTerminado())
                ||
                (personajeJugador1.getEstado() == EstadoPersonaje::TUMBADO))
            &&
                ((personajeJugador2.getEstado() == EstadoPersonaje::DERROTA && personajeJugador2.getAnimacionSegunEstado(personajeJugador2.getEstado())->haTerminado())
                ||
                (personajeJugador2.getEstado() == EstadoPersonaje::TUMBADO))
            )
        {
            sf::Color nuevoColor(rectanguloOscuro.getFillColor());
            nuevoColor.a += 5;
            rectanguloOscuro.setFillColor(nuevoColor);
        }

        // Nadie gana, así que los dos se lamentan de no haberse hinchado a yoyas durante
        // el combate cuando tenían oportunidad
        if(personajeJugador1.getEstado() == EstadoPersonaje::QUIETO && personajeJugador2.getEstado() == EstadoPersonaje::QUIETO)
        {
            personajeJugador1.cambiarEstado(EstadoPersonaje::DERROTA);
            personajeJugador2.cambiarEstado(EstadoPersonaje::DERROTA);
        }
    }
    else
    {
        Personaje &ganador = personajeJugador1.getPuntosDeVida() > personajeJugador2.getPuntosDeVida() ? personajeJugador1 : personajeJugador2;
        Personaje &perdedor = personajeJugador1.getPuntosDeVida() < personajeJugador2.getPuntosDeVida() ? personajeJugador1 : personajeJugador2;

        if ((ganador.getEstado() == EstadoPersonaje::QUIETO && perdedor.getEstado() == EstadoPersonaje::TUMBADO && perdedor.getPuntosDeVida() == 0) ||
            (ganador.getEstado() == EstadoPersonaje::QUIETO && perdedor.getEstado() == EstadoPersonaje::QUIETO && perdedor.getPuntosDeVida() > 0))
        {
            // El ganador celebra
            ganador.cambiarEstado(EstadoPersonaje::CELEBRANDO);
            
            // Si el perdedor sigue en pie es porque ha perdido por tiempo (ya me
            // jodería) así que se lamenta de su derrota
            if(perdedor.getEstado() == EstadoPersonaje::QUIETO){
                perdedor.cambiarEstado(EstadoPersonaje::DERROTA);
            }
            
            if (ganador.getJugador() == Jugador::JUGADOR1)
                GUIJugador1.ganarRonda();
            else
                GUIJugador2.ganarRonda();
            actualizarRichPresence();
        }
        // Si ya se le ha dicho que celebre, se oscurece el rectángulo si ha terminado de celebrar
        else if (ganador.getEstado() == EstadoPersonaje::CELEBRANDO && ganador.getAnimacionSegunEstado(EstadoPersonaje::CELEBRANDO)->haTerminado() &&
                    ((ganador.getJugador() == Jugador::JUGADOR1 && cartelJugador1Gana->haTerminado()) ||
                    ((ganador.getJugador() == Jugador::JUGADOR2 && cartelJugador2Gana->haTerminado())) ||
                    (ganador.getJugador() == Jugador::JUGADOR1 && cartelJugador1Perfecto->haTerminado()) ||
                    (ganador.getJugador() == Jugador::JUGADOR2 && cartelJugador2Perfecto->haTerminado())))
        {
            sf::Color nuevoColor(rectanguloOscuro.getFillColor());
            nuevoColor.a += 5;
            rectanguloOscuro.setFillColor(nuevoColor);
        }
    }

    // Se actualizan los combos
    ContenedorDeCombos::unicaInstancia()->actualizar();

    // CUARTO PASO: DIBUJAR EL ESCENARIO, LOS PERSONAJES Y LAS ANIMACIONES

    ventana->clear(sf::Color(100, 100, 100));
    ventana->draw(escenario);

    // Dependiendo de lo que estén haciendo los personajes, se dibuja uno encima de otro. Esto
    // sobre todo es para que el jugador que haya ganado se dibuje encima del que ha perdido
    int prioridadDibujoJugador1 = util::getPrioridadDibujo(personajeJugador1.getEstado());
    int prioridadDibujoJugador2 = util::getPrioridadDibujo(personajeJugador2.getEstado());

    // Puede parecer raro que si el jugador 1 tiene más prioridad se dibuje
    // después, pero esto ocurre porque, al dibujarse después, se dibuja encima
    // del jugador 2, haciendo que el jugador 1 esté en frente. A eso se refiere
    // la prioridad más bien
    if (prioridadDibujoJugador1 > prioridadDibujoJugador2)
    {
        ventana->draw(personajeJugador2);
        ventana->draw(personajeJugador1);
    }
    else
    {
        ventana->draw(personajeJugador1);
        ventana->draw(personajeJugador2);
    }

    for (auto iter = efectos.begin(); iter != efectos.end(); iter++)
    {
        ventana->draw(**iter);
    }

    ventana->draw(*ContenedorDeCombos::unicaInstancia());

    ventana->draw(temporizador);

    ventana->draw(GUIJugador1);
    ventana->draw(GUIJugador2);

    // Se dibuja el cartel que se tenga que dibujar
    if (personajeJugador1.getPuntosDeVida() > personajeJugador2.getPuntosDeVida() && personajeJugador1.getEstado() == EstadoPersonaje::CELEBRANDO)
    {
        if(personajeJugador1.getPuntosDeVida() == personajeJugador1.getMaxPuntosDeVida())
            ventana->draw(*cartelJugador1Perfecto);
        else
            ventana->draw(*cartelJugador1Gana);
    }
    else if (personajeJugador2.getPuntosDeVida() > personajeJugador1.getPuntosDeVida() && personajeJugador2.getEstado() == EstadoPersonaje::CELEBRANDO)
    {
        if(personajeJugador2.getPuntosDeVida() == personajeJugador2.getMaxPuntosDeVida())
            ventana->draw(*cartelJugador2Perfecto);
        else
            ventana->draw(*cartelJugador2Gana);
    }
    else if (personajeJugador1.getPuntosDeVida() == personajeJugador2.getPuntosDeVida())
    {
        ventana->draw(*cartelEmpate);
    }

    ventana->draw(rectanguloOscuro);

    ventana->display();
}

void Combate::actualizarRichPresence()
{
    ClienteDiscord::unicaInstancia()->actualizarRichPresence("En una Batalla VS","J1: "+std::to_string(GUIJugador1.getRondasGanadas())+" - J2: "+std::to_string(GUIJugador2.getRondasGanadas()));
}

void Combate::comenzar()
{
    actualizarRichPresence();

    // En esta lista hay efectos como objetos voladores o efectos de golpe
    std::list<std::shared_ptr<Animacion>> efectos;

    // Este bucle contiene varias rondas dentro y podría ser más bonito y compacto pero prefiero
    // repetir código porque virgen santisima qué por culo sería compactarlo todo
    while (GUIJugador1.getRondasGanadas() != 2 && GUIJugador2.getRondasGanadas() != 2)
    {
        // Se reproduce una canción de combate
        ReproductorDeMusica::unicaInstancia()->reproducirCancionCombate();

        // Se resetean los carteles, los personajes y el escenario
        resetear();

        // El bucle de cada ronda realiza acciones en un orden muy específico para evitar problemas

        while (personajeJugador1.getPuntosDeVida() > 0 && personajeJugador2.getPuntosDeVida() > 0 && !temporizador.haTerminado())
        {
            // Se prepara un reloj para ver cuánto tiempo pasa entre fotogramas
            sf::Clock reloj;

            // Se aclara el rectángulo que cubre el combate
            if (rectanguloOscuro.getFillColor().a > 0)
            {
                sf::Color nuevoColor(rectanguloOscuro.getFillColor());
                // Se le baja 10 a la transparencia hasta que sea totalmente transparente
                // (con cuidado para que el contador no se dé la vuelta)
                nuevoColor.a = nuevoColor.a < 10 ? 0 : nuevoColor.a - 10;
                rectanguloOscuro.setFillColor(nuevoColor);
            }

            // Este es un momento especial en el que al menos uno de los personajes está preparando su súper
            // ataque, por lo que todo se pone oscuro y el tiempo se para por un momento
            if (personajeJugador1.getEstado() == EstadoPersonaje::PREPARANDO_SUPER || personajeJugador2.getEstado() == EstadoPersonaje::PREPARANDO_SUPER)
            {
                actualizarFotogramaPreparandoSuper(efectos);
            }
            else
            {
                actualizarFotogramaNormal(efectos);
            }

            // El juego se duerme hasta que dé tiempo a dibujar el siguiente fotograma, teniendo en cuenta
            // que se deben dibujar 60 fotogramas por segundo y que cada fotograma además necesita un tiempo
            // previo de preparación para actualizar y dibujar y tal
            sf::sleep(sf::seconds(1.f / Configuracion::unicaInstancia()->getFPS()) - reloj.reset());
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

        // El bucle termina cuando el rectángulo que cubre la pantalla se oscurezca por completo (esto ocurre después de que el personaje ganador
        // haya celebrado su victoria y se haya terminado de reproducir la canción de fin de ronda)
        while (rectanguloOscuro.getFillColor().a != 255)
        {
            sf::Clock reloj;

            actualizarFotogramaCelebracion(efectos);

            sf::sleep(sf::seconds(1.f / Configuracion::unicaInstancia()->getFPS()) - reloj.reset());
        }
    }
}
