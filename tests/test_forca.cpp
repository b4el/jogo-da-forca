// Testes automaticos da classe Forca.
// Sem bibliotecas externas: cada CHECK que falha conta como erro.
// Rode com: ctest (veja o README).

#include <iostream>
#include <stdexcept>

#include "Forca.h"

static int falhas = 0;

#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::cerr << "FALHOU: " #cond " (linha " << __LINE__ << ")\n";       \
            falhas++;                                                            \
        }                                                                        \
    } while (0)

void testaMascaraInicial() {
    Forca f("casa");
    CHECK(f.palavraMascarada() == "_ _ _ _");
    CHECK(f.palavra() == "CASA");
}

void testaAcertoRevelaTodasAsOcorrencias() {
    Forca f("casa");
    CHECK(f.chutar('a') == Resultado::Acerto);
    CHECK(f.palavraMascarada() == "_ A _ A");
    CHECK(f.erros() == 0);
}

void testaErroContaTentativa() {
    Forca f("casa", 6);
    CHECK(f.chutar('z') == Resultado::Erro);
    CHECK(f.erros() == 1);
    CHECK(f.errosRestantes() == 5);
    CHECK(f.letrasErradas() == "Z");
}

void testaRepetidaEInvalidaNaoContamErro() {
    Forca f("casa");
    f.chutar('z');
    CHECK(f.chutar('Z') == Resultado::Repetida);  // maiuscula/minuscula tanto faz
    CHECK(f.chutar('3') == Resultado::Invalida);
    CHECK(f.erros() == 1);
}

void testaVitoria() {
    Forca f("sol");
    f.chutar('s');
    f.chutar('o');
    CHECK(!f.venceu());
    f.chutar('l');
    CHECK(f.venceu());
    CHECK(f.terminou());
}

void testaDerrota() {
    Forca f("sol", 2);
    f.chutar('x');
    CHECK(!f.perdeu());
    f.chutar('y');
    CHECK(f.perdeu());
    CHECK(f.terminou());
}

void testaPalavraInvalidaLancaExcecao() {
    bool lancou = false;
    try {
        Forca f("abc123");
    } catch (const std::invalid_argument&) {
        lancou = true;
    }
    CHECK(lancou);
}

int main() {
    testaMascaraInicial();
    testaAcertoRevelaTodasAsOcorrencias();
    testaErroContaTentativa();
    testaRepetidaEInvalidaNaoContamErro();
    testaVitoria();
    testaDerrota();
    testaPalavraInvalidaLancaExcecao();

    if (falhas == 0) {
        std::cout << "Todos os testes passaram!\n";
        return 0;
    }
    std::cerr << falhas << " teste(s) falharam.\n";
    return 1;
}
