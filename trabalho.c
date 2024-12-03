#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h> // Para usar Sleep
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

    // Abrir o arquivo para gravação
    FILE *fp = fopen("relatorio_atendimentos.txt", "w");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de relatórios.\n");
        return;
    }

    while(tempo_total_simulacao < TEMPSIMJ) {
        Sleep(1000); // Pausa a execução por 1 segundo
        tempo_total_simulacao++;

        // Geração aleatória de usuários
        int num_novos_usuarios = rand() % 5; // Gera de 0 a 4 novos usuários
        for (int i = 0; i < num_novos_usuarios; i++) {
            int usuario_id = rand() % 1000; // ID aleatório para o usuário
            int bancada_id = rand() % QTDBANMAX; // Escolhe uma bancada aleatória
            enfileirar(&bancadas[bancada_id].fila_vegana, usuario_id); // Adiciona na fila da bancada escolhida
        }

        if(tempo_total_simulacao < TEMPSIMAM) {
            printf("\nPeríodo: Manhã\n");
        } else if(tempo_total_simulacao < TEMPSIMPM) {
            printf("\nPeríodo: Almoço\n");
        } else {
            printf("\nPeríodo: Jantar\n");
        }

        atualizarBancadas(tempo_total_simulacao);

        // Simulação de atendimento
        for (int i = 0; i < QTDBANMAX; i++) {
            if (bancadas[i].ativa) {
                while (!filaVazia(&bancadas[i].fila_vegana)) { // Atende todos os usuários na fila
                    int usuario_id = desenfileirar(&bancadas[i].fila_vegana);
                    bancadas[i].atendidos++; // Incrementa atendidos da bancada
                    int tempo_atendimento = TEMPUSUMIN + rand() % (TEMPUSUMAX - TEMPUSUMIN + 1);
                    bancadas[i].tempo_atendimento += tempo_atendimento; // Acumula o tempo de atendimento

                    // Grava os dados no arquivo
                    fprintf(fp, "Bancada %d, Servente %d, Tempo de Atendimento: %d segundos, Usuário ID: %d\n", 
                            bancadas[i].id, bancadas[i].serventes[0].id, tempo_atendimento, usuario_id); // Exemplo com o primeiro servente

                    // Aqui você pode adicionar lógica para determinar qual servente atende
                    for (int j = 0; j < bancadas[i].num_serventes; j++) {
                        bancadas[i].serventes[j].atendidos++; // Incrementa atendidos do servente
                        bancadas[i].serventes[j].tempo_atendimento += tempo_atendimento; // Acumula o tempo de atendimento do servente
                    }
                }
            }
        }

        // Atualização de vasilhas e outros processos
        for(int i = 0; i < NING; i++) {
            if(vasilhas[i].quantidade_atual < QTDMINING1 || 
               difftime(time(NULL), vasilhas[i].ultimo_reabastecimento) > TEMPING1) {
                
                vasilhas[i].precisa_repor = 1;
                tempo_total_simulacao += TEMPING1;
                vasilhas[i].quantidade_atual = CAPING1;
                vasilhas[i].ultimo_reabastecimento = time(NULL);
                printf("\nVasilha %d foi reabastecida\n", i+1);
            }

            if(rand() % 100 < NPROBING1) {
                int quantidade = QTDMINING1 + rand() % (QTDMAXING1 - QTDMINING1 + 1);
                if(quantidade <= vasilhas[i].quantidade_atual) {
                    armazenarConsumo(i+1, quantidade);
                    vasilhas[i].quantidade_atual -= quantidade;
                }
            }
        }
    }

    fclose(fp); // Fecha o arquivo após a simulação
    gerarRelatorios();
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

void gerarRelatorios() {
    printf("\nRelatório Final:\n");
    int total_atendidos = 0;

    // Declaração das variáveis relatorios_bancadas e relatorios_serventes
    RelatorioBancada relatorios_bancadas[QTDBANMAX]; 
    RelatorioServente relatorios_serventes[QTDBANMAX * BANSERMAX]; // Supondo que cada bancada tenha o máximo de serventes

    // Inicialização das variáveis
    for (int i = 0; i < QTDBANMAX; i++) {
        relatorios_bancadas[i].bancada_id = bancadas[i].id;
        relatorios_bancadas[i].total_atendidos = bancadas[i].atendidos;
        relatorios_bancadas[i].tempo_medio_atendimento = (bancadas[i].atendidos > 0) ? (bancadas[i].tempo_atendimento / bancadas[i].atendidos) : 0.0;

        printf("Bancada %d: %d atendidos\n", relatorios_bancadas[i].bancada_id, relatorios_bancadas[i].total_atendidos);
        total_atendidos += relatorios_bancadas[i].total_atendidos;

        for (int j = 0; j < bancadas[i].num_serventes; j++) {
            relatorios_serventes[j].servente_id = bancadas[i].serventes[j].id;
            relatorios_serventes[j].total_atendidos = bancadas[i].serventes[j].atendidos;
            relatorios_serventes[j].tempo_medio_atendimento = (bancadas[i].serventes[j].atendidos > 0) ? (bancadas[i].serventes[j].tempo_atendimento / bancadas[i].serventes[j].atendidos) : 0.0;

            printf("  Servente %d: %d atendidos\n", relatorios_serventes[j].servente_id, relatorios_serventes[j].total_atendidos);
        }
    }

    printf("\nTotal de usuários atendidos no dia: %d\n", total_atendidos);
    exibirTemposMedios(relatorios_bancadas, relatorios_serventes); // Passar os parâmetros necessários
    exibirTotaisIngredientes(); // Chama a função para exibir os totais de ingredientes
}

void exibirTemposMedios(RelatorioBancada *relatorios_bancadas, RelatorioServente *relatorios_serventes) {
    printf("\n=== Tempo Médio de Atendimento ===\n");
    
    // Tempo médio de atendimento por bancada
    for (int i = 0; i < QTDBANMAX; i++) {
        if (bancadas[i].atendidos > 0) {
            float tempo_medio_bancada = bancadas[i].tempo_atendimento / bancadas[i].atendidos;
            printf("Bancada %d: %.2f segundos\n", bancadas[i].id, tempo_medio_bancada);
        }
        
        // Tempo médio de atendimento por servente
        for (int j = 0; j < bancadas[i].num_serventes; j++) {
            if (bancadas[i].serventes[j].atendidos > 0) {
                float tempo_medio_servente = bancadas[i].serventes[j].tempo_atendimento / bancadas[i].serventes[j].atendidos;
                printf("  Servente %d: %.2f segundos\n", bancadas[i].serventes[j].id, tempo_medio_servente);
            }
        }
    }
}

void exibirTotaisIngredientes() {
    printf("\nQuantidade total consumida de cada ingrediente:\n");
    for (int i = 0; i < NING; i++) {
        printf("Ingrediente %d: %d gramas consumidos\n", ingredientes[i].id, ingredientes[i].quantidade_consumida);
    }
}