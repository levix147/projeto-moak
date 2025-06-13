#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX 100

struct Pessoa {
    char nome[50];
    char cpf[20];
    int idade;
    char senha[20];
    char email[30];
    char tel[20];
    float saldo;
};

struct Admin {
    char nome[50];
    char senha[20];
};

struct Pessoa pessoas[MAX];
int totalCadastros = 0;

struct Admin admins[MAX];
int totalAdmins = 0;

void salvarClientes() {
    FILE *arquivo = fopen("clientes.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita!\n");
        return;
    }
    for (int i = 0; i < totalCadastros; i++) {
        fprintf(arquivo, "%s %s %d %s %s %s %.2f\n",
                pessoas[i].nome,
                pessoas[i].cpf,
                pessoas[i].idade,
                pessoas[i].senha,
                pessoas[i].email,
                pessoas[i].tel,
                pessoas[i].saldo);
    }
    fclose(arquivo);
}

void carregarClientes() {
    FILE *arquivo = fopen("clientes.txt", "r");
    if (arquivo == NULL) return;

    while (fscanf(arquivo, "%s %s %d %s %s %s %f",
                  pessoas[totalCadastros].nome,
                  pessoas[totalCadastros].cpf,
                  &pessoas[totalCadastros].idade,
                  pessoas[totalCadastros].senha,
                  pessoas[totalCadastros].email,
                  pessoas[totalCadastros].tel,
                  &pessoas[totalCadastros].saldo) != EOF) {
        totalCadastros++;
    }
    fclose(arquivo);
}

void cadastroCliente() {
    if (totalCadastros >= MAX) {
        printf("Limite de cadastro atingido!\n");
        return;
    }

    printf("=== Cadastro de Cliente ===\n");
    printf("Nome: ");
    fflush(stdin);
    gets(pessoas[totalCadastros].nome);

    printf("CPF: ");
    scanf("%s", pessoas[totalCadastros].cpf);

    printf("Idade: ");
    scanf("%d", &pessoas[totalCadastros].idade);

    printf("Senha: ");
    scanf("%s", pessoas[totalCadastros].senha);

    printf("Email: ");
    scanf("%s", pessoas[totalCadastros].email);

    printf("Telefone: ");
    scanf("%s", pessoas[totalCadastros].tel);

    pessoas[totalCadastros].saldo = 0.0;

    totalCadastros++;
    salvarClientes();

    printf("Cadastro realizado com sucesso!\n");
}

int loginCliente() {
    char cpf[20], senha[20];
    printf("CPF: ");
    scanf("%s", cpf);
    printf("Senha: ");
    scanf("%s", senha);

    for (int i = 0; i < totalCadastros; i++) {
        if (strcmp(pessoas[i].cpf, cpf) == 0 && strcmp(pessoas[i].senha, senha) == 0) {
            printf("Login realizado com sucesso! Bem-vindo(a), %s.\n", pessoas[i].nome);
            return i;
        }
    }
    printf("CPF ou senha incorretos!\n");
    return -1;
}

void extrato(int index) {
    printf("=== Extrato ===\n");
    printf("Nome: %s\n", pessoas[index].nome);
    printf("Saldo atual: R$ %.2f\n", pessoas[index].saldo);
}

void transferencia(int index) {
    char cpfDestino[20];
    float valor;

    printf("CPF do destinatário: ");
    scanf("%s", cpfDestino);
    printf("Valor a transferir: R$ ");
    scanf("%f", &valor);

    if (valor <= 0 || valor > pessoas[index].saldo) {
        printf("Valor inválido ou saldo insuficiente!\n");
        return;
    }

    for (int i = 0; i < totalCadastros; i++) {
        if (strcmp(pessoas[i].cpf, cpfDestino) == 0) {
            pessoas[index].saldo -= valor;
            pessoas[i].saldo += valor;
            salvarClientes();
            printf("Transferência realizada com sucesso!\n");
            return;
        }
    }
    printf("Destinatário não encontrado!\n");
}

void salvarAdmins() {
    FILE *arquivo = fopen("admins.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de admins!\n");
        return;
    }
    for (int i = 0; i < totalAdmins; i++) {
        fprintf(arquivo, "%s %s\n", admins[i].nome, admins[i].senha);
    }
    fclose(arquivo);
}

void carregarAdmins() {
    FILE *arquivo = fopen("admins.txt", "r");
    if (arquivo == NULL) return;

    while (fscanf(arquivo, "%s %s", admins[totalAdmins].nome, admins[totalAdmins].senha) != EOF) {
        totalAdmins++;
    }
    fclose(arquivo);
}

