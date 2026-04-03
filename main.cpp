#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>
#include <windows.h>
#include <time.h>
#include "TADProcesso.h"

void Moldura(int CI, int LI, int CF, int LF, int cortexto, int background){
    int i;

    textcolor(cortexto);
    textbackground(background);

    gotoxy(CI,LI); printf("%c",201);
    gotoxy(CF,LI); printf("%c",187);
    gotoxy(CI,LF); printf("%c",200);
    gotoxy(CF,LF); printf("%c",188);

    for(i=CI+1; i<CF; i++){
        gotoxy(i,LI); printf("%c",205);
        gotoxy(i,LF); printf("%c",205);
    }

    for(i=LI+1; i<LF; i++){
        gotoxy(CI,i); printf("%c",186);
        gotoxy(CF,i); printf("%c",186);
    }

    textcolor(7);
    textbackground(0);
}

void MolduraMenu(){
    clrscr();
    Moldura(1,1,119,30,3,0);
    Moldura(1,1,119,5,3,0);
}

void Titulo(){
    gotoxy(34,3);
    textcolor(8);
    printf("##### SIMULADOR - CICLO DE VIDA DE PROCESSOS #####");
}

void pausarTela(){
    gotoxy(45,29);
    textcolor(7);
    printf("Pressione qualquer tecla...");
    getch();
    //Sleep(2000);

}

void exibirFilas(Fila *pronto, Fila *espera, Processo *exec, int tempoGlobal)
{
    Processo *aux;
    int linha;

    MolduraMenu();
    Titulo();

    Moldura(3,6,38,14,8,0);
    gotoxy(5,7);
    textcolor(7);
    printf("TEMPO GLOBAL: ");
    textcolor(10);
    printf("%d", tempoGlobal);

    gotoxy(5,9);
    textcolor(11);
    printf("EXECUCAO:");
    if(exec != NULL){
        gotoxy(5,10);
        textcolor(7);
        printf("PID: %d", exec->pid);
        gotoxy(5,11);
        printf("CPU: %d/%d", exec->tempoExecutadoCPU, exec->tempoTotalCPU);
        gotoxy(5,12);
        printf("Estado: %s", nomeEstado(exec->estado));
    }else{
        gotoxy(5,10);
        textcolor(12);
        printf("Nenhum processo executando");
    }

    Moldura(40,6,78,22,14,0);
    gotoxy(42,7);
    textcolor(14);
    printf("FILA DE PRONTO");

    aux = pronto->ini;
    linha = 9;
    while(aux != NULL && linha < 21){
        gotoxy(42,linha);
        textcolor(7);
        printf("PID %-3d CPU %2d/%2d", aux->pid, aux->tempoExecutadoCPU, aux->tempoTotalCPU);
        aux = aux->prox;
        linha++;
    }

    if(pronto->ini == NULL){
        gotoxy(42,9);
        textcolor(12);
        printf("Fila vazia");
    }

    Moldura(80,6,116,22,12,0);
    gotoxy(82,7);
    textcolor(12);
    printf("FILA DE ESPERA");

    aux = espera->ini;
    linha = 9;
    while(aux != NULL && linha < 21){
        gotoxy(82,linha);
        textcolor(7);
        if(aux->esperandoFilho == 1)
            printf("PID %-3d WAIT FILHO", aux->pid);
        else
            printf("PID %-3d BLQ:%2d", aux->pid, aux->tempoRestanteBloqueio);

        aux = aux->prox;
        linha++;
    }

    if(espera->ini == NULL){
        gotoxy(82,9);
        textcolor(10);
        printf("Fila vazia");
    }
}

void telaEvento(char msg1[], char msg2[]){
    MolduraMenu();
    Titulo();

    Moldura(20,10,100,18,11,0);
    gotoxy(24,13);
    textcolor(15);
    printf("%s", msg1);

    gotoxy(24,15);
    textcolor(10);
    printf("%s", msg2);

    pausarTela();
}

