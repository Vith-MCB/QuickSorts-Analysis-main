typedef struct
{
    double tempo;
    unsigned long int qtdComparacoes;
    unsigned long int qtdMovimentacoes;
} Tupla;

void inicializaTupla(Tupla *valores);

void quickSortRecursivo(int *arr, int inicio, int final, Tupla *valores);

void quickSortMediana(int *arr, int k, int inicio, int final, Tupla *valores);

void quickSortInsercao(int *arr, int m, int inicio, int final, Tupla *valores);

void quickSortEmpilha(int *arr, int inicio, int final, Tupla *valores);

void quickSortIterativo(int *arr, int inicio, int final, Tupla *valores);
