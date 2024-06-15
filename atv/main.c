#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avl.h"
#include "arn.h"

int* gerarValores(int n, char ordem, int *tamanho) {
    int i;
    int *valores = (int*)malloc(n * sizeof(int));

    if (ordem == 'o'){
        // Gerar valores ordenados
        for (i = 0; i < n; i++) {
            valores[i] = i + 1;
        }
    }
    else{
        // Gerar valores aleatórios
        srand(time(NULL)); // Inicializar a semente do gerador de números aleatórios
        for (i = 0; i < n; i++) {
            valores[i] = rand() % 10000; // Valores aleatórios entre 0 e 9999
        }
    }

    // Atribui o tamanho do array à variável passada por referência
    *tamanho = n;

    return valores;
}

void removerValoresAVL(ArvoreAVL *arvore, int *valores, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        removerAVL(arvore, valores[i]);
    }
}

void removerValoresRN(ArvoreRN *arvore, int *valores, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        removerRN(arvore, valores[i]);
    }
}

int main(){

    // O loop será composto de:
    // * Receber valores de tamanho dos valores e ordem. Chamar função que gera os valores baseado neles.
    // * Implementação da ávore AVL: inserção, remoção, contagem de tempo, impressão das rotações.
    // * Implementação da ávore RubroNegra: inserção, remoção, contagem de tempo, impressão das rotações. A REMOÇÃO ESTÁ FAZENDO O CÓDIGO SE ENCERRAR INESPERADAMENTE
    // * FALTA IMPLEMENTAR BUSCA EM AMBAS AS ÁRVORES
    while(1)
    {   

        int x;
        char ordem;
        int rotaInser;

        printf("\nInsira o tamanho dos valores: ");
        scanf("%d", &x);

        printf("Insira o metodo de ordenacão dos valores (o = ordenado, a = aleatorio): ");
        scanf(" %c", &ordem);

        // Árvore AVL

        printf("\nArvore AVL\n");
        ArvoreAVL *arvore_avl = criar_arvoreAVL();
        int tamanho_avl;
        int *valores_avl = gerarValores(x, ordem, &tamanho_avl);

        // Início do cálculo do tempo de execução da inserção
        clock_t inicioAVL = clock();

        for (int i = 0; i < tamanho_avl; i++) {
            inserirAVL(arvore_avl, valores_avl[i]);
        }

        // Fim do cálculo do tempo de execução da inserção
        clock_t fimAVL = clock();

        double tempoAVL = ((double)(fimAVL - inicioAVL)) / CLOCKS_PER_SEC;
        printf("Numero de rotacoes na Insercao: %d\n", arvore_avl->contador_rotacoes);
        rotaInser = arvore_avl->contador_rotacoes;
        printf("Tempo de execucao da Insercao: %.6f segundos\n", tempoAVL);

        // cálculo do tempo de busca de 1000 elementos
        clock_t BinicioAVL = clock();

        for (int i = 0; i < 1000; i++){
            buscarAVL(arvore_avl->raiz, valores_avl[i]);
        }
        
        clock_t BfimAVL = clock();

        double BtempoAVL = ((double)(BfimAVL - BinicioAVL)) / CLOCKS_PER_SEC;
        printf("Tempo de execucao da Busca de 1000 elementos: %.6f segundos\n", BtempoAVL);

        // Início do cálculo do tempo de execução da remoção
        clock_t RinicioAVL = clock();

        removerValoresAVL(arvore_avl, valores_avl, tamanho_avl);

        // Fim do cálculo do tempo de execução da remoção
        clock_t RfimAVL = clock();

        double RtempoAVL = ((double)(RfimAVL - RinicioAVL)) / CLOCKS_PER_SEC;

        printf("Numero de rotacoes na Remocao: %d\n", arvore_avl->contador_rotacoes - rotaInser);
        rotaInser = 0;
        printf("Tempo de execucao da Remocao: %.6f segundos\n", RtempoAVL);

        free(valores_avl); // Liberar memória alocada para os valores

        // Árvore Rubro-Negra

        printf("\nArvore Rubro Negra\n");
        ArvoreRN *arvore_rn = criar_arvoreRN();
        int tamanho_rn;
        int *valores_rn = gerarValores(x, ordem, &tamanho_rn);

        // Início do cálculo do tempo de execução da inserção na RN
        clock_t inicioRN = clock();

        for (int i = 0; i < tamanho_rn; i++) {
            inserirRN(arvore_rn, valores_rn[i]);
        }

        // Fim do cálculo do tempo de execução da inserção na RN
        clock_t fimRN = clock();

        double tempoRN = ((double)(fimRN - inicioRN)) / CLOCKS_PER_SEC;
        printf("Numero de rotacoes na Insercao: %d\n", arvore_rn->contador_rotacoes);
        rotaInser = arvore_rn->contador_rotacoes;
        printf("Tempo de execucao da Insercao: %.6f segundos\n", tempoRN);
        
        // cálculo do tempo de busca de 1000 elementos
        clock_t BinicioRN = clock();

        for (int i = 0; i < 1000; i++){
            buscarRN(arvore_rn->raiz, valores_rn[i]);
        }
        
        clock_t BfimRN = clock();

        double BtempoRN = ((double)(BfimRN - BinicioRN)) / CLOCKS_PER_SEC;
        printf("Tempo de execucao da Busca de 1000 elementos: %.6f segundos\n", BtempoRN);

        // Início do cálculo do tempo de execução da remoção na RN
        clock_t RinicioRN = clock();

        // Remover valores na árvore rubro-negra
        removerValoresRN(arvore_rn, valores_rn, tamanho_rn);

        // Fim do cálculo do tempo de execução da remoção na RN
        clock_t RfimRN = clock();

        double RtempoRN = ((double)(RfimRN - RinicioRN)) / CLOCKS_PER_SEC;

        printf("Numero de rotacoes na Remocao: %d\n", arvore_rn->contador_rotacoes - rotaInser);
        rotaInser = 0;
        printf("Tempo de execucao da Remocao: %.6f segundos\n", RtempoRN);

        free(valores_rn); // Liberar memória alocada para os valores
    }

}