void relatorioFinal(Processo *todos[], int total, int finalizados, int totalBloqueios, int somaTempoBloqueio, int totalRetornosPronto){
 
}

void principal(){
    Fila pronto, espera;
    Processo *todos[MAX_PROCESSOS];
    Processo *exec = NULL;
    Processo *filho = NULL;

    int totalProcessos = 0;
    int proxPID = 1;
    int qtdIniciais;
    int tempoGlobal = 0;
    int finalizados = 0;
    int totalBloqueios = 0;
    int somaTempoBloqueio = 0;
    int totalRetornosPronto = 0;

    int encerrar = 0;
    int restante, fatiaMax, tempoExecutar;
    int t;
    int criarFilho;
    int bloquearRecurso;
    int finalizouAgora;
    char msg1[120], msg2[120];

    inicializarFila(&pronto);
    inicializarFila(&espera);

    MolduraMenu();
    Titulo();
    Moldura(30,10,90,16,8,0);
    gotoxy(35,13);
    textcolor(15);
    printf("Digite a qtde de processos iniciais(1 < n < 10): ");
    textcolor(14);
    scanf("%d", &qtdIniciais);

    if(qtdIniciais < 1)
        qtdIniciais = 1;
    if(qtdIniciais > 10)
        qtdIniciais = 10;

    for(int i=0; i<qtdIniciais; i++){
        Processo *p = criarProcesso(proxPID++, -1, 5 + rand()%30);
        todos[totalProcessos] = p;
        totalProcessos++;
        enfileirar(&pronto, p);
    }

    while(encerrar == 0){
        if(finalizados == totalProcessos && filaVazia(&pronto) && filaVazia(&espera))
            encerrar = 1;

        if(encerrar == 0){
            if(filaVazia(&pronto)){
                tempoGlobal++;
                incrementarTempoBloqueados(&espera);
                desbloquearProcessos(&espera, &pronto);

                exibirFilas(&pronto, &espera, NULL, tempoGlobal);
                gotoxy(35,24);
                textcolor(12);
                printf("Nao ha processo pronto. Avancando tempo...");
                Sleep(700);
            }else{
                exec = desenfileirar(&pronto);
                exec->estado = EXECUTANDO;

                restante = exec->tempoTotalCPU - exec->tempoExecutadoCPU;
                if(restante < QUANTUM)
                    fatiaMax = restante;
                else
                    fatiaMax = QUANTUM;

                //tempoExecutar = 1 + rand()%fatiaMax;//pega um numero aleatorio de 1 a 10 ut pra cada processo, vai mais devagar e é legal
                tempoExecutar = fatiaMax;//executa de 10 ut cada processo, aqui vai mais rapido

                t = 0;
                while(t < tempoExecutar && exec->tempoExecutadoCPU < exec->tempoTotalCPU){
                    tempoGlobal++;
                    exec->tempoExecutadoCPU++;
                    exec->tempoTotalSistema++;

                    incrementarTempoFila(&pronto);
                    incrementarTempoBloqueados(&espera);
                    desbloquearProcessos(&espera, &pronto);

                    t++;
                }

                exibirFilas(&pronto, &espera, exec, tempoGlobal);
                gotoxy(30,24);
                textcolor(10);
                printf("PID %d executou %d unidade(s) de tempo.", exec->pid, tempoExecutar);
                Sleep(1000);

                finalizouAgora = 0;
                if(exec->tempoExecutadoCPU >= exec->tempoTotalCPU)
                    finalizouAgora = 1;

                if(finalizouAgora == 1){
                    exec->estado = FINALIZADO;
                    finalizados++;

                    sprintf(msg1, "Processo PID %d finalizado.", exec->pid);
                    sprintf(msg2, "Tempo total no sistema: %d", exec->tempoTotalSistema);
                    getch();
					telaEvento(msg1, msg2);

                    tentarDesbloquearPai(todos, totalProcessos, exec->pidPai, &espera, &pronto);
                }else{
                    criarFilho = 0;
                    bloquearRecurso = 0;

                    if(totalProcessos < MAX_PROCESSOS){
                        if(rand()%100 < 20)
                            criarFilho = 1;
                    }

                    if(criarFilho == 1){
                        filho = criarProcesso(proxPID++, exec->pid, 3 + rand()%20);
                        todos[totalProcessos] = filho;
                        totalProcessos++;

                        enfileirar(&pronto, filho);

                        exec->filhosCriados++;
                        exec->filhosAtivos++;
                        exec->waitsExecutados++;
                        exec->esperandoFilho = 1;
                        exec->bloqueios++;
                        exec->estado = ESPERANDO;

                        totalBloqueios++;

                        enfileirar(&espera, exec);

                        sprintf(msg1, "PID %d criou o filho PID %d.", exec->pid, filho->pid);
                        sprintf(msg2, "Pai executou WAIT e foi para ESPERA.");
                        getch();
						telaEvento(msg1, msg2);
                    }else{
                        if(rand()%100 < 30)
                            bloquearRecurso = 1;

                        if(bloquearRecurso == 1){
                            exec->estado = ESPERANDO;
                            exec->bloqueios++;
                            exec->tempoRestanteBloqueio = 1 + rand()%8;
                            totalBloqueios++;
                            somaTempoBloqueio += exec->tempoRestanteBloqueio;

                            enfileirar(&espera, exec);

                            sprintf(msg1, "PID %d bloqueado por recurso.", exec->pid);
                            sprintf(msg2, "Tempo de bloqueio: %d", exec->tempoRestanteBloqueio);
                            getch();
							telaEvento(msg1, msg2);
                        }else{
                            exec->estado = PRONTO;
                            exec->retornosPronto++;
                            totalRetornosPronto++;
                            enfileirar(&pronto, exec);
 
                            sprintf(msg1, "PID %d voltou para a fila de PRONTO.", exec->pid);
                            sprintf(msg2, "Nao terminou dentro da execucao atual.");
                            getch();
							telaEvento(msg1, msg2);
                        }
                    }
                }
            }
        }
    }

    relatorioFinal(todos, totalProcessos, finalizados, totalBloqueios, somaTempoBloqueio, totalRetornosPronto);

    for(int i=0; i<totalProcessos; i++)
        free(todos[i]);
}

