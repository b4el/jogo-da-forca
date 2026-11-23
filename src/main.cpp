// Jogo da Forca - interface de terminal.
// As regras ficam na classe Forca; aqui so lemos o teclado e desenhamos a tela.

#include <iostream>
#include <string>

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
        if (entrada.size() != 1) {
            std::cout << ">> Digite apenas UMA letra.\n";
            continue;
        }

        switch (jogo.chutar(entrada[0])) {
            case Resultado::Acerto:   std::cout << ">> Boa! Tem essa letra.\n"; break;
            case Resultado::Erro:     std::cout << ">> Nao tem essa letra.\n"; break;
            case Resultado::Repetida: std::cout << ">> Voce ja chutou essa letra.\n"; break;
            case Resultado::Invalida: std::cout << ">> Isso nao e uma letra.\n"; break;
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
    BancoDePalavras banco("palavras.txt");
    std::cout << "=== JOGO DA FORCA ===\n";
    if (!banco.carregouDoArquivo())
        std::cout << "(palavras.txt nao encontrado, usando lista padrao)\n";

    std::string resposta = "s";
    while (resposta == "s" || resposta == "S") {
        jogarPartida(banco.sortear());
        std::cout << "\nJogar de novo? (s/n): ";
        if (!std::getline(std::cin, resposta)) break;
    }

    std::cout << "Valeu por jogar!\n";
    return 0;
}
