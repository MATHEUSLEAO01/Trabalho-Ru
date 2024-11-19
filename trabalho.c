#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "trabalho.h"

// Definição das variáveis globais
Bancada bancadas[QTDBANMAX];
Ingrediente ingredientes[NING];
float tempo_total_simulacao = 0.0;

void inicializar() {
    srand(time(NULL));
    for (int i = 0; i < QTDBANMAX; i++) {
        bancadas[i].id = i + 1;
        bancadas[i].num_serventes = BANSERMIN + rand() % (BANSERMAX - BANSERMIN + 1);
        bancadas[i].atendidos = 0;
        bancadas[i].tempo_atendimento = 0.0;
        bancadas[i].ativa = 1;
        bancadas[i].vegana = (i == 0); // Primeira bancada é vegana
        
        inicializarFila(&bancadas[i].fila_vegana);

        for (int j = 0; j < bancadas[i].num_serventes; j++) {
            bancadas[i].serventes[j].id = j + 1;
            bancadas[i].serventes[j].atendidos = 0;
            bancadas[i].serventes[j].tempo_atendimento = 0.0;
            bancadas[i].serventes[j].em_intervalo = 0;
        }
    }

    for (int i = 0; i < NING; i++) {
        ingredientes[i].id = i + 1;
        ingredientes[i].quantidade_consumida = 0;
    }
}

void inicializarFila(Fila *fila) {
    fila->frente = NULL;
    fila->tras = NULL;
}

void enfileirar(Fila *fila, int usuario_id) {
    Usuario *novo_usuario = (Usuario *)malloc(sizeof(Usuario));
    novo_usuario->id = usuario_id;
    novo_usuario->proximo = NULL;

    if (fila->tras) {
        fila->tras->proximo = novo_usuario;
    }
    fila->tras = novo_usuario;

    if (!fila->frente) {
        fila->frente = novo_usuario;
    }
}

int desenfileirar(Fila *fila) {
    if (!fila->frente) {
        return -1;
    }

    Usuario *usuario_removido = fila->frente;
    int id = usuario_removido->id;
    fila->frente = fila->frente->proximo;

    if (!fila->frente) {
        fila->tras = NULL;
    }

    free(usuario_removido);
    return id; 
}

int filaVazia(Fila *fila) {
    return fila->frente == NULL;
}

void simular() {
    struct {
        int quantidade_atual;
        time_t ultimo_reabastecimento;
        int precisa_repor;
    } vasilhas[NING];
    
    for(int i = 0; i < NING; i++) {
        vasilhas[i].quantidade_atual = CAPING1;
        vasilhas[i].ultimo_reabastecimento = time(NULL);
        vasilhas[i].precisa_repor = 0;
    }

    while(tempo_total_simulacao < TEMPSIMJ) {
        if(tempo_total_simulacao < TEMPSIMAM) {
            printf("Período: Manhã\n");
        } else if(tempo_total_simulacao < TEMPSIMPM) {
            printf("Período: Almoço\n");
        } else {
            printf("Período: Jantar\n");
        }

        atualizarBancadas(tempo_total_simulacao);

        int tempo_atendimento = TEMPUSUMIN + (rand() % (TEMPUSUMAX - TEMPUSUMIN + 1));
        
        for(int i = 0; i < NING; i++) {
            if(vasilhas[i].quantidade_atual < QTDMINING1 || 
               difftime(time(NULL), vasilhas[i].ultimo_reabastecimento) > TEMPING1) {
                
                vasilhas[i].precisa_repor = 1;
                tempo_total_simulacao += TEMPING1;
                vasilhas[i].quantidade_atual = CAPING1;
                vasilhas[i].ultimo_reabastecimento = time(NULL);
                printf("Vasilha %d foi reabastecida\n", i+1);
            }

            if(rand() % 100 < NPROBING1) {
                int quantidade = QTDMINING1 + rand() % (QTDMAXING1 - QTDMINING1 + 1);
                if(quantidade <= vasilhas[i].quantidade_atual) {
                    armazenarConsumo(i+1, quantidade);
                    vasilhas[i].quantidade_atual -= quantidade;
                }
            }
        }

        tempo_total_simulacao += tempo_atendimento;
    }

    relatorio_final();
}

void atualizarBancadas(int tempo_total_simulacao) {
    if(tempo_total_simulacao < TEMPSIMAM) {
        int bancadas_ativas = 0;
        for(int i = 0; i < QTDBANMAX; i++) {
            if(bancadas[i].ativa) bancadas_ativas++;
        }
        
        if(bancadas_ativas < QTDBANMIN) {
            for(int i = 0; i < QTDBANMAX && bancadas_ativas < QTDBANMIN; i++) {
                if(!bancadas[i].ativa) {
                    bancadas[i].ativa = 1;
                    bancadas_ativas++;
                }
            }
        }
    }
    else if(tempo_total_simulacao < TEMPSIMPM) {
        for(int i = 0; i < QTDBANMAX; i++) {
            bancadas[i].ativa = 1;
        }
    }
    else {
        int bancada_desativada = 0;
        for(int i = 0; i < QTDBANMAX && !bancada_desativada; i++) {
            if(bancadas[i].ativa && !bancadas[i].vegana) {
                bancadas[i].ativa = 0;
                bancada_desativada = 1;
            }
        }
    }
}

void armazenarConsumo(int ingrediente_id, int quantidade) {
    if(ingrediente_id >= 1 && ingrediente_id <= NING) {
        ingredientes[ingrediente_id - 1].quantidade_consumida += quantidade;
        
        FILE *fp = fopen("consumo_log.txt", "a");
        if(fp != NULL) {
            time_t now = time(NULL);
            fprintf(fp, "Ingrediente %d: %d gramas consumidos em %s", 
                    ingrediente_id, quantidade, ctime(&now));
            fclose(fp);
        }
    }
}

void relatorio_final() {
    printf("\nRelatório Final:\n");
    printf("\nConsumo de Ingredientes:\n");
    for (int i = 0; i < NING; i++) {
        printf("Ingrediente %d: %d gramas consumidos\n", 
               ingredientes[i].id, ingredientes[i].quantidade_consumida);
    }

    for (int i = 0; i < QTDBANMAX; i++) {
        if(bancadas[i].atendidos > 0) {
            printf("Bancada %d: %d atendidos, Tempo médio de atendimento: %.2f\n",
                   bancadas[i].id, bancadas[i].atendidos,
                   bancadas[i].tempo_atendimento / bancadas[i].atendidos);
        }
    }
}
