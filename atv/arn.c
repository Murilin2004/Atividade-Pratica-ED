#include <stdio.h>
#include <stdlib.h>
#include "arn.h"

#define VERMELHO 0
#define PRETO 1

// Funções de criação
No* criar_noRN(int dado) {
    No *novo_no = (No *)malloc(sizeof(No));
    novo_no->dado = dado;
    novo_no->cor = VERMELHO;
    novo_no->esquerda = novo_no->direita = novo_no->pai = NULL;
    return novo_no;
}

ArvoreRN* criar_arvoreRN() {
    ArvoreRN *arvore = (ArvoreRN *)malloc(sizeof(ArvoreRN));
    arvore->raiz = NULL;
    arvore->contador_rotacoes = 0;
    return arvore;
}

No* buscarRN(No* no, int dado) {
    while (no != NULL && dado != no->dado) {
        if (dado < no->dado) {
            no = no->esquerda;
        } else {
            no = no->direita;
        }
    }
    return no;
}

// Rotações
void rotacao_esquerdaRN(ArvoreRN *arvore, No *x) {
    No *y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != NULL) {
        y->esquerda->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == NULL) {
        arvore->raiz = y;
    } else if (x == x->pai->esquerda) {
        x->pai->esquerda = y;
    } else {
        x->pai->direita = y;
    }
    y->esquerda = x;
    x->pai = y;
    arvore->contador_rotacoes++;
}

void rotacao_direitaRN(ArvoreRN *arvore, No *y) {
    No *x = y->esquerda;
    y->esquerda = x->direita;
    if (x->direita != NULL) {
        x->direita->pai = y;
    }
    x->pai = y->pai;
    if (y->pai == NULL) {
        arvore->raiz = x;
    } else if (y == y->pai->esquerda) {
        y->pai->esquerda = x;
    } else {
        y->pai->direita = x;
    }
    x->direita = y;
    y->pai = x;
    arvore->contador_rotacoes++;
}

int cor(No* h) {
// Retorna a cor do nó; nós nulos são considerados PRETO
    return h == NULL ? PRETO : h->cor;
}

void troca_cor(No* h) {
// Troca a cor do nó e de seus filhos
    h->cor = !h->cor;
    if (h->esquerda != NULL) 
        h->esquerda->cor = !h->esquerda->cor;
    if (h->direita != NULL) 
        h->direita->cor = !h->direita->cor;
}

