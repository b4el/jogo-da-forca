#include "Forca.h"

#include <stdexcept>

#include "Utf8.h"

Forca::Forca(const std::string& palavraUtf8, int maxErros) : maxErros_(maxErros) {
    if (maxErros <= 0) throw std::invalid_argument("maxErros deve ser positivo.");

    std::u32string letras = utf8::decodificar(palavraUtf8);  // lanca se nao for UTF-8 valido
    if (letras.empty()) throw std::invalid_argument("A palavra nao pode ser vazia.");

    // Guarda tudo em maiusculas, mantendo os acentos para mostrar na tela
    for (char32_t c : letras) {
        if (utf8::letraBase(c) == 0) throw std::invalid_argument("A palavra deve conter apenas letras.");
        palavra_.push_back(utf8::maiuscula(c));
    }
}

Resultado Forca::chutar(const std::string& entradaUtf8) {
    std::u32string letras;
    try {
        letras = utf8::decodificar(entradaUtf8);
    } catch (const std::invalid_argument&) {
        return Resultado::Invalida;
    }
    if (letras.size() != 1) return Resultado::Invalida;  // vazio ou mais de uma letra

    // Compara sempre pela letra base: chutar "ã" e o mesmo que chutar "A"
    char32_t base = utf8::letraBase(letras[0]);
    if (base == 0) return Resultado::Invalida;

    if (acertos_.count(base) || erradas_.count(base)) return Resultado::Repetida;

    for (char32_t c : palavra_) {
        if (utf8::letraBase(c) == base) {
            acertos_.insert(base);
            return Resultado::Acerto;
        }
    }
    erradas_.insert(base);
    erros_++;
    return Resultado::Erro;
}

std::string Forca::palavraMascarada() const {
    std::string resultado;
    for (size_t i = 0; i < palavra_.size(); i++) {
        if (i > 0) resultado += ' ';
        // Mostra a letra com acento, do jeito que esta na palavra
        resultado += acertos_.count(utf8::letraBase(palavra_[i])) ? utf8::codificar(palavra_[i]) : "_";
    }
    return resultado;
}

std::string Forca::letrasErradas() const {
    std::string resultado;
    for (char32_t c : erradas_) {  // std::set ja mantem em ordem alfabetica
        if (!resultado.empty()) resultado += ' ';
        resultado += utf8::codificar(c);
    }
    return resultado;
}

std::string Forca::palavra() const { return utf8::codificar(palavra_); }

bool Forca::venceu() const {
    for (char32_t c : palavra_)
        if (!acertos_.count(utf8::letraBase(c))) return false;
    return true;
}

bool Forca::perdeu() const { return erros_ >= maxErros_; }
