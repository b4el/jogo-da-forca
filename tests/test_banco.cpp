// Testes do banco de palavras

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>

#include "BancoDePalavras.h"

namespace {

// Cria um arquivo temporario com o conteudo (em bytes) e devolve o nome
std::string arquivoTemporario(const std::string& conteudo) {
    const std::string nome = "palavras_teste_tmp.txt";
    std::ofstream f(nome, std::ios::binary);
    f << conteudo;
    return nome;
}

}  // namespace

TEST(BancoDePalavras, LeAcentosBomEFinaisDeLinhaDoWindows) {
    // BOM no comeco, linhas terminadas em \r\n, palavras com acento, linhas invalidas no meio
    // O BOM vem num texto comum ("..."), nao em u8"...": em u8"...", o Visual Studio
    // converteria cada \x em um caractere acentuado, em vez de gravar o byte exato.
    const std::string bom = "\xEF\xBB\xBF";
    std::string nome = arquivoTemporario(bom +
                                         u8"CORAÇÃO\r\n"
                                         u8"\r\n"
                                         u8"abc123\r\n"
                                         u8"  maçã  \r\n");
    BancoDePalavras banco(nome);
    std::remove(nome.c_str());

    EXPECT_TRUE(banco.carregouDoArquivo());
    ASSERT_EQ(banco.quantidade(), 2u);
    EXPECT_EQ(banco.palavras()[0], u8"CORAÇÃO");
    EXPECT_EQ(banco.palavras()[1], u8"maçã");
}

TEST(BancoDePalavras, ArquivoEmOutraCodificacaoEIgnorado) {
    // "MAÇÃ" salvo em Latin-1 (como o Bloco de Notas antigo fazia): nao e UTF-8
    std::string nome = arquivoTemporario("MA\xC7\xC3\nSOL\n");
    BancoDePalavras banco(nome);
    std::remove(nome.c_str());
    ASSERT_EQ(banco.quantidade(), 1u);
    EXPECT_EQ(banco.palavras()[0], "SOL");
}

TEST(BancoDePalavras, SemArquivoUsaListaPadrao) {
    BancoDePalavras banco("arquivo_que_nao_existe.txt");
    EXPECT_FALSE(banco.carregouDoArquivo());
    EXPECT_GT(banco.quantidade(), 0u);
    EXPECT_FALSE(banco.sortear().empty());
}
