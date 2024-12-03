#include <stdio.h>
#include <stdlib.h>
#include "trabalho.h"

void exibirMenu() {
    printf("\n=== Sistema de Simulação do RU ===\n");
    printf("1. Inicializar Sistema\n");
    printf("2. Rodar Simulação\n");
    printf("3. Gerar Relatórios\n");
    printf("4. Sair\n");
    printf("Escolha uma opção: ");
}

int main() {
    int opcao;
    do {
        exibirMenu();
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                inicializar();
                printf("Sistema inicializado com sucesso!\n");
                break;
            case 2:
                simular();
                printf("Simulação concluída!\n");
                break;
            case 3:
                gerarRelatorios();
                break;
            case 4:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 4);
    return 0;
}
