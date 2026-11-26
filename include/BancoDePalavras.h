#pragma once

#include <string>
#include <vector>

// Carrega palavras de um arquivo texto em UTF-8 (uma por linha) e sorteia uma.
// Linhas vazias ou com algo que nao e letra sao ignoradas.
class BancoDePalavras {
public:
    // Tenta ler o arquivo; se nao conseguir, usa uma lista embutida.
    explicit BancoDePalavras(const std::string& caminhoArquivo);

    std::string sortear() const;
    size_t quantidade() const { return palavras_.size(); }
    bool carregouDoArquivo() const { return doArquivo_; }
    const std::vector<std::string>& palavras() const { return palavras_; }

private:
    std::vector<std::string> palavras_;
    bool doArquivo_ = false;
};
