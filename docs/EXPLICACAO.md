# Explicação do programa

## Continuação da Etapa 2

O programa mantém o menu, as validações, os arrays paralelos, as regras de saldo e as opções de consulta, listagem, depósito, saque, transferência e extrato da Etapa 2. Os arrays continuam limitados a 100 contas e 500 registros de movimentação.

As funções próprias organizam a persistência:

- `carregarContas()` lê e valida as linhas de `contas.txt`.
- `carregarMovimentacoes()` lê e valida as linhas de `movimentacoes.txt`.
- `salvarContas()` regrava todas as contas do array.
- `salvarMovimentacoes()` regrava todo o histórico do array.
- `reconstruirSaldosIniciais()` recupera o saldo de abertura necessário ao extrato.

As funções recebem os arrays e os contadores usados pelo programa. A leitura é feita com `ifstream`, `is_open()` e `getline()`. A gravação usa `ofstream`, formatação com duas casas, `close()` e verificação de falha.

## Arquivos texto

Cada conta é gravada como `numero;nome;cpf;tipo;saldo;situacao`. A situação é 1 para ativa e 0 para inativa. Cada movimentação é gravada como `numeroConta;tipo;valor`, com `D` para entrada e `S` para saída. O histórico de transferência segue a Etapa 2: uma linha S para a origem e uma linha D para o destino.

Os arquivos são abertos para gravação sem `ios::app`, então o conteúdo anterior é substituído pelo conteúdo completo dos arrays atuais. Assim, o histórico anterior permanece porque ele foi carregado no array antes da gravação.

O programa procura os arquivos no diretório de execução. A primeira execução pode começar sem eles. Eles são criados após o primeiro cadastro.

## Recuperação do saldo inicial

O formato de contas solicitado na Etapa 3 contém o saldo atual, mas não inclui o saldo inicial usado no extrato da Etapa 2. O programa começa com o saldo atual carregado e desfaz o efeito líquido do histórico: subtrai cada D e soma cada S da conta. As transferências funcionam com esse cálculo porque são registradas como S na origem e D no destino.

## Validação e limites

O programa rejeita registros com campos inválidos, números de conta duplicados, tipos diferentes de 1 ou 2, CPFs fora do formato de 11 algarismos e movimentações que apontem para contas inexistentes. Registros inválidos são ignorados com aviso. Se um arquivo tiver mais registros do que o limite dos arrays, os excedentes são ignorados e informados.

As operações continuam usando `getline()` e `istringstream` para validar entradas. Valores monetários devem ter no máximo duas casas decimais; saldos podem começar em zero e movimentações devem ser de pelo menos um centavo.
