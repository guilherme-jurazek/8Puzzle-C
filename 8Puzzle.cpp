#include<conio.h>
#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<ctype.h>
#include<string.h>
#include<conio2.h>
#include <time.h>
#include "fila.h"
#include <sys\timeb.h> 
//Alunos: Mateus Bonassa Ederli - Guilherme Jurazek Guedes - Gustavo da Rosa Ferruci




// === ESTRUTURAS === //

struct No
{
	int fscore, gscore, hscore, qtdFilho;
	char matriz[10];
	struct No* filhos[4];
	struct No* noPai;
};
typedef struct No No;

struct Fila
{
	struct NoFila *cabeca, *last;
};
typedef struct Fila Fila;


struct NoFila
{
	No* no, *noPai;
	struct NoFila *prox;
};
typedef struct NoFila NoFila;



// === FUNÃ‡Ã•ES === //
void Quadro(int CI, int LI, int CF, int LF, int CorT, int CorF);
bool verificar(char estado_final[20]);
int qtd_errado(char novo[10], char final[10]);
void printBoard(char matriz[20], int x, int y);
void embaralhar(char novo[20]);
int get_index(char* string, char c);
int gerarFilhos(No* no);
char * criarFilho(int pos_zero,int pos_mov,char atual[10]);
void criarNo(TpElemento pai,TpFila &fila,char final[10],Fila lista,int &qtd_no_total);
void buscaCega(char ini[10], char final[10]);
char* possiveisMov(int pos);
void mostrar_historico(TpElemento F);
	

// === FILA === //



void enqueue(Fila* fila, No* no){
	NoFila* nofila = (NoFila*)malloc(sizeof(NoFila));	
	nofila -> prox = NULL;
	nofila -> no = no;
	
	if(fila->cabeca == NULL){
		fila->cabeca = nofila;
		fila->last = nofila;
	}
	else{
		NoFila* noAux = fila -> cabeca;
		while(noAux -> prox != NULL){
			noAux = noAux ->prox;
		}	
		noAux -> prox = nofila;
		fila->last = nofila;
	}
}

bool isempty(Fila* fila){
	return fila->cabeca == NULL;
}

No* dequeue(Fila* fila){
	NoFila* noAux = fila->cabeca;
	NoFila* noAnterior;
	if(noAux != NULL){
		if(noAux->prox!=NULL)
			fila->cabeca = noAux -> prox;
		else fila->cabeca = NULL;
		return noAux->no;
	}
	return NULL;
}

bool possui(Fila* fila, char matriz[10]){
	if(fila->cabeca==NULL)
		return 0;
	else
	{
		NoFila* noAux = fila->cabeca;
	while(noAux -> prox != NULL)
		if(strcmp(matriz, noAux->no->matriz) == 0)
			return 1;
		else
			noAux = noAux ->prox;

	if(strcmp(matriz, noAux->no->matriz) == 0)
			return 1;
		else
			return 0;
	}
	
}
void exibirPassos(Fila *fila)
{
	NoFila* noAux = fila->cabeca;
	while(noAux!=NULL)
	{
		printf("%s \n",noAux->no->matriz);
		noAux = noAux->prox;
	}
}

int tamanhoFila(Fila* fila){
    int tam = 0;
    NoFila* noAux = fila->cabeca;
    while(noAux != NULL){
        tam++;
        noAux = noAux->prox;
    }
    return tam;
}

void removerFilhos(Fila* filaOrdenada, No* no) {
    NoFila* noAux = filaOrdenada->cabeca;
    NoFila* noAnterior = NULL;
    while (noAux != NULL) {
        int remove = 0;
        for (int i = 0; i < no->qtdFilho; i++) {
            if (noAux->no == no->filhos[i]) {
                remove = 1;
                break;
            }
        }
        if (remove) {
            if (noAnterior != NULL) {
                noAnterior->prox = noAux->prox;
                noAux = noAnterior->prox;
            } else {
                filaOrdenada->cabeca = noAux->prox;
                noAux = filaOrdenada->cabeca;
            }
        } else {
            noAnterior = noAux;
            noAux = noAux->prox;
        }
    }
}