void Menu(void){
    char op;
    	
	gotoxy(48,10);
	textcolor(15);
	printf("TRABALHO SO ");
	textcolor(8);
	printf("[BIMESTRE 1]");
	Moldura(1, 1, 119, 30, 3, 0);
	Moldura(30, 12, 90, 20, 8, 0);
	
	gotoxy(37,14);
	textcolor(7);
	printf("Guilherme Sales Pereira - ");
	textcolor(15);
	printf("RA: ");
	textcolor(9);
	printf("262411873");
	
	gotoxy(37,16);
	textcolor(7);
	printf("Igor Marcelino Santos - ");
	textcolor(15);
	printf("RA: ");
	textcolor(9);
	printf("262412985");
	
	gotoxy(37,18);
	textcolor(7);
	printf("Otavio Gomes De Souza - ");
	textcolor(15);
	printf("RA: ");
	textcolor(9);
	printf("262412608");
	
	getch();

    do{
        MolduraMenu();
        Titulo();

        Moldura(30,10,90,22,8,0);

        gotoxy(56,12);
        textcolor(15);
        printf("MENU");

        gotoxy(43,15);
        textcolor(10);
        printf("[ENTER] ");
        textcolor(7);
        printf("INICIAR SIMULACAO");

        gotoxy(43,17);
        textcolor(12);
        printf("[ESC] ");
        textcolor(7);
        printf("SAIR");

        gotoxy(43,20);
        textcolor(14);
        printf("Tecle a opcao: ");

        op = getch();

        if(op != 27){
            srand(time(NULL));
            principal();
        }

    }while(op != 27);
}

int main(){
    Menu();
    return 0;
}
