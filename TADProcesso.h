#ifndef TADPROCESSO_H
#define TADPROCESSO_H

#define QUANTUM 10
#define MAX_PROCESSOS 200

#include <stdio.h>
#include <stdlib.h>

enum estado {
    PRONTO,
    EXECUTANDO,
    ESPERANDO,
    FINALIZADO
};typedef enum estado Estado;

struct processo{
    int pid;
    int pidPai;

    int tempoTotalCPU;
    int tempoExecutadoCPU;
    int tempoTotalSistema;

    int tempoBloqueadoTotal;
    int bloqueios;
    int retornosPronto;

    int filhosCriados;
    int waitsExecutados;
    int filhosAtivos;

    int esperandoFilho;
    int tempoRestanteBloqueio;

    Estado estado;
    struct processo *prox;
};typedef struct processo Processo;

struct fila{ 
    Processo *ini, *fim;
};typedef struct fila Fila;

//================inicializador

//fila
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
void enfileirar(Fila *f, Processo *p);
Processo* desenfileirar(Fila *f);
Processo* removerEspecifico(Fila *f, Processo *alvo);

// processos 
Processo* criarProcesso(int pid, int pidPai, int tempoCPU);
void incrementarTempoFila(Fila *f);
void incrementarTempoBloqueados(Fila *espera);
void desbloquearProcessos(Fila *espera, Fila *pronto);
void tentarDesbloquearPai(Processo *todos[], int total, int pidPai, Fila *espera, Fila *pronto);
const char* nomeEstado(Estado e);

void inicializarFila(Fila *f){
    f->ini = NULL;
    f->fim = NULL;
}

int filaVazia(Fila *f){
    return (f->ini == NULL);
}

void enfileirar(Fila *f, Processo *p){
    p->prox = NULL;
    if(f->fim == NULL){
        f->ini = p;
        f->fim = p;
    }else{
        f->fim->prox = p;
        f->fim = p;
    }
}

Processo* desenfileirar(Fila *f){
    Processo *aux = NULL;

    if(f->ini != NULL){
        aux = f->ini;
        f->ini = aux->prox;
        if(f->ini == NULL)
            f->fim = NULL;
        aux->prox = NULL;
    }
    return aux;
}

Processo* removerEspecifico(Fila *f, Processo *alvo){
    Processo *ant = NULL, *atual = f->ini, *ret = NULL;

    while(atual != NULL && atual != alvo){
        ant = atual;
        atual = atual->prox;
    }

    if(atual != NULL){
        ret = atual;

        if(ant == NULL)
            f->ini = atual->prox;
        else
            ant->prox = atual->prox;

        if(atual == f->fim)
            f->fim = ant;

        ret->prox = NULL;
    }

    return ret;
}

Processo* criarProcesso(int pid, int pidPai, int tempoCPU){
    Processo *p = (Processo*) malloc(sizeof(Processo));

    if(p != NULL){
        p->pid = pid;
        p->pidPai = pidPai;
        p->tempoTotalCPU = tempoCPU;
        p->tempoExecutadoCPU = 0;
        p->tempoTotalSistema = 0;
        p->tempoBloqueadoTotal = 0;
        p->bloqueios = 0;
        p->retornosPronto = 0;
        p->filhosCriados = 0;
        p->waitsExecutados = 0;
        p->filhosAtivos = 0;
        p->esperandoFilho = 0;
        p->tempoRestanteBloqueio = 0;
        p->estado = PRONTO;
        p->prox = NULL;
    }

    return p;
}

void incrementarTempoFila(Fila *f){
    Processo *aux = f->ini;
    while(aux != NULL){
        aux->tempoTotalSistema++;
        aux = aux->prox;
    }
}

void incrementarTempoBloqueados(Fila *espera){
    Processo *aux = espera->ini;
    while(aux != NULL){
        aux->tempoTotalSistema++;
        aux->tempoBloqueadoTotal++;

        if(aux->tempoRestanteBloqueio > 0)
            aux->tempoRestanteBloqueio--;

        aux = aux->prox;
    }
}

void desbloquearProcessos(Fila *espera, Fila *pronto){
    Processo *aux = espera->ini;
    Processo *proxAux;
    Processo *mov;

    while(aux != NULL){
        proxAux = aux->prox;

        if(aux->tempoRestanteBloqueio <= 0 && aux->esperandoFilho == 0){
            mov = removerEspecifico(espera, aux);
            if(mov != NULL){
                mov->estado = PRONTO;
                enfileirar(pronto, mov);
            }
        }

        aux = proxAux;
    }
}

void tentarDesbloquearPai(Processo *todos[], int total, int pidPai, Fila *espera, Fila *pronto){
    int i = 0;
    Processo *pai = NULL;
    Processo *mov = NULL;

    while(i < total && pai == NULL){
        if(todos[i]->pid == pidPai)
            pai = todos[i];
        i++;
    }

    if(pai != NULL){
        if(pai->filhosAtivos > 0)
            pai->filhosAtivos--;

        if(pai->filhosAtivos == 0 && pai->esperandoFilho == 1){
            pai->esperandoFilho = 0;
            pai->tempoRestanteBloqueio = 0;

            mov = removerEspecifico(espera, pai);
            if(mov != NULL){
                mov->estado = PRONTO;
                enfileirar(pronto, mov);
            }
        }
    }
}

const char* nomeEstado(Estado e){
    if(e == PRONTO) 
		return "PRONTO";
    if(e == EXECUTANDO) 
		return "EXECUTANDO";
    if(e == ESPERANDO) 
		return "ESPERANDO";
		
    return "FINALIZADO";
}

#endif
