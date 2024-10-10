#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEMPSIMAM 3600 // 6h30
#define TEMPSIMPM 2100 // 11h00
#define TEMPSIMJ 9000 // 17h00
#define TEMPUSUMIN 3 // Tempo mínimo de atendimento em segundos
#define TEMPUSUMAX 6 // Tempo máximo de atendimento em segundos
#define NFILAS 3 // Número de filas
#define QTDBANMIN 1 // Mínimo de bancadas
#define QTDBANMAX 5 // Máximo de bancadas
#define BANSERMIN 3 // Mínimo de serventes por bancada
#define BANSERMAX 6 // Máximo de serventes por bancada
#define TEMPSERMAX 3600 // Tempo máximo de trabalho sem pausa em segundos
#define TEMPSERINT 1800 // Tempo de intervalo em segundos
#define QTDSERMIN 3 // Mínimo de serventes
#define QTSERMAX 10 // Máximo de serventes
#define NING 6 // Número de ingredientes
#define CAPING1 1000 // Capacidade da vasilha em gramas
#define NPROBING1 70 // Probabilidade de aceitação do ingrediente 1
#define QTDMINING1 200 // Quantidade mínima servida do ingrediente 1 em gramas
#define QTDMAXING1 400 // Quantidade máxima servida do ingrediente 1 em gramas
#define TEMPING1 120 // Tempo para substituição da vasilha em segundos

// Estruturas para os componentes do RU
typedef struct {
    int id;
    int atendidos;
    double tempo_atendimento;
} Servente;

typedef struct {
    int id;
    Servente serventes[BANSERMAX];
    int num_serventes;
} Bancada;

typedef struct {
    int id;
    int tempo_espera;
} Usuario;

typedef struct {
    int id;
    int quantidade_consumida;
} Ingrediente;

// Variáveis globais
Bancada bancadas[QTDBANMAX];
Ingrediente ingredientes[NING];
Usuario fila[NFILAS][100]; // Fila para cada fila de usuários
int num_usuarios[NFILAS] = {0}; // Número de usuários em cada fila

void inicializar() {
    // Inicializa as bancadas e ingredientes
    for (int i = 0; i < QTDBANMAX; i++) {
        bancadas[i].id = i + 1;
        bancadas[i].num_serventes = BANSERMIN + rand() % (BANSERMAX - BANSERMIN + 1);
        bancadas[i].atendidos = 0;
        bancadas[i].tempo_atendimento = 0.0;
        
        for (int j = 0; j < bancadas[i].num_serventes; j++) {
            bancadas[i].serventes[j].id = j + 1;
            bancadas[i].serventes[j].atendidos = 0;
            bancadas[i].serventes[j].tempo_atendimento = 0.0;
        }
    }

    for (int i = 0; i < NING; i++) {
        ingredientes[i].id = i + 1;
        ingredientes[i].quantidade_consumida = 0;
    }
}

void simular() {
    // Simulação da operação do RU
    // Implementar a lógica de simulação segundo as regras definidas
    // Esta parte deve incluir a lógica de atendimento dos usuários, controle de tempo,
    // ativação/desativação de bancadas, intervalos dos serventes, etc.
}

void relatorio_final() {
    // Impressão do relatório final
    printf("Relatório Final:\n");
    for (int i = 0; i < QTDBANMAX; i++) {
        printf("Bancada %d: %d atendidos, Tempo médio de atendimento: %.2f\n",
               bancadas[i].id, bancadas[i].atendidos,
               bancadas[i].tempo_atendimento / bancadas[i].atendidos);
    }

    for (int i = 0; i < NING; i++) {
        printf("Ingrediente %d consumido: %d gramas\n", ingredientes[i].id,
               ingredientes[i].quantidade_consumida);
    }
}

int main() {
    srand(time(NULL));
    inicializar();
    simular();
    relatorio_final();
    return 0;
}
