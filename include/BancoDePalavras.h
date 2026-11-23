#pragma once

#include <string>
#include <vector>

// Carrega palavras de um arquivo texto (uma por linha) e sorteia uma.
class BancoDePalavras {
public:
    // Tenta ler o arquivo; se nao conseguir, usa uma lista embutida.
    explicit BancoDePalavras(const std::string& caminhoArquivo);

    std::string sortear() const;
    size_t quantidade() const { return palavras_.size(); }
    bool carregouDoArquivo() const { return doArquivo_; }

private:
    std::vector<std::string> palavras_;
    bool doArquivo_ = false;
};
