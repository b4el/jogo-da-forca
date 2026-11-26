// Testes das regras do jogo (classe Forca)

#include <gtest/gtest.h>

#include <stdexcept>

#include "Forca.h"

TEST(Forca, ComecaComTudoEscondido) {
    Forca f("casa");
    EXPECT_EQ(f.palavraMascarada(), "_ _ _ _");
    EXPECT_EQ(f.palavra(), "CASA");
    EXPECT_EQ(f.tamanho(), 4u);
}

TEST(Forca, AcertoRevelaTodasAsOcorrencias) {
    Forca f("casa");
    EXPECT_EQ(f.chutar('a'), Resultado::Acerto);
    EXPECT_EQ(f.palavraMascarada(), "_ A _ A");
    EXPECT_EQ(f.erros(), 0);
}

TEST(Forca, ErroContaTentativa) {
    Forca f("casa", 6);
    EXPECT_EQ(f.chutar('z'), Resultado::Erro);
    EXPECT_EQ(f.erros(), 1);
    EXPECT_EQ(f.errosRestantes(), 5);
    EXPECT_EQ(f.letrasErradas(), "Z");
}

TEST(Forca, RepetidaEInvalidaNaoContamErro) {
    Forca f("casa");
    f.chutar('z');
    EXPECT_EQ(f.chutar('Z'), Resultado::Repetida);  // maiuscula ou minuscula tanto faz
    EXPECT_EQ(f.chutar('3'), Resultado::Invalida);
    EXPECT_EQ(f.chutar(""), Resultado::Invalida);
    EXPECT_EQ(f.chutar("ab"), Resultado::Invalida);  // mais de uma letra
    EXPECT_EQ(f.erros(), 1);
}

TEST(Forca, Vitoria) {
    Forca f("sol");
    f.chutar('s');
    f.chutar('o');
    EXPECT_FALSE(f.venceu());
    f.chutar('l');
    EXPECT_TRUE(f.venceu());
    EXPECT_TRUE(f.terminou());
}

TEST(Forca, Derrota) {
    Forca f("sol", 2);
    f.chutar('x');
    EXPECT_FALSE(f.perdeu());
    f.chutar('y');
    EXPECT_TRUE(f.perdeu());
    EXPECT_TRUE(f.terminou());
}

TEST(Forca, PalavraInvalidaLancaExcecao) {
    EXPECT_THROW(Forca("abc123"), std::invalid_argument);
    EXPECT_THROW(Forca(""), std::invalid_argument);
    EXPECT_THROW(Forca("\xC3"), std::invalid_argument);  // UTF-8 cortado no meio de uma letra
    EXPECT_THROW(Forca("casa", 0), std::invalid_argument);
}

// ---------- Acentos ----------

TEST(ForcaComAcentos, ContaLetrasENaoBytes) {
    Forca f(u8"coração");  // 7 letras, mas 9 bytes em UTF-8
    EXPECT_EQ(f.tamanho(), 7u);
    EXPECT_EQ(f.palavra(), u8"CORAÇÃO");
    EXPECT_EQ(f.palavraMascarada(), "_ _ _ _ _ _ _");
}

TEST(ForcaComAcentos, LetraSemAcentoRevelaAsAcentuadas) {
    Forca f(u8"coração");
    EXPECT_EQ(f.chutar('a'), Resultado::Acerto);  // revela o A e o Ã
    EXPECT_EQ(f.palavraMascarada(), u8"_ _ _ A _ Ã _");
    EXPECT_EQ(f.chutar('c'), Resultado::Acerto);  // revela o C e o Ç
    EXPECT_EQ(f.palavraMascarada(), u8"C _ _ A Ç Ã _");
}

TEST(ForcaComAcentos, ChuteComAcentoValeComoALetraBase) {
    Forca f(u8"maçã");
    EXPECT_EQ(f.chutar(u8"ã"), Resultado::Acerto);  // mesmo que chutar "A"
    EXPECT_EQ(f.chutar("A"), Resultado::Repetida);
    EXPECT_EQ(f.chutar(u8"Ç"), Resultado::Acerto);
    EXPECT_EQ(f.chutar("m"), Resultado::Acerto);
    EXPECT_TRUE(f.venceu());
}

TEST(ForcaComAcentos, ErradasSaoMostradasSemAcento) {
    Forca f("sol");
    EXPECT_EQ(f.chutar(u8"é"), Resultado::Erro);
    EXPECT_EQ(f.letrasErradas(), "E");
}

TEST(ForcaComAcentos, EntradaQueNaoEUtf8EInvalida) {
    // No Windows, o console pode mandar o "ã" em outra codificacao (um byte 0xE3 sozinho)
    Forca f("sol");
    EXPECT_EQ(f.chutar("\xE3"), Resultado::Invalida);
    EXPECT_EQ(f.erros(), 0);
}
