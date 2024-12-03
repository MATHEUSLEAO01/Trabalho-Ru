#ifndef TRABALHO_H
#define TRABALHO_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEMPSIMAM 90   // 06h30 = 90 minutos simulados (1.5 horas)
#define TEMPSIMPM 180  // 11h00 = 180 minutos simulados (3 horas)
#define TEMPSIMJ  360  // 17h00 = 360 minutos simulados (6 horas no total)
#define TEMPUSUMIN 3   // Tempo mínimo de atendimento em segundos
#define TEMPUSUMAX 6   // Tempo máximo de atendimento em segundos
#define NFILAS 3       // Número de filas
#define QTDBANMIN 1    // Mínimo de bancadas
#define QTDBANMAX 5    // Máximo de bancadas
#define BANSERMIN 3    // Mínimo de serventes por bancada
#define BANSERMAX 6    // Máximo de serventes por bancada
#define TEMPSERMAX 60  // Tempo máximo de trabalho sem pausa (1h simulada = 60s)
#define TEMPSERINT 30  // Tempo de intervalo em segundos (30 minutos simulados)
#define QTDSERMIN 3    // Mínimo de serventes
#define QTSERMAX 10    // Máximo de serventes
#define NING 6         // Número de ingredientes
#define CAPING1 1000   // Capacidade da vasilha em gramas
#define NPROBING1 70   // Probabilidade de aceitação do ingrediente 1
#define QTDMINING1 200 // Quantidade mínima servida do ingrediente 1 em gramas
#define QTDMAXING1 400 // Quantidade máxima servida do ingrediente 1 em gramas
#define TEMPING1 2     // Tempo para substituição da vasilha (2 minutos simulados)

typedef struct {
    int id;
    int atendidos; // Total de atendidos
    double tempo_atendimento; // Tempo total de atendimento
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
    float tempo_atendimento; // Tempo total de atendimento da bancada
    int vegana;
    int ativa;
    int atendidos; // Total de atendidos na bancada
    Fila fila_vegana;
} Bancada;

typedef struct {
    int id;
    int quantidade_consumida;
} Ingrediente;

// Estrutura para armazenar dados de relatórios
typedef struct {
    int bancada_id;
    int total_atendidos;
    double tempo_medio_atendimento;
} RelatorioBancada;

typedef struct {
    int servente_id;
    int total_atendidos;
    double tempo_medio_atendimento;
} RelatorioServente;

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
void gerarRelatorios();
void exibirTemposMedios(RelatorioBancada *relatorios_bancadas, RelatorioServente *relatorios_serventes);
void exibirTotaisIngredientes();

#endif
