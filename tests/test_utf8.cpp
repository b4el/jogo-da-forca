// Testes das ferramentas de UTF-8

#include <gtest/gtest.h>

#include <stdexcept>

#include "Utf8.h"

TEST(Utf8, DecodificaLetrasDeTamanhosDiferentes) {
    // A (1 byte), Ã (2 bytes), € (3 bytes), 😀 (4 bytes)
    std::u32string t = utf8::decodificar(u8"AÃ€\U0001F600");
    ASSERT_EQ(t.size(), 4u);
    EXPECT_EQ(t[0], U'A');
    EXPECT_EQ(t[1], char32_t{0xC3});
    EXPECT_EQ(t[2], char32_t{0x20AC});
    EXPECT_EQ(t[3], char32_t{0x1F600});
}

TEST(Utf8, IdaEVolta) {
    const char* textos[] = {"", "casa", u8"CORAÇÃO", u8"ônibus", u8"€ e 😀"};
    for (const char* texto : textos) EXPECT_EQ(utf8::codificar(utf8::decodificar(texto)), texto) << texto;
}

TEST(Utf8, RejeitaTextoInvalido) {
    EXPECT_THROW(utf8::decodificar("\xC3"), std::invalid_argument);          // cortado no meio
    EXPECT_THROW(utf8::decodificar("\xE3o"), std::invalid_argument);         // "ão" salvo em Latin-1
    EXPECT_THROW(utf8::decodificar("\xC1\x81"), std::invalid_argument);      // 'A' em 2 bytes (forma longa demais)
    EXPECT_THROW(utf8::decodificar("\xED\xA0\x80"), std::invalid_argument);  // metade de um par UTF-16
    EXPECT_THROW(utf8::decodificar("\xFF"), std::invalid_argument);
}

TEST(Utf8, Maiusculas) {
    EXPECT_EQ(utf8::maiuscula(U'a'), U'A');
    EXPECT_EQ(utf8::maiuscula(char32_t{0xE3}), char32_t{0xC3});  // ã -> Ã
    EXPECT_EQ(utf8::maiuscula(char32_t{0xE7}), char32_t{0xC7});  // ç -> Ç
    EXPECT_EQ(utf8::maiuscula(char32_t{0xF7}), char32_t{0xF7});  // ÷ continua igual
    EXPECT_EQ(utf8::maiuscula(U'1'), U'1');
}

TEST(Utf8, LetraBase) {
    EXPECT_EQ(utf8::letraBase(char32_t{0xE1}), U'A');  // á
    EXPECT_EQ(utf8::letraBase(char32_t{0xC3}), U'A');  // Ã
    EXPECT_EQ(utf8::letraBase(char32_t{0xE7}), U'C');  // ç
    EXPECT_EQ(utf8::letraBase(char32_t{0xF4}), U'O');  // ô
    EXPECT_EQ(utf8::letraBase(char32_t{0xFC}), U'U');  // ü
    EXPECT_EQ(utf8::letraBase(U'z'), U'Z');
    EXPECT_EQ(utf8::letraBase(U'3'), char32_t{0});
    EXPECT_EQ(utf8::letraBase(char32_t{0xD7}), char32_t{0});  // ×
    EXPECT_EQ(utf8::letraBase(char32_t{0xDF}), char32_t{0});  // ß: o jogo nao usa
}
