#include<conio.h>
#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<ctype.h>
#include<string.h>
#include<conio2.h>
#include <time.h>
//#include "fila.h"

// === ESTRUTURAS === //

struct No
{
	int fscore, gscore, hscore;
	char matriz[10];
	struct No *filhos[4];
};
typedef struct No No;



// === FUN��ES === //
void Quadro(int CI, int LI, int CF, int LF, int CorT, int CorF);
bool verificar(char estado_final[20]);
int qtd_errado(char novo[10], char final[10]);
void printBoard(char matriz[20], int x, int y);
void embaralhar(char novo[20]);
int get_index(char* string, char c);




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


// === VERIFICA��O === //
bool verificar(char estado_final[20]){
	int tamanho = 0, tamanho_mutiplicado = 1;
	for(int i = 0; i<10; i++)
		if(estado_final[i] > 48 &&  estado_final[i] <= 57){
			tamanho += (int)estado_final[i] - 48;
			tamanho_mutiplicado *= (int)estado_final[i] - 48;
		}
	return !(tamanho == 36 && strlen(estado_final) == 9 && tamanho_mutiplicado == 40320);
}


// === FUN��ES AUXILIARES === //
int qtd_errado(char novo[10], char final[10]){
	int i, qtd_errado = 0;
	for(i = 0; i<9; i++){
		if(novo[i] != final[i])
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



// === SHUFFLE SORT === //
void embaralhar(char novo[20]){
	int i, j, aux;
	char letra;
	for(i = 0 ; i < 100 ; i++){
		srand ( time(NULL) );
		aux = rand() % 10;
		if(aux < 9)
			for(j = aux; j < 9; j++){
				aux = get_index(novo, 48);
				novo[get_index(novo, j+48)] = 48;
				novo[aux] = j+48;
			}
	}
}


// === A* Sort === //




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
	system("cls");
	fflush(stdin);

	return algoritmo;
}

