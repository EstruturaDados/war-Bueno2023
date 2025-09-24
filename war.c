#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da estrutura para representar um Territorio
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// --- Protótipos das Funções ---
void cadastrarTerritorios(struct Territorio* mapa, int num);
void exibirTerritorios(struct Territorio* mapa, int num);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void liberarMemoria(struct Territorio* mapa);

int main() {
    int num_territorios;
    int atacante_idx, defensor_idx;

    // Inicializa a semente para a geração de números aleatórios
    srand(time(NULL));

    printf("=== BEM-VINDO AO WAR ESTRUTURADO ===\n\n");
    printf("Quantos territorios voce deseja cadastrar? ");
    scanf("%d", &num_territorios);

    // Limpar o buffer após o scanf para a proxima entrada
    while (getchar() != '\n'); 

    // Alocacao dinamica de memoria para o vetor de territorios
    struct Territorio* mapa = (struct Territorio*)calloc(num_territorios, sizeof(struct Territorio));
    if (mapa == NULL) {
        printf("Erro: Nao foi possivel alocar memoria.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, num_territorios);

    printf("\n=== SIMULACAO DE BATALHA ===\n");
    printf("Digite -1 para sair a qualquer momento.\n\n");

    // Loop principal da simulacao de ataques
    while (1) {
        exibirTerritorios(mapa, num_territorios);

        printf("Escolha o numero do territorio ATACANTE: ");
        scanf("%d", &atacante_idx);
        if (atacante_idx == -1) break;

        printf("Escolha o numero do territorio DEFENSOR: ");
        scanf("%d", &defensor_idx);
        if (defensor_idx == -1) break;

        // Ajusta os indices para o vetor (0 a n-1)
        atacante_idx--;
        defensor_idx--;

        // Validacao dos indices
        if (atacante_idx < 0 || atacante_idx >= num_territorios || defensor_idx < 0 || defensor_idx >= num_territorios) {
            printf("Erro: Escolha um numero de territorio valido.\n");
            continue;
        }

        // Validacao: nao pode atacar um territorio da mesma cor
        if (strcmp(mapa[atacante_idx].cor, mapa[defensor_idx].cor) == 0) {
            printf("Erro: Voce nao pode atacar um territorio da sua propria cor.\n");
            continue;
        }

        // Chama a funcao de ataque com ponteiros para os territorios
        atacar(&mapa[atacante_idx], &mapa[defensor_idx]);

        // Espera por uma tecla para continuar
        printf("\nPressione ENTER para continuar...\n");
        while (getchar() != '\n');
        getchar();
    }
    
    // Libera a memoria alocada antes de sair do programa
    liberarMemoria(mapa);
    printf("\nSimulacao encerrada. Memoria liberada.\n");

    return 0;
}

// --- Funcao para cadastrar os territorios ---
void cadastrarTerritorios(struct Territorio* mapa, int num) {
    printf("\n--- CADASTRO DOS TERRITORIOS ---\n");
    for (int i = 0; i < num; i++) {
        printf("\nTerritorio %d:\n", i + 1);

        printf("  Nome: ");
        while (getchar() != '\n'); // Limpa o buffer de entrada
        fgets((mapa + i)->nome, 30, stdin);
        (mapa + i)->nome[strcspn((mapa + i)->nome, "\n")] = '\0';

        printf("  Cor: ");
        fgets((mapa + i)->cor, 10, stdin);
        (mapa + i)->cor[strcspn((mapa + i)->cor, "\n")] = '\0';

        printf("  Tropas: ");
        scanf("%d", &((mapa + i)->tropas));
    }
}

// --- Funcao para exibir os dados dos territorios ---
void exibirTerritorios(struct Territorio* mapa, int num) {
    printf("\n=== MAPA ATUAL ===\n");
    for (int i = 0; i < num; i++) {
        printf("------------------\n");
        printf("Territorio %d\n", i + 1);
        printf("  Nome: %s\n", (mapa + i)->nome);
        printf("  Cor: %s\n", (mapa + i)->cor);
        printf("  Tropas: %d\n", (mapa + i)->tropas);
    }
    printf("------------------\n");
}

// --- Funcao de ataque com simulacao de dados ---
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    int dado_ataque = (rand() % 6) + 1;
    int dado_defesa = (rand() % 6) + 1;

    printf("\nSimulando a batalha entre '%s' e '%s'...\n", atacante->nome, defensor->nome);
    printf("Rolagem de dados: Atacante (%d) vs. Defensor (%d)\n", dado_ataque, dado_defesa);

    if (dado_ataque > dado_defesa) {
        // Atacante vence
        printf("VITORIA! O territorio '%s' foi conquistado por '%s'!\n", defensor->nome, atacante->nome);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        printf("'%s' agora possui %d tropas.\n", defensor->nome, defensor->tropas);
    } else {
        // Atacante perde
        printf("DERROTA! O ataque falhou.\n");
        atacante->tropas--;
        if (atacante->tropas <= 0) {
            atacante->tropas = 1; // Garante que o territorio nao seja eliminado
            printf("O ataque falhou. '%s' perdeu uma tropa, mas nao foi eliminado.\n", atacante->nome);
        } else {
            printf("O ataque falhou. '%s' perdeu uma tropa e agora tem %d.\n", atacante->nome, atacante->tropas);
        }
    }
}

// --- Funcao para liberar memoria alocada ---
void liberarMemoria(struct Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
    }
}