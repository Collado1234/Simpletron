//Aluno: Rennan Furlaneto Collado
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

//definições de tamanho de memória e condição de parada
#define TAM_MEM 100
#define PARADA -99999

// Operações de entrada e saída
#define READ 10
#define WRITE 11

// Operações de Carregamento/Armazenamento
#define LOAD 20
#define STORE 21

// Operações Aritméticas
#define ADD 30
#define SUBTRACT 31
#define DIVIDE 32
#define MULTIPLY 33

// Operações de transferência de Controle
#define BRANCH 40
#define BRANCHNEG 41
#define BRANCHZERO 42
#define HALT 43

// --- Funções auxiliares para nova entrada ---
int getOpcodeFromName(const char *name) {
    if (strcmp(name, "READ") == 0) return READ;
    if (strcmp(name, "WRITE") == 0) return WRITE;
    if (strcmp(name, "LOAD") == 0) return LOAD;
    if (strcmp(name, "STORE") == 0) return STORE;
    if (strcmp(name, "ADD") == 0) return ADD;
    if (strcmp(name, "SUBTRACT") == 0) return SUBTRACT;
    if (strcmp(name, "DIVIDE") == 0) return DIVIDE;
    if (strcmp(name, "MULTIPLY") == 0) return MULTIPLY;
    if (strcmp(name, "BRANCH") == 0) return BRANCH;
    if (strcmp(name, "BRANCHNEG") == 0) return BRANCHNEG;
    if (strcmp(name, "BRANCHZERO") == 0) return BRANCHZERO;
    if (strcmp(name, "HALT") == 0) return HALT;
    return -1;
}

void toUpperStr(char *str) {
    for (; *str; str++) *str = toupper((unsigned char)*str);
}

void printRegister(int *accumulator, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand){
    printf("\nREGISTERS: \n");
    printf("accumulator            %.4d\n", *accumulator);
    printf("instructionCounter     %.2d\n", *instructionCounter);
    printf("instructionRegister    %.2d\n", *instructionRegister);
    printf("operationCode          %.2d\n", *operationCode);
    printf("operand                %.2d\n", *operand);
}

void zerarMemoria(int *memory){
    for (int i = 0; i < TAM_MEM; i++){
        memory[i] = 0;
    }
}

// --- Nova versão do insertCode ---
void insertCode(int *memory) {
    int i = 0;
    char opName[20];
    int operand;

    do {
        printf("%02d ? ", i);
        scanf("%19s", opName);

        if (strcmp(opName, "-99999") == 0) break;

        toUpperStr(opName);
        int opcode = getOpcodeFromName(opName);

        if (opcode == -1) {
            printf("Operação inválida! Tente novamente.\n");
            continue;
        }

        if (opcode != HALT) {
            scanf("%d", &operand);
            if (operand < 0 || operand >= TAM_MEM) {
                printf("Operando fora do intervalo!\n");
                continue;
            }
        } else {
            operand = 0;
        }

        memory[i] = opcode * 100 + operand;
        i++;

    } while (i < TAM_MEM);

    printf("\n*** Carga do programa concluída ***\n");
}

void executeCode(int *memory, int *accumulator, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand){
    int Aux;
    printf("*** Iniciando execução do programa ***\n");
    do{
        *instructionRegister = memory[*instructionCounter];
        *operationCode = *instructionRegister / 100;
        *operand = *instructionRegister % 100;

        switch (*operationCode){
        case READ:
            printf("Digite um número inteiro: ");
            scanf("%d", &Aux);
            if(Aux > 9999 || Aux < -9999){
                printf("*** Número fora do intervalo de armazenamento. Tente novamente ***\n");
                continue;
            }else{
                memory[*operand] = Aux;
            }
            break;
        case WRITE:
            printf("Valor em %02d: %d\n", *operand, memory[*operand]);
            break;
        case LOAD:
            *accumulator = memory[*operand];
            break;
        case STORE:
            memory[*operand] = *accumulator;
            break;
        case ADD:
            Aux = *accumulator + memory[*operand];
            if(Aux > 9999 || Aux < -9999){
                printf("\n*** Erro fatal. Estouro de Acumulador ***\n");
                return;
            }
            *accumulator = Aux;
            break;
        case SUBTRACT:
            Aux = *accumulator - memory[*operand];
            if(Aux > 9999 || Aux < -9999){
                printf("\n*** Erro fatal. Estouro de Acumulador ***\n");
                return;
            }
            *accumulator = Aux;
            break;
        case DIVIDE:
            if (memory[*operand] != 0){
                *accumulator /= memory[*operand];
            }else{
                printf("\n*** Erro Fatal. Tentativa de divisão por zero ***\n");
                printf("*** Execução do Simpletron encerrada ***\n\n");
                return;
            }
            break;
        case MULTIPLY:
            Aux = *accumulator * memory[*operand];
            if(Aux > 9999 || Aux < -9999){
                printf("*** Erro fatal. Estouro de Acumulador ***\n");
                return;
            }
            *accumulator *= memory[*operand];
            break;
        case BRANCH:
            *instructionCounter = *operand;
            continue;
        case BRANCHNEG:
            if (*accumulator < 0){
                *instructionCounter = *operand;
                continue;
            }
            break;
        case BRANCHZERO:
            if (*accumulator == 0){
                *instructionCounter = *operand;
                continue;
            }
            break;
        case HALT:
            printf("*** Execução do Simpletron encerrada ***\n");
            (*instructionCounter)++;
            return;
        default:
            printf("*** Código inválido! ***\n");
            printf("*** Execução encerrada ***\n");
            return;
        }

        (*instructionCounter)++;
    } while (*instructionCounter < TAM_MEM);
}

void dump(int *memory, int *accumulator, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand){
    printRegister(accumulator, instructionCounter, instructionRegister, operationCode, operand);

    printf("\t  0\t  1\t  2\t  3\t  4\t  5\t  6\t  7\t  8\t  9\t");
    for(int i = 0; i < 10; i++){
        printf("\n%.2d\t", i*10);
        for(int j = i * 10; j < (i+1)*10; j++){
            if(memory[j]<0) printf("%.4d\t", memory[j]);
            else printf("+%.4d\t", memory[j]);
        }
    }
    printf("\n");
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    int memory[TAM_MEM], accumulator = 0, instructionCounter = 0, instructionRegister = 0, operationCode = 0, operand = 0;

    zerarMemoria(memory);

    printf("*** Bem-vindo ao Simpletron!                         ***\n");
    printf("*** Digite seu programa usando nomes de operações    ***\n");
    printf("*** Ex: READ 10 / ADD 15 / HALT                      ***\n");
    printf("*** Digite -99999 para encerrar a entrada             ***\n\n");

    insertCode(memory);
    executeCode(memory, &accumulator, &instructionCounter, &instructionRegister, &operationCode, &operand);
    dump(memory, &accumulator, &instructionCounter, &instructionRegister, &operationCode, &operand);

    return 0;
}
