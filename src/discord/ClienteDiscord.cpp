#include "ClienteDiscord.hpp"
#include "Bitacora.hpp"
#include <thread>
#include "Constantes.hpp"

ClienteDiscord *ClienteDiscord::clienteDiscord = nullptr;

ClienteDiscord *ClienteDiscord::unicaInstancia()
{
    if (clienteDiscord == nullptr)
        clienteDiscord = new ClienteDiscord();
    return clienteDiscord;
}

ClienteDiscord::~ClienteDiscord()
{
    if (clienteDiscord != nullptr)
        delete clienteDiscord;
}

void ClienteDiscord::inicializar()
{
    cliente = std::make_shared<discordpp::Client>();

    cliente->AddLogCallback([](auto message, auto severity) {
        Bitacora::unicaInstancia()->escribir("DISCORD - ["+std::string(EnumToString(severity))+"] "+message);
    }, discordpp::LoggingSeverity::Info);

    cliente->SetStatusChangedCallback([this](discordpp::Client::Status status, discordpp::Client::Error error, int32_t errorDetail) {
        Bitacora::unicaInstancia()->escribir("DISCORD - Cambiado estado: "+discordpp::Client::StatusToString(status));

        if (status == discordpp::Client::Status::Ready) {
            Bitacora::unicaInstancia()->escribir("DISCORD - Cliente correctamente configurado");
        } else if (error != discordpp::Client::Error::None) {
            Bitacora::unicaInstancia()->escribir("DISCORD - Error de conexión: "+discordpp::Client::ErrorToString(error)+" - Details: "+std::to_string(errorDetail));
        }
    });

    cliente->SetApplicationId(APPLICATION_ID);

    // discordpp::AuthorizationCodeVerifier verificadorDeCodigo = cliente->CreateAuthorizationCodeVerifier();

    // // Set up authentication arguments
    // discordpp::AuthorizationArgs args{};
    // args.SetClientId(APPLICATION_ID);
    // args.SetScopes(discordpp::Client::GetDefaultPresenceScopes());
    // args.SetCodeChallenge(verificadorDeCodigo.Challenge());

    // // Begin authentication process
    // cliente->Authorize(args, [this, verificadorDeCodigo](auto result, auto code, auto redirectUri) {
    //     if (!result.Successful()) {
    //         Bitacora::unicaInstancia()->escribir("DISCORD - Error de autenticación: "+result.Error());
    //         return;
    //     } else {    
    //         Bitacora::unicaInstancia()->escribir("DISCORD - Autorización exitosa. Obteniendo token de acceso...");

    //         // Exchange auth code for access token
    //         cliente->GetToken(APPLICATION_ID, code, verificadorDeCodigo.Verifier(), redirectUri,
    //         [this](discordpp::ClientResult result,
    //         std::string accessToken,
    //         std::string refreshToken,
    //         discordpp::AuthorizationTokenType tokenType,
    //         int32_t expiresIn,
    //         std::string scope) {
    //             Bitacora::unicaInstancia()->escribir("DISCORD - Token de acceso recibido. Estableciendo conexión...");
    //             // Next Step: Update the token and connect
    //             cliente->UpdateToken(discordpp::AuthorizationTokenType::Bearer,  accessToken, [this](discordpp::ClientResult result) {
    //             if(result.Successful()) {
    //                 Bitacora::unicaInstancia()->escribir("DISCORD - Token actualizado, conectando a Discord...");
    //                 cliente->Connect();
    //             }
    //             });
    //         });
    //     }
    // });
}

void ClienteDiscord::actualizarRichPresence(const std::string& descripcion, const std::string& estado)
{
    // Configure rich presence details
    discordpp::Activity activity;
    activity.SetType(discordpp::ActivityTypes::Playing);
    activity.SetDetails(descripcion);
    activity.SetState(estado);

    // Update rich presence
    cliente->UpdateRichPresence(activity, [](discordpp::ClientResult result) {
    if(result.Successful()) {
        Bitacora::unicaInstancia()->escribir("DISCORD - Rich Presence actualizado con éxito");
    } else {
        Bitacora::unicaInstancia()->escribir("DISCORD - No se pudo actualizar el Rich Presence");
    }
    });
}