void enqueueOrdenado(Fila* fila, No* no, No* noPai){
	NoFila* nofila = (NoFila*)malloc(sizeof(NoFila));	
	nofila -> prox = NULL;
	nofila -> no = no;
	no -> noPai = noPai;
	if(fila->cabeca == NULL){
		fila->cabeca = nofila;
	}
	else{
		NoFila* noAux = fila -> cabeca;
		NoFila* noAnt = noAux;
		while(noAux -> prox != NULL && noAux->no->hscore < no->hscore){
			noAnt = noAux;
			noAux = noAux ->prox;
		}	
		if(noAnt == noAux){
			if(noAux->no->fscore > no->hscore){
				fila->cabeca = nofila;
				nofila -> prox = noAux;
			}
			else{
				nofila -> prox = noAux ->prox;
				noAux -> prox = nofila;
			}
				
		}else{
			if(noAux -> prox != NULL){
				noAnt -> prox = nofila;
				nofila -> prox = noAux;
			}else
				noAux -> prox = nofila;
		}
	}
}

void removeNo(Fila* fila, No* no) {
    NoFila* noAtual = fila->cabeca;
    NoFila* noAnterior = NULL;

    while (noAtual != NULL && noAtual->no != no) {
        noAnterior = noAtual;
        noAtual = noAtual->prox;
    }

    if (noAtual == NULL) {
        printf("NÃ³ nÃ£o encontrado na fila.\n");
        return;
    }

    if (noAnterior == NULL) {
        fila->cabeca = noAtual->prox;
    } else {
        noAnterior->prox = noAtual->prox;
    }
}



NoFila* getLast(Fila* fila){
	NoFila* noAux = fila->cabeca;
    NoFila* noAnterior = NULL;
    if(noAux != NULL){
        while(noAux->prox != NULL){
            noAnterior = noAux;
            noAux = noAux->prox;
        }
        if(noAnterior != NULL){
            noAnterior->prox = NULL;
        } else {
            fila->cabeca = NULL;
        }
    }
    
	return noAux;
}

No* pegarNoCerto(Fila* fila, Fila* filaOrdenada){
	int achou = 0, qtdFilho, i, posMelhor, nivel, aux_heuristico, pos;
	No* noMelhor = dequeue(filaOrdenada);
	
	if(possui(fila, noMelhor->noPai->matriz)){
		NoFila* noFila = getLast(fila);
		//printf("\nFILA ORDENADA: %s, %d\n", noMelhor -> matriz, noMelhor -> fscore);
		while(achou == 0){
			//printf("\nFILA: %s %d\n", noFila->no->matriz, noFila->no->fscore);
			for(i = 0; i<noFila->no-> qtdFilho; i++){
				//printf("\nFILA NÃƒO ORDENADA: %s %d\n", noFila->no->filhos[i]->matriz,noFila->no->filhos[i]->fscore);
				if(noFila->no->filhos[i] != NULL && strcmp(noFila->no->filhos[i]->matriz, noMelhor->matriz) == 0){
					achou = 1;
					posMelhor = i;
				}
			}
			if(achou != 1){
				removerFilhos(filaOrdenada, noFila->no);
				noFila = getLast(fila);
			}
		}
		enqueue(fila, noFila->no);
		enqueue(fila, noFila->no->filhos[posMelhor]);
		if(noFila->no->filhos[posMelhor] != NULL)
			return noFila->no->filhos[posMelhor];
	}
	else{
		No* no = dequeue(fila);
		enqueue(fila, no);
		while(strcmp(no->matriz, noMelhor->matriz) != 0){
			nivel =  tamanhoFila(fila);
			aux_heuristico = -1;
			qtdFilho = gerarFilhos(no);
			no->qtdFilho = qtdFilho;
			for(i = 0 ; i< qtdFilho; i++){
				no->filhos[i]->hscore = qtd_errado(no->filhos[i]->matriz, noMelhor->matriz);
				no->filhos[i]->gscore = nivel;
				no->filhos[i]->fscore = no->filhos[i]->hscore + no->filhos[i]->gscore;
				if(!possui(fila, no->filhos[i]->matriz)){
					if(aux_heuristico == -1 || aux_heuristico > no->filhos[i]->fscore)
					{
						aux_heuristico =  no->filhos[i]->fscore;
						pos = i;
					}
				}
			}
			no = no->filhos[i];
			enqueue(fila, no);
		}
		return no;
	}
}




