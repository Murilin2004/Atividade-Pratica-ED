#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

NoAVL* criar_noAVL(int dado) {
    NoAVL *novo_no = (NoAVL *)malloc(sizeof(NoAVL));
    novo_no->dado = dado;
    novo_no->altura = 1;
    novo_no->esquerda = novo_no->direita = NULL;
    return novo_no;
}

ArvoreAVL* criar_arvoreAVL() {
    ArvoreAVL *arvore = (ArvoreAVL *)malloc(sizeof(ArvoreAVL));
    arvore->raiz = NULL;
    arvore->contador_rotacoes = 0;
    return arvore;
}

int altura(NoAVL *n) {
    if (n == NULL) {
        return 0;
    }
    return n->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

NoAVL* buscarAVL(NoAVL* raiz, int dado) {
    // Percorre a árvore até encontrar o nó com o dado desejado ou até chegar a um nó nulo
    while (raiz != NULL && dado != raiz->dado) {
        if (dado < raiz->dado) {
            raiz = raiz->esquerda;
        } else {
            raiz = raiz->direita;
        }
    }
    return raiz; // Retorna o nó encontrado ou NULL se não encontrado
}

NoAVL* rotacao_direitaAVL(NoAVL *y, ArvoreAVL *arvore) {
    NoAVL *x = y->esquerda;
    NoAVL *T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;

    arvore->contador_rotacoes++;
    return x;
}

NoAVL* rotacao_esquerdaAVL(NoAVL *x, ArvoreAVL *arvore) {
    NoAVL *y = x->direita;
    NoAVL *T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;

    arvore->contador_rotacoes++;
    return y;
}

int obter_balanceamento(NoAVL *n) {
    if (n == NULL) {
        return 0;
    }
    return altura(n->esquerda) - altura(n->direita);
}

NoAVL* inserir_no(NoAVL* no, int dado, ArvoreAVL *arvore) {
    if (no == NULL) {
        return criar_noAVL(dado);
    }

    if (dado < no->dado) {
        no->esquerda = inserir_no(no->esquerda, dado, arvore);
    } else if (dado > no->dado) {
        no->direita = inserir_no(no->direita, dado, arvore);
    } else {
        return no;
    }

    no->altura = 1 + max(altura(no->esquerda), altura(no->direita));

    int balance = obter_balanceamento(no);

    if (balance > 1 && dado < no->esquerda->dado) {
        return rotacao_direitaAVL(no, arvore);
    }

    if (balance < -1 && dado > no->direita->dado) {
        return rotacao_esquerdaAVL(no, arvore);
    }

    if (balance > 1 && dado > no->esquerda->dado) {
        no->esquerda = rotacao_esquerdaAVL(no->esquerda, arvore);
        return rotacao_direitaAVL(no, arvore);
    }

    if (balance < -1 && dado < no->direita->dado) {
        no->direita = rotacao_direitaAVL(no->direita, arvore);
        return rotacao_esquerdaAVL(no, arvore);
    }

    return no;
}

void inserirAVL(ArvoreAVL *arvore, int dado) {
    arvore->raiz = inserir_no(arvore->raiz, dado, arvore);
}

NoAVL* menorNoAVL(NoAVL* no) {
    NoAVL* atual = no;
    while (atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}

NoAVL* remover_no(NoAVL* raiz, int dado, ArvoreAVL *arvore) {
    if (raiz == NULL) {
        return raiz;
    }

    if (dado < raiz->dado) {
        raiz->esquerda = remover_no(raiz->esquerda, dado, arvore);
    } else if (dado > raiz->dado) {
        raiz->direita = remover_no(raiz->direita, dado, arvore);
    } else {
        // Nó com apenas um filho ou nenhum filho
        if ((raiz->esquerda == NULL) || (raiz->direita == NULL)) {
            NoAVL *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;

            // Caso sem filho
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else { // Caso de um filho
                *raiz = *temp;
            }
            free(temp);
        } else {
            // Nó com dois filhos: obter o menor nó à direita
            NoAVL *temp = menorNoAVL(raiz->direita);

            // Copiar os dados do nó mínimo para este nó
            raiz->dado = temp->dado;

            // Remover o nó mínimo
            raiz->direita = remover_no(raiz->direita, temp->dado, arvore);
        }
    }

    // Se a árvore tinha apenas um nó, então retorna
    if (raiz == NULL) {
        return raiz;
    }

    // Atualizar a altura deste nó
    raiz->altura = 1 + max(altura(raiz->esquerda), altura(raiz->direita));

    // Obter o fator de balanceamento deste nó
    int balance = obter_balanceamento(raiz);

    // Se o nó ficou desbalanceado, existem quatro casos possíveis

    // Caso Esquerda-Esquerda
    if (balance > 1 && obter_balanceamento(raiz->esquerda) >= 0) {
        return rotacao_direitaAVL(raiz, arvore);
    }

    // Caso Esquerda-Direita
    if (balance > 1 && obter_balanceamento(raiz->esquerda) < 0) {
        raiz->esquerda =  rotacao_esquerdaAVL(raiz->esquerda, arvore);
        return rotacao_direitaAVL(raiz, arvore);
    }

    // Caso Direita-Direita
    if (balance < -1 && obter_balanceamento(raiz->direita) <= 0) {
        return rotacao_esquerdaAVL(raiz, arvore);
    }

    // Caso Direita-Esquerda
    if (balance < -1 && obter_balanceamento(raiz->direita) > 0) {
        raiz->direita = rotacao_direitaAVL(raiz->direita, arvore);
        return rotacao_esquerdaAVL(raiz, arvore);
    }

    return raiz;
}

NoAVL* removerAVL(ArvoreAVL *arvore, int dado) {
    arvore->raiz = remover_no(arvore->raiz, dado, arvore);
}

void imprimirOrdemAVL(NoAVL *raiz) {
    if (raiz != NULL) {
        imprimirOrdemAVL(raiz->esquerda);
        printf("%d ", raiz->dado);
        imprimirOrdemAVL(raiz->direita);
    }
}