// Função de correção após inserção
void corrigir_insercao(ArvoreRN *arvore, No *z) {
    while (z->pai != NULL && z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esquerda) {
            No *tio = z->pai->pai->direita;
            if (tio != NULL && tio->cor == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->direita) {
                    z = z->pai;
                    rotacao_esquerdaRN(arvore, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacao_direitaRN(arvore, z->pai->pai);
            }
        } else {
            No *tio = z->pai->pai->esquerda;
            if (tio != NULL && tio->cor == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esquerda) {
                    z = z->pai;
                    rotacao_direitaRN(arvore, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacao_esquerdaRN(arvore, z->pai->pai);
            }
        }
    }
    arvore->raiz->cor = PRETO;
}

// Função de inserção
void inserirRN(ArvoreRN *arvore, int dado) {
    No *novo_no = criar_noRN(dado);
    No *y = NULL;
    No *x = arvore->raiz;

    while (x != NULL) {
        y = x;
        if (novo_no->dado < x->dado) {
            x = x->esquerda;
        } else {
            x = x->direita;
        }
    }

    novo_no->pai = y;
    if (y == NULL) {
        arvore->raiz = novo_no;
    } else if (novo_no->dado < y->dado) {
        y->esquerda = novo_no;
    } else {
        y->direita = novo_no;
    }

    corrigir_insercao(arvore, novo_no);
}

No* balancear(No* h, ArvoreRN *arvore) {
// Realiza o balanceamento da árvore rubro-negra após inserção ou remoção
    if (cor(h->direita) == VERMELHO) {
        rotacao_esquerdaRN(arvore, h);
    }
    if (cor(h->esquerda) == VERMELHO && cor(h->esquerda->esquerda) == VERMELHO) {
        rotacao_direitaRN(arvore, h);
    }
    if (cor(h->esquerda) == VERMELHO && cor(h->direita) == VERMELHO) {
        troca_cor(h);
    }
    return h;
}

void removerRN(ArvoreRN *arvore, int dado) {
    No *z = buscarRN(arvore->raiz, dado);
    if (z == NULL) {
        return; // O nó a ser removido não foi encontrado
    }
    
    No *y = z;
    No *x;
    int yCorOriginal = y->cor;

    if (z->esquerda == NULL) {
        x = z->direita;
        transplantarRN(arvore, z, z->direita);
    } else if (z->direita == NULL) {
        x = z->esquerda;
        transplantarRN(arvore, z, z->esquerda);
    } else {
        y = minRN(z->direita);
        yCorOriginal = y->cor;
        x = y->direita;
        if (y->pai == z) {
            if (x != NULL) {
                x->pai = y;
            }
        } else {
            transplantarRN(arvore, y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }
        transplantarRN(arvore, z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }
    if (yCorOriginal == PRETO) {
        removerFixUpRN(arvore, x);
    }
    free(z);
}

void removerFixUpRN(ArvoreRN *arvore, No *x) {
    // Enquanto x não for a raiz e x for NULL ou preto
    while (x != arvore->raiz && (x == NULL || x->cor == PRETO)) {
        if (x == NULL) {
            break; // Saída antecipada se x for nulo
        }

        if (x == x->pai->esquerda) {
            // x é filho esquerdo
            No *w = x->pai->direita; // w é o irmão de x
            
            // Verifica se w é nulo antes de acessar seus membros
            if (w != NULL && w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerdaRN(arvore, x->pai);
                w = x->pai->direita;
            }
            
            // Verifica novamente se w é nulo antes de acessar seus membros
            if (w == NULL || (w->esquerda == NULL || w->esquerda->cor == PRETO) &&
                (w->direita == NULL || w->direita->cor == PRETO)) {
                if (w != NULL) {
                    w->cor = VERMELHO;
                }
                x = x->pai; // Move para cima na árvore
            } else {
                // Verifica se w e w->direita são nulos antes de acessar w->direita
                if (w->direita == NULL || w->direita->cor == PRETO) {
                    if (w->esquerda != NULL) {
                        w->esquerda->cor = PRETO;
                    }
                    if (w != NULL) {
                        w->cor = VERMELHO;
                    }
                    rotacaoDireitaRN(arvore, w);
                    if (x->pai != NULL) {
                        w = x->pai->direita;
                    }
                }
                
                // Verifica se w é nulo antes de acessar seus membros
                if (w != NULL) {
                    w->cor = x->pai->cor;
                }
                if (x->pai != NULL) {
                    x->pai->cor = PRETO;
                }
                if (w != NULL && w->direita != NULL) {
                    w->direita->cor = PRETO;
                }
                rotacaoEsquerdaRN(arvore, x->pai);
                x = arvore->raiz; // Força a saída do loop
            }
        } else {
            // x é filho direito (simétrico ao caso anterior)
            No *w = x->pai->esquerda;
            
            if (w != NULL && w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireitaRN(arvore, x->pai);
                w = x->pai->esquerda;
            }
            
            if (w == NULL || (w->direita == NULL || w->direita->cor == PRETO) &&
                (w->esquerda == NULL || w->esquerda->cor == PRETO)) {
                if (w != NULL) {
                    w->cor = VERMELHO;
                }
                x = x->pai;
            } else {
                if (w->esquerda == NULL || w->esquerda->cor == PRETO) {
                    if (w->direita != NULL) {
                        w->direita->cor = PRETO;
                    }
                    if (w != NULL) {
                        w->cor = VERMELHO;
                    }
                    rotacaoEsquerdaRN(arvore, w);
                    if (x->pai != NULL) {
                        w = x->pai->esquerda;
                    }
                }
                
                if (w != NULL) {
                    w->cor = x->pai->cor;
                }
                if (x->pai != NULL) {
                    x->pai->cor = PRETO;
                }
                if (w != NULL && w->esquerda != NULL) {
                    w->esquerda->cor = PRETO;
                }
                rotacaoDireitaRN(arvore, x->pai);
                x = arvore->raiz;
            }
        }
    }
    
    // Garante que x seja preto quando sair do loop
    if (x != NULL) {
        x->cor = PRETO;
    }
}

void transplantarRN(ArvoreRN *arvore, No *u, No *v) {
    if (u->pai == NULL) {
        arvore->raiz = v;
    } else if (u == u->pai->esquerda) {
        u->pai->esquerda = v;
    } else {
        u->pai->direita = v;
    }
    if (v != NULL) {
        v->pai = u->pai;
    }
}

No* minRN(No* no) {
    while (no->esquerda != NULL) {
        no = no->esquerda;
    }
    return no;
}

void rotacaoEsquerdaRN(ArvoreRN *arvore, No *x) {
    No *y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != NULL) {
        y->esquerda->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == NULL) {
        arvore->raiz = y;
    } else if (x == x->pai->esquerda) {
        x->pai->esquerda = y;
    } else {
        x->pai->direita = y;
    }
    y->esquerda = x;
    x->pai = y;
    arvore->contador_rotacoes++;
}

void rotacaoDireitaRN(ArvoreRN *arvore, No *x) {
    No *y = x->esquerda;
    x->esquerda = y->direita;
    if (y->direita != NULL) {
        y->direita->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == NULL) {
        arvore->raiz = y;
    } else if (x == x->pai->direita) {
        x->pai->direita = y;
    } else {
        x->pai->esquerda = y;
    }
    y->direita = x;
    x->pai = y;
    arvore->contador_rotacoes++;
}