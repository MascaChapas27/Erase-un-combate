#pragma once

#include <memory>
#include <discordpp.h>

/*
    Clase Singleton que se encarga de la conexión con
    Discord para poder tener una Rich Presence bien guapa
    (para que el Discord diga qué estás jugando)
*/
class ClienteDiscord
{
private:
    ClienteDiscord() {}
    ~ClienteDiscord();

    static ClienteDiscord *clienteDiscord;

    // El cliente del SDK de Discord
    std::shared_ptr<discordpp::Client> cliente;

public:
    // Inicializa el cliente, para lo que es necesario iniciar
    // sesión en Discord
    void inicializar();

    // Actualiza el Rich Presence de Discord
    void actualizarRichPresence(const std::string& estado, const std::string& descripcion);

    // NUNCA SE COPIA UN SINGLETON
    ClienteDiscord(ClienteDiscord &otro) = delete;

    // NUNCA SE ASIGNA UN SINGLETON
    void operator=(const ClienteDiscord &) = delete;

    // Devuelve la única instancia
    static ClienteDiscord *unicaInstancia();
};
