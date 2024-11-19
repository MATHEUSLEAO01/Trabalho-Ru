#include <stdio.h> // nesse codigo apenas mudar voida simular
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "trabalho.h"
void inicializar() {
    for (int i = 0; i < QTDBANMAX; i++) {
        bancadas[i].id = i + 1;
        bancadas[i].num_serventes = BANSERMIN + rand() % (BANSERMAX - BANSERMIN + 1);
        bancadas[i].atendidos = 0;
        bancadas[i].tempo_atendimento = 0.0;
        
        // Inicializa a fila vegana
        inicializarFila(&bancadas[i].fila_vegana);

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
        fila->frente = novo_usuario; // Se a fila estava vazia, frente e trás apontam para o novo usuário
    }
}

int desenfileirar(Fila *fila) {
    if (!fila->frente) {
        return -1; // Fila vazia
    }

    Usuario *usuario_removido = fila->frente;
    int id = usuario_removido->id;
    fila->frente = fila->frente->proximo;

    if (!fila->frente) {
        fila->tras = NULL; // Se a fila ficou vazia, trás também deve ser nulo
    }

    free(usuario_removido);
    return id; 
}

int filaVazia(Fila *fila) {
    return fila->frente == NULL;
}

void simular() {
    srand(time(NULL));
    int total_usuarios = 100;
    int usuarios_atendidos = 0;
    float tempo_total_simulacao = 0.0;

    while (usuarios_atendidos < total_usuarios) {
        // Simule se o usuário é vegano (vai ter que mudar quem vai fornecer esse dado é o usuario)
        int usuario_vegano = rand() % 5 == 0; // 20% de chance de ser vegano

        if (usuario_vegano) {
            // Enfileirar usuário vegano na fila 
            enfileirar(&bancadas[VEGA_BAN_ID].fila_vegana, usuarios_atendidos + 1); // ID do usuário
        } else {
            // Escolher uma bancada ativa que não seja vegana 
            int bancada_idx = rand() % QTDBANMAX;
            while (!bancadas[bancada_idx].ativa) {
                bancada_idx = rand() % QTDBANMAX;
            }

            // Escolher um servente da bancada
            int servente_idx = rand() % bancadas[bancada_idx].num_serventes;
            Servente *servente = &bancadas[bancada_idx].serventes[servente_idx];

            if (servente->em_intervalo) {
                continue; // Se o servente estiver em intervalo, pula
            }

            // Simulação de atendimento
            float tempo_atendimento = 1.0 + (rand() % 5);
            tempo_total_simulacao += tempo_atendimento;

            servente->tempo_atendimento += tempo_atendimento;
            servente->atendidos++;
            bancadas[bancada_idx].atendidos++;
            bancadas[bancada_idx].tempo_atendimento += tempo_atendimento;

            // Intervalo do servente (vai ter que mudar)
            if (servente->atendidos % 5 == 0) {
                servente->em_intervalo = 1; // Coloca em intervalo
                tempo_total_simulacao += 15.0; // Adiciona tempo de intervalo
                servente->em_intervalo = 0; // Retorna ao normal
            }
        }

        // Atendimento de usuários veganos da fila
        while (!filaVazia(&bancadas[VEGA_BAN_ID].fila_vegana)) {
            int usuario_id = desenfileirar(&bancadas[VEGA_BAN_ID].fila_vegana);
            // Escolhe um servente para atender o usuário vegano
            int servente_idx = rand() % bancadas[VEGA_BAN_ID].num_serventes;
            Servente *servente = &bancadas[VEGA_BAN_ID].serventes[servente_idx];

            if (servente->em_intervalo) {
                continue; // Se o servente estiver em intervalo, pula
            }

            float tempo_atendimento = 1.0 + (rand() % 5);
            tempo_total_simulacao += tempo_atendimento;

            servente->tempo_atendimento += tempo_atendimento;
            servente->atendidos++;
            bancadas[VEGA_BAN_ID].atendidos++;
            bancadas[VEGA_BAN_ID].tempo_atendimento += tempo_atendimento;

            // Intervalo do servente (vai ter que mudar)
            if (servente->atendidos % 5 == 0) {
                servente->em_intervalo = 1; // Coloca em intervalo
                tempo_total_simulacao += 15.0; // Adiciona tempo de intervalo
                servente->em_intervalo = 0; // Retorna ao normal
            }
        }

        usuarios_atendidos++;
    }

    relatorio_final();
}
void atualizarBancadas(int tempo_total_simulacao) {
    for (int i = 0; i < QTDBANMAX; i++) {
        // Ativa bancadas em horários de pico (por exemplo, a cada 30 minutos) vai ter que mudar para uma verificação de permissão
        if (tempo_total_simulacao % 30 == 0) {
            bancadas[i].ativa = 1; // Ativa a bancada
        }

        // Desativa bancadas após certo tempo de inatividade
        if (bancadas[i].atendidos < 5 && tempo_total_simulacao > 60) {
            bancadas[i].ativa = 0; // Desativa a bancada
        }
    }
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