// === QUADRO === //
void Quadro(int CI, int LI, int CF, int LF, int CorT, int CorF)
{
	int i;
	textcolor(CorT);
	textbackground(CorF);
	gotoxy(CI,LI);
	printf("%c",201);
	gotoxy(CF,LI);
	printf("%c",187);
	gotoxy(CI,LF);
	printf("%c",200);
	gotoxy(CF,LF);
	printf("%c",188);
	for(i=CI+1; i<CF; i++)
	{
		gotoxy(i,LI);
		printf("%c",205);
		gotoxy(i,LF);
		printf("%c",205);
	}
	for(i=LI+1; i<LF; i++)
	{
		gotoxy(CI,i);
		printf("%c",186);
		gotoxy(CF,i);
		printf("%c",186);
	}
}


// === VERIFICAÃ‡ÃƒO === //
bool verificar(char estado_final[20]){
	int tamanho = 0, tamanho_mutiplicado = 1;
	for(int i = 0; i<10; i++)
		if(estado_final[i] > 48 &&  estado_final[i] <= 57){
			tamanho += (int)estado_final[i] - 48;
			tamanho_mutiplicado *= (int)estado_final[i] - 48;
		}
	return !(tamanho == 36 && strlen(estado_final) == 9 && tamanho_mutiplicado == 40320);
}


// === FUNÃ‡Ã•ES AUXILIARES === //
int qtd_errado(char novo[10], char final[10]){
	int i, qtd_errado = 0;
	for(i = 0; i<10; i++){
		if(novo[i] != final[i] && novo[i] != 48)
			qtd_errado++;
	}
	return qtd_errado;
}


void printBoard(char matriz[20], int x, int y) {
	int i, aux;
	gotoxy(x,y);
    printf("+---+---+---+\n");
    y++;
    aux = x;
    for(i = 0 ; i < 9; i++){
    	gotoxy(aux,y);
    	printf("| %c ", matriz[i]);
    	aux+=4;
    	if((i+1)%3 == 0){
    		gotoxy(aux,y++);
    		printf("|\n");
    		aux = x;
    		gotoxy(aux,y++);
    		printf("+---+---+---+\n");
		}	
	}
    
}

int get_index(char* string, char c) {
    char *e = strchr(string, c);
    if (e == NULL) {
        return -1;
    }
    return (int)(e - string);
}


char* trocarPos(char matriz[10], int a, int b){
	char aux; 
	char aux_matriz[10];	
	//printf("%s\n", matriz);
	strcpy(aux_matriz, matriz);
	aux = aux_matriz[a];
	aux_matriz[a] = aux_matriz[b];
	aux_matriz[b] = aux;
	return aux_matriz;
}


void mostrar_resultado(Fila* fila){
	int i = 0, x = 10, y = 5;
	char auxMatriz[10];
	while(!isempty(fila)){
		strcpy(auxMatriz, dequeue(fila)->matriz);
		printBoard(auxMatriz, x, y);
		x += 20;
		i ++;
		if( i%3 ==0)
		{
			x = 10;
			y += 10;
		}
	}
}

// === SHUFFLE SORT === //
void embaralhar(char novo[20]){
	int i, qtd_filho;
	srand (time(NULL));
	No* no = (No*)malloc(sizeof(No));
	strcpy(no->matriz, novo);
	for(i = 0; i<30 ; i++)
	{
		qtd_filho = gerarFilhos(no);
		no = no->filhos[rand() % (qtd_filho)];
	}
	strcpy(novo, no->matriz);
	
}


