# Paso 0: instalar dependencias. CMake, g++, curl, etc etc etc no sé,
# yo me instalé el MSYS2 y el mingw64 y ahí ya me lo daban todo (consultar
# el tutorial del fichero README.md para más info)
#
# Paso 1: Crear el directorio build con `mkdir build`
#
# Paso 2: Ir al directorio build recién creado con `cd build`
#
# Paso 3: Usar CMake para crear toda la morralla en el directorio
# build. `cmake .. -G Ninja`
# Paso 4: Compilar usando `ninja` o lo que hayas utilizado para compilar
# si en vez de `-G Ninja` has puesto algo distinto en el comando de arriba
#
# Paso 5: Ejecutar el juego EraseUnCombate.exe en el directorio principal
#
# ADICIONAL: si al ejecutar `cmake .. -G Ninja` parece que todo va bien pero
# de repente se para y no sale ningún mensaje de error, lo más seguro es que
# tu antivirus haya parado la compilación por pensar que e

cmake_minimum_required(VERSION 3.28)
project(EraseUnCombate LANGUAGES CXX)

# Se especifica que el estándar de C++ que se va a utilizar es el de 2020
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Se trae el SDK de Discord desde mi GitHub
include(FetchContent)
FetchContent_Declare(
    discord_sdk
    URL      https://github.com/MascaChapas27/Erase-Un-Combate/releases/download/deps/discord-social-sdk.zip
)
FetchContent_MakeAvailable(discord_sdk)

add_library(discord SHARED IMPORTED)

# El programa se creará en el directorio raíz del proyecto
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

# Si hay algún error saldrán colorines
set(CMAKE_COLOR_DIAGNOSTICS ON)

# Aunque se use CMake super moderno se permitirá que el CMake de SFML sea más viejuno
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# Cosa rara que se trae el repositorio entero de SFML de GitHub
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG master
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Lo mismo pero para traer el repositorio de yaml-cpp
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG master
)
FetchContent_MakeAvailable(yaml-cpp)

# Enlazar OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Para usar cpr para poder hacer peticiones a GitHub y así
# ver si hay actualizaciones del jueguito. Por cierto, cpr
# necesita curl, por lo que se descarga el código e intenta
# compilarlo. El problema es que da problemas al compilar, así
# que directamente le digo que use el curl que hay en el sistema
# y ya está, no me complico la vida
set(CPR_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE) # Esto porque da problemas al compilar
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG master)
FetchContent_MakeAvailable(cpr)

# Para usar el parseador de JSON y así permitir que el actualizador
# se comunique con la API de GitHub
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

# Opciones para que libzip compile lo justo y lo necesario y se deje
# de pachochadas
# set(BUILD_TOOLS OFF CACHE BOOL "" FORCE)
# set(BUILD_REGRESS OFF CACHE BOOL "" FORCE)
# set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# set(BUILD_DOC OFF CACHE BOOL "" FORCE)

# Opción para que libzip se compile como librería estática porque si
# no da errores to raros
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Libzip para poder extraer la nueva versión del juego al actualizarlo
FetchContent_Declare(
  libzip
  GIT_REPOSITORY https://github.com/nih-at/libzip.git
  # Cada dos por tres da problemas el libzip así que prefiero quedarme
  # en una versión estable antes que descargarme el main y que me salgan
  # 81923 errores nuevos
  GIT_TAG v1.11.1
)
FetchContent_MakeAvailable(libzip)

# Se indica dónde están los headers
include_directories(src/animaciones src/combate src/general src/menus src/recursos src/discord)

# Variables de utilidad para el SDK de Discord
set(DISCORD_SDK_LIB_DIR "${discord_sdk_SOURCE_DIR}/lib/release")
set(DISCORD_SDK_BIN_DIR "${discord_sdk_SOURCE_DIR}/bin/release")
set(DISCORD_SDK_INCLUDE_DIR "${discord_sdk_SOURCE_DIR}/include")

# Dependiendo de la plataforma se especifica dónde están
# las bibliotecas necesarias para el SDK de Discord
if(WIN32)
    set_target_properties(discord PROPERTIES
        IMPORTED_LOCATION "${DISCORD_SDK_BIN_DIR}/discord_partner_sdk.dll"
        IMPORTED_IMPLIB "${DISCORD_SDK_LIB_DIR}/discord_partner_sdk.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${DISCORD_SDK_INCLUDE_DIR}"
    )
    set(DISCORD_SHARED_LIB "${DISCORD_SDK_BIN_DIR}/discord_partner_sdk.dll")
