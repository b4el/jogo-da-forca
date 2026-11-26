#pragma once  // evita que este arquivo seja incluido duas vezes

#include <set>
#include <string>

// Resultado de um chute de letra
enum class Resultado {
    Acerto,    // a letra existe na palavra
    Erro,      // a letra nao existe na palavra
    Repetida,  // a letra ja tinha sido chutada
    Invalida   // nao e uma letra (ex.: '3', '!') ou e mais de uma letra
};

// Guarda o estado de uma partida de forca.
// Nao le teclado nem imprime nada: so as regras do jogo.
// Assim fica facil de testar (veja tests/test_forca.cpp).
//
// Acentos: a palavra pode ter acentos (ex.: "CORAÇÃO"). O jogador chuta a letra
// sem acento, e o chute "A" revela A, Á, Â e Ã de uma vez; "C" revela C e Ç.
// Todo texto que entra e sai desta classe esta em UTF-8.
class Forca {
public:
    // Lanca std::invalid_argument se a palavra estiver vazia ou tiver algo que nao e letra
    explicit Forca(const std::string& palavraUtf8, int maxErros = 6);

    // Chute de uma letra digitada pelo jogador (UTF-8: aceita "a", "A", "ã", "ç"...)
    Resultado chutar(const std::string& entradaUtf8);
    Resultado chutar(char letra) { return chutar(std::string(1, letra)); }

    std::string palavraMascarada() const;               // ex.: "C O R A _ Ã O"
    std::string letrasErradas() const;                  // ex.: "B E X"
    std::string palavra() const;                        // a palavra inteira, em maiusculas e com acentos
    size_t tamanho() const { return palavra_.size(); }  // em letras, nao em bytes

    int erros() const { return erros_; }
    int maxErros() const { return maxErros_; }
    int errosRestantes() const { return maxErros_ - erros_; }

    bool venceu() const;
    bool perdeu() const;
    bool terminou() const { return venceu() || perdeu(); }

private:
    std::u32string palavra_;  // uma letra por posicao (veja Utf8.h)
    int maxErros_;
    int erros_ = 0;
    std::set<char32_t> acertos_;  // letras base (sem acento) ja acertadas
    std::set<char32_t> erradas_;
};
