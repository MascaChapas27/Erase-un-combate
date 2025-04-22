#pragma once

#include <optional>
#include "Animacion.hpp"

/*
    Esta clase representa animaciones que caen con gravedad y rebotan con las paredes
*/
class AnimacionConGravedad : public Animacion
{
private:
    // Hitbox del objeto (como va a girar es bueno que tenga solo una,
    // y que sea lo más cuadrada y centrada posible)
    std::optional<Hitbox> hitbox;

    // Velocidad del objeto volador
    sf::Vector2f velocidad;

    // Velocidad de giro del objeto volador en grados sexagesimales por frame
    double velocidadGiro;

    // Indica si el objeto se ha chocado, por lo que empieza a parpadear y tal
    bool haChocado;

    // Se va sumando 1 por cada frame que pase desde que el objeto se choca contra algo,
    // permitiendo que parpadee y finalmente desaparezca.
    int contadorParpadeo;

public:
    // Constructor que crea una animación con gravedad y todas sus pescas
    AnimacionConGravedad(sf::Texture &textura, sf::Vector2f posicion, sf::Vector2f velocidad, double velocidadGiro, std::string rutaSonido);

    // Actualiza la animación
    void actualizar(std::list<std::shared_ptr<Animacion>> &nuevasAnimaciones);

    // Resetea la animación al estado inicial
    void resetear();

    // Devuelve una lista con las hitboxes
    std::list<Hitbox> getHitboxes();

    // Determina si una animación ha terminado
    bool haTerminado();

    // Le da la vuelta al sprite, a la hitbox y a la velocidad
    void voltear();

    // Establece la velocidad de movimiento de la animación
    void setVelocidad(sf::Vector2f velocidad);

    // Establece la velocidad de giro de la animación
    void setVelocidadGiro(const double &velocidadGiro);

    // Establece la hitbox
    void setHitbox(Hitbox hitbox);

    // Devuelve un clon de la animación
    std::shared_ptr<Animacion> clonar();

    // Indica a la animación que ha chocado con una hitbox
    void chocar(Hitbox hitbox);

    // Dibuja la animación con gravedad, pudiendo no dibujarla a veces para el efecto
    // de parpadeo al chocarse
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

