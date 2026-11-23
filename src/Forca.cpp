#include "Forca.h"

#include <cctype>
#include <stdexcept>

Forca::Forca(const std::string& palavra, int maxErros) : maxErros_(maxErros) {
    if (palavra.empty()) throw std::invalid_argument("A palavra nao pode ser vazia.");
    if (maxErros <= 0) throw std::invalid_argument("maxErros deve ser positivo.");

    // Guarda tudo em maiusculas para comparar sem se preocupar com caixa
    for (char c : palavra) {
        if (!std::isalpha(static_cast<unsigned char>(c)))
            throw std::invalid_argument("A palavra deve conter apenas letras.");
        palavra_ += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
}

Resultado Forca::chutar(char letra) {
    if (!std::isalpha(static_cast<unsigned char>(letra))) return Resultado::Invalida;
    letra = static_cast<char>(std::toupper(static_cast<unsigned char>(letra)));

    if (acertos_.count(letra) || erradas_.count(letra)) return Resultado::Repetida;

    if (palavra_.find(letra) != std::string::npos) {
        acertos_.insert(letra);
        return Resultado::Acerto;
    }
    erradas_.insert(letra);
    erros_++;
    return Resultado::Erro;
}

std::string Forca::palavraMascarada() const {
    std::string resultado;
    for (size_t i = 0; i < palavra_.size(); i++) {
        if (i > 0) resultado += ' ';
        resultado += acertos_.count(palavra_[i]) ? palavra_[i] : '_';
    }
    return resultado;
}

std::string Forca::letrasErradas() const {
    std::string resultado;
    for (char c : erradas_) {  // std::set ja mantem em ordem alfabetica
        if (!resultado.empty()) resultado += ' ';
        resultado += c;
    }
    return resultado;
}

bool Forca::venceu() const {
    for (char c : palavra_)
        if (!acertos_.count(c)) return false;
    return true;
}

bool Forca::perdeu() const {
    return erros_ >= maxErros_;
}
