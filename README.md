# Jogo da Forca em C++

![Build](https://github.com/b4el/jogo-da-forca/actions/workflows/build.yml/badge.svg)

Jogo da forca para o terminal, escrito em C++17. Projeto de estudo focado em organização de código, classes e testes automatizados.

```
  +---+
  |   |
  O   |
 /|   |
      |
=========

Palavra: _ A _ A _ A _ A
Erradas: E I O
Tentativas restantes: 3
Chute uma letra:
```

## Funcionalidades

- Palavras sorteadas a partir de `data/palavras.txt` (é só editar para adicionar mais)
- Desenho do boneco a cada erro
- Avisa sobre letras repetidas e entradas inválidas
- Não diferencia maiúsculas de minúsculas
- Opção de jogar várias partidas seguidas

## Estrutura do projeto

```
jogo-da-forca/
├── include/              # Cabeçalhos (.h): o que cada classe oferece
│   ├── Forca.h
│   └── BancoDePalavras.h
├── src/                  # Implementações (.cpp)
│   ├── Forca.cpp         # Regras do jogo
│   ├── BancoDePalavras.cpp
│   └── main.cpp          # Interface de terminal
├── data/palavras.txt     # Lista de palavras
├── tests/test_forca.cpp  # Testes automáticos
├── CMakeLists.txt        # Receita de compilação
└── .github/workflows/    # Build e testes automáticos no GitHub
```

A classe `Forca` contém **só as regras** (não lê teclado nem imprime nada). Isso separa a lógica da interface e permite testar o jogo automaticamente.

## Como compilar e rodar

### Visual Studio (Windows)

1. **Arquivo → Abrir → Pasta...** e escolha a pasta `jogo-da-forca`.
2. O Visual Studio detecta o `CMakeLists.txt` sozinho e configura o projeto.
3. No seletor de item de inicialização (ao lado do botão ▶ verde), escolha **forca.exe**.
4. Pressione **Ctrl + F5**.

Para rodar os testes: **Testar → Executar todos os testes**, ou escolha **testes.exe** e rode.

### Linha de comando (qualquer sistema)

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build        # roda os testes
./build/forca                 # Linux/macOS
build\Debug\forca.exe         # Windows
```

## Tecnologias e conceitos

- C++17, CMake, CTest
- Classes, `enum class`, `std::set`, `std::vector`
- Leitura de arquivos com `std::ifstream`
- Números aleatórios com `<random>`
- Integração contínua com GitHub Actions (Windows + Linux)

## Próximos passos

- [ ] Categorias de palavras (frutas, animais, países...)
- [ ] Dicas para cada palavra
- [ ] Placar salvo em arquivo
- [ ] Suporte a palavras com acento

## Licença

[MIT](LICENSE)
