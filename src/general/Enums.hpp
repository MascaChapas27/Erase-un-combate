#pragma once

/*
    Este fichero contiene enumerados (enums)
*/

// Enumerado para acciones que se pueden llevar a cabo al pulsar teclas
enum class Accion {
    NADA, IZQUIERDA, DERECHA, ARRIBA, ABAJO, ATACAR, ESCAPE
};

// Tipo de bucle en una animación
enum class TipoBucle {
    SIN_BUCLE,      // No hay bucle, cuando la animación termina se deja de actualizar
    NORMAL          // Cuando termina la animación, vuelve a empezar desde el principio
};

// Enumerado para estados en los que se puede encontrar un personaje
enum class EstadoPersonaje {
    QUIETO,             // No hace nada, solo está quieto en el sitio
    AGACHADO,           // Está agachado y quieto
    ATAQUE_AGACHADO,    // Está atacando mientras está agachado
    ATAQUE_NORMAL_1,    // Está atacando de pie (primer ataque)
    ATAQUE_NORMAL_2,    // Está atacando de pie (segundo ataque)
    ATAQUE_NORMAL_3,    // Está atacando de pie (tercer ataque)
    ATAQUE_ALEJANDOSE,  // Está atacando mientras anda al revés del enemigo
    ATAQUE_ACERCANDOSE, // Está atacando mientras anda hacia el enemigo
    ATAQUE_AEREO,       // Está atacando en el aire
    ATAQUE_CORRIENDO,   // Está atacando tras correr
    ATAQUE_ESPECIAL,    // Está haciendo el ataque especial
    GOLPEADO_PEQUE,     // Ha sufrido un ataque pequeño
    GOLPEADO_MEDIO,     // Ha sufrido un ataque medio
    GOLPEADO_BAJANDO,   // Ha sufrido un ataque que lo ha levantado por los aires
    GOLPEADO_SUBIENDO,  // Ha sufrido un ataque que lo ha levantado y ahora está cayendo
    SALTANDO_SUBIENDO,  // Está en el aire subiendo después de saltar
    SALTANDO_BAJANDO,   // Está en el aire bajando después de saltar y alcanzar la máxima altura
    TOCANDO_SUELO,      // Acaba de caer bien después de saltar y se está colocando
    ANDANDO_ACERCANDOSE,// Está andando hacia el enemigo
    CORRIENDO,          // Está corriendo con intención asesina hacia el enemigo
    FRENANDO,           // Está arrastrando los pies contra el suelo para frenar después de correr
    ANDANDO_ALEJANDOSE, // Está andando alejándose del enemigo
    BLOQUEANDO,         // Está bloqueando un ataque
    ESQUIVE_SUPER,      // Está esquivando un ataque súper
    PREPARANDO_SUPER,   // Está preparándose para lanzar el super ataque
    ATAQUE_SUPER,       // Está haciendo el super ataque
    TUMBADO,            // Está en el suelo tumbado después de que un ataque lo mandara volando
    LEVANTANDOSE,       // Se está levantando del suelo después de estar tumbado
    CELEBRANDO,         // Está celebrando su victoria
    DERROTA             // Ha perdido por tiempo o ha empatado, así que se está lamentando
};

enum class TipoPlantilla {
    PERSONAJE, EFECTO
};

// Enumerado para los jugadores
enum class Jugador {
    NADIE, JUGADOR1, JUGADOR2
};

// Posibles controles que un jugador puede utilizar
enum class Control{
    TECLADO_IZQUIERDA, TECLADO_DERECHA,
    MANDO0, MANDO1, MANDO2, MANDO3,
    MANDO4, MANDO5, MANDO6, MANDO7
};

// Indica el tipo de la sombra para una AnimacionPorFotogramas
enum class TipoSombra {
    NORMAL,
    LARGA,
    SIN_SOMBRA
};

// Especifica el tipo de un selector del menú principal
enum class TipoSelectorMenuPrincipal {
    MODO_HISTORIA, BATALLA_VS, OPCIONES,
};

// Especifica en qué dirección se mueven los AnimacionFondoBucleLineal
enum class Direccion {
    ARRIBA_IZQUIERDA,   ARRIBA,     ARRIBA_DERECHA,
    IZQUIERDA,                      DERECHA,
    ABAJO_IZQUIERDA,    ABAJO,      ABAJO_DERECHA
};