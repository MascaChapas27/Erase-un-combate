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

// Indica si se quieren mostrar las hitboxes
const bool HITBOX_MOSTRAR = true;

// Color para las hitboxes que hacen da�o
const sf::Color HITBOX_ATAQUE_COLOR = sf::Color::Red;

// Color para las hitboxes que no hacen da�o
const sf::Color HITBOX_NO_ATAQUE_COLOR = sf::Color::Blue;

/*
    CONSTANTES PARA LA BIT�CORA
*/

const std::string BITACORA_RUTA = "ActaVecinal.log";

const int MAXIMA_LONGITUD_TEXTURA = 1024;

/*
    CONSTANTES PARA LA VENTANA
*/

// Anchura y altura de la ventana principal
const int VENTANA_ANCHURA = 300;
const int VENTANA_ALTURA = 200;

// FPS m�ximos de la ventana principal
const int VENTANA_FPS = 60;

// Nombre de la ventana principal
const std::string VENTANA_NOMBRE = "�rase un combate";

// Estilo de la ventana principal
const int VENTANA_ESTILO = sf::Style::Close;

/*
    CONSTANTES PARA LOS CONTROLES
*/

// N�mero total de controles posibles (dos partes del teclado y ocho mandos)
const int NUMERO_CONTROLES = 10;

// El movimiento m�nimo que se tiene que hacer con un joystick para considerar la
// entrada v�lida (esto evita que con mover el joystick 0.00001 milimetros ya est�
// movi�ndose el personaje)
const double UMBRAL_JOYSTICK = 0.3;

// Tecla para salir del juego
const sf::Keyboard::Key TECLA_SALIDA = sf::Keyboard::Key::Escape;

/*
    CONSTANTES PARA EL SONIDO
*/

// Volumen m�ximo del reproductor de m�sica
const float VOLUMEN_MAXIMO_MUSICA = 100.0;

#endif // __CONSTANTES_HPP__
