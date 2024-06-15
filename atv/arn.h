#ifndef ARN_H
#define ARN_H

typedef struct No {
    int dado;
    int cor;
    struct No *esquerda, *direita, *pai;
} No;

typedef struct ArvoreRN {
    No *raiz;
    int contador_rotacoes;
} ArvoreRN;

ArvoreRN* criar_arvoreRN();
No* criar_noRN(int dado);
void rotacao_esquerdaRN(ArvoreRN *arvore, No *x);
void rotacao_direitaRN(ArvoreRN *arvore, No *y);
void corrigir_insercao(ArvoreRN *arvore, No *z);
void inserirRN(ArvoreRN *arvore, int dado);
//int cor(No* h);
//void troca_cor(No* h);
No* balancear(No* h, ArvoreRN *arvore);
void removerRN(ArvoreRN *arvore, int dado);
No* buscarRN(No* no, int dado);
void removerFixUpRN(ArvoreRN *arvore, No *x);
void transplantarRN(ArvoreRN *arvore, No *u, No *v);
No* minRN(No* no);
void rotacaoEsquerdaRN(ArvoreRN *arvore, No *x);
void rotacaoDireitaRN(ArvoreRN *arvore, No *x);

#endif // ARN_H