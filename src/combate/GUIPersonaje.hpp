#pragma once

#include "Personaje.hpp"
#include "SFML/Graphics.hpp"

// Una GUI (Graphical User Interface (Interfaz gráfica de usuario)) es una
// forma que tienen los programas informáticos de mostrar información hacia fuera.
// En este caso, esta GUI muestra la barra de vida de un personaje, una foto suya,
// el medidor de súper ataque, su nombre y el número de rondas que ha ganado
class GUIPersonaje : public sf::Drawable {

    private:

        // El personaje del que esta GUI muestra información
        Personaje& personaje;

        // Indica si la GUI está en la parte izquierda o en la parte derecha de la pantalla
        bool parteIzquierda;

        // Rectángulo que representa el medidor de vida en tiempo real (este
        // medidor muestra en todo momento la cantidad exacta de puntos de vida
        // del personaje)
        sf::RectangleShape rectanguloVidaReal;

        // Rectángulo que representa el medidor de vida atrasado (este rectángulo aparece
        // detrás del rectángulo en tiempo real y permite ver mejor la diferencia de puntos
        // de vida entre los que había antes y después de un golpe)
        sf::RectangleShape rectanguloVidaAtrasada;

        // Rectángulo que representa el medidor del ataque súper
        sf::RectangleShape rectanguloSuper;

        // Sprite para el nombre del personaje (mejor así en vez de hacer que sea un texto
        // porque se pueden dibujar cosillas chulas)
        sf::Sprite spriteNombre;

        // Sprite para el "portrait" del personaje (una foto que aparece en la esquina)
        sf::Sprite spritePortrait;

        // Los sprites que indican los movimientos que hay que hacer para
        // hacer el ataque especial del personaje
        std::vector<sf::Sprite> spritesEspecial;

        // Cantidad de puntos de vida que está mostrando actualmente el medidor
        // de vida atrasado
        int contadorVidaAtrasado;

        // Este retraso permite que el rectángulo de vida atrasado se mueva lentamente en vez de un
        // píxel por fotograma, lo cual es demasiado rápido para 60 FPS y 100 puntos de vida totales
        int retrasoContadorVidaAtrasado;

        // Permite que la GUI vibre cuando el personaje sea golpeado
        int vibracion;

        // Contador para que no vibre demasiado rápido
        int contadorVibracion;

        // El sprite principal de la GUI (base y frente (la base es el fondo y el
        // frente son las marcas transparentes que hay en las barras de vida y súper))
        sf::Sprite spritePrincipalBase;
        sf::Sprite spritePrincipalFrente;

        // Número de rondas ganadas
        int rondasGanadas;

        // Permite saber si el medidor de súper está lleno. Aunque se
        // puede comprobar manualmente con el medidor de súper del personaje,
        // esto permite saber si se ha llenado ahora mismo o si se llenó antes,
        // así que se puede poner el efecto de destello cuando 
        bool medidorDeSuperLleno;

        // Indica si la GUI piensa que el jugador está mirando hacia la derecha
        // o no. Esto permite darle la vuelta a las flechitas del cartel de ataque
        // especial cuando el personaje se da la vuelta
        bool personajeMirandoDerecha;

        // Los efectos generados por la GUI tienen que ir aquí o si no los
        // efectos se moverán con la pantalla y queda raro
        std::list<std::shared_ptr<Animacion>> efectos;
    
    public:

        // Construye la GUI dada una referencia a un personaje
        GUIPersonaje(Personaje &personaje, bool parteIzquierda);

        // Actualiza la GUI en base a los datos del personaje (aprovechando que GUIPersonaje
        // es clase amiga (friend class) de Personaje, puede acceder a sus partes privadas
        // sin preguntar ni nada, por lo que no hace falta pasar argumentos)
        void actualizar();

        // Aumenta el recuento de rondas ganadas en uno
        void ganarRonda();

        // Devuelve el número de rondas ganadas
        int getRondasGanadas();

        // Resetea los contadores de vibración y el contador atrasado de puntos de vida, pero no el
        // contador de rondas ganadas. Se usa cuando se cambia de ronda y los personajes vuelven a
        // tener todos sus puntos de vida
        void restablecerVida();

        // Le da la vuelta a los simbolitos de flechas del cartel que indica las
        // acciones que hay que llevar a cabo para poder realizar el ataque especial
        void voltear();

        // Dibuja la GUI en pantalla
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