// === A* Sort === //
void AEstrela(char novo[10],char final[10]){
	// double time_spent = 0.0;
	struct timeb start, end;
    int diff;
	//double time_spent = 0.0;
	ftime(&start);
    //clock_t begin = clock();
	No* no = (No*)malloc(sizeof(No));
	No* noAnterior;
	Fila* fila = (Fila*)malloc(sizeof(Fila));
	fila->cabeca = NULL;
	Fila* filaTodosNos = (Fila*)malloc(sizeof(Fila));
	filaTodosNos->cabeca = NULL;
	strcpy(no->matriz, novo);
	enqueue(fila, no);
	int i, qtd_no_total = 1, aux_heuristico, qtd_filho, pos_heuristico, nivel = 0, passou;
	while(strcmp(no->matriz, final) != 0){
		nivel =  tamanhoFila(fila);
		aux_heuristico = -1;
		qtd_filho = gerarFilhos(no);
		no->qtdFilho = qtd_filho;
		passou = 0;
		for(i = 0 ; i< qtd_filho; i++){
			no->filhos[i]->hscore = qtd_errado(no->filhos[i]->matriz, final);
			no->filhos[i]->gscore = nivel;
			no->filhos[i]->fscore = no->filhos[i]->hscore + no->filhos[i]->gscore;
			//printf("%d -> %s \n",nivel, no->filhos[i]->matriz);
			if(!possui(fila, no->filhos[i]->matriz)){
				//printf("%s ",no->filhos[i]->matriz);
				passou = 1;
				enqueueOrdenado(filaTodosNos, no->filhos[i], no);
				//printf("  %d   \n", no->filhos[i]->hscore);	
				if(aux_heuristico == -1 || aux_heuristico > no->filhos[i]->fscore){
					aux_heuristico = no->filhos[i]->fscore;
					pos_heuristico = i;
				}
				
			}
		}
//		printf("\n");
		if(passou == 1){
			nivel++;
			no = no->filhos[pos_heuristico];
			removeNo(filaTodosNos, no);
			enqueue(fila, no);
			qtd_no_total++;
		}
		else{
			no = pegarNoCerto(fila, filaTodosNos);
			qtd_no_total++;
		}
	}

	//clock_t end = clock();
	ftime(&end);
    diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
	system("cls");
	int sol = tamanhoFila(fila) -1, nv = tamanhoFila(filaTodosNos) + sol;
	printf("Tempo de execucao: %u milissegundos\n",diff);
	printf("NOS visitados: %d\n", nv);
	printf("Passos para solucao: %d", sol);
	mostrar_resultado(fila);
	getche();
}

