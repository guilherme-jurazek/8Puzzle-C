
#include<conio.h>
#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<ctype.h>
#include<string.h>
#include<conio2.h>
#include <time.h>
#include "fila.h"

// === ESTRUTURAS === //

struct No
{
	int fscore, gscore, hscore;
	char matriz[10];
	struct No* filhos[4];
};
typedef struct No No;



	

// === FILA === //


struct Fila
{
	struct NoFila *cabeca;
};
typedef struct Fila Fila;


struct NoFila
{
	char matriz[10];
	struct NoFila *prox;
};
typedef struct NoFila NoFila;

void enqueue(Fila* fila, char matriz[10]){
	NoFila* nofila = (NoFila*)malloc(sizeof(NoFila));	
	nofila -> prox = NULL;
	strcpy(nofila->matriz, matriz);
	
	if(fila->cabeca == NULL){
		fila->cabeca = nofila;
	}
	else{
		NoFila* noAux = fila -> cabeca;
		while(noAux -> prox != NULL){
			noAux = noAux ->prox;
		}	
		noAux -> prox = nofila;
	}
}

bool isempty(Fila* fila){
	return fila->cabeca == NULL;
}

char* dequeue(Fila* fila){
	NoFila* noAux = fila->cabeca;
	NoFila* noAnterior;
	if(noAux != NULL){
		if(noAux->prox!=NULL)
			fila->cabeca = noAux -> prox;
		else fila->cabeca = NULL;
		return noAux->matriz;
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
		if(strcmp(matriz, noAux->matriz) == 0)
			return 1;
		else
			noAux = noAux ->prox;

	if(strcmp(matriz, noAux->matriz) == 0)
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
		printf("%s \n",noAux->matriz);
		noAux = noAux->prox;
	}
}
// === FUNÇÕES === //
void Quadro(int CI, int LI, int CF, int LF, int CorT, int CorF);
bool verificar(char estado_final[20]);
int qtd_errado(char novo[10], char final[10]);
void printBoard(char matriz[20], int x, int y);
void embaralhar(char novo[20]);
int get_index(char* string, char c);
int gerarFilhos(No* no);
char * criarFilho(int pos_zero,int pos_mov,char atual[10]);
void criarNo(TpElemento pai,TpFila &fila,char final[10],Fila lista,int tam);
void buscaCega(char ini[10], char final[10]);
char* possiveisMov(int pos);
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


// === VERIFICAÇÃO === //
bool verificar(char estado_final[20]){
	int tamanho = 0, tamanho_mutiplicado = 1;
	for(int i = 0; i<10; i++)
		if(estado_final[i] > 48 &&  estado_final[i] <= 57){
			tamanho += (int)estado_final[i] - 48;
			tamanho_mutiplicado *= (int)estado_final[i] - 48;
		}
	return !(tamanho == 36 && strlen(estado_final) == 9 && tamanho_mutiplicado == 40320);
}


// === FUNÇÕES AUXILIARES === //
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
	char aux, aux_matriz[10];	
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
		strcpy(auxMatriz, dequeue(fila));
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
	int i, j, aux;
	char letra;
	srand ( time(NULL) );
	for(i = 0 ; i < 100 ; i++){
		aux = rand() % 10;
		if(aux < 9)
			for(j = aux; j < 9; j++){
				aux = get_index(novo, 48);
				novo[get_index(novo, j+48)] = 48;
				novo[aux] = j+48;
			}
	}
	
	strcpy(novo, "087462315");
}


