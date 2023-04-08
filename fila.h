#define MAXFILA 10000

struct TpElemento
{
	char Elemento[10];
	char *historico[30];
	int tam,hist_tam;
};

struct TpFila
{
	int fim,inicio,cont;
	TpElemento FILA[MAXFILA];
};
void inicializar(TpFila &f)
{
	f.fim = -1;
	f.inicio=0;
	f.cont = 0;

}
int vazia(int inicio,int fim)
{
	return (inicio>fim);
}
int cheia(int inicio,int fim)
{
	return (inicio==0 && fim==MAXFILA-1);
}

TpElemento retirar(TpFila &F)
{
	F.cont--;
	return F.FILA[F.inicio++];
}
void inserir(TpFila &F, TpElemento Elem )
{
	F.FILA[++F.fim] = Elem;
	F.cont++;
}

void Exibir(TpFila F)
{
	TpElemento aux;
	while(!vazia(F.inicio,F.fim))
	{
		aux = retirar(F);
		printf("%s\n",aux.Elemento);
	}
		
}

void mostra_historico(TpElemento F)
{
	int i = 0, x = 10, y = 5;
	for(int i=0;i<=F.hist_tam;i++)
		{
			printf("%s\n",F.historico[i]);
		}
}
