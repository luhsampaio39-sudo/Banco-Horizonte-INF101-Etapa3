#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <fstream>

using namespace std;

const int MAX_CONTAS = 100;
const int MAX_MOVIMENTACOES = 500;
const double LIMITE_SALDO = 1000000000.00;

bool lerInteiro(const string &texto, int &valor) {
    istringstream leitura(texto);
    if (!(leitura >> valor)) return false;
    leitura >> ws;
    return leitura.eof();
}

bool lerValorMonetario(const string &texto, double &valor, bool aceitarZero) {
    istringstream leitura(texto);
    if (!(leitura >> valor)) return false;
    leitura >> ws;
    if (!leitura.eof() || !isfinite(valor)) return false;
    if (valor < 0 || valor > LIMITE_SALDO) return false;
    if (!aceitarZero && valor == 0) return false;
    return fabs(valor * 100 - round(valor * 100)) < 0.0001;
}

bool contaExiste(int numero, const int numeroConta[], int quantidadeContas) {
    for (int i = 0; i < quantidadeContas; i++) {
        if (numeroConta[i] == numero) return true;
    }
    return false;
}

bool carregarContas(int numeroConta[], string nomeCliente[], string cpf[],
                    int tipoConta[], double saldo[], bool contaAtiva[],
                    double saldoInicial[], int &quantidadeContas) {
    ifstream arquivo("contas.txt");
    if (!arquivo.is_open()) return true;

    string linha;
    int registrosInvalidos = 0;
    int registrosExcedentes = 0;
    while (getline(arquivo, linha)) {
        size_t p1 = linha.find(';');
        size_t p6 = linha.rfind(';');
        if (p1 == string::npos || p6 == string::npos || p6 == p1) {
            registrosInvalidos++;
            continue;
        }
        size_t p5 = linha.rfind(';', p6 - 1);
        size_t p4 = (p5 == string::npos ? string::npos : linha.rfind(';', p5 - 1));
        size_t p3 = (p4 == string::npos ? string::npos : linha.rfind(';', p4 - 1));
        if (p3 == string::npos || p3 <= p1 || p4 <= p3 || p5 <= p4 || p6 <= p5) {
            registrosInvalidos++;
            continue;
        }

        int numero = 0, tipo = 0, situacao = 0;
        double valorSaldo = 0;
        string nome = linha.substr(p1 + 1, p3 - p1 - 1);
        string documento = linha.substr(p3 + 1, p4 - p3 - 1);
        bool registroValido = lerInteiro(linha.substr(0, p1), numero)
            && numero > 0
            && nome.find_first_not_of(" \t\r") != string::npos
            && documento.size() == 11
            && lerInteiro(linha.substr(p4 + 1, p5 - p4 - 1), tipo)
            && (tipo == 1 || tipo == 2)
            && lerValorMonetario(linha.substr(p5 + 1, p6 - p5 - 1), valorSaldo, true)
            && lerInteiro(linha.substr(p6 + 1), situacao)
            && (situacao == 0 || situacao == 1);
        for (size_t i = 0; i < documento.size(); i++) {
            if (documento[i] < '0' || documento[i] > '9') registroValido = false;
        }
        if (!registroValido || contaExiste(numero, numeroConta, quantidadeContas)) {
            registrosInvalidos++;
            continue;
        }
        if (quantidadeContas >= MAX_CONTAS) {
            registrosExcedentes++;
            continue;
        }

        int indice = quantidadeContas;
        numeroConta[indice] = numero;
        nomeCliente[indice] = nome;
        cpf[indice] = documento;
        tipoConta[indice] = tipo;
        saldo[indice] = round(valorSaldo * 100) / 100;
        saldoInicial[indice] = saldo[indice];
        contaAtiva[indice] = (situacao == 1);
        quantidadeContas++;
    }
    arquivo.close();

    if (registrosInvalidos > 0)
        cout << "Aviso: " << registrosInvalidos << " registro(s) invalido(s) ignorado(s) em contas.txt.\n";
    if (registrosExcedentes > 0)
        cout << "Aviso: " << registrosExcedentes << " conta(s) excedem o limite de 100 e foram ignoradas.\n";
    return !arquivo.bad();
}

