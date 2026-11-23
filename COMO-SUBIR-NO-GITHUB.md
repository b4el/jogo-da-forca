# Como subir este projeto no GitHub

> Este arquivo é um guia só para você. Pode apagá-lo antes de subir, se quiser.

Antes de tudo, crie uma conta em https://github.com, se ainda não tiver.

---

## Opção A — Pelo Visual Studio (mais fácil)

1. No Visual Studio: **Arquivo → Abrir → Pasta...** e escolha `jogo-da-forca`.
2. Menu **Git → Criar Repositório Git...**
3. Na janela que abrir:
   - Em **Enviar por push para um novo repositório remoto**, escolha **GitHub**.
   - Faça login na sua conta do GitHub quando ele pedir.
   - **Nome do repositório:** `jogo-da-forca`
   - Desmarque **Privado** se quiser que outras pessoas vejam (bom para portfólio).
4. Clique em **Criar e Enviar por Push**. Pronto!

Depois, sempre que mudar algo:
- Abra a janela **Git → Alterações do Git** (Ctrl + 0, Ctrl + G).
- Escreva uma mensagem curta dizendo o que mudou (ex.: "Adiciona categorias de palavras").
- Clique em **Confirmar Tudo** (commit) e depois na setinha **Enviar por Push** (↑).

---

## Opção B — Pelo terminal (vale a pena aprender)

### 1. Instale o Git
Baixe em https://git-scm.com/download/win e instale com as opções padrão.

### 2. Configure seu nome (só na primeira vez)
Abra o **Git Bash** ou o **PowerShell** e rode:
```bash
git config --global user.name "Seu Nome"
git config --global user.email "seu-email-do-github@exemplo.com"
```

### 3. Crie o repositório vazio no GitHub
No site: botão **+** (canto superior direito) → **New repository**
- Nome: `jogo-da-forca`
- **Não** marque "Add a README", ".gitignore" nem "license" (o projeto já tem)
- Clique em **Create repository**

### 4. Envie o projeto
No terminal, entre na pasta do projeto e rode (troque `SEU-USUARIO`):
```bash
cd "C:\Users\gabriel\Desktop\New projects\jogo-da-forca"
git init
git add .
git commit -m "Primeira versao do jogo da forca"
git branch -M main
git remote add origin https://github.com/SEU-USUARIO/jogo-da-forca.git
git push -u origin main
```
Na primeira vez, vai abrir uma janela para você fazer login no GitHub.

### 5. No dia a dia
```bash
git status                       # ver o que mudou
git add .                        # preparar as mudancas
git commit -m "O que voce fez"   # salvar um "ponto" no historico
git push                         # enviar para o GitHub
```

---

## Depois de subir

- No `README.md`, troque `SEU-USUARIO` pelo seu usuário do GitHub (na linha do selo "Build" lá em cima).
- Abra a aba **Actions** do repositório: o GitHub vai compilar e testar o projeto sozinho no Windows e no Linux. Se aparecer ✅ verde, está tudo certo.
- Se quiser, fixe o repositório no seu perfil (**Customize your pins**) para ele aparecer como portfólio.
