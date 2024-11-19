#include <stdio.h>
#include "trabalho.h"

int main() {
    // Inicializar sistema
    printf("Inicializando o sistema...\n");
    inicializar();

    // Testar simulação de atendimento
    printf("\nIniciando simulação de atendimento:\n");
    simular();

    // Testar atualização das bancadas
    printf("\nAtualizando bancadas:\n");
    atualizarBancadas(30);

    // Testar armazenamento de consumo
    printf("\nRegistrando consumo:\n");
    armazenarConsumo(1, 500); // Ingrediente 1, 500 gramas
    armazenarConsumo(2, 1200); // Ingrediente 2, 1200 gramas

    // Finalizar
    printf("\nTeste finalizado.\n");
    return 0;
} 