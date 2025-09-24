#include <stdio.h>
#include <string.h>

// Estrutura para representar um território
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
    // Vetor para armazenar os 5 territórios
    struct Territorio territorios[5];
    int i; 

    printf("=== Cadastro de 5 Territorios ===\n\n");

    // Loop para preencher os dados
    for (i = 0; i < 5; i++) {
        printf("Territorio %d:\n", i + 1);
        
        printf("  Nome: ");
        fgets(territorios[i].nome, 30, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = 0;

        printf("  Cor: ");
        fgets(territorios[i].cor, 10, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = 0;

        printf("  Tropas: ");
        scanf("%d", &territorios[i].tropas);
        
        // Limpar o buffer após o scanf
        while (getchar() != '\n');
        
        printf("\n");
    }

    // Exibir todos os dados cadastrados
    printf("=== Dados dos Territorios ===\n");
    for (i = 0; i < 5; i++) {
        printf("-----------------------------\n");
        printf("Territorio %d\n", i + 1);
        printf("  Nome: %s\n", territorios[i].nome);
        printf("  Cor: %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
    }
    printf("-----------------------------\n");

    return 0;
}