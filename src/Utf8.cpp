#include "Utf8.h"

#include <stdexcept>

namespace utf8 {

std::u32string decodificar(const std::string& texto) {
    std::u32string resultado;
    size_t i = 0;
    while (i < texto.size()) {
        const auto b0 = static_cast<unsigned char>(texto[i]);

        // Quantos bytes tem esta letra? O primeiro byte diz:
        //   0xxxxxxx -> 1 byte   110xxxxx -> 2   1110xxxx -> 3   11110xxx -> 4
        size_t tamanho = 0;
        char32_t valor = 0;
        if (b0 < 0x80) {
            tamanho = 1;
            valor = b0;
        } else if ((b0 & 0xE0) == 0xC0) {
            tamanho = 2;
            valor = b0 & 0x1F;
        } else if ((b0 & 0xF0) == 0xE0) {
            tamanho = 3;
            valor = b0 & 0x0F;
        } else if ((b0 & 0xF8) == 0xF0) {
            tamanho = 4;
            valor = b0 & 0x07;
        } else {
            throw std::invalid_argument("Texto nao e UTF-8 valido.");
        }
        if (i + tamanho > texto.size()) throw std::invalid_argument("Texto UTF-8 cortado no meio de uma letra.");

        // Os bytes seguintes comecam com 10xxxxxx e trazem 6 bits cada
        for (size_t k = 1; k < tamanho; k++) {
            const auto b = static_cast<unsigned char>(texto[i + k]);
            if ((b & 0xC0) != 0x80) throw std::invalid_argument("Texto nao e UTF-8 valido.");
            valor = (valor << 6) | (b & 0x3F);
        }

        // Rejeita formas "longas demais" (ex.: 'A' escrito com 2 bytes), que o UTF-8 proibe
        const char32_t minimo[] = {0, 0, 0x80, 0x800, 0x10000};
        if (valor < minimo[tamanho] || valor > 0x10FFFF || (valor >= 0xD800 && valor <= 0xDFFF))
            throw std::invalid_argument("Texto nao e UTF-8 valido.");

        resultado.push_back(valor);
        i += tamanho;
    }
    return resultado;
}

std::string codificar(char32_t c) {
    std::string s;
    if (c < 0x80) {
        s += static_cast<char>(c);
    } else if (c < 0x800) {
        s += static_cast<char>(0xC0 | (c >> 6));
        s += static_cast<char>(0x80 | (c & 0x3F));
    } else if (c < 0x10000) {
        s += static_cast<char>(0xE0 | (c >> 12));
        s += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
        s += static_cast<char>(0x80 | (c & 0x3F));
    } else {
        s += static_cast<char>(0xF0 | (c >> 18));
        s += static_cast<char>(0x80 | ((c >> 12) & 0x3F));
        s += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
        s += static_cast<char>(0x80 | (c & 0x3F));
    }
    return s;
}

std::string codificar(const std::u32string& texto) {
    std::string s;
    for (char32_t c : texto) s += codificar(c);
    return s;
}

// No Unicode, as letras acentuadas do portugues ficam no bloco "Latin-1" (U+00C0 a U+00FF).
// Nesse bloco, cada minuscula fica exatamente 0x20 depois da sua maiuscula, como no ASCII:
// 'a' (0x61) e 'A' (0x41); 'ã' (0xE3) e 'Ã' (0xC3). As excecoes sao os sinais × e ÷.
bool ehLetra(char32_t c) {
    if ((c >= U'A' && c <= U'Z') || (c >= U'a' && c <= U'z')) return true;
    return c >= 0xC0 && c <= 0xFF && c != 0xD7 && c != 0xF7;
}

char32_t maiuscula(char32_t c) {
    if (c >= U'a' && c <= U'z') return c - 0x20;
    if (c >= 0xE0 && c <= 0xFE && c != 0xF7) return c - 0x20;
    return c;
}

char32_t letraBase(char32_t c) {
    if (!ehLetra(c)) return 0;
    c = maiuscula(c);
    if (c >= U'A' && c <= U'Z') return c;

    // Os codigos U+00C0.. sao as maiusculas acentuadas (veja a tabela Latin-1 do Unicode)
    switch (c) {
        case 0xC0:
        case 0xC1:
        case 0xC2:
        case 0xC3:
        case 0xC4:
        case 0xC5: return U'A';  // À Á Â Ã Ä Å
        case 0xC7: return U'C';  // Ç
        case 0xC8:
        case 0xC9:
        case 0xCA:
        case 0xCB: return U'E';  // È É Ê Ë
        case 0xCC:
        case 0xCD:
        case 0xCE:
        case 0xCF: return U'I';  // Ì Í Î Ï
        case 0xD1: return U'N';  // Ñ
        case 0xD2:
        case 0xD3:
        case 0xD4:
        case 0xD5:
        case 0xD6: return U'O';  // Ò Ó Ô Õ Ö
        case 0xD9:
        case 0xDA:
        case 0xDB:
        case 0xDC: return U'U';  // Ù Ú Û Ü
        case 0xDD: return U'Y';  // Ý
        default: return 0;       // letras como Æ, Ð, Ø, Þ, ß: o jogo nao usa
    }
}

}  // namespace utf8