elseif(APPLE)
    set_target_properties(discord PROPERTIES
        IMPORTED_LOCATION "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.dylib"
        IMPORTED_IMPLIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.dylib"
        INTERFACE_INCLUDE_DIRECTORIES "${DISCORD_SDK_INCLUDE_DIR}"
    )
    set(DISCORD_SHARED_LIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.dylib")
else() # Linux
    set_target_properties(discord PROPERTIES
        IMPORTED_LOCATION "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.so"
        IMPORTED_IMPLIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.so"
        INTERFACE_INCLUDE_DIRECTORIES "${DISCORD_SDK_INCLUDE_DIR}"
    )
    set(DISCORD_SHARED_LIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.so")
endif()

# Se ponen todos los ficheros .cpp
add_executable(EraseUnCombate

src/animaciones/Animacion.cpp
src/animaciones/AnimacionAgrandable.cpp
src/animaciones/AnimacionConGravedad.cpp
src/animaciones/AnimacionDesvaneciente.cpp
src/animaciones/AnimacionParticulaLineal.cpp
src/animaciones/AnimacionPorFotogramas.cpp
src/animaciones/AnimacionFondoBucleLineal.cpp

src/combate/AtaqueEspecial.cpp
src/combate/Combate.cpp
src/combate/ConectorOnline.cpp
src/combate/ContenedorDeCombos.cpp
src/combate/Escenario.cpp
src/combate/GUIPersonaje.cpp
src/combate/Hitbox.cpp
src/combate/InfoCombo.cpp
src/combate/Personaje.cpp
src/combate/Temporizador.cpp

src/discord/ClienteDiscord.cpp

src/general/Bitacora.cpp
src/general/Configuracion.cpp
src/general/Utilidades.cpp
src/general/VentanaPrincipal.cpp

src/main.cpp

src/menus/SelectorMenuPrincipal.cpp
src/menus/FondoMenuPrincipal.cpp
src/menus/MenuPrincipal.cpp
src/menus/SelectorPersonaje.cpp
src/menus/FondoPersonaje.cpp
src/menus/MenuSeleccionPersonaje.cpp

src/recursos/ContenedorDeEfectos.cpp
src/recursos/ContenedorDePersonajes.cpp
src/recursos/ContenedorDeRecursos.cpp
src/recursos/GestorDeControles.cpp
src/recursos/ReproductorDeMusica.cpp
src/recursos/ReproductorDeSonidos.cpp
src/recursos/SelectorJugadorParaMando.cpp

)

# Este otro ejecutable es para el actualizador
add_executable(EraseUnActualizador
src/actualizador/main.cpp)

# Se especifican directorios de include específicos al actualizador
target_include_directories(EraseUnActualizador PRIVATE
    ${zlib_SOURCE_DIR}
    ${minizip_SOURCE_DIR}/mz
    ${minizip_SOURCE_DIR}/lib
)

# Se especifican los cinco módulos de SFML, el módulo de yaml-cpp y el módulo de discord
target_link_libraries(EraseUnCombate PRIVATE SFML::Graphics SFML::Network SFML::Window SFML::System SFML::Audio yaml-cpp::yaml-cpp discord)
target_link_libraries(EraseUnActualizador PRIVATE SFML::Graphics SFML::Network SFML::Window SFML::System SFML::Audio cpr::cpr nlohmann_json::nlohmann_json zip)

# Se especifica la ruta en la que se buscarán las cosillas necesarias
# durante la ejecución (RPATH) para Linux y macOS
if(UNIX)
    # Se usa RPATH al compilar
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    # El RPATH usará el directorio lib relativo al ejecutable
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    if(APPLE)
        set(CMAKE_INSTALL_RPATH "@executable_path")
    endif()
endif()

# Se especifica que el juego no debería tener una ventana de consola de comandos (pero
# del actualizador no se dice nada porque este sí debería tener)
target_link_options(EraseUnCombate PRIVATE) # -mwindows)

# Opciones de compilación para que salgan warnings
add_compile_options(-Wall -Wextra -Wpedantic)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

# Se copia la biblioteca compartida del SDK de Discord para que esté
# disponible durante la ejecución
add_custom_command(TARGET EraseUnCombate POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${DISCORD_SHARED_LIB}"
    $<TARGET_FILE_DIR:EraseUnCombate>
)

# Se copian las bibliotecas compartidas de SFML
# add_custom_command(TARGET EraseUnCombate POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     "${SFML_BINARY_DIR}/bin/sfml-audio-3.dll"
#     $<TARGET_FILE_DIR:EraseUnCombate>
# )
# add_custom_command(TARGET EraseUnCombate POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     "${SFML_BINARY_DIR}/bin/sfml-system-3.dll"
#     $<TARGET_FILE_DIR:EraseUnCombate>
# )
# add_custom_command(TARGET EraseUnCombate POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     "${SFML_BINARY_DIR}/bin/sfml-graphics-3.dll"
#     $<TARGET_FILE_DIR:EraseUnCombate>
# )
# add_custom_command(TARGET EraseUnCombate POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     "${SFML_BINARY_DIR}/bin/sfml-window-3.dll"
#     $<TARGET_FILE_DIR:EraseUnCombate>
# )