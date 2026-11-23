# Banco Horizonte - Etapa 3

Este é o projeto da terceira etapa do trabalho de INF101, da UNIVIÇOSA. Ele continua o sistema da Etapa 2, mantendo o cadastro de contas, consultas, depósitos, saques, transferências e extratos. Nesta etapa, as contas e movimentações ficam salvas em arquivos para que continuem disponíveis quando o programa for aberto novamente.

## Como compilar e executar

Na pasta do projeto, use um compilador C++11 ou mais recente.

No Windows com GCC/MinGW:

```powershell
g++ -std=c++11 -Wall -Wextra -Wpedantic banco_horizonte.cpp -o banco_horizonte.exe
.\banco_horizonte.exe
```

No Linux com GCC:

```bash
g++ -std=c++11 -Wall -Wextra -Wpedantic banco_horizonte.cpp -o banco_horizonte
./banco_horizonte
```

O arquivo-fonte do programa é `banco_horizonte.cpp`. Na IDE, deixe a pasta do projeto como diretório de execução para que o programa encontre os arquivos de dados.

## Arquivos de dados

`contas.txt` guarda os dados e o saldo atual das contas. `movimentacoes.txt` guarda depósitos, saques e transferências. Os arquivos estão vazios no primeiro uso; ao cadastrar uma conta, o programa grava os dados. Ao iniciar, ele carrega o conteúdo salvo.

Cada arquivo tem limite de 100 contas e 500 movimentações. Transferências aparecem como uma saída (`S`) na conta de origem e uma entrada (`D`) na conta de destino. Digite os valores com ponto nos centavos, como `150.50`.

Mais detalhes sobre o funcionamento e os testes estão em `docs/EXPLICACAO.md` e `docs/TESTES.md`.
