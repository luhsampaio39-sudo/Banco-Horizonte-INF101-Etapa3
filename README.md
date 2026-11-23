# Banco Horizonte — Etapa 3

Sistema de Registro e Gestão de Contas Bancárias da disciplina INF101 — Programação de Computadores I, UNIVIÇOSA. Continuação direta da [Etapa 2](https://github.com/luhsampaio39-sudo/Banco-Horizonte-INF101-Etapa2), com as operações e os arrays mantidos e a inclusão de persistência em arquivos de texto.

- **Aluna:** Luisa Dias Sampaio Sumam Silva
- **Professores:** Anderson R. Lamas e Vanderlea Queiroz
- **Entrega:** 25/11/2026

## Arquivos

| Arquivo | Uso |
|---|---|
| `banco_horizonte.cpp` | Programa completo em C++ |
| `contas.txt` | Cadastro e saldo atual das contas |
| `movimentacoes.txt` | Histórico de depósitos, saques e transferências |
| `docs/EXPLICACAO.md` | Organização e decisões da Etapa 3 |
| `docs/TESTES.md` | Cenários de verificação |

## Compilar e executar

O programa usa C++11 e apenas bibliotecas padrão. No terminal, abra a pasta deste projeto e execute:

**Windows com GCC/MinGW:**

```powershell
g++ -std=c++11 -Wall -Wextra -Wpedantic banco_horizonte.cpp -o banco_horizonte.exe
.\banco_horizonte.exe
```

**Linux com GCC:**

```bash
g++ -std=c++11 -Wall -Wextra -Wpedantic banco_horizonte.cpp -o banco_horizonte
./banco_horizonte
```

Na IDE, defina a pasta do projeto como diretório de execução para que os arquivos de dados fiquem junto ao código. Adicione apenas `banco_horizonte.cpp` ao projeto.

## Menu

| Opção | Operação |
|---|---|
| 1 | Cadastrar conta |
| 2 | Listar contas |
| 3 | Consultar conta pelo número |
| 4 | Depositar |
| 5 | Sacar |
| 6 | Transferir entre contas |
| 7 | Exibir extrato |
| 8 | Verificar saldo |
| 9 | Alterar tipo da conta |
| 10 | Ativar ou desativar conta |
| 0 | Sair |

## Persistência

Ao iniciar, o programa lê `contas.txt` e `movimentacoes.txt`. Se ainda não existirem, começa sem registros e cria os arquivos ao cadastrar a primeira conta. Os limites continuam sendo 100 contas e 500 movimentações.

Uma linha de conta segue o formato `numero;nome;cpf;tipo;saldo;situacao`. Uma linha de movimentação segue `numeroConta;tipo;valor`. `D` representa entrada e `S` representa saída. Transferências gravam uma saída na origem e uma entrada no destino.

O sistema regrava os arrays completos nos arquivos depois de cadastrar, alterar tipo ou situação, depositar, sacar e transferir. Os valores são salvos em reais com duas casas decimais.

O formato definido no enunciado não guarda o saldo inicial. Para manter essa informação no extrato após reiniciar, o programa a reconstrói usando o saldo atual carregado e os registros D/S da conta.

Os arquivos de dados incluídos neste projeto estão vazios; não há dados pessoais de clientes no repositório. Use ponto nos centavos ao digitar valores, por exemplo `150.50`.

## Regras mantidas da Etapa 2

- Número de conta positivo e único; CPF com 11 algarismos; tipo 1 = corrente e 2 = poupança.
- Contas começam ativas. Depósitos, saques, transferências, saldo e extrato exigem conta ativa.
- Saques e transferências exigem saldo suficiente. Transferências exigem contas ativas e diferentes.
- As movimentações registradas são positivas e identificadas como D ou S.
- O limite técnico de saldo continua em R$ 1.000.000.000,00 e movimentações precisam ter pelo menos R$ 0,01.

## Testes

Veja [docs/TESTES.md](docs/TESTES.md) para a verificação de cadastro, transações, reabertura e extratos.
