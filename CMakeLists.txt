# 1: Crear el directorio build con `mkdir build`
# 2: Ir al directorio build recién creado con `cd build`
# 3: Usar CMake para crear toda la morralla en el directorio build. `cmake .. -G Ninja`
# 4: Compilar usando `ninja` o lo que hayas utilizado para compilar si en vez de `-G Ninja` has puesto algo distinto en el comando de arriba
# 5: Ejecutar el juego EraseUnCombate.exe en el directorio principal

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

# Se indica dónde están los hpp
include_directories(src/animaciones src/combate src/general src/menus src/recursos)

# Se ponen todos los ficheros .cpp
add_executable(EraseUnCombate
src/animaciones/Animacion.cpp
src/animaciones/AnimacionAgrandable.cpp
src/animaciones/AnimacionConGravedad.cpp
src/animaciones/AnimacionDesvaneciente.cpp
src/animaciones/AnimacionPorFrames.cpp
src/combate/AtaqueEspecial.cpp
src/combate/Combate.cpp
src/combate/ConectorOnline.cpp
src/combate/Escenario.cpp
src/combate/GUIPersonaje.cpp
src/combate/Hitbox.cpp
src/combate/Personaje.cpp
src/general/Bitacora.cpp
src/general/Utilidades.cpp
src/general/VentanaPrincipal.cpp
src/main.cpp
src/menus/MenuPrincipal.cpp
src/recursos/ContenedorDeEfectos.cpp
src/recursos/ContenedorDePersonajes.cpp
src/recursos/ContenedorDeRecursos.cpp
src/recursos/GestorDeControles.cpp
src/recursos/ReproductorDeMusica.cpp
src/recursos/ReproductorDeSonidos.cpp
src/recursos/SelectorJugadorParaMando.cpp)

# Se especifica que el estándar de C++ que se quiere usar es el de 2020
target_compile_features(EraseUnCombate PRIVATE cxx_std_20)

# Se especifican los cinco módulos de SFML y el módulo de yaml-cpp
target_link_libraries(EraseUnCombate PRIVATE SFML::Graphics SFML::Network SFML::Window SFML::System SFML::Audio yaml-cpp::yaml-cpp)

# Enlazar OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Opciones de compilación para que salgan warnings
add_compile_options(-Wall -Wextra -Wpedantic)