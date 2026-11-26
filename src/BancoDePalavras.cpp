#include "BancoDePalavras.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <random>
#include <stdexcept>

#include "Utf8.h"

namespace {  // namespace anonimo: estas funcoes so existem neste arquivo

// A palavra e valida se for UTF-8 e tiver so letras (com ou sem acento)
bool palavraValida(const std::string& p) {
    try {
        std::u32string letras = utf8::decodificar(p);
        return !letras.empty() &&
               std::all_of(letras.begin(), letras.end(), [](char32_t c) { return utf8::letraBase(c) != 0; });
    } catch (const std::invalid_argument&) {
        return false;  // arquivo salvo em outra codificacao (ex.: ANSI do Bloco de Notas antigo)
    }
}

std::string limpar(std::string s) {
    // Remove o "BOM" (EF BB BF): alguns editores do Windows colocam esses 3 bytes
    // invisiveis no comeco de arquivos UTF-8
    if (s.size() >= 3 && s.compare(0, 3, "\xEF\xBB\xBF") == 0) s.erase(0, 3);

    // Remove espacos e '\r' (arquivos salvos no Windows terminam linhas com \r\n)
    s.erase(std::remove_if(s.begin(), s.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }),
            s.end());
    return s;
}

}  // namespace

BancoDePalavras::BancoDePalavras(const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo, std::ios::binary);  // binario: le os bytes UTF-8 sem conversao
    std::string linha;
    while (std::getline(arquivo, linha)) {
        linha = limpar(linha);
        if (palavraValida(linha)) palavras_.push_back(linha);
    }

    doArquivo_ = !palavras_.empty();
    if (!doArquivo_) {
        // u8"...": o compilador grava o texto em UTF-8, com os acentos
        palavras_ = {u8"COMPUTADOR", u8"TECLADO", u8"PROGRAMA", u8"VARIÁVEL", u8"FUNÇÃO"};
    }
}

std::string BancoDePalavras::sortear() const {
    static std::mt19937 gerador{std::random_device{}()};
    std::uniform_int_distribution<size_t> dist(0, palavras_.size() - 1);
    return palavras_[dist(gerador)];
}
