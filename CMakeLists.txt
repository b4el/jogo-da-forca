cmake_minimum_required(VERSION 3.16)
project(JogoDaForca LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Avisos do compilador ligados: ajudam a achar bugs cedo
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Biblioteca com as regras do jogo (usada pelo jogo E pelos testes)
add_library(forca_core
    src/Forca.cpp
    src/BancoDePalavras.cpp
)
target_include_directories(forca_core PUBLIC include)

# Executavel do jogo
add_executable(forca src/main.cpp)
target_link_libraries(forca PRIVATE forca_core)

# Copia palavras.txt para ao lado do executavel depois de compilar
add_custom_command(TARGET forca POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/data/palavras.txt
            $<TARGET_FILE_DIR:forca>/palavras.txt
)

# Testes
enable_testing()
add_executable(testes tests/test_forca.cpp)
target_link_libraries(testes PRIVATE forca_core)
add_test(NAME testes_forca COMMAND testes)
