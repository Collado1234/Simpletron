//ALuno: Rennan Furlaneto Collado
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//definições de tamanho de memória e condição de parada
#define TAM_MEM 100
#define PARADA -99999

// Operações de entrada e saída
#define READ 10   // lê no terminal para local da memória específico
#define WRITE 11  // Escreve a palavra de um local específico pro terminal

// Operações de Carregamento/Armazenamento
#define LOAD 20  // carrega uma palavra de um local pro acumulador
#define STORE 21 // Armazena uma palavra do acumulador para o local na memória

// Operações Aritméticas (resultado no acumulador)
#define ADD 30       // soma palavra de um local esp com a do acumulador
#define SUBTRACT 31  // subtrai palavra de um local esp com a do acumulador
#define DIVIDE 32    // divide palavra de um local esp na mem pela do acumulador
#define MULTIPLY 33  // Multiplica local pelo acumulador

// Operações de transferência de Controle
#define BRANCH 40     // desvia para local especifico na memoria
#define BRANCHNEG 41  // desvia para local especifico se acumulador negativo
#define BRANCHZERO 42 // desvia para local especifico se acumulador zero
#define HALT 43       // Para programa

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

void insertCode(int *memory){
    int i = 0, word;
    do{
        printf("%02d ? ", i);
        scanf("%d", &word);

        if (word == PARADA){
            break;
        }
        if (word > 9999 || word < -9999){
            printf("Número fora do intervalo. Por favor insira novamente!\n");
        }else{
            memory[i] = word;
            i++;
        }
    }while(i < TAM_MEM);
    printf("\n*** Carga do programa concluída     ***\n");
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
            scanf("%d", &Aux);       //
            if(Aux > 9999 || Aux < -9999){
                printf("*** Número fora do intervalo de armazenamento. Tente novamente ***\n");
                continue;   //não afeta o contador de instrução
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
            printf("SOMA: ACUMULADOR: %d", *accumulator);
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
                printf("*** Execução do Simpletron encerrada de forma anormal. ***\n\n");
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
            printf("*** Execução do Simpletron encerrada de forma anormal. ***\n");
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
    printf("*** Favor digitar seu programa, uma instrução        ***\n");
    printf("*** (ou palavra de dados) por vez. Mostrarei         ***\n");
    printf("*** o número do local e uma interrogação (?).        ***\n");
    printf("*** Você, então, deverá digitar a palavra para esse  ***\n");
    printf("*** local. Digite a sentinela -99999 para            ***\n");
    printf("*** encerrar a entrada do seu programa.              ***\n\n");

    insertCode(memory);
    printf("*** Iniciado a execução do programa ***\n");
    executeCode(memory, &accumulator, &instructionCounter, &instructionRegister, &operationCode, &operand);
    dump(memory, &accumulator, &instructionCounter, &instructionRegister, &operationCode, &operand);

    return 0;
}
