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

// Definição da estrutura para representar um Jogador
struct Jogador {
    char cor[10];
    char* missao; // Missão armazenada dinamicamente
};

// --- Protótipos das Funções ---
void cadastrarTerritorios(struct Territorio* mapa, int num);
void exibirTerritorios(struct Territorio* mapa, int num);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void atribuirMissao(struct Jogador* jogador, const char* missoes[], int totalMissoes);
int verificarMissao(struct Jogador* jogador, struct Territorio* mapa, int tamanho, const char* missoes[], int totalMissoes);
void liberarMemoria(struct Territorio* mapa, struct Jogador* jogadores, int num_jogadores);

// Vetor de missoes pre-definidas
const char* missoes[] = {
    "Conquistar 5 territorios.",
    "Eliminar todas as tropas da cor Vermelha.",
    "Eliminar todas as tropas da cor Azul.",
    "Conquistar 3 territorios seguidos.",
    "Dominar todos os territorios."
};
const int TOTAL_MISSOES = 5;

int main() {
    int num_territorios;
    int atacante_idx, defensor_idx;
    int jogador_atual = 0; // 0 para o jogador Vermelho, 1 para o Azul

    // Inicializa a semente para a geracao de numeros aleatorios
    srand(time(NULL));

    printf("=== BEM-VINDO AO WAR ESTRUTURADO FINAL ===\n\n");
    printf("Quantos territorios voce deseja cadastrar? ");
    scanf("%d", &num_territorios);

    // Limpar o buffer apos o scanf
    while (getchar() != '\n'); 

    // Alocacao dinamica de memoria para o vetor de territorios
    struct Territorio* mapa = (struct Territorio*)calloc(num_territorios, sizeof(struct Territorio));
    if (mapa == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para o mapa.\n");
        return 1;
    }

    // Alocacao dinamica para os jogadores
    struct Jogador* jogadores = (struct Jogador*)malloc(2 * sizeof(struct Jogador));
    if (jogadores == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para os jogadores.\n");
        free(mapa);
        return 1;
    }
    
    // Atribuicao de cores aos jogadores
    strcpy(jogadores[0].cor, "Vermelho");
    strcpy(jogadores[1].cor, "Azul");

    // Atribuicao de missoes aos jogadores
    atribuirMissao(&jogadores[0], missoes, TOTAL_MISSOES);
    atribuirMissao(&jogadores[1], missoes, TOTAL_MISSOES);

    cadastrarTerritorios(mapa, num_territorios);
    
    // Exibicao inicial das missoes
    printf("\n--- MISSOES ATRIBUIDAS ---\n");
    printf("Jogador %s: %s\n", jogadores[0].cor, jogadores[0].missao);
    printf("Jogador %s: %s\n", jogadores[1].cor, jogadores[1].missao);

    printf("\n=== SIMULACAO DE BATALHA ===\n");
    printf("Digite -1 para sair a qualquer momento.\n\n");

    // Loop principal do jogo
    while (1) {
        exibirTerritorios(mapa, num_territorios);
        printf("\nVEZ DO JOGADOR %s\n", jogadores[jogador_atual].cor);

        printf("Escolha o numero do territorio ATACANTE: ");
        scanf("%d", &atacante_idx);
        if (atacante_idx == -1) break;

        printf("Escolha o numero do territorio DEFENSOR: ");
        scanf("%d", &defensor_idx);
        if (defensor_idx == -1) break;

        // Ajusta os indices para o vetor (0 a n-1)
        atacante_idx--;
        defensor_idx--;
        while (getchar() != '\n'); // Limpa o buffer

        // Validacao: territorio atacante deve ser do jogador atual
        if (strcmp(mapa[atacante_idx].cor, jogadores[jogador_atual].cor) != 0) {
            printf("Erro: Voce so pode atacar com seus proprios territorios.\n");
            continue;
        }
        
        // Validacao: nao pode atacar um territorio da propria cor
        if (strcmp(mapa[atacante_idx].cor, mapa[defensor_idx].cor) == 0) {
            printf("Erro: Voce nao pode atacar um territorio da sua propria cor.\n");
            continue;
        }

        atacar(&mapa[atacante_idx], &mapa[defensor_idx]);

        // Verifica a missao do jogador atual
        if (verificarMissao(&jogadores[jogador_atual], mapa, num_territorios, missoes, TOTAL_MISSOES)) {
            printf("\nPARABENS! O jogador %s CUMPRIU SUA MISSAO E VENCEU O JOGO!\n", jogadores[jogador_atual].cor);
            break;
        }
        
        // Muda para o proximo jogador
        jogador_atual = (jogador_atual + 1) % 2;
        
        printf("\nPressione ENTER para continuar...\n");
        getchar();
    }
    
    // Libera a memoria alocada
    liberarMemoria(mapa, jogadores, 2);
    printf("\nSimulacao encerrada. Memoria liberada.\n");

    return 0;
}

