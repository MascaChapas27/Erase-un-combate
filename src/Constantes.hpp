#ifndef __CONSTANTES_HPP__
#define __CONSTANTES_HPP__

#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

/*
    Este fichero contiene todas las constantes del programa, como su
    propio nombre indica vamos
*/

/*
    CONSTANTES PARA EL DEBUG
*/

// Indica si se quiere activar el modo debug (mostrar hitboxes en pantalla y acta vecinal en la consola)
constexpr bool DEBUG = false;

// Color para las hitboxes que hacen daño
constexpr sf::Color HITBOX_ATAQUE_COLOR = sf::Color::Red;

// Color para las hitboxes que no hacen daño
constexpr sf::Color HITBOX_NO_ATAQUE_COLOR = sf::Color::Blue;

/*
    CONSTANTES PARA LA BITÁCORA
*/

const std::string BITACORA_RUTA = "ActaVecinal.log";

constexpr int MAXIMA_LONGITUD_TEXTURA = 1024;

/*
    CONSTANTES PARA LA VENTANA
*/

// Anchura y altura de la ventana principal
constexpr int VENTANA_ANCHURA = 300;
constexpr int VENTANA_ALTURA = 200;

// Numero de frames por segundo
constexpr int NUMERO_FPS = 60;

// Nombre de la ventana principal
const std::string VENTANA_NOMBRE = "Erase un combate";

// Estilo de la ventana principal
constexpr int VENTANA_ESTILO = sf::Style::Close;

// Vibración de la ventana para un ataque mediano
constexpr int VIBRACION_ATAQUE_MEDIO = 7;

// Vibración de la ventana para un ataque súper
constexpr int VIBRACION_ATAQUE_SUPER = 30;

// Cada cuántos frames se actualiza la vibración de la ventana principal para que el efecto no
// sea tan violento a los ojos
constexpr int VIBRACION_RETRASO = 1;

/*
    CONSTANTES PARA LOS CONTROLES
*/

// Número total de controles posibles (dos partes del teclado y ocho mandos)
constexpr int NUMERO_CONTROLES = 10;

// El movimiento mínimo que se tiene que hacer con un joystick para considerar la
// entrada válida (esto evita que con mover el joystick 0.00001 milimetros ya está
// moviéndose el personaje). Hay que tener en cuenta que el máximo al que puede llegar
// un joystick sin drift ni defectos es 100
constexpr double UMBRAL_JOYSTICK = 50;

// Tecla para salir del juego
constexpr sf::Keyboard::Scancode TECLA_SALIDA = sf::Keyboard::Scancode::Escape;

// Frames que te da el juego desde que haces un input de un ataque especial hasta
// que haces el siguiente como máximo
constexpr int COOLDOWN_ATAQUE_ESPECIAL = 30;

/*
    CONSTANTES PARA EL SONIDO Y LA MÚSICA
*/

// Volumen máximo del reproductor de música
constexpr double VOLUMEN_MAXIMO_MUSICA = 100.0f;

// Volumen máximo del reproductor de efectos de sonido
constexpr double VOLUMEN_MAXIMO_SONIDOS = 100.0f;

/*
    CONSTANTES PARA LOS FICHEROS
*/

// Secuencia de caracteres que delimita los distintos elementos de un fichero
const std::string SECUENCIA_DELIMITADORA_FICHERO = "----------------------------------------";

// Secuencia de caracteres que indica el fin de un fichero
const std::string SECUENCIA_FIN_FICHERO = "####";

/*
    CONSTANTES PARA LAS ANIMACIONES
*/

// Valor máximo que debería tener el contador de parpadeo para hacer que una animación
// con gravedad se dé por terminada
constexpr int MAX_CONTADOR_PARPADEO = 40;

// Cada cuántos frames las animaciones con gravedad pasan a ser visibles o invisibles para
// el efecto de parpadeo
constexpr int DURACION_PARPADEO = 2;

// Máxima velocidad de giro de una partícula en cualquier dirección
constexpr double MAX_VELOCIDAD_GIRO_PART = 10;

// Máxima velocidad en cualquier eje de una partícula pequeña
constexpr int MAX_VELOCIDAD_PARTICULA_PEQUE = 3;

