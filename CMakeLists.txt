# Paso 0: instalar dependencias. CMake, g++, curl, etc etc etc no sé, yo me instalé el MSYS2 y el mingw64 y ahí ya me lo daban todo
# Paso 1: Crear el directorio build con `mkdir build`
# Paso 2: Ir al directorio build recién creado con `cd build`
# Paso 3: Usar CMake para crear toda la morralla en el directorio build. `cmake .. -G Ninja`
# Paso 4: Compilar usando `ninja` o lo que hayas utilizado para compilar si en vez de `-G Ninja` has puesto algo distinto en el comando de arriba
# Paso 5: Ejecutar el juego EraseUnCombate.exe en el directorio principal

cmake_minimum_required(VERSION 3.28)
project(EraseUnCombate LANGUAGES CXX)

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
# y ya está, no me complico la vida (para instalar curl con MSYS2:
# pacman -S mingw-w64-x86_64-curl)
# (Para instalarlo en Linux: sudo apt-get install curl libssl-dev libcurl4-openssl-dev)
set(CPR_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE) # Esto porque da problemas al compilar
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG master)
FetchContent_MakeAvailable(cpr)

# Para usar el parseador de JSON y así permitir que el actualizador
# se comunique con la API de GitHub
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

# Libzip para poder extraer la nueva versión del juego al actualizarlo
FetchContent_Declare(
  libzip
  GIT_REPOSITORY https://github.com/nih-at/libzip.git
  GIT_TAG main
)
FetchContent_MakeAvailable(libzip)

# Se indica dónde están los hpp
include_directories(src/animaciones src/combate src/general src/menus src/recursos)

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

# Se especifica que el estándar de C++ que se quiere usar es el de 2020
target_compile_features(EraseUnCombate PRIVATE cxx_std_20)
# target_compile_features(EraseUnActualizador PRIVATE cxx_std_20)

# Se especifican directorios de include específicos al actualizador
target_include_directories(EraseUnActualizador PRIVATE
    ${zlib_SOURCE_DIR}
    ${minizip_SOURCE_DIR}/mz
    ${minizip_SOURCE_DIR}/lib
)

# Se especifican los cinco módulos de SFML y el módulo de yaml-cpp
target_link_libraries(EraseUnCombate PRIVATE SFML::Graphics SFML::Network SFML::Window SFML::System SFML::Audio yaml-cpp::yaml-cpp)
target_link_libraries(EraseUnActualizador PRIVATE SFML::Graphics SFML::Network SFML::Window SFML::System SFML::Audio cpr::cpr nlohmann_json::nlohmann_json zip)

# Se especifica que el juego no debería tener una ventana de consola de comandos (pero
# del actualizador no se dice nada porque este sí debería tener)
target_link_options(EraseUnCombate PRIVATE) # -mwindows)

# Opciones de compilación para que salgan warnings
add_compile_options(-Wall -Wextra -Wpedantic)
