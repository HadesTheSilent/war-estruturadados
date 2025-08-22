// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h> // Para malloc, free, srand, rand
#include <string.h> // Para strcpy, strlen, strcspn
#include <time.h>   // Para time
#include <locale.h> // Para setlocale

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define MAX_NOME_TERRITORIO 30
#define MAX_COR_EXERCITO 10
#define MAX_DESCRICAO_MISSAO 100
#define NUM_TERRITORIOS 5 // Conforme o desafio anterior, mantendo 5 para o exemplo
#define NUM_MISSOES 3 // Exemplo: 3 tipos de missões

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_NOME_TERRITORIO];
    char cor[MAX_COR_EXERCITO];
    int tropas;
} Territorio;

// Define a estrutura para uma missão, contendo seu ID e uma descrição.
typedef struct {
    int id;
    char descricao[MAX_DESCRICAO_MISSAO];
} Missao;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa);
void exibirMissao(int idMissao);

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio* mapa);
void simularAtaque(Territorio* atacante, Territorio* defensor);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, int idMissao, const char* corJogador);

// Função utilitária:
void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    Territorio* mapa = alocarMapa();
    if (mapa == NULL) {
        return 1; // Erro na alocação de memória
    }
    inicializarTerritorios(mapa);

    char corJogador[MAX_COR_EXERCITO];
    strcpy(corJogador, "Vermelho"); // Exemplo: jogador começa com exército Vermelho
    int idMissaoJogador = sortearMissao();

    int opcao;
    int jogoAtivo = 1;

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
    do {
        exibirMapa(mapa);
        exibirMissao(idMissaoJogador);
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa);
                break;
            case 2:
                if (verificarVitoria(mapa, idMissaoJogador, corJogador)) {
                    printf("\nPARABÉNS! Você cumpriu sua missão e venceu o jogo!\n");
                    jogoAtivo = 0; // Encerra o jogo
                } else {
                    printf("\nSua missão ainda não foi cumprida. Continue lutando!\n");
                }
                break;
            case 0:
                printf("\nSaindo do jogo. Até a próxima!\n");
                jogoAtivo = 0; // Encerra o jogo
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }
        if (jogoAtivo) {
            printf("\nPressione ENTER para continuar...");
            getchar(); // Pausa a execução
        }

    } while (jogoAtivo);

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(mapa);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarMapa() {
    Territorio* mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    if (mapa == NULL) {
        perror("Erro ao alocar memória para o mapa");
    }
    return mapa;
}

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(Territorio* mapa) {
    if (mapa == NULL) {
        fprintf(stderr, "Erro: Mapa não alocado para inicialização.\n");
        return;
    }

    // Exemplo de inicialização de territórios
    strcpy(mapa[0].nome, "Territorio A");
    strcpy(mapa[0].cor, "Vermelho");
    mapa[0].tropas = 10;

    strcpy(mapa[1].nome, "Territorio B");
    strcpy(mapa[1].cor, "Azul");
    mapa[1].tropas = 12;

    strcpy(mapa[2].nome, "Territorio C");
    strcpy(mapa[2].cor, "Verde");
    mapa[2].tropas = 8;

    strcpy(mapa[3].nome, "Territorio D");
    strcpy(mapa[3].cor, "Amarelo");
    mapa[3].tropas = 15;

    strcpy(mapa[4].nome, "Territorio E");
    strcpy(mapa[4].cor, "Preto");
    mapa[4].tropas = 7;
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        mapa = NULL; // Evita ponteiros dangling
    }
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal() {
    printf("\n--- Menu Principal ---\n");
    printf("1. Atacar\n");
    printf("2. Verificar Missão\n");
    printf("0. Sair do Jogo\n");
    printf("Escolha uma opção: ");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio* mapa) {
    printf("\n--- Mapa dos Territórios ---\n");
    printf("%-15s %-10s %-8s\n", "Território", "Cor", "Tropas");
    printf("------------------------------------\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-15s %-10s %-8d\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------------------------\n");
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(int idMissao) {
    Missao missoes[NUM_MISSOES];
    // Inicializa as missões (poderia ser lido de um arquivo ou configurado de forma mais robusta)
    missoes[0].id = 1;
    strcpy(missoes[0].descricao, "Conquistar 3 territórios do exército Azul.");
    missoes[1].id = 2;
    strcpy(missoes[1].descricao, "Destruir o exército Vermelho (ocupar todos os seus territórios).");
    missoes[2].id = 3;
    strcpy(missoes[2].descricao, "Conquistar 5 territórios quaisquer.");

    printf("\n--- Sua Missão ---\n");
    for (int i = 0; i < NUM_MISSOES; i++) {
        if (missoes[i].id == idMissao) {
            printf("Missão %d: %s\n", missoes[i].id, missoes[i].descricao);
            return;
        }
    }
    printf("Missão desconhecida (ID: %d).\n", idMissao);
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
int sortearMissao() {
    return (rand() % NUM_MISSOES) + 1; // Retorna um ID entre 1 e NUM_MISSOES
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(Territorio* atacante, Territorio* defensor) {
    if (atacante == NULL || defensor == NULL) {
        fprintf(stderr, "Erro: Territórios inválidos para simular ataque.\n");
        return;
    }

    if (atacante->tropas <= 1) {
        printf("\n%s não tem tropas suficientes para atacar (mínimo 2 tropas).\n", atacante->nome);
        return;
    }

    printf("\n--- Simulação de Ataque ---\n");
    printf("%s (%s, %d tropas) ataca %s (%s, %d tropas).\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    // Simulação de rolagem de dados (simplificada)
    int dadosAtacante = (rand() % 6) + 1; // 1 dado para o atacante
    int dadosDefensor = (rand() % 6) + 1; // 1 dado para o defensor

    printf("Dados do atacante: %d\n", dadosAtacante);
    printf("Dados do defensor: %d\n", dadosDefensor);

    if (dadosAtacante > dadosDefensor) {
        printf("Atacante venceu a batalha!\n");
        defensor->tropas--; // Defensor perde 1 tropa
        if (defensor->tropas == 0) {
            printf("%s foi conquistado por %s!\n", defensor->nome, atacante->cor);
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = 1; // Move 1 tropa para o território conquistado
            atacante->tropas--; // Atacante perde 1 tropa que moveu
        }
    } else {
        printf("Defensor venceu a batalha!\n");
        atacante->tropas--; // Atacante perde 1 tropa
    }
    printf("Tropas restantes em %s: %d\n", atacante->nome, atacante->tropas);
    printf("Tropas restantes em %s: %d\n", defensor->nome, defensor->tropas);
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(Territorio* mapa) {
    int origem_idx, destino_idx;

    printf("\n--- Fase de Ataque ---\n");
    printf("Escolha o território de ORIGEM (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &origem_idx);
    limparBufferEntrada();

    printf("Escolha o território de DESTINO (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &destino_idx);
    limparBufferEntrada();

    // Ajusta para índice baseado em 0
    origem_idx--;
    destino_idx--;

    if (origem_idx < 0 || origem_idx >= NUM_TERRITORIOS ||
        destino_idx < 0 || destino_idx >= NUM_TERRITORIOS) {
        printf("Seleção de território inválida.\n");
        return;
    }

    if (strcmp(mapa[origem_idx].cor, mapa[destino_idx].cor) == 0) {
        printf("Não é possível atacar seu próprio território.\n");
        return;
    }

    simularAtaque(&mapa[origem_idx], &mapa[destino_idx]);
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarVitoria(const Territorio* mapa, int idMissao, const char* corJogador) {
    int i;
    int territoriosConquistados = 0;
    int exercitoDestruido = 1; // Assume que o exército está destruído até encontrar um território

    switch (idMissao) {
        case 1: // Conquistar 3 territórios do exército Azul.
            for (i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor, corJogador) == 0) {
                    territoriosConquistados++;
                }
            }
            return territoriosConquistados >= 3; // Simplificado para 3 territórios quaisquer para teste
        case 2: // Destruir o exército Vermelho (ocupar todos os seus territórios).
            for (i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor, "Vermelho") == 0) {
                    exercitoDestruido = 0; // Encontrou um território Vermelho
                    break;
                }
            }
            return exercitoDestruido;
        case 3: // Conquistar 5 territórios quaisquer.
            for (i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor, corJogador) == 0) {
                    territoriosConquistados++;
                }
            }
            return territoriosConquistados >= 5;
        default:
            return 0; // Missão desconhecida
    }
}