// Máxima velocidad en cualquier eje de una partícula mediana
constexpr int MAX_VELOCIDAD_PARTICULA_MEDIA = 6;

// Máxima velocidad en cualquier eje de una partícula grande
constexpr int MAX_VELOCIDAD_PARTICULA_GRANDE = 8;

// Cómo de desplazada está la animación del polvo en el eje X con respecto al centro del personaje
constexpr int OFFSET_POLVO = 15;

// Cuánto se agrandan o disminuyen las animaciones agrandables cada frame
constexpr double TASA_CRECIMIENTO_ANIMACION_AGRANDABLE = 0.1;

// Posición de los carteles que aparecen durante los combates (los de "¿Todo listo?" y tal)
constexpr sf::Vector2f POSICION_CARTELES_COMBATE = sf::Vector2f(VENTANA_ANCHURA/2, VENTANA_ALTURA/3);

/*
    CONSTANTES PARA EL MENÚ PRINCIPAL
*/

// Posición del centro del cartel de título de ÉRASE UN COMBATE
constexpr sf::Vector2f POSICION_TITULO = sf::Vector2f(VENTANA_ANCHURA/4,VENTANA_ALTURA/6);

// Posición en el eje X de los selectores de opción
constexpr float POSICION_X_SELECTOR_SIN_SELECCIONAR = VENTANA_ANCHURA/8;
constexpr float POSICION_X_SELECTOR_SELECCIONADO = VENTANA_ANCHURA/7;

// Posición en el eje Y de los selectores
constexpr float POSICION_Y_SELECTOR_MODO_HISTORIA = 7*VENTANA_ALTURA/15;
constexpr float POSICION_Y_SELECTOR_BATALLA_VS = 9*VENTANA_ALTURA/15;
constexpr float POSICION_Y_SELECTOR_OPCIONES = 11*VENTANA_ALTURA/15;

// Color de las opciones sin seleccionar
constexpr sf::Color COLOR_SELECTOR_SIN_SELECCIONAR = sf::Color(50,50,50);

/*
    CONSTANTES PARA LOS PERSONAJES
*/

// Origen de los personajes (usando la plantilla)
constexpr sf::Vector2f PERSONAJE_PLANTILLA_ORIGEN = sf::Vector2f(64, 100);

// Gravedad del escenario
constexpr float GRAVEDAD = 0.3;

// Puntos de vida máximos de un personaje
constexpr int MAX_PUNTOS_DE_VIDA = 100;

// Valor máximo del medidor de súper de un personaje
constexpr int MAX_MEDIDOR_SUPER = 50;

// Retraso entre movimientos de píxel y píxel del medidor de vida atrasado
// de un personaje, para evitar que vaya muy rápido
constexpr int MAX_RETRASO_MEDIDOR_VIDA_ATRASADO = 3;

// Velocidad máxima en el eje X a la que se puede mover un personaje
constexpr float VELOCIDAD_ANDAR_MAX = 2.5;

// Aumento de velocidad cada frame al andar
constexpr float VELOCIDAD_ANDAR_AUMENTO = 0.3;

// Velocidad que se consigue en el eje Y al saltar
constexpr float VELOCIDAD_SALTO = -6;

// Cuando un personaje GOLPEADO_BAJANDO choca el suelo con una velocidad menor a esta, pasa
// a estar TIRADO
constexpr float VELOCIDAD_PASAR_A_TIRADO = 3.2;

// Posición en el eje Y del suelo
constexpr float ALTURA_SUELO = VENTANA_ALTURA - 10;

// Impulso inicial en el eje X al recibir un golpe pequeño
constexpr float IMPULSO_GOLPE_PEQUE = 1;

// Impulso inicial en el eje X al recibir un golpe mediano
constexpr float IMPULSO_GOLPE_MEDIO = 3;

// Impulso inicial en los ejes X e Y al recibir un golpe grande
constexpr float IMPULSO_X_GOLPE_GRANDE = 7;
constexpr float IMPULSO_Y_GOLPE_GRANDE = -10;

// Impulso inicial en el eje Y al recibir un golpe bajo mediano
constexpr float IMPULSO_GOLPE_BAJO_MEDIO = -5;

