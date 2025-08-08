//ALuno: Rennan Furlaneto Collado
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#define TAM_MEM 1000

typedef enum boolean{
    False = 0,
    True = 1
}boolean;

typedef struct QUEUE_NODE{
    int instruction;
    QUEUE_NODE *nextInstruction;
}QUEUE_NODE;

typedef struct QUEUE{
    QUEUE_NODE *begin;
    QUEUE_NODE *end;
}QUEUE;

QUEUE *initializingQUEUE(){
    QUEUE *q = (QUEUE*) malloc (sizeof (QUEUE));
    q->begin = NULL;
    q->end = NULL;

    return q;
}

QUEUE_NODE *new_QUEUE_Node(int instruction){
    QUEUE_NODE *qn = (QUEUE_NODE*) malloc (sizeof(QUEUE_NODE));
    qn->instruction = instruction;
    qn->nextInstruction = NULL;

    return qn;
}
void enqueue(QUEUE *q,int instruction){
    if(q == NULL){
        q = initializingQUEUE();
    }
    QUEUE_NODE *new_Node = new_QUEUE_Node(instruction);

    if(q->begin == NULL){
        q->begin = new_Node;
        q->end = new_Node;
    }else{
        q->end->nextInstruction = new_Node;
        q->end = new_Node;
    }
}

int dequeue(QUEUE *q) {
    if (q == NULL || q->begin == NULL) {
        printf("Fila vazia! Não há elementos para remover.\n");
        return -1; // Valor sentinela para indicar falha
    }

    QUEUE_NODE *temp = q->begin;
    int removedInstruction = temp->instruction;

    q->begin = q->begin->nextInstruction;

    if (q->begin == NULL) { // Se a fila ficou vazia após a remoção
        q->end = NULL;
    }

    free(temp); // Libera o nó removido
    return removedInstruction;
}

// Estrutura do nó da pilha
typedef struct STACK_NODE {
    int value;
    struct STACK_NODE *nextInstruction;
} STACK_NODE;

// Estrutura da pilha
typedef struct STACK {
    STACK_NODE *top; // Topo da pilha
} STACK;

// Inicializa a pilha
STACK *initializeSTACK() {
    STACK *s = (STACK*) malloc(sizeof(STACK));
    if (s == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    s->top = NULL;
    return s;
}

// Cria um novo nó da pilha
STACK_NODE *new_STACK_Node(int value) {
    STACK_NODE *node = (STACK_NODE*) malloc(sizeof(STACK_NODE));
    if (node == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    node->value = value;
    node->nextInstruction = NULL;
    return node;
}

// Inserir na pilha (Push)
void push(STACK *s, int value) {
    if (s == NULL) {
        s = initializeSTACK();
    }

    STACK_NODE *new_Node = new_STACK_Node(value);
    new_Node->nextInstruction = s->top; // O novo nó aponta para o antigo topo
    s->top = new_Node;                  // Atualiza o topo da pilha
}

// Remover da pilha (Pop)
int pop(STACK *s) {
    if (s == NULL || s->top == NULL) {
        printf("Pilha vazia! Não há elementos para remover.\n");
        return -1; // Valor sentinela indicando erro
    }

    STACK_NODE *temp = s->top;    // Nó a ser removido
    int removedValue = temp->value;

    s->top = temp->nextInstruction; // Atualiza o topo da pilha
    free(temp);                     // Libera a memória do nó removido

    return removedValue;
}

// Exibir os elementos da pilha
void printSTACK(STACK *s) {
    if (s == NULL || s->top == NULL) {
        printf("Pilha vazia.\n");
        return;
    }

    STACK_NODE *current = s->top;
    printf("Elementos da pilha (do topo para base): ");
    while (current != NULL) {
        printf("%d ", current->value);
        current = current->nextInstruction;
    }
    printf("\n");
}



typedef struct {
    char mnemonic[10];
    int opcode;
} Instruction;


Instruction instructionSet[] = {
    {"ADD", 30},
    {"INC", 31},
    {"ADC", 32},
    {"SUB", 33},
    {"DEC", 34},
    {"LOAD", 20},
    {"MUL", 35},
    {"DIV", 36},
    {"STORE", 21},
    {"HALT", 43},

    // Novas instruções de rotação e deslocamento
    {"RCL", 50},  // Rotate Left through Carry
    {"RCR", 51},  // Rotate Right through Carry
    {"ROL", 52},  // Rotate Left
    {"ROR", 53},  // Rotate Right
    {"SAL", 54},  // Shift Arithmetic Left
    {"SHL", 55},  // Shift Logical Left
    {"SHR", 56},  // Shift Logical Right

    // Novas instruções lógicas
    {"AND", 60},  // AND bit a bit
    {"OR",  61},  // OR bit a bit
    {"XOR", 62},  // XOR bit a bit
    {"NOT", 63},  // NOT bit a bit

    // Comandos de depuração
    {"R", 80},             // Mostrar conteúdo de registradores
    {"Q", 81},             // Sair do programa de depuração
    {"A", 82},             // Montar instruções simbólicas em código de máquina
    {"U", 83},             // Desmontar código de máquina em instruções simbólicas
    {"D", 84},             // Mostrar conteúdo de área da memória
    {"E", 85},             // Entrar dados na memória, iniciando num endereço específico
    {"T", 86},             // Executar passo a passo
    {"G", 87},             // Rodar um programa executável
    {"N", 88},             // Dar nome ao programa
    {"W", 89},             // Gravar programa em disco

};



typedef struct RegisterFlags{
    boolean CarryFlag;
    boolean AuxiliarCarryFlag;
    boolean ParityFlag;
    boolean OverflowFlag;
    boolean SinalFlag;
    boolean ZeroFlag;
}RegisterFlags;

typedef struct RegisterCommon{
    int regA;
    int regB;
    int regC ;
    int regD ;
}RegisterCommon;

typedef struct Micro{
    RegisterFlags *regFlags;
    RegisterCommon *RegCommon;
    int dataMemory[TAM_MEM];
    int instructionMemory[TAM_MEM];
}Micro;

RegisterFlags *initializingRegisterFlags(){
    RegisterFlags * reg = (RegisterFlags*) malloc (sizeof(RegisterFlags));
    if(reg != NULL){
        printf("Error in registerFlags allocation memory\n");
    }
    reg->AuxiliarCarryFlag = False;
    reg->CarryFlag = False;
    reg->OverflowFlag = False;
    reg->ParityFlag = False;
    reg->SinalFlag = False;
    reg->ZeroFlag = False;

    return reg;
}


int main(){
    int acumulador;
    int memory[TAM_MEM];
    char command[10];
    do{
        printf("Welcome to the Rennas Microprocessor Project\n");
        printf("C:\>");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;  // Remove o newline no final da string

    }while(command != "Q");

    return 0;
}