int gerarFilhos(No* no){
	int posZero = get_index(no->matriz, 48), qtd_filhos;
	char matrizAux[10]; 
	switch(posZero)
	{
		case 0: {
			//2 casos --> 1Â°: 1->0 || 2Â°: 3->0
			strcpy(matrizAux,trocarPos(no->matriz, 0, 1));
			No* no1 = (No*)malloc(sizeof(No));
			strcpy(no1->matriz, matrizAux);  
			no->filhos[0] = no1;
			
			strcpy(matrizAux,trocarPos(no->matriz, 0, 3));
			No* no2 = (No*)malloc(sizeof(No));
			strcpy(no2->matriz, matrizAux);  
			no->filhos[1] = no2;	
			return 2;
			}
		case 1: {
			//3 casos --> 1Â°: 0->1 || 2Â°: 2->1 || 3Â°: 4->1
			strcpy(matrizAux,trocarPos(no->matriz, 0, 1));
			No* no1 = (No*)malloc(sizeof(No));
			strcpy(no1->matriz, matrizAux);  
			no->filhos[0] = no1;
			
			strcpy(matrizAux,trocarPos(no->matriz, 2, 1));
			No* no2 = (No*)malloc(sizeof(No));
			strcpy(no2->matriz, matrizAux);  
			no->filhos[1] = no2;
			
			strcpy(matrizAux,trocarPos(no->matriz, 4, 1));
			No* no3 = (No*)malloc(sizeof(No));
			strcpy(no3->matriz, matrizAux);  
			no->filhos[2] = no3;
			
			return 3;
		}
		case 2: {
			//2 casos --> 1Â°: 1->2 || 2Â°: 5->2
			strcpy(matrizAux,trocarPos(no->matriz, 1, 2));
			No* no1 = (No*)malloc(sizeof(No));
			strcpy(no1->matriz, matrizAux);  
			no->filhos[0] = no1;
			
			strcpy(matrizAux,trocarPos(no->matriz, 5, 2));
			No* no2 = (No*)malloc(sizeof(No));
			strcpy(no2->matriz, matrizAux);  
			no->filhos[1] = no2;
			return 2;
		}
		case 3: {
			//3 casos --> 1Â°: 0->3 || 2Â°: 4->3 || 3Â°: 6->3
			strcpy(matrizAux,trocarPos(no->matriz, 0, 3));
			No* no1 = (No*)malloc(sizeof(No));
			strcpy(no1->matriz, matrizAux);  
			no->filhos[0] = no1;
			
			strcpy(matrizAux,trocarPos(no->matriz, 4, 3));
			No* no2 = (No*)malloc(sizeof(No));
			strcpy(no2->matriz, matrizAux);  
			no->filhos[1] = no2;
			
			strcpy(matrizAux,trocarPos(no->matriz, 6, 3));
			No* no3 = (No*)malloc(sizeof(No));
			strcpy(no3->matriz, matrizAux);  
			no->filhos[2] = no3;
			
			return 3;
		}
		case 4: {
			//4 casos --> 1Â°: 1->4 || 2Â°: 3->4 || 3Â°: 5->4 || 4Â°: 7->4
			strcpy(matrizAux,trocarPos(no->matriz, 1, 4));
			No* no1 = (No*)malloc(sizeof(No));
			strcpy(no1->matriz, matrizAux);  
			no->filhos[0] = no1;
			
			strcpy(matrizAux,trocarPos(no->matriz, 3, 4));
			No* no2 = (No*)malloc(sizeof(No));
			strcpy(no2->matriz, matrizAux);  
			no->filhos[1] = no2;
			
			strcpy(matrizAux,trocarPos(no->matriz, 5, 4));
			No* no3 = (No*)malloc(sizeof(No));
			strcpy(no3->matriz, matrizAux);  
			no->filhos[2] = no3;
			
			strcpy(matrizAux,trocarPos(no->matriz, 7, 4));
			No* no4 = (No*)malloc(sizeof(No));
			strcpy(no4->matriz, matrizAux);  
			no->filhos[3] = no4;
			return 4;
		}
		case 5: {
			//3 casos --> 1Â°: 2->5 || 2Â°: 4->5 || 3Â°: 8->5
			strcpy(matrizAux,trocarPos(no->matriz, 2, 5));
			No* no1 = (No*)malloc(sizeof(No));
			strcpy(no1->matriz, matrizAux);  
			no->filhos[0] = no1;
			
			strcpy(matrizAux,trocarPos(no->matriz, 4, 5));
			No* no2 = (No*)malloc(sizeof(No));
			strcpy(no2->matriz, matrizAux);  
			no->filhos[1] = no2;
			
			strcpy(matrizAux,trocarPos(no->matriz, 8, 5));
			No* no3 = (No*)malloc(sizeof(No));
			strcpy(no3->matriz, matrizAux);  
			no->filhos[2] = no3;
			return 3;
		}
		case 6: {
			//2 casos --> 1Â°: 3->6 || 2Â°: 7->6
			strcpy(matrizAux,trocarPos(no->matriz, 3, 6));
			No* no1 = (No*)malloc(sizeof(No));
			strcpy(no1->matriz, matrizAux);  
			no->filhos[0] = no1;
			
			strcpy(matrizAux,trocarPos(no->matriz, 7, 6));
			No* no2 = (No*)malloc(sizeof(No));
			strcpy(no2->matriz, matrizAux);  
			no->filhos[1] = no2;					
			return 2;
		}
		case 7: {
			//3 casos --> 1Â°: 4->7 || 2Â°: 6->7 || 3Â°: 8->7
			strcpy(matrizAux,trocarPos(no->matriz, 4, 7));
			No* no1 = (No*)malloc(sizeof(No));
			strcpy(no1->matriz, matrizAux);  
			no->filhos[0] = no1;
			
			strcpy(matrizAux,trocarPos(no->matriz, 6, 7));
			No* no2 = (No*)malloc(sizeof(No));
			strcpy(no2->matriz, matrizAux);  
			no->filhos[1] = no2;
			
			strcpy(matrizAux,trocarPos(no->matriz, 8, 7));
			No* no3 = (No*)malloc(sizeof(No));
			strcpy(no3->matriz, matrizAux);  
			no->filhos[2] = no3;
			return 3;
		}
		case 8: {
			//2 casos --> 1Â°: 7->8 || 2Â°: 5->8
			strcpy(matrizAux,trocarPos(no->matriz, 7, 8));
			No* no1 = (No*)malloc(sizeof(No));
			strcpy(no1->matriz, matrizAux);  
			no->filhos[0] = no1;
			
			strcpy(matrizAux,trocarPos(no->matriz, 5, 8));
			No* no2 = (No*)malloc(sizeof(No));
			strcpy(no2->matriz, matrizAux);  
			no->filhos[1] = no2;	
			return 2;
		}
	}
	
}



// === Busca Cega === //

char * criarFilho(int pos_zero,int pos_mov,char atual[10])
{
	char novo[10],aux,filho[10];
	aux = atual[pos_zero];
	strcpy(filho,atual);
	filho[pos_zero] = filho[pos_mov];
	filho[pos_mov] = aux;
	return filho;
}

