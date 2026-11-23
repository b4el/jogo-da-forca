#pragma once  // evita que este arquivo seja incluido duas vezes

#include <set>
#include <string>

// Resultado de um chute de letra
enum class Resultado {
    Acerto,    // a letra existe na palavra
    Erro,      // a letra nao existe na palavra
    Repetida,  // a letra ja tinha sido chutada
    Invalida   // nao e uma letra (ex.: '3', '!')
};

// Guarda o estado de uma partida de forca.
// Nao le teclado nem imprime nada: so as regras do jogo.
// Assim fica facil de testar (veja tests/test_forca.cpp).
class Forca {
public:
    explicit Forca(const std::string& palavra, int maxErros = 6);

    Resultado chutar(char letra);

    std::string palavraMascarada() const;  // ex.: "C _ _ _ _ _ _"
    std::string letrasErradas() const;     // ex.: "A E X"
    const std::string& palavra() const { return palavra_; }

    int erros() const { return erros_; }
    int maxErros() const { return maxErros_; }
    int errosRestantes() const { return maxErros_ - erros_; }

    bool venceu() const;
    bool perdeu() const;
    bool terminou() const { return venceu() || perdeu(); }

private:
    std::string palavra_;
    int maxErros_;
    int erros_ = 0;
    std::set<char> acertos_;
    std::set<char> erradas_;
};
