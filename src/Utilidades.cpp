#include "Utilidades.hpp"
#include "Constantes.hpp"
#include "Bitacora.hpp"

namespace util{

    std::vector<std::string> separarString(std::string string, char separador){

        std::stringstream stringstream(string);

        std::vector<std::string> vectorParaDevolver;

        std::string datos;

        while(std::getline(stringstream,datos,separador)){
            vectorParaDevolver.push_back(datos);
        }

        return vectorParaDevolver;
    }

    TipoBucle stringATipoBucle(std::string string){
        if(string == "Normal")
            return TipoBucle::NORMAL;
        if(string == "Ping pong")
            return TipoBucle::PING_PONG;

        return TipoBucle::SIN_BUCLE;
    }

    EstadoPersonaje stringAEstadoPersonaje(std::string string){
        if(string == "quieto")
            return EstadoPersonaje::QUIETO;
        if(string == "agachado")
            return EstadoPersonaje::AGACHADO;
        if(string == "ataque-agachado")
            return EstadoPersonaje::ATAQUE_AGACHADO;
        if(string == "ataque-normal-1")
            return EstadoPersonaje::ATAQUE_NORMAL_1;
        if(string == "ataque-normal-2")
            return EstadoPersonaje::ATAQUE_NORMAL_2;
        if(string == "ataque-normal-3")
            return EstadoPersonaje::ATAQUE_NORMAL_3;
        if(string == "ataque-aereo")
            return EstadoPersonaje::ATAQUE_AEREO;
        if(string == "ataque-especial")
            return EstadoPersonaje::ATAQUE_ESPECIAL;
        if(string == "golpeado-peque")
            return EstadoPersonaje::GOLPEADO_PEQUE;
        if(string == "golpeado-medio")
            return EstadoPersonaje::GOLPEADO_MEDIO;
        if(string == "golpeado-subiendo")
            return EstadoPersonaje::GOLPEADO_SUBIENDO;
        if(string == "golpeado-bajando")
            return EstadoPersonaje::GOLPEADO_BAJANDO;
        if(string == "saltando-subiendo")
            return EstadoPersonaje::SALTANDO_SUBIENDO;
        if(string == "saltando-bajando")
            return EstadoPersonaje::SALTANDO_BAJANDO;
        if(string == "tocando-suelo")
            return EstadoPersonaje::TOCANDO_SUELO;
        if(string == "andando-acercandose")
            return EstadoPersonaje::ANDANDO_ACERCANDOSE;
        if(string == "andando-alejandose")
            return EstadoPersonaje::ANDANDO_ALEJANDOSE;
        if(string == "bloqueando")
            return EstadoPersonaje::BLOQUEANDO;
        if(string == "esquive-super")
            return EstadoPersonaje::ESQUIVE_SUPER;
        if(string == "preparando-super")
            return EstadoPersonaje::PREPARANDO_SUPER;
        if(string == "ataque-super")
            return EstadoPersonaje::ATAQUE_SUPER;
        if(string == "tumbado")
            return EstadoPersonaje::TUMBADO;
        if(string == "levantandose")
            return EstadoPersonaje::LEVANTANDOSE;
        if(string == "celebrando")
            return EstadoPersonaje::CELEBRANDO;

        Bitacora::unicaInstancia()->escribir("Emilio: Oiga señor Juan, esto de \"" + string + "\"... ¿qué estado de personaje es?");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: Pues ninguno, que yo sepa. ¿Por?");
        Bitacora::unicaInstancia()->escribir("Emilio: No, es que se supone que tiene que ser alguno, porque estoy aquí en util::stringAEstadoPersonaje y me acaba de llegar.");
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: ¡¿Pero cómo te va a llegar eso?! Esto es un sinsentido... se suspende la junta.");
        
        exit(EXIT_FAILURE);
    }

    Accion stringAAccion(const std::string &string){
        if(string == "ARRIBA")
            return Accion::ARRIBA;
        if(string == "ABAJO")
            return Accion::ABAJO;
        if(string == "DERECHA")
            return Accion::DERECHA;
        if(string == "IZQUIERDA")
            return Accion::IZQUIERDA;
        if(string == "ATACAR")
            return Accion::ATACAR;
        
        Bitacora::unicaInstancia()->escribir("Juan Cuesta: A ver, ¿cuántas veces lo tengo que decir? En la función stringAAccion solo se deben pasar acciones válidas, y " + string + " no lo es. Esta gente no se lee las circulares... se suspende la junta.");
        exit(EXIT_FAILURE);
    }

    sf::Vector2f centroDeInterseccion(sf::IntRect r1, sf::IntRect r2){
        
        int izquierda = std::max(r1.position.x, r2.position.x);
        int arriba = std::max(r1.position.y, r2.position.y);

        int derecha = std::min(r1.position.x+r1.size.x, r2.position.x+r2.size.x);
        int abajo = std::min(r1.position.y+r1.size.y, r2.position.y+r2.size.y);

        return sf::Vector2f(izquierda + (derecha - izquierda)/2, arriba + (abajo-arriba)/2);
    }

    double realAleatorio(){
        return ((double)rand()) / RAND_MAX;
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

    sf::Color aproximarColor(const sf::Color& primerColor, const sf::Color& segundoColor, double factorPrimero){
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

    float aproximarFloat(const float& primerFloat, const float& segundoFloat, double factorPrimero)
    {
        return primerFloat*factorPrimero + segundoFloat*(1-factorPrimero);
    }
}