char* possiveisMov(int pos)
{
	char p[2]="";
	switch(pos)
	{
		case 0 :strcpy(p,strcat(p,"13"));
				break;
		case 1: strcpy(p,strcat(p,"042"));
				 break;
		case 2: strcpy(p,strcat(p,"15"));
				 break;
		case 3 : strcpy(p,strcat(p,"046"));
				 break;
		case 4: strcpy(p,strcat(p,"3157"));
				 break;
		case 5: strcpy(p,strcat(p,"842"));
				 break;
		case 6: strcpy(p,strcat(p,"37"));
				 break;
		case 7: strcpy(p,strcat(p,"648"));
				 break;
		case 8: strcpy(p,strcat(p,"75"));
				 break;
	}

}
void mostrar_historico(TpElemento F)
{
	int i = 0, x = 10, y = 5,j=0;
	for(int i=0;i<=F.hist_tam;i++)
		{
			printBoard(F.historico[i],x,y);
				x += 20;
		j ++;
		if( j%3 ==0)
		{
			x = 10;
			y += 10;
		}
		}
}
void criarNo(TpElemento pai,TpFila &fila,char final[10],Fila lista,int &qtd_no_total)
{
	char filho[10];
	TpElemento elem;
	int pos = get_index(pai.Elemento,'0');
	char movs[4];
	strcpy(movs,possiveisMov(pos));
	No* no = (No*)malloc(sizeof(No));	

	int qtd = strlen(movs);
	for(int i=0;i<qtd;i++){
		strcpy(filho,criarFilho(pos,movs[i]-'0',pai.Elemento));
		if(!possui(&lista,filho)){
			strcpy(elem.Elemento,filho);
			elem.hist_tam = pai.hist_tam+1;
			for(int i=0;i<=pai.hist_tam;i++)
			{
				elem.historico[i] = (char*)malloc(10);
				strcpy(elem.historico[i],pai.historico[i]);
			}
			elem.historico[elem.hist_tam] = (char*)malloc(10);
			strcpy(elem.historico[elem.hist_tam],filho);
			inserir(fila,elem);
			no = (No*)malloc(sizeof(No));
			strcpy(no->matriz,filho);
			enqueue(&lista,no);
			qtd_no_total++;
		}

	}


	
}
void buscaCega(char ini[10], char final[10])
{
	struct timeb start, end;
    int diff;
	ftime(&start);
	TpFila fila;
	Fila* lista=(Fila*)malloc(sizeof(Fila));
	lista->cabeca = NULL;
	No* nolista = (No*)malloc(sizeof(No));
	strcpy(nolista->matriz,ini);
	enqueue(lista,nolista);
	TpElemento no;
     int tam =0,qtd_no_total = 1; 
	strcpy(no.Elemento,ini);
	no.hist_tam =0;
	no.historico[0] = (char*)malloc(10);
	strcpy(no.historico[0],ini);
	inicializar(fila);
	criarNo(no,fila,final,*lista,qtd_no_total);
	int dif,achou=0;
	while(achou==0 && !vazia(fila.inicio,fila.fim))
	{
		no = retirar(fila);
	
		dif = qtd_errado(no.Elemento,final);		
		if(dif==0)
			achou=1;
		else
			criarNo(no,fila,final,*lista,qtd_no_total);
	}
	system("cls");
	ftime(&end);
    diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
	printf("Tempo de execucao: %u milissegundos\n",diff);
	printf("NOS visitados: %d\n", qtd_no_total);
	printf("Passos para solucao: %d\n", no.hist_tam);
	printf("--------------------------------------------------------\n");
	mostrar_historico(no);
}
// === MAIN === //
int main(){
	int resp;
	char algoritmo, estado_final[10], novo[10];
	system("cls");
	Quadro(1,1,80,17,0,15);
	gotoxy(10,6);
	printf("Algoritmo a Utilizar (A-Algoritmo A* | B-Algoritmo CEGO)?");
	algoritmo = getche();
	do{
	gotoxy(10,8);
	printf("Definir estado final:           ");
	gotoxy(32,8);
	gets(estado_final);
	}while(verificar(estado_final));
	gotoxy(10,10);
	printf("Embaralhar (Enter)");
	strcpy(novo, estado_final);
	embaralhar(novo);
	textcolor(15);
	textbackground(0);
	system("cls");
	printBoard(novo, 5,2);
	gotoxy(5,10);
	printf("Resolver (Enter)");
	getche();
	if(algoritmo=='A' || algoritmo=='a')
		AEstrela(novo, estado_final);
	else if(algoritmo=='B' || algoritmo=='b')
		buscaCega(novo,estado_final);
	
	fflush(stdin);

	
}
