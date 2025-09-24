#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura que representa um território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Função para exibir o mapa com todos os territórios
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n===== MAPA =====\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Territorio %d: %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("================\n");
}

// Função de ataque entre dois territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = rand() % 6 + 1; // número entre 1 e 6
    int dadoDefensor = rand() % 6 + 1;

    printf("\n%s (cor %s) atacou %s (cor %s)!\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Atacante venceu! %s agora pertence à cor %s.\n",
               defensor->nome, atacante->cor);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2; // metade das tropas do atacante
    } else {
        printf("Defensor resistiu! O atacante perdeu 1 tropa.\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

// Função que atribui uma missão a um jogador
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes; // pega uma missão aleatória
    strcpy(destino, missoes[sorteio]);   // copia a string para a missão do jogador
}

// Função simples para verificar se uma missão foi cumprida
// (aqui deixamos a lógica bem básica para iniciante)
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    // Exemplo: se a missão contém a palavra "3", verificamos se algum jogador tem 3 territórios seguidos
    if (strstr(missao, "3 territorios") != NULL) {
        int contador = 1;
        for (int i = 1; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, mapa[i-1].cor) == 0) {
                contador++;
                if (contador >= 3) return 1; // missão cumprida
            } else {
                contador = 1;
            }
        }
    }
    // Outras missões poderiam ser verificadas aqui...
    return 0; // missão não cumprida
}

// Função para liberar a memória alocada
void liberarMemoria(Territorio* mapa, char** missoesJogadores, int qtdJogadores) {
    free(mapa); // libera territórios
    for (int i = 0; i < qtdJogadores; i++) {
        free(missoesJogadores[i]); // libera cada missão
    }
    free(missoesJogadores); // libera vetor de missões
}

int main() {
    srand(time(NULL)); // garante aleatoriedade

    int qtdTerritorios;
    int qtdJogadores = 2; // para simplificar, 2 jogadores

    printf("Digite o numero de territorios: ");
    scanf("%d", &qtdTerritorios);
    getchar(); // limpa buffer

    // Alocação dinâmica do vetor de territórios
    Territorio* mapa = (Territorio*) malloc(qtdTerritorios * sizeof(Territorio));

    // Cadastro dos territórios
    for (int i = 0; i < qtdTerritorios; i++) {
        printf("\nDigite o nome do territorio %d: ", i);
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0'; // remove \n
        printf("Digite a cor (exercito) do territorio %d: ", i);
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';
        printf("Digite o numero de tropas do territorio %d: ", i);
        scanf("%d", &mapa[i].tropas);
        getchar();
    }

    // Definindo missões possíveis
    char* missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Manter pelo menos 5 tropas em um territorio",
        "Dominar o primeiro territorio",
        "Resistir a 2 ataques sem perder"
    };
    int totalMissoes = 5;

    // Alocação das missões dos jogadores
    char** missoesJogadores = (char**) malloc(qtdJogadores * sizeof(char*));
    for (int i = 0; i < qtdJogadores; i++) {
        missoesJogadores[i] = (char*) malloc(100 * sizeof(char));
        atribuirMissao(missoesJogadores[i], missoes, totalMissoes);
        printf("\nMissao do Jogador %d: %s\n", i+1, missoesJogadores[i]);
    }

    // Exibir mapa inicial
    exibirMapa(mapa, qtdTerritorios);

    // Exemplo: um ataque simples
    int atacante, defensor;
    printf("\nEscolha o territorio atacante (indice): ");
    scanf("%d", &atacante);
    printf("Escolha o territorio defensor (indice): ");
    scanf("%d", &defensor);

    // Validar ataque (não pode ser mesmo exército)
    if (strcmp(mapa[atacante].cor, mapa[defensor].cor) != 0) {
        atacar(&mapa[atacante], &mapa[defensor]);
    } else {
        printf("\nNao e permitido atacar territorio da mesma cor!\n");
    }

    // Exibir mapa atualizado
    exibirMapa(mapa, qtdTerritorios);

    // Verificar se algum jogador cumpriu missão
    for (int i = 0; i < qtdJogadores; i++) {
        if (verificarMissao(missoesJogadores[i], mapa, qtdTerritorios)) {
            printf("\nO Jogador %d cumpriu a missao: %s\n", i+1, missoesJogadores[i]);
        }
    }

    // Liberação de memória
    liberarMemoria(mapa, missoesJogadores, qtdJogadores);

    return 0;
}