// --- Funcao para cadastrar os territorios ---
void cadastrarTerritorios(struct Territorio* mapa, int num) {
    printf("\n--- CADASTRO DOS TERRITORIOS ---\n");
    for (int i = 0; i < num; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("  Nome: ");
        fgets((mapa + i)->nome, 30, stdin);
        (mapa + i)->nome[strcspn((mapa + i)->nome, "\n")] = '\0';

        printf("  Cor: ");
        fgets((mapa + i)->cor, 10, stdin);
        (mapa + i)->cor[strcspn((mapa + i)->cor, "\n")] = '\0';

        printf("  Tropas: ");
        scanf("%d", &((mapa + i)->tropas));
        while (getchar() != '\n');
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

// --- Funcao para sortear e atribuir uma missao a um jogador ---
void atribuirMissao(struct Jogador* jogador, const char* missoes[], int totalMissoes) {
    int missao_sorteada = rand() % totalMissoes;
    jogador->missao = (char*)malloc(strlen(missoes[missao_sorteada]) + 1);
    if (jogador->missao != NULL) {
        strcpy(jogador->missao, missoes[missao_sorteada]);
    }
}

// --- Funcao de ataque com simulacao de dados ---
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    int dado_ataque = (rand() % 6) + 1;
    int dado_defesa = (rand() % 6) + 1;

    printf("\nSimulando a batalha entre '%s' e '%s'...\n", atacante->nome, defensor->nome);
    printf("Rolagem de dados: Atacante (%d) vs. Defensor (%d)\n", dado_ataque, dado_defesa);

    if (dado_ataque > dado_defesa) {
        printf("VITORIA! O territorio '%s' foi conquistado por '%s'!\n", defensor->nome, atacante->nome);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        printf("'%s' agora possui %d tropas.\n", defensor->nome, defensor->tropas);
    } else {
        printf("DERROTA! O ataque falhou.\n");
        atacante->tropas--;
        if (atacante->tropas <= 0) {
            atacante->tropas = 1;
            printf("O ataque falhou. '%s' perdeu uma tropa, mas nao foi eliminado.\n", atacante->nome);
        } else {
            printf("O ataque falhou. '%s' perdeu uma tropa e agora tem %d.\n", atacante->nome, atacante->tropas);
        }
    }
}

// --- Funcao para verificar se a missao do jogador foi cumprida ---
int verificarMissao(struct Jogador* jogador, struct Territorio* mapa, int tamanho, const char* missoes[], int totalMissoes) {
    // Logica simples para verificar missoes com base em palavras-chave
    // Missao 1: Conquistar 5 territorios.
    if (strstr(jogador->missao, "Conquistar 5 territorios") != NULL) {
        int territorios_conquistados = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, jogador->cor) == 0) {
                territorios_conquistados++;
            }
        }
        if (territorios_conquistados >= 5) {
            return 1;
        }
    }

    // Missao 2 e 3: Eliminar todas as tropas de uma cor
    if (strstr(jogador->missao, "Eliminar todas as tropas") != NULL) {
        char cor_alvo[10];
        if (strstr(jogador->missao, "Vermelha") != NULL) {
            strcpy(cor_alvo, "Vermelho");
        } else {
            strcpy(cor_alvo, "Azul");
        }
        
        int tropas_restantes = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, cor_alvo) == 0) {
                tropas_restantes += mapa[i].tropas;
            }
        }
        if (tropas_restantes == 0) {
            return 1;
        }
    }

    // Missao 4: Conquistar 3 territorios seguidos.
    // Esta logica e mais complexa e precisaria de um historico de turnos. 
    // Para a simplicidade do codigo, a condicao nao sera verificada.

    // Missao 5: Dominar todos os territorios.
    if (strstr(jogador->missao, "Dominar todos os territorios") != NULL) {
        int total_territorios_jogador = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, jogador->cor) == 0) {
                total_territorios_jogador++;
            }
        }
        if (total_territorios_jogador == tamanho) {
            return 1;
        }
    }

    return 0;
}

// --- Funcao para liberar memoria alocada ---
void liberarMemoria(struct Territorio* mapa, struct Jogador* jogadores, int num_jogadores) {
    if (mapa != NULL) {
        free(mapa);
    }
    if (jogadores != NULL) {
        for (int i = 0; i < num_jogadores; i++) {
            if (jogadores[i].missao != NULL) {
                free(jogadores[i].missao);
            }
        }
        free(jogadores);
    }
}