// Máximo daño que puede hacer un ataque pequeño
constexpr int MAX_ATAQUE_PEQUE = 3;

// Máximo daño que puede hacer un ataque medio
constexpr int MAX_ATAQUE_MEDIO = 6;

// Valor que tiene que alcanzar contadorTumbado para que se levante un personaje
constexpr int MAX_CONTADOR_TUMBADO = NUMERO_FPS;

// Valor inicial del contador de celebración. Al llegar a 0, el personaje que acaba de ganar celebra
constexpr int MAX_CONTADOR_CELEBRACION = NUMERO_FPS*2;

// Máximo de frames que tienen que pasar desde que se empiece a caminar al revés del rival para poder
// hacerle parry a un ataque súper
constexpr int MAX_CONTADOR_ESQUIVE_SUPER = 4;

/*
    CONSTANTES PARA EL ESCENARIO
*/

// Si un personaje está a esta distancia o menos de un borde, el escenario se moverá para alejar a ese
// personaje del borde (siempre y cuando el otro personaje no esté también en el umbral)
constexpr int ESCENARIO_UMBRAL_MOVIMIENTO = 20;

/*
    CONSTANTES PARA LA GUI DE PERSONAJE
*/

// Posición de la GUI del personaje izquierdo (esquina superior izquierda (la
// posición del personaje derecho es igual pero en espejo))
constexpr sf::Vector2f POSICION_GUI_IZQUIERDA(4,4);

// Posición de las barras de vida de la GUI izquierda (real y atrasada) (relativa a la esquina superior
// izquierda del sprite de la GUI)
constexpr sf::Vector2f POSICION_BARRA_VIDA_IZQUIERDA(40,3);

// Tamaño de la barra de vida
constexpr sf::Vector2f TAMANO_BARRA_VIDA(70,8);

// Posición de la barra de súper de la GUI izquierda  (relativa a la esquina superior
// izquierda del sprite de la GUI)
constexpr sf::Vector2f POSICION_BARRA_SUPER_IZQUIERDA(40,19);

// Tamaño de la barra de súper
constexpr sf::Vector2f TAMANO_BARRA_SUPER(50,6);

// Posición del "portrait" del personaje de la GUI izquierda  (relativa a la esquina superior
// izquierda del sprite de la GUI)
constexpr sf::Vector2f POSICION_PORTRAIT_IZQUIERDA(3,3);

// Posición del nombre del personaje de la GUI izquierda  (relativa a la esquina superior
// izquierda del sprite de la GUI)
constexpr sf::Vector2f POSICION_NOMBRE_IZQUIERDA(40,28);

// Color de la barra de vida en tiempo real
constexpr sf::Color COLOR_BARRA_VIDA_REAL(106,206,64);

// Color de la barra de vida atrasada
constexpr sf::Color COLOR_BARRA_VIDA_ATRASADA(204,65,65);

// Color de la barra de súper
constexpr sf::Color COLOR_BARRA_SUPER(89,114,255);

// Cada cuántos frames se actualiza la vibración
constexpr int CONTADOR_VIBRACION_MAX = 2;

/*
    CONSTANTES PARA EL ONLINE
*/

// El puerto utilizado por el líder para aceptar conexiones de invitados
constexpr unsigned short NUMERO_PUERTO = 21107;

// Bit que indica que el otro jugador se ha movido a la izquierda
constexpr uint8_t BIT_IZQUIERDA = 1;

// Bit que indica que el otro jugador se ha movido a la derecha
constexpr uint8_t BIT_DERECHA = 2;

// Bit que indica que el otro jugador se ha movido a la arriba
constexpr uint8_t BIT_ARRIBA = 4;

// Bit que indica que el otro jugador se ha movido a la abajo
constexpr uint8_t BIT_ABAJO = 8;

// Bit que indica que el otro jugador ha atacado
constexpr uint8_t BIT_ATAQUE = 16;

/*
    CONSTANTES AUXILIARES PARA COSAS DE C++
*/

// Si dos floats se diferencian en menos que este número, se consideran iguales
constexpr float UMBRAL_FLOAT = 0.0001;

#endif // __CONSTANTES_CPP__