// === A* Sort === //
void AEstrela(char novo[10],char final[10]){
	double time_spent = 0.0;
    clock_t begin = clock();
	No* no = (No*)malloc(sizeof(No));
	No* noAnterior;
	Fila* fila = (Fila*)malloc(sizeof(Fila));
	fila->cabeca = NULL;
	Fila* filaTodosNos = (Fila*)malloc(sizeof(Fila));
	filaTodosNos->cabeca = NULL;
	strcpy(no->matriz, novo);
	enqueue(fila, no->matriz);
	enqueue(filaTodosNos, no->matriz);
	int i, qtd_no_total = 1, aux_heuristico, qtd_filho, pos_heuristico, nivel = 0, passou;
	while(strcmp(no->matriz, final) != 0){
		aux_heuristico = -1;
		qtd_filho = gerarFilhos(no);
		nivel++;
		passou = 0;
		for(i = 0 ; i< qtd_filho; i++){
			qtd_no_total++;
			printf("%s ",no->filhos[i]->matriz);
			if(!possui(fila, no->filhos[i]->matriz)){
				enqueue(filaTodosNos, no->filhos[i]->matriz);
				no->filhos[i]->hscore = qtd_errado(no->filhos[i]->matriz, final);
				no->filhos[i]->gscore = nivel;
				no->filhos[i]->fscore = no->filhos[i]->hscore + no->filhos[i]->gscore;
				printf("  %d   ", no->filhos[i]->hscore);
				if(aux_heuristico == -1 || aux_heuristico >= no->filhos[i]->fscore){
					aux_heuristico = no->filhos[i]->fscore;
					pos_heuristico = i;
				}
			}
		}
		printf("\n");
		no = no->filhos[pos_heuristico];
		enqueue(fila, no->matriz);
			
	}

	clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	system("cls");
	printf("Tempo de execucao: %f segundos\n", time_spent);
	printf("NOS visitados: %d\n", qtd_no_total);
	printf("Passos para solucao: %d", nivel);
	mostrar_resultado(fila);
	getche();
}

int gerarFilhos(No* no){
	int posZero = get_index(no->matriz, 48), qtd_filhos;
	char matrizAux[10]; 
	switch(posZero)
	{
		case 0: {
			//2 casos --> 1°: 1->0 || 2°: 3->0
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
			//3 casos --> 1°: 0->1 || 2°: 2->1 || 3°: 4->1
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
			//2 casos --> 1°: 1->2 || 2°: 5->2
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
			//3 casos --> 1°: 0->3 || 2°: 4->3 || 3°: 6->3
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
			//4 casos --> 1°: 1->4 || 2°: 3->4 || 3°: 5->4 || 4°: 7->4
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
			//3 casos --> 1°: 2->5 || 2°: 4->5 || 3°: 8->5
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
			//2 casos --> 1°: 3->6 || 2°: 7->6
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
			//3 casos --> 1°: 4->7 || 2°: 6->7 || 3°: 8->7
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
			//2 casos --> 1°: 7->8 || 2°: 5->8
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
void criarNo(TpElemento pai,TpFila &fila,char final[10],Fila lista,int tam)
{
	char filho[10];
	TpElemento elem;
	int pos = get_index(pai.Elemento,'0');
	char movs[4];
	strcpy(movs,possiveisMov(pos));

	int qtd = strlen(movs);
	for(int i=0;i<qtd;i++){
		strcpy(filho,criarFilho(pos,movs[i]-'0',pai.Elemento));
		if(!possui(&lista,filho)){
			strcpy(elem.Elemento,filho);
			elem.tam = tam;
			inserir(fila,elem);
			enqueue(&lista,filho);
		}
	}

	
}
void buscaCega(char ini[10], char final[10])
{
	TpFila fila;
	Fila* lista=(Fila*)malloc(sizeof(Fila));
	lista->cabeca = NULL;
	enqueue(lista,ini);
	TpElemento no;
     int tam =0; 
	strcpy(no.Elemento,ini);
	inicializar(fila);
	criarNo(no,fila,final,*lista,++tam);
	int dif,achou=0;
	printf("--------------------------------\n");
	while(achou==0 && !vazia(fila.inicio,fila.fim))
	{
		no = retirar(fila);
	
		dif = qtd_errado(no.Elemento,final);
		
		if(dif==0){
			achou=1;
			printf("achou o %s em um caminho de tamanho %d\n",no.Elemento,no.tam);
		}
		else
		{
			criarNo(no,fila,final,*lista,++tam);
		}
	}
	exibirPassos(lista);
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
	strcpy(novo, "123046578");
	embaralhar(novo);
	textcolor(15);
	textbackground(0);
	system("cls");
	printBoard(novo, 5,2);
	gotoxy(5,10);
	printf("Resolver (Enter)");
	getche();
	AEstrela(novo, estado_final);
	system("cls");
	fflush(stdin);

	return algoritmo;
}

