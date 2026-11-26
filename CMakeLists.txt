cmake_minimum_required(VERSION 3.16)
project(JogoDaForca LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gera compile_commands.json (usado pelo clang-tidy e por editores)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Opcao de qualidade (desligada por padrao):
#   cmake -S . -B build-asan -DFORCA_SANITIZERS=ON  -> AddressSanitizer + UndefinedBehaviorSanitizer
option(FORCA_SANITIZERS "Compilar com AddressSanitizer e UBSan (GCC/Clang)" OFF)
if(FORCA_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all)
    add_link_options(-fsanitize=address,undefined)
endif()

# Avisos do compilador ligados no nosso codigo: ajudam a achar bugs cedo.
# /utf-8: o Visual Studio le os arquivos-fonte como UTF-8 e grava os textos com acento em UTF-8.
if(MSVC)
    set(AVISOS /W4 /utf-8)
else()
    set(AVISOS -Wall -Wextra -Wpedantic)
endif()

# Biblioteca com as regras do jogo (usada pelo jogo E pelos testes)
add_library(forca_core
    src/BancoDePalavras.cpp
    src/Forca.cpp
    src/Utf8.cpp
)
target_include_directories(forca_core PUBLIC include)
target_compile_options(forca_core PRIVATE ${AVISOS})

# Executavel do jogo
add_executable(forca src/main.cpp)
target_link_libraries(forca PRIVATE forca_core)
target_compile_options(forca PRIVATE ${AVISOS})

# Copia palavras.txt para ao lado do executavel depois de compilar
add_custom_command(TARGET forca POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/data/palavras.txt
            $<TARGET_FILE_DIR:forca>/palavras.txt
)

# ---------- Testes (GoogleTest, baixado automaticamente) ----------
include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
    GIT_SHALLOW    TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # Windows: mesma biblioteca de runtime do projeto
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(testes
    tests/test_forca.cpp
    tests/test_utf8.cpp
    tests/test_banco.cpp
)
target_link_libraries(testes PRIVATE forca_core GTest::gtest_main)
target_compile_options(testes PRIVATE ${AVISOS})
include(GoogleTest)
gtest_discover_tests(testes)  # cada TEST vira um teste separado no ctest e no Visual Studio
