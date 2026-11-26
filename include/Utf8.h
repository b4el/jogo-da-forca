#pragma once

#include <string>

// Ferramentas para texto em UTF-8.
//
// Por que isso existe: um std::string guarda BYTES, nao letras. Em UTF-8, letras
// sem acento ocupam 1 byte, mas "A" com til ("Ã") ocupa 2. Entao "PÃO" tem 3 letras
// e 4 bytes, e palavra[1] nao e o "Ã", e so metade dele.
//
// A solucao e decodificar o texto para std::u32string: cada char32_t guarda uma
// letra inteira (um "code point" do Unicode). Ai sim, palavra[1] e o "Ã".
namespace utf8 {

// Converte UTF-8 em code points. Lanca std::invalid_argument se o texto nao for UTF-8 valido.
std::u32string decodificar(const std::string& texto);

// Converte code points de volta para UTF-8 (para mostrar na tela)
std::string codificar(char32_t letra);
std::string codificar(const std::u32string& texto);

// Letra maiuscula, inclusive acentuadas (a -> A, ã -> Ã, ç -> Ç). Outros caracteres voltam iguais.
char32_t maiuscula(char32_t c);

// Letra "base", sem acento e em maiuscula: á, Ã, â -> 'A'; ç -> 'C'.
// Devolve 0 se o caractere nao for uma letra que o jogo aceita.
char32_t letraBase(char32_t c);

// true para A-Z, a-z e as letras acentuadas do portugues (e outras do Latin-1)
bool ehLetra(char32_t c);

}  // namespace utf8