void cadastroAdmin() {
    char codigoDigitado[10];
    char codigosValidos[10][10] = {
        "ADM2024", "ADM3051", "ADM4629", "ADM5730", "ADM6847",
        "ADM7953", "ADM8491", "ADM9204", "ADM1035", "ADM2148"
    };

    printf("Informe o código de segurança para criar um administrador: ");
    scanf("%s", codigoDigitado);

    int valido = 0;
    for (int i = 0; i < 10; i++) {
        if (strcmp(codigoDigitado, codigosValidos[i]) == 0) {
            valido = 1;
            break;
        }
    }

    if (!valido) {
        printf("Código inválido! Não é possível criar um administrador.\n");
        return;
    }

    if (totalAdmins >= MAX) {
        printf("Limite de administradores atingido!\n");
        return;
    }

    printf("=== Cadastro de Administrador ===\n");
    printf("Nome: ");
    fflush(stdin);
    gets(admins[totalAdmins].nome);

    printf("Senha: ");
    scanf("%s", admins[totalAdmins].senha);

    totalAdmins++;
    salvarAdmins();

    printf("Administrador cadastrado com sucesso!\n");
}

int loginAdmin() {
    char nome[50], senha[20];
    printf("Nome: ");
    scanf("%s", nome);
    printf("Senha: ");
    scanf("%s", senha);

    for (int i = 0; i < totalAdmins; i++) {
        if (strcmp(admins[i].nome, nome) == 0 && strcmp(admins[i].senha, senha) == 0) {
            printf("Login de administrador realizado com sucesso! Bem-vindo, %s.\n", admins[i].nome);
            return i;
        }
    }
    printf("Nome ou senha incorretos!\n");
    return -1;
}

void listarClientes() {
    printf("=== Lista de Clientes ===\n");
    for (int i = 0; i < totalCadastros; i++) {
        printf("%d - Nome: %s | CPF: %s | Saldo: R$ %.2f\n",
               i + 1, pessoas[i].nome, pessoas[i].cpf, pessoas[i].saldo);
    }
}

void buscarCliente() {
    char cpf[20];
    printf("CPF do cliente a buscar: ");
    scanf("%s", cpf);

    for (int i = 0; i < totalCadastros; i++) {
        if (strcmp(pessoas[i].cpf, cpf) == 0) {
            printf("Cliente encontrado: %s | Saldo: R$ %.2f\n", pessoas[i].nome, pessoas[i].saldo);
            return;
        }
    }
    printf("Cliente não encontrado.\n");
}

void alterarSaldo() {
    char cpf[20];
    float novoSaldo;

    printf("CPF do cliente para alterar saldo: ");
    scanf("%s", cpf);

    for (int i = 0; i < totalCadastros; i++) {
        if (strcmp(pessoas[i].cpf, cpf) == 0) {
            printf("Saldo atual: R$ %.2f\n", pessoas[i].saldo);
            printf("Novo saldo: R$ ");
            scanf("%f", &novoSaldo);
            pessoas[i].saldo = novoSaldo;
            salvarClientes();
            printf("Saldo alterado com sucesso.\n");
            return;
        }
    }
    printf("Cliente não encontrado.\n");
}

void menuAdmin() {
    int opcao;
    do {
        printf("\n=== Menu Administrador ===\n");
        printf("1 - Listar clientes\n");
        printf("2 - Buscar cliente\n");
        printf("3 - Alterar saldo de cliente\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                listarClientes();
                break;
            case 2:
                buscarCliente();
                break;
            case 3:
                alterarSaldo();
                break;
            case 0:
                printf("Saindo do menu admin...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

void menu() {
    int opcao;
    do {
        printf("\n=== Banco Digital ===\n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Login cliente\n");
        printf("3 - Cadastrar administrador\n");
        printf("4 - Login administrador\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastroCliente();
                break;
            case 2: {
                int user = loginCliente();
                if (user != -1) {
                    int opcaoLogin;
                    do {
                        printf("\n1 - Extrato\n");
                        printf("2 - Transferência\n");
                        printf("0 - Sair\n");
                        printf("Escolha: ");
                        scanf("%d", &opcaoLogin);

                        if (opcaoLogin == 1)
                            extrato(user);
                        else if (opcaoLogin == 2)
                            transferencia(user);
                    } while (opcaoLogin != 0);
                }
                break;
            }
            case 3:
                cadastroAdmin();
                break;
            case 4:
                if (loginAdmin() != -1) {
                    menuAdmin();
                }
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    carregarClientes();
    carregarAdmins();
    menu();
    printf("BUMBUM GRANADA NA NUCA");
    return 0;
}
