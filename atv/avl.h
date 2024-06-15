#ifndef AVL_H
#define AVL_H

typedef struct NoAVL {
    int dado;
    int altura;
    struct NoAVL *esquerda, *direita;
} NoAVL;

typedef struct {
    NoAVL *raiz;
    int contador_rotacoes;
} ArvoreAVL;

ArvoreAVL* criar_arvoreAVL();
NoAVL* criar_noAVL(int dado);
int altura(NoAVL *n);
int max(int a, int b);
NoAVL* rotacao_direitaAVL(NoAVL *y, ArvoreAVL *arvore);
NoAVL* rotacao_esquerdaAVL(NoAVL *x, ArvoreAVL *arvore);
int obter_balanceamento(NoAVL *n);
NoAVL* inserir_no(NoAVL* no, int dado, ArvoreAVL *arvore);
void inserirAVL(ArvoreAVL *arvore, int dado);
void imprimirOrdemAVL(NoAVL *raiz);
NoAVL* remover_no(NoAVL* raiz, int dado, ArvoreAVL *arvore);
NoAVL* removerAVL(ArvoreAVL *arvore, int dado);
NoAVL* menorNoAVL(NoAVL* no);
NoAVL* buscarAVL(NoAVL* raiz, int dado);

#endif // AVL_H
