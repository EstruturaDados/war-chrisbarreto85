/* guerra_atacar.c
 *
 * Implementação da funcionalidade de ataque entre territórios conforme especificação.
 *
 * Compilar:
 *   gcc -std=c11 -Wall -Wextra -O2 guerra_atacar.c -o guerra_atacar
 *
 * Executar:
 *   ./guerra_atacar
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NOME_LEN 30
#define COR_LEN 10

/* Estrutura do território conforme especificação */
typedef struct {
    char nome[NOME_LEN];
    char cor[COR_LEN];
    int tropas;
} Territorio;

/* ---------- Protótipos ---------- */

/* Cadastro, exibição e seleção */
Territorio* cadastrarTerritorios(size_t n);
void exibirMapa(const Territorio* mapa, size_t n);
int lerInteiro(const char *prompt, int min, int max);

/* Ataque */
void atacar(Territorio* atacante, Territorio* defensor);

/* Liberar memória */
void liberarMemoria(Territorio* mapa);

/* Utilitários de string */
void lerStringLimpa(const char* prompt, char* destino, size_t maxlen);

/* ---------- Implementação ---------- */

/* Lê uma string do stdin e remove '\n' final */
void lerStringLimpa(const char* prompt, char* destino, size_t maxlen) {
    if (prompt) printf("%s", prompt);
    if (fgets(destino, (int)maxlen, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }
    size_t len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n') destino[len - 1] = '\0';
}

/* Lê um inteiro entre min e max inclusivo com prompt (valida entrada) */
int lerInteiro(const char *prompt, int min, int max) {
    char buffer[64];
    int val;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        if (sscanf(buffer, "%d", &val) != 1) {
            printf("Entrada inválida. Digite um número inteiro.\n");
            continue;
        }
        if (val < min || val > max) {
            printf("Valor fora do intervalo [%d - %d]. Tente novamente.\n", min, max);
            continue;
        }
        return val;
    }
}

/* Cadastro de territórios: retorna ponteiro para vetor alocado dinamicamente */
Territorio* cadastrarTerritorios(size_t n) {
    /* Usamos calloc para inicializar com zeros */
    Territorio *mapa = calloc(n, sizeof(Territorio));
    if (!mapa) {
        fprintf(stderr, "Erro: falha na alocação de memória para os territórios.\n");
        return NULL;
    }

    for (size_t i = 0; i < n; ++i) {
        char prompt[80];
        printf("\n--- Cadastro do território %zu ---\n", i);
        snprintf(prompt, sizeof(prompt), "Nome do território %zu: ", i);
        lerStringLimpa(prompt, mapa[i].nome, NOME_LEN);

        /* Ler cor */
        snprintf(prompt, sizeof(prompt), "Cor (ex: Vermelho, Azul) do território %zu: ", i);
        lerStringLimpa(prompt, mapa[i].cor, COR_LEN);

        /* Ler tropas (no mínimo 1) */
        int tropas = lerInteiro("Número de tropas (inteiro >= 1): ", 1, 1000000);
        mapa[i].tropas = tropas;
    }

    return mapa;
}

