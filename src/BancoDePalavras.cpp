#include "BancoDePalavras.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <random>

namespace {  // namespace anonimo: estas funcoes so existem neste arquivo

bool palavraValida(const std::string& p) {
    return !p.empty() && std::all_of(p.begin(), p.end(), [](char c) {
        return std::isalpha(static_cast<unsigned char>(c));
    });
}

std::string limpar(std::string s) {
    // remove espacos e '\r' (arquivos salvos no Windows terminam linhas com \r\n)
    s.erase(std::remove_if(s.begin(), s.end(),
                           [](char c) { return std::isspace(static_cast<unsigned char>(c)); }),
            s.end());
    return s;
}

}  // namespace

BancoDePalavras::BancoDePalavras(const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    std::string linha;
    while (std::getline(arquivo, linha)) {
        linha = limpar(linha);
        if (palavraValida(linha)) palavras_.push_back(linha);
    }

    doArquivo_ = !palavras_.empty();
    if (!doArquivo_) {
        palavras_ = {"COMPUTADOR", "TECLADO", "PROGRAMA", "VARIAVEL", "FUNCAO"};
    }
}

std::string BancoDePalavras::sortear() const {
    static std::mt19937 gerador{std::random_device{}()};
    std::uniform_int_distribution<size_t> dist(0, palavras_.size() - 1);
    return palavras_[dist(gerador)];
}
