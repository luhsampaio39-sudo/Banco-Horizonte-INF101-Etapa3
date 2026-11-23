# Testes realizados

## Compilação

O código foi compilado com GCC 16.2.0 em C++11, ativando avisos como erros:

```powershell
g++ -std=c++11 -Wall -Wextra -Wpedantic -Werror banco_horizonte.cpp -o banco_horizonte.exe
```

Resultado: compilação concluída sem erros nem avisos.

## Persistência entre execuções

Os testes foram feitos em uma pasta temporária, sem alterar os arquivos vazios distribuídos com o projeto.

1. Cadastrar três contas, com saldos iniciais de R$ 1.000,00, R$ 500,00 e R$ 1.500,00.
2. Depositar R$ 50,00 na primeira conta e sacar R$ 20,00 da terceira.
3. Transferir R$ 300,00 da primeira conta para a segunda.
4. Conferir que `contas.txt` contém os saldos R$ 750,00, R$ 800,00 e R$ 1.480,00, e que `movimentacoes.txt` contém os quatro registros D/S esperados.
5. Encerrar e abrir novamente. Confirmar a leitura das três contas e das quatro movimentações, consultar os saldos e abrir os extratos das contas de origem e destino.
6. Confirmar nos extratos que os saldos iniciais (R$ 1.000,00 e R$ 500,00) e os registros da transferência continuam corretos após a reabertura.
7. Alterar o tipo da primeira conta e desativar a terceira; encerrar e abrir uma terceira vez para confirmar que essas alterações cadastrais também foram salvas.

**Resultado:** todos os cenários passaram. As contas, os saldos, os registros D/S, os extratos e as alterações de tipo e situação permaneceram após reabrir o programa.