/* Exibe mapa com índices, nomes, cores e tropas */
void exibirMapa(const Territorio* mapa, size_t n) {
    printf("\n--- Mapa de Territórios ---\n");
    for (size_t i = 0; i < n; ++i) {
        printf("[%zu] %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("---------------------------\n");
}

/* Função de ataque
 * - Usa rand() para simular um dado 1..6 para atacante e defensor.
 * - Valida que as cores sejam diferentes e que atacante tenha >1 tropas (necessário para mover).
 * - Se atacante vence (dado maior): defensor recebe a cor do atacante e metade das tropas do atacante
 *   (transferidas). O atacante perde estas tropas.
 * - Se atacante perde ou empata: atacante perde 1 tropa.
 *
 * Todas as modificações são feitas via ponteiros (Territorio*).
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    if (!atacante || !defensor) {
        printf("Erro: ponteiro de território inválido.\n");
        return;
    }

    /* Validações básicas */
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque inválido: atacante e defensor têm a mesma cor ('%s').\n", atacante->cor);
        return;
    }

    if (atacante->tropas <= 1) {
        printf("Ataque inválido: atacante precisa ter mais de 1 tropa para atacar (atual: %d).\n", atacante->tropas);
        return;
    }

    /* Roll dos dados */
    int dado_atacante = (rand() % 6) + 1; /* 1..6 */
    int dado_defensor  = (rand() % 6) + 1; /* 1..6 */

    printf("\nRolagem de dados: Atacante -> %d | Defensor -> %d\n", dado_atacante, dado_defensor);

    if (dado_atacante > dado_defensor) {
        /* Atacante vence */
        int metade = atacante->tropas / 2; /* metade das tropas do atacante (inteiro) */
        if (metade < 1) metade = 1; /* garante que pelo menos 1 seja transferido se atacante tiver >1 */

        /* Transferência: defensor perde suas tropas anteriores (conquistado) e recebe 'metade' */
        strncpy(defensor->cor, atacante->cor, COR_LEN - 1);
        defensor->cor[COR_LEN - 1] = '\0';

        defensor->tropas = metade;
        atacante->tropas -= metade;
        if (atacante->tropas < 1) atacante->tropas = 1; /* proteção */

        printf("Resultado: Atacante venceu! Território '%s' foi conquistado.\n", defensor->nome);
        printf("%d tropas foram movidas para o território conquistado.\n", metade);
    } else {
        /* Atacante perde (empate favorece defensor) */
        atacante->tropas -= 1;
        if (atacante->tropas < 1) atacante->tropas = 1; /* garantindo no mínimo 1 */
        printf("Resultado: Atacante perdeu (ou empate). Atacante perde 1 tropa.\n");
    }

    /* Exibição do estado atualizado dos dois territórios */
    printf("\nEstado pós-ataque:\n");
    printf("Atacante: %s | Cor: %s | Tropas: %d\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("Defensor: %s | Cor: %s | Tropas: %d\n", defensor->nome, defensor->cor, defensor->tropas);
}

/* Libera memória alocada para o mapa (vetor de Territorio) */
void liberarMemoria(Territorio* mapa) {
    if (mapa) free(mapa);
}

/* ---------- Função main: interação com o usuário ---------- */

int main(void) {
    srand((unsigned int)time(NULL)); /* inicializa semente do RNG */

    printf("=== Guerra da Programacao: Simulador de Ataque ===\n");

    /* Quantidade de territórios */
    int n = lerInteiro("Digite o número total de territórios a cadastrar (>=1): ", 1, 1000);

    /* Cadastro (alocação dinâmica com calloc) */
    Territorio *mapa = cadastrarTerritorios((size_t)n);
    if (!mapa) {
        return EXIT_FAILURE;
    }

    /* Loop de interação: permitir múltiplos ataques até o usuário sair */
    while (1) {
        exibirMapa(mapa, (size_t)n);

        printf("\nOpções:\n");
        printf("1 - Realizar ataque\n");
        printf("2 - Exibir mapa\n");
        printf("0 - Sair e liberar memória\n");

        int opc = lerInteiro("Escolha uma opção: ", 0, 2);

        if (opc == 0) {
            printf("Saindo...\n");
            break;
        } else if (opc == 2) {
            continue; /* já exibimos o mapa no início do loop */
        } else if (opc == 1) {
            int idx_atq = lerInteiro("Índice do território atacante: ", 0, n - 1);
            int idx_def = lerInteiro("Índice do território defensor: ", 0, n - 1);

            if (idx_atq == idx_def) {
                printf("Escolha inválida: atacante e defensor não podem ser o mesmo território.\n");
                continue;
            }

            /* Chamamos a função atacar passando ponteiros para os territórios */
            atacar(&mapa[idx_atq], &mapa[idx_def]);
        }
    }

    /* Liberar memória do mapa dinamicamente alocado */
    liberarMemoria(mapa);

    printf("Memória liberada. Programa finalizado.\n");
    return EXIT_SUCCESS;
}
