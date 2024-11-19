#ifndef TRABALHO_H
#define TRABALHO_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEMPSIMAM 3600 // 6h30
#define TEMPSIMPM 2100 // 11h00
#define TEMPSIMJ 9000  // 17h00
#define TEMPUSUMIN 3   // Tempo mínimo de atendimento em segundos
#define TEMPUSUMAX 6   // Tempo máximo de atendimento em segundos
#define NFILAS 3       // Número de filas
#define QTDBANMIN 1    // Mínimo de bancadas
#define QTDBANMAX 5    // Máximo de bancadas
#define BANSERMIN 3    // Mínimo de serventes por bancada
#define BANSERMAX 6    // Máximo de serventes por bancada
#define TEMPSERMAX 3600 // Tempo máximo de trabalho sem pausa em segundos
#define TEMPSERINT 1800 // Tempo de intervalo em segundos
#define QTDSERMIN 3    // Mínimo de serventes
#define QTSERMAX 10    // Máximo de serventes
#define NING 6         // Número de ingredientes
#define CAPING1 1000   // Capacidade da vasilha em gramas
#define NPROBING1 70   // Probabilidade de aceitação do ingrediente 1
#define QTDMINING1 200 // Quantidade mínima servida do ingrediente 1 em gramas
#define QTDMAXING1 400 // Quantidade máxima servida do ingrediente 1 em gramas
#define TEMPING1 120   // Tempo para substituição da vasilha em segundos

typedef struct {
    int id;
    int atendidos;
    double tempo_atendimento;
    int em_intervalo;
} Servente;

typedef struct Usuario {
    int id;
    struct Usuario *proximo;
} Usuario;

typedef struct {
    Usuario *frente;
    Usuario *tras;
} Fila;

typedef struct {
    int id;
    Servente serventes[BANSERMAX];
    int num_serventes;
    float tempo_atendimento;
    int vegana;
    int ativa;
    int atendidos;
    Fila fila_vegana;
} Bancada;

typedef struct {
    int id;
    int quantidade_consumida;
} Ingrediente;

// Variáveis globais
extern Bancada bancadas[QTDBANMAX];
extern Ingrediente ingredientes[NING];
extern float tempo_total_simulacao;

// Protótipos das funções
void inicializar();
void inicializarFila(Fila *fila);
void enfileirar(Fila *fila, int usuario_id);
int desenfileirar(Fila *fila);
int filaVazia(Fila *fila);
void simular();
void atualizarBancadas(int tempo_total_simulacao);
void armazenarConsumo(int ingrediente_id, int quantidade);
void relatorio_final();

#endif