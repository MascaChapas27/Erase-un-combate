#include "VentanaPrincipal.hpp"
#include "Constantes.hpp"

sf::RenderWindow * VentanaPrincipal::ventanaPrincipal = nullptr;
int VentanaPrincipal::zoom = 1;
int VentanaPrincipal::potenciaVibracion = 0;
int VentanaPrincipal::contadorVibracion = 0;
bool VentanaPrincipal::vibracionActivada = true;

sf::RenderWindow * VentanaPrincipal::unicaInstancia()
{
    if(ventanaPrincipal==nullptr){
        ventanaPrincipal = new sf::RenderWindow(sf::VideoMode({VENTANA_ANCHURA, VENTANA_ALTURA}), VENTANA_NOMBRE, VENTANA_ESTILO);
        // Se desactiva lo de que si mantienes pulsada una tecla se vuelve a pulsar muchas veces
        // porque que pereza
        ventanaPrincipal->setKeyRepeatEnabled(false);
        //vibracionActivada = false;

        aumentarZoom();
        aumentarZoom();
    }
    return ventanaPrincipal;
}

VentanaPrincipal::~VentanaPrincipal(){
    if(ventanaPrincipal != nullptr) delete ventanaPrincipal;
}

void VentanaPrincipal::aumentarZoom(){
    zoom++;
    ventanaPrincipal->setSize(sf::Vector2u(VENTANA_ANCHURA * zoom,VENTANA_ALTURA * zoom));
}

void VentanaPrincipal::disminuirZoom(){
    if(zoom == 1) return;
    zoom--;
    ventanaPrincipal->setSize(sf::Vector2u(VENTANA_ANCHURA * zoom,VENTANA_ALTURA * zoom));
}

void VentanaPrincipal::setZoom(int zoom){
    if(zoom < 1) return;
    VentanaPrincipal::zoom = zoom;
    ventanaPrincipal->setSize(sf::Vector2u(VENTANA_ANCHURA * zoom,VENTANA_ALTURA * zoom));
}

int VentanaPrincipal::getZoom(){
    return zoom;
}

void VentanaPrincipal::actualizar(){
    if(potenciaVibracion == 0) return;

    if(contadorVibracion > 0) {
        contadorVibracion--;
        return;
    }

    ventanaPrincipal->setPosition({ventanaPrincipal->getPosition().x,ventanaPrincipal->getPosition().y+potenciaVibracion});

    if(potenciaVibracion < 0) potenciaVibracion++;

    potenciaVibracion *= -1;

    contadorVibracion = VIBRACION_RETRASO;
}

void VentanaPrincipal::vibrar(int potenciaVibracion){
    if(vibracionActivada)
        VentanaPrincipal::potenciaVibracion = potenciaVibracion;
}