#include "Utilidades.hpp"
#include "Constantes.hpp"
#include "Bitacora.hpp"
#include <cmath>

namespace util{

    std::vector<std::string> separarString(std::string string, char separador)
    {
        std::stringstream stringstream(string);

        std::vector<std::string> vectorParaDevolver;

        std::string datos;

        while(std::getline(stringstream,datos,separador)){
            vectorParaDevolver.push_back(datos);
        }

        return vectorParaDevolver;
    }

    float distanciaEuclidiana(const sf::Vector2f& punto1, const sf::Vector2f& punto2)
    {
        return std::sqrt(std::pow(punto2.x - punto1.x, 2.f) + std::pow(punto2.y - punto1.y, 2.f));
    }

    std::unordered_map<std::string,TipoBucle> tablaStringATipoBucle =
    {
      {"Normal", TipoBucle::NORMAL},
      {"Sin bucle", TipoBucle::SIN_BUCLE}  
    };

    TipoBucle stringATipoBucle(std::string string){
        return tablaStringATipoBucle.at(string);
    }

    std::unordered_map<std::string,EstadoPersonaje> tablaStringAEstadoPersonaje =
    {
        {"quieto", EstadoPersonaje::QUIETO},
        {"agachado", EstadoPersonaje::AGACHADO},
        {"ataque-agachado", EstadoPersonaje::ATAQUE_AGACHADO},
        {"ataque-normal-1", EstadoPersonaje::ATAQUE_NORMAL_1},
        {"ataque-normal-2", EstadoPersonaje::ATAQUE_NORMAL_2},
        {"ataque-normal-3", EstadoPersonaje::ATAQUE_NORMAL_3},
        {"ataque-alejandose", EstadoPersonaje::ATAQUE_ALEJANDOSE},
        {"ataque-acercandose", EstadoPersonaje::ATAQUE_ACERCANDOSE},
        {"ataque-aereo", EstadoPersonaje::ATAQUE_AEREO},
        {"ataque-especial", EstadoPersonaje::ATAQUE_ESPECIAL},
        {"golpeado-peque", EstadoPersonaje::GOLPEADO_PEQUE},
        {"golpeado-medio", EstadoPersonaje::GOLPEADO_MEDIO},
        {"golpeado-subiendo", EstadoPersonaje::GOLPEADO_SUBIENDO},
        {"golpeado-bajando", EstadoPersonaje::GOLPEADO_BAJANDO},
        {"saltando-subiendo", EstadoPersonaje::SALTANDO_SUBIENDO},
        {"saltando-bajando", EstadoPersonaje::SALTANDO_BAJANDO},
        {"tocando-suelo", EstadoPersonaje::TOCANDO_SUELO},
        {"andando-acercandose", EstadoPersonaje::ANDANDO_ACERCANDOSE},
        {"corriendo", EstadoPersonaje::CORRIENDO},
        {"frenando", EstadoPersonaje::FRENANDO},
        {"andando-alejandose", EstadoPersonaje::ANDANDO_ALEJANDOSE},
        {"bloqueando", EstadoPersonaje::BLOQUEANDO},
        {"esquive-super", EstadoPersonaje::ESQUIVE_SUPER},
        {"preparando-super", EstadoPersonaje::PREPARANDO_SUPER},
        {"ataque-super", EstadoPersonaje::ATAQUE_SUPER},
        {"tumbado", EstadoPersonaje::TUMBADO},
        {"levantandose", EstadoPersonaje::LEVANTANDOSE},
        {"celebrando", EstadoPersonaje::CELEBRANDO},
        {"derrota", EstadoPersonaje::DERROTA}
    };

    EstadoPersonaje stringAEstadoPersonaje(std::string string)
    {
        if(!tablaStringAEstadoPersonaje.count(string))
        {
            Bitacora::unicaInstancia()->escribir("Emilio: señor Juan, una duda que me acaba de surgir... ¿esto de " + string + " qué estado de personaje es?");
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: pues, que yo sepa, ninguno... ¿por qué lo dices?");
            Bitacora::unicaInstancia()->escribir("Emilio: no, es que estaba aquí buscándolo en tablaStringAEstadoPersonaje y nada, que no lo encuentro.");
            Bitacora::unicaInstancia()->escribir("Juan Cuesta: qué follón... se suspende la junta.");
        }

        return tablaStringAEstadoPersonaje.at(string);
    }

    std::unordered_map<std::string,Accion> tablaStringAAccion =
    {
      {"ARRIBA", Accion::ARRIBA},
      {"ABAJO", Accion::ABAJO},
      {"DERECHA", Accion::DERECHA},
      {"IZQUIERDA", Accion::IZQUIERDA},
      {"ATACAR", Accion::ATACAR}
    };

    Accion stringAAccion(const std::string &string)
    {
        return tablaStringAAccion.at(string);
    }

    sf::Vector2f centroDeInterseccion(sf::IntRect r1, sf::IntRect r2){
        
        int izquierda = std::max(r1.position.x, r2.position.x);
        int arriba = std::max(r1.position.y, r2.position.y);

        int derecha = std::min(r1.position.x+r1.size.x, r2.position.x+r2.size.x);
        int abajo = std::min(r1.position.y+r1.size.y, r2.position.y+r2.size.y);

        return sf::Vector2f(izquierda + (derecha - izquierda)/2, arriba + (abajo-arriba)/2);
    }

    float realAleatorio(){
        return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    }

    uint8_t accionABit(Accion accion){
        switch(accion){
            case Accion::ARRIBA:
                return BIT_ARRIBA;
                break;
            case Accion::ABAJO:
                return BIT_ABAJO;
                break;
            case Accion::IZQUIERDA:
                return BIT_IZQUIERDA;
                break;
            case Accion::DERECHA:
                return BIT_DERECHA;
                break;
            case Accion::ATACAR:
                return BIT_ATAQUE;
                break;
            default:
                return 0;
                break;
        }
    }

    Accion bitAAccion(uint8_t bit){
        switch(bit){
            case BIT_ARRIBA:
                return Accion::ARRIBA;
                break;
            case BIT_ABAJO:
                return Accion::ABAJO;
                break;
            case BIT_IZQUIERDA:
                return Accion::IZQUIERDA;
                break;
            case BIT_DERECHA:
                return Accion::DERECHA;
                break;
            case BIT_ATAQUE:
                return Accion::ATACAR;
                break;
            default:
                return Accion::NADA;
                break;
        }
    }

    sf::Color aproximarColor(const sf::Color& primerColor, const sf::Color& segundoColor, float factorPrimero){
        sf::Color colorResultado = primerColor;
        colorResultado.r = colorResultado.r*factorPrimero + segundoColor.r*(1-factorPrimero);
        colorResultado.g = colorResultado.g*factorPrimero + segundoColor.g*(1-factorPrimero);
        colorResultado.b = colorResultado.b*factorPrimero + segundoColor.b*(1-factorPrimero);
        colorResultado.a = colorResultado.a*factorPrimero + segundoColor.a*(1-factorPrimero);

        // Se comprueba si los colores se han quedado pillados y no avanzan, teniendo que avanzarlos
        // a mano
        if(colorResultado.r == primerColor.r){
            if(colorResultado.r < segundoColor.r)
                colorResultado.r++;
            else if(colorResultado.r > segundoColor.r)
                colorResultado.r--;
        }

        if(colorResultado.g == primerColor.g){
            if(colorResultado.g < segundoColor.g)
                colorResultado.g++;
            else if(colorResultado.g > segundoColor.g)
                colorResultado.g--;
        }

        if(colorResultado.b == primerColor.b){
            if(colorResultado.b < segundoColor.b)
                colorResultado.b++;
            else if(colorResultado.b > segundoColor.b)
                colorResultado.b--;
        }

        if(colorResultado.a == primerColor.a){
            if(colorResultado.a < segundoColor.a)
                colorResultado.a++;
            else if(colorResultado.a > segundoColor.a)
                colorResultado.a--;
        }

        return colorResultado;
    }

    float aproximarFloat(const float& primerFloat, const float& segundoFloat, float factorPrimero)
    {
        return primerFloat*factorPrimero + segundoFloat*(1-factorPrimero);
    }

    sf::Vector2f aproximarVector2f(const sf::Vector2f& primerVector, const sf::Vector2f& segundoVector, float factorPrimero)
    {
        return primerVector*factorPrimero + segundoVector*(1-factorPrimero);
    }

    int getPrioridadDibujo(EstadoPersonaje estado)
    {
        switch(estado)
        {
            // Prioridad 1 (baja)
            case EstadoPersonaje::GOLPEADO_PEQUE:
            case EstadoPersonaje::GOLPEADO_MEDIO:
            case EstadoPersonaje::DERROTA:
                return 1;
                break;
            
            // Prioridad 2 (media)
            case EstadoPersonaje::QUIETO:
            case EstadoPersonaje::SALTANDO_SUBIENDO:
            case EstadoPersonaje::SALTANDO_BAJANDO:
            case EstadoPersonaje::TOCANDO_SUELO:
            case EstadoPersonaje::AGACHADO:
            case EstadoPersonaje::ANDANDO_ACERCANDOSE:
            case EstadoPersonaje::ANDANDO_ALEJANDOSE:
            case EstadoPersonaje::FRENANDO:
            case EstadoPersonaje::BLOQUEANDO:
            case EstadoPersonaje::ESQUIVE_SUPER:
                return 2;
                break;

            // Prioridad 3 (alta)
            case EstadoPersonaje::ATAQUE_AEREO:
            case EstadoPersonaje::ATAQUE_AGACHADO:
            case EstadoPersonaje::ATAQUE_ALEJANDOSE:
            case EstadoPersonaje::ATAQUE_ACERCANDOSE:
            case EstadoPersonaje::ATAQUE_ESPECIAL:
            case EstadoPersonaje::ATAQUE_NORMAL_1:
            case EstadoPersonaje::ATAQUE_NORMAL_2:
            case EstadoPersonaje::ATAQUE_NORMAL_3:
            case EstadoPersonaje::ATAQUE_SUPER:
            case EstadoPersonaje::CORRIENDO:
            case EstadoPersonaje::PREPARANDO_SUPER:
            case EstadoPersonaje::CELEBRANDO:
                return 3;
                break;
            
            // Prioridad 4 (altísima)
            case EstadoPersonaje::GOLPEADO_BAJANDO:
            case EstadoPersonaje::GOLPEADO_SUBIENDO:
            case EstadoPersonaje::TUMBADO:
            case EstadoPersonaje::LEVANTANDOSE:
                return 4;
                break;
        }

        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Pero bueno, Emilio, ¿se puede saber qué es esto?");
        Bitacora::unicaInstancia()->escribir("Emilio: ¿Qué ha hecho mi padre ahora? Le juro que en cuanto pueda le meto en una residencia, es que se me van todas de presupuesto.");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: No, no es eso... es que he estado intentando encontrar la prioridad de un EstadoPersonaje, pero... no sé cuál es. ¿Te suena el número "+std::to_string(static_cast<int>(estado))+"?");
        Bitacora::unicaInstancia()->escribir("Emilio: ¿El "+std::to_string(static_cast<int>(estado))+"...? Pues, que yo sepa no, vamos, pero tendría que hacer memoria.");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Qué follón...");
        exit(EXIT_FAILURE);

        return 1;
    }

    bool floatsIguales(float float1, float float2)
    {
        return std::abs(float1-float2) < UMBRAL_FLOAT;
    }
}