bool carregarMovimentacoes(int contaMovimentacao[], char tipoMovimentacao[],
                           double valorMovimentacao[], int numeroConta[],
                           int quantidadeContas, int &quantidadeMovimentacoes) {
    ifstream arquivo("movimentacoes.txt");
    if (!arquivo.is_open()) return true;

    string linha;
    int registrosInvalidos = 0;
    int registrosExcedentes = 0;
    while (getline(arquivo, linha)) {
        size_t p1 = linha.find(';');
        size_t p2 = (p1 == string::npos ? string::npos : linha.find(';', p1 + 1));
        if (p1 == string::npos || p2 == string::npos
            || linha.find(';', p2 + 1) != string::npos) {
            registrosInvalidos++;
            continue;
        }
        int numero = 0;
        double valor = 0;
        string tipoTexto = linha.substr(p1 + 1, p2 - p1 - 1);
        bool registroValido = lerInteiro(linha.substr(0, p1), numero)
            && numero > 0
            && tipoTexto.size() == 1
            && (tipoTexto[0] == 'D' || tipoTexto[0] == 'S')
            && lerValorMonetario(linha.substr(p2 + 1), valor, false)
            && contaExiste(numero, numeroConta, quantidadeContas);
        if (!registroValido) {
            registrosInvalidos++;
            continue;
        }
        if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) {
            registrosExcedentes++;
            continue;
        }

        contaMovimentacao[quantidadeMovimentacoes] = numero;
        tipoMovimentacao[quantidadeMovimentacoes] = tipoTexto[0];
        valorMovimentacao[quantidadeMovimentacoes] = round(valor * 100) / 100;
        quantidadeMovimentacoes++;
    }
    arquivo.close();

    if (registrosInvalidos > 0)
        cout << "Aviso: " << registrosInvalidos << " registro(s) invalido(s) ignorado(s) em movimentacoes.txt.\n";
    if (registrosExcedentes > 0)
        cout << "Aviso: " << registrosExcedentes << " movimentacao(oes) excedem o limite de 500 e foram ignoradas.\n";
    return !arquivo.bad();
}

void reconstruirSaldosIniciais(double saldoInicial[], const double saldo[],
                               const int numeroConta[], int quantidadeContas,
                               const int contaMovimentacao[],
                               const char tipoMovimentacao[],
                               const double valorMovimentacao[],
                               int quantidadeMovimentacoes) {
    for (int i = 0; i < quantidadeContas; i++) {
        saldoInicial[i] = saldo[i];
        for (int j = 0; j < quantidadeMovimentacoes; j++) {
            if (contaMovimentacao[j] == numeroConta[i]) {
                if (tipoMovimentacao[j] == 'D') saldoInicial[i] -= valorMovimentacao[j];
                else if (tipoMovimentacao[j] == 'S') saldoInicial[i] += valorMovimentacao[j];
            }
        }
        saldoInicial[i] = round(saldoInicial[i] * 100) / 100;
    }
}

bool salvarContas(const int numeroConta[], const string nomeCliente[],
                  const string cpf[], const int tipoConta[], const double saldo[],
                  const bool contaAtiva[], int quantidadeContas) {
    ofstream arquivo("contas.txt");
    if (!arquivo.is_open()) return false;
    arquivo << fixed << setprecision(2);
    for (int i = 0; i < quantidadeContas; i++) {
        arquivo << numeroConta[i] << ';' << nomeCliente[i] << ';' << cpf[i] << ';'
                << tipoConta[i] << ';' << saldo[i] << ';' << (contaAtiva[i] ? 1 : 0) << '\n';
    }
    arquivo.close();
    return !arquivo.fail();
}

bool salvarMovimentacoes(const int contaMovimentacao[],
                         const char tipoMovimentacao[],
                         const double valorMovimentacao[],
                         int quantidadeMovimentacoes) {
    ofstream arquivo("movimentacoes.txt");
    if (!arquivo.is_open()) return false;
    arquivo << fixed << setprecision(2);
    for (int i = 0; i < quantidadeMovimentacoes; i++) {
        arquivo << contaMovimentacao[i] << ';' << tipoMovimentacao[i] << ';'
                << valorMovimentacao[i] << '\n';
    }
    arquivo.close();
    return !arquivo.fail();
}

void informarResultadoGravacao(bool sucesso, const string &arquivosSalvos) {
    if (sucesso) {
        cout << "Dados salvos em " << arquivosSalvos << ".\n";
    } else {
        cout << "Aviso: nao foi possivel salvar todos os dados. Verifique os arquivos e as permissoes da pasta.\n";
    }
}

int main() {
    int numeroConta[MAX_CONTAS] = {};
    string nomeCliente[MAX_CONTAS];
    string cpf[MAX_CONTAS];
    int tipoConta[MAX_CONTAS] = {};
    double saldo[MAX_CONTAS] = {};
    bool contaAtiva[MAX_CONTAS] = {};
    double saldoInicial[MAX_CONTAS] = {};
    int quantidadeContas = 0;

    int contaMovimentacao[MAX_MOVIMENTACOES] = {};
    char tipoMovimentacao[MAX_MOVIMENTACOES] = {};
    double valorMovimentacao[MAX_MOVIMENTACOES] = {};
    int quantidadeMovimentacoes = 0;

    bool contasLidas = carregarContas(numeroConta, nomeCliente, cpf, tipoConta,
        saldo, contaAtiva, saldoInicial, quantidadeContas);
    bool movimentacoesLidas = carregarMovimentacoes(contaMovimentacao,
        tipoMovimentacao, valorMovimentacao, numeroConta, quantidadeContas,
        quantidadeMovimentacoes);
    if (!contasLidas) cout << "Aviso: ocorreu um erro ao ler contas.txt.\n";
    if (!movimentacoesLidas) cout << "Aviso: ocorreu um erro ao ler movimentacoes.txt.\n";
    reconstruirSaldosIniciais(saldoInicial, saldo, numeroConta, quantidadeContas,
                               contaMovimentacao, tipoMovimentacao,
                               valorMovimentacao, quantidadeMovimentacoes);
    cout << "Contas carregadas: " << quantidadeContas
         << " | Movimentacoes carregadas: " << quantidadeMovimentacoes << '\n';

    int opcao = -1;
    string entrada;
    bool entradaValida;
    cout << fixed << setprecision(2);

    do {
        cout << "\n========================================\n"
             << "             BANCO HORIZONTE\n"
             << "          INF101 - ETAPA 3\n"
             << "========================================\n"
             << "1 - Cadastrar conta\n"
             << "2 - Listar contas\n"
             << "3 - Consultar conta\n"
             << "4 - Depositar\n"
             << "5 - Sacar\n"
             << "6 - Transferir\n"
             << "7 - Exibir extrato\n"
             << "8 - Verificar saldo\n"
             << "9 - Alterar tipo da conta\n"
             << "10 - Ativar/Desativar conta\n"
             << "0 - Sair\n";

        do {
            cout << "Escolha: ";
            if (!getline(cin, entrada)) return 0;
            istringstream leitura(entrada);
            entradaValida = static_cast<bool>(leitura >> opcao);
            if (entradaValida) {
                leitura >> ws;
                entradaValida = leitura.eof() && opcao >= 0 && opcao <= 10;
            }
            if (!entradaValida) cout << "Opcao invalida. Digite de 0 a 10.\n";
        } while (!entradaValida);

        if (opcao == 0) {
            cout << "Banco Horizonte encerrado. Dados preservados nos arquivos.\n";
            break;
        }

        if (opcao == 1) {
            if (quantidadeContas == MAX_CONTAS) {
                cout << "Limite de 100 contas atingido.\n";
                continue;
            }
            int indice = quantidadeContas;
            do {
                cout << "Numero da nova conta: ";
                if (!getline(cin, entrada)) return 0;
                istringstream leitura(entrada);
                entradaValida = static_cast<bool>(leitura >> numeroConta[indice]);
                if (entradaValida) {
                    leitura >> ws;
                    entradaValida = leitura.eof() && numeroConta[indice] > 0;
                }
                for (int i = 0; i < quantidadeContas; i++) {
                    if (numeroConta[i] == numeroConta[indice]) entradaValida = false;
                }
                if (!entradaValida) cout << "Numero invalido ou ja cadastrado.\n";
            } while (!entradaValida);
            do {
                cout << "Nome do titular: ";
                if (!getline(cin, nomeCliente[indice])) return 0;
                entradaValida = nomeCliente[indice].find_first_not_of(" \t\r") != string::npos;
                if (!entradaValida) cout << "O nome nao pode ficar vazio.\n";
            } while (!entradaValida);
            do {
                cout << "CPF (11 digitos, sem pontuacao): ";
                if (!getline(cin, cpf[indice])) return 0;
                entradaValida = cpf[indice].size() == 11;
                for (size_t i = 0; i < cpf[indice].size(); i++) {
                    if (cpf[indice][i] < '0' || cpf[indice][i] > '9') entradaValida = false;
                }
                if (!entradaValida) cout << "Informe exatamente 11 digitos numericos.\n";
            } while (!entradaValida);
            do {
                cout << "Tipo (1 = Corrente; 2 = Poupanca): ";
                if (!getline(cin, entrada)) return 0;
                istringstream leitura(entrada);
                entradaValida = static_cast<bool>(leitura >> tipoConta[indice]);
                if (entradaValida) {
                    leitura >> ws;
                    entradaValida = leitura.eof() && (tipoConta[indice] == 1 || tipoConta[indice] == 2);
                }
                if (!entradaValida) cout << "Tipo invalido. Digite 1 ou 2.\n";
            } while (!entradaValida);
            do {
                cout << "Saldo inicial (ponto nos centavos): R$ ";
                if (!getline(cin, entrada)) return 0;
                entradaValida = lerValorMonetario(entrada, saldo[indice], true);
                if (!entradaValida) cout << "Saldo invalido. Use de 0 a 1000000000, com ate 2 casas decimais.\n";
            } while (!entradaValida);
            saldo[indice] = round(saldo[indice] * 100) / 100;
            saldoInicial[indice] = saldo[indice];
            contaAtiva[indice] = true;
            quantidadeContas++;
            cout << "Conta cadastrada com sucesso e ativada.\n";
            bool contasSalvas = salvarContas(numeroConta, nomeCliente, cpf, tipoConta,
                                               saldo, contaAtiva, quantidadeContas);
            bool movimentacoesSalvas = salvarMovimentacoes(contaMovimentacao,
                tipoMovimentacao, valorMovimentacao, quantidadeMovimentacoes);
            informarResultadoGravacao(contasSalvas && movimentacoesSalvas,
                                      "contas.txt e movimentacoes.txt");
            continue;
        }

        if (quantidadeContas == 0) {
            cout << "Nenhuma conta cadastrada. Use a opcao 1.\n";
            continue;
        }
        if (opcao == 2) {
            cout << "\n--- CONTAS CADASTRADAS ---\n";
            for (int i = 0; i < quantidadeContas; i++) {
                cout << "Conta: " << numeroConta[i] << " | Titular: " << nomeCliente[i]
                     << " | Tipo: " << (tipoConta[i] == 1 ? "Corrente" : "Poupanca")
                     << " | " << (contaAtiva[i] ? "Ativa" : "Inativa") << '\n';
            }
            cout << "Total de contas: " << quantidadeContas << '\n';
            continue;
        }

        int numero = 0;
        do {
            cout << (opcao == 6 ? "Numero da conta de origem: " : "Numero da conta: ");
            if (!getline(cin, entrada)) return 0;
            istringstream leitura(entrada);
            entradaValida = static_cast<bool>(leitura >> numero);
            if (entradaValida) {
                leitura >> ws;
                entradaValida = leitura.eof() && numero > 0;
            }
            if (!entradaValida) cout << "Informe um numero inteiro positivo.\n";
        } while (!entradaValida);
        int indice = -1;
        for (int i = 0; i < quantidadeContas; i++) {
            if (numeroConta[i] == numero) indice = i;
        }
        if (indice == -1) {
            cout << "Conta nao encontrada.\n";
            continue;
        }
        if (opcao == 3) {
            cout << "\n--- DADOS DA CONTA ---\n"
                 << "Numero: " << numeroConta[indice] << '\n'
                 << "Titular: " << nomeCliente[indice] << '\n'
                 << "CPF: " << cpf[indice] << '\n'
                 << "Tipo: " << (tipoConta[indice] == 1 ? "Corrente" : "Poupanca") << '\n'
                 << "Situacao: " << (contaAtiva[indice] ? "Ativa" : "Inativa") << '\n';
            if (contaAtiva[indice]) cout << "Saldo atual: R$ " << saldo[indice] << '\n';
            else cout << "Saldo indisponivel: ative a conta para consultar.\n";
            continue;
        }
        if (opcao == 10) {
            contaAtiva[indice] = !contaAtiva[indice];
            cout << (contaAtiva[indice] ? "Conta ativada.\n" : "Conta desativada.\n");
            informarResultadoGravacao(salvarContas(numeroConta, nomeCliente, cpf,
                tipoConta, saldo, contaAtiva, quantidadeContas), "contas.txt");
            continue;
        }
        if (!contaAtiva[indice]) {
            cout << "Conta inativa. Ative-a antes desta operacao.\n";
            continue;
        }
        if (opcao == 8) {
            cout << "Saldo atual: R$ " << saldo[indice] << '\n';
            continue;
        }
        if (opcao == 9) {
            int novoTipo = 0;
            do {
                cout << "Novo tipo (1 = Corrente; 2 = Poupanca): ";
                if (!getline(cin, entrada)) return 0;
                istringstream leitura(entrada);
                entradaValida = static_cast<bool>(leitura >> novoTipo);
                if (entradaValida) {
                    leitura >> ws;
                    entradaValida = leitura.eof() && (novoTipo == 1 || novoTipo == 2);
                }
                if (!entradaValida) cout << "Tipo invalido. Digite 1 ou 2.\n";
            } while (!entradaValida);
            tipoConta[indice] = novoTipo;
            cout << "Tipo da conta atualizado.\n";
            informarResultadoGravacao(salvarContas(numeroConta, nomeCliente, cpf,
                tipoConta, saldo, contaAtiva, quantidadeContas), "contas.txt");
            continue;
        }
        if (opcao == 7) {
            cout << "\n--- EXTRATO DA CONTA " << numeroConta[indice] << " ---\n"
                 << "Titular: " << nomeCliente[indice] << '\n'
                 << "Saldo inicial: R$ " << saldoInicial[indice] << '\n'
                 << "D = entrada/deposito; S = saida/saque (incluem transferencias).\n";
            int total = 0;
            for (int i = 0; i < quantidadeMovimentacoes; i++) {
                if (contaMovimentacao[i] == numeroConta[indice]) {
                    total++;
                    cout << total << " | " << tipoMovimentacao[i] << " | R$ "
                         << valorMovimentacao[i] << '\n';
                }
            }
            if (total == 0) cout << "Nenhuma movimentacao nesta conta.\n";
            cout << "Total de movimentacoes da conta: " << total << '\n'
                 << "Saldo atual: R$ " << saldo[indice] << '\n';
            continue;
        }

        int destino = -1;
        if (opcao == 6) {
            do {
                cout << "Numero da conta de destino: ";
                if (!getline(cin, entrada)) return 0;
                istringstream leitura(entrada);
                entradaValida = static_cast<bool>(leitura >> numero);
                if (entradaValida) {
                    leitura >> ws;
                    entradaValida = leitura.eof() && numero > 0;
                }
                if (!entradaValida) cout << "Informe um numero inteiro positivo.\n";
            } while (!entradaValida);
            for (int i = 0; i < quantidadeContas; i++) {
                if (numeroConta[i] == numero) destino = i;
            }
            if (destino == -1) {
                cout << "Conta de destino nao encontrada.\n";
                continue;
            }
            if (destino == indice) {
                cout << "Escolha duas contas diferentes.\n";
                continue;
            }
            if (!contaAtiva[destino]) {
                cout << "Conta de destino inativa.\n";
                continue;
            }
        }
        int registrosNecessarios = (opcao == 6 ? 2 : 1);
        if (quantidadeMovimentacoes + registrosNecessarios > MAX_MOVIMENTACOES) {
            cout << "Historico sem espaco. Operacao cancelada; saldos preservados.\n";
            continue;
        }
        double valor = 0;
        do {
            cout << "Valor (ponto nos centavos): R$ ";
            if (!getline(cin, entrada)) return 0;
            entradaValida = lerValorMonetario(entrada, valor, false);
            if (!entradaValida) cout << "Valor invalido. Use de 0.01 a 1000000000, com ate 2 casas decimais.\n";
        } while (!entradaValida);
        valor = round(valor * 100) / 100;
        if (opcao != 4 && valor > saldo[indice]) {
            cout << "Saldo insuficiente. Operacao cancelada.\n";
            continue;
        }
        int recebedor = (opcao == 4 ? indice : destino);
        if (recebedor != -1 && saldo[recebedor] + valor > LIMITE_SALDO) {
            cout << "Limite de saldo excedido. Operacao cancelada.\n";
            continue;
        }
        if (opcao == 4) saldo[indice] = round((saldo[indice] + valor) * 100) / 100;
        else saldo[indice] = round((saldo[indice] - valor) * 100) / 100;
        contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indice];
        tipoMovimentacao[quantidadeMovimentacoes] = (opcao == 4 ? 'D' : 'S');
        valorMovimentacao[quantidadeMovimentacoes] = valor;
        quantidadeMovimentacoes++;

        if (opcao == 6) {
            saldo[destino] = round((saldo[destino] + valor) * 100) / 100;
            contaMovimentacao[quantidadeMovimentacoes] = numeroConta[destino];
            tipoMovimentacao[quantidadeMovimentacoes] = 'D';
            valorMovimentacao[quantidadeMovimentacoes] = valor;
            quantidadeMovimentacoes++;
            cout << "Transferencia realizada com sucesso.\n";
        } else if (opcao == 4) cout << "Deposito realizado com sucesso.\n";
        else cout << "Saque realizado com sucesso.\n";

        bool contasSalvas = salvarContas(numeroConta, nomeCliente, cpf, tipoConta,
                                         saldo, contaAtiva, quantidadeContas);
        bool movimentacoesSalvas = salvarMovimentacoes(contaMovimentacao,
            tipoMovimentacao, valorMovimentacao, quantidadeMovimentacoes);
        informarResultadoGravacao(contasSalvas && movimentacoesSalvas,
                                  "contas.txt e movimentacoes.txt");
        cout << "Saldo atual: R$ " << saldo[indice] << '\n';
    } while (opcao != 0);
    return 0;
}
