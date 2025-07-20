#ifndef __INDICACIONES_SOBRE_ANIMACION_HPP__
#define __INDICACIONES_SOBRE_ANIMACION_HPP__

#include <SFML/System.hpp>
#include <string>

// Este struct se utiliza en la clase AnimacionPorFrames para
// indicar las características de las animaciones que aparecen dentro
// de otras animaciones (por ejemplo, cuando Juan Cuesta dispara
// el rifle en su superataque, la animación de Juan Cuesta disparando
// hace aparecer la animación de la bala moviendo el aire, la cual
// tiene la hitbox que golpea al enemigo. Este struct indica las
// características de esa animación).
struct IndicacionesSobreAnimacion {
    
    // Ruta de la animación para poder encontrarla después
    std::string rutaAnimacion;

    // Posición en la que aparece la animación
    sf::Vector2f posicionInicial;

    // Velocidad inicial de la animación (si es una animación con gravedad)
    sf::Vector2f velocidadInicial;
    
};

#endif