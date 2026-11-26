// Jogo da Forca - interface de terminal.
// As regras ficam na classe Forca; aqui so lemos o teclado e desenhamos a tela.

#include <iostream>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>  // so para colocar o console do Windows em UTF-8
#endif

#include "BancoDePalavras.h"
#include "Forca.h"

namespace {

// Desenho do boneco para 0..6 erros
const char* const DESENHOS[] = {
    "  +---+\n  |   |\n      |\n      |\n      |\n=========",
    "  +---+\n  |   |\n  O   |\n      |\n      |\n=========",
    "  +---+\n  |   |\n  O   |\n  |   |\n      |\n=========",
    "  +---+\n  |   |\n  O   |\n /|   |\n      |\n=========",
    "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n=========",
    "  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n=========",
    "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n=========",
};

void desenharTela(const Forca& jogo) {
    std::cout << "\n" << DESENHOS[jogo.erros()] << "\n\n";
    std::cout << "Palavra: " << jogo.palavraMascarada() << "\n";
    std::cout << "Erradas: " << jogo.letrasErradas() << "\n";
    std::cout << "Tentativas restantes: " << jogo.errosRestantes() << "\n";
}

void jogarPartida(const std::string& palavra) {
    Forca jogo(palavra);

    while (!jogo.terminou()) {
        desenharTela(jogo);
        std::cout << "Chute uma letra: ";

        std::string entrada;
        if (!std::getline(std::cin, entrada)) return;  // fim da entrada (Ctrl+Z / Ctrl+D)

        // A classe Forca entende UTF-8: "a", "A", "ã" e "Ã" contam como o mesmo chute
        switch (jogo.chutar(entrada)) {
            case Resultado::Acerto: std::cout << ">> Boa! Tem essa letra.\n"; break;
            case Resultado::Erro: std::cout << ">> Nao tem essa letra.\n"; break;
            case Resultado::Repetida: std::cout << ">> Voce ja chutou essa letra.\n"; break;
            case Resultado::Invalida: std::cout << ">> Digite UMA letra (sem acento tambem vale).\n"; break;
        }
    }

    desenharTela(jogo);
    if (jogo.venceu())
        std::cout << "\n*** PARABENS! Voce acertou: " << jogo.palavra() << " ***\n";
    else
        std::cout << "\n*** Enforcado! A palavra era: " << jogo.palavra() << " ***\n";
}

}  // namespace

int main() {
#ifdef _WIN32
    // O console do Windows nao usa UTF-8 por padrao: sem isso, "Ã" apareceria como "Ãƒ"
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    BancoDePalavras banco("palavras.txt");
    std::cout << "=== JOGO DA FORCA ===\n";
    std::cout << "Dica: chute as letras sem acento. O A revela Á, Â e Ã; o C revela Ç.\n";
    if (!banco.carregouDoArquivo()) std::cout << "(palavras.txt nao encontrado, usando lista padrao)\n";

    std::string resposta = "s";
    while (resposta == "s" || resposta == "S") {
        jogarPartida(banco.sortear());
        std::cout << "\nJogar de novo? (s/n): ";
        if (!std::getline(std::cin, resposta)) break;
    }

    std::cout << "Valeu por jogar!\n";
    return 0;
}
