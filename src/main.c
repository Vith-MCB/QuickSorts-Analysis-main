#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <ctype.h>

#include "quickSorts.h"

// Varia conforme o sistema operacional.
#if defined WIN32 || defined _WIN32 || defined __CYGWIN__
#define CAMINHO_ARQUIVO "..\\arquivos\\"
#else
#define CAMINHO_ARQUIVO "../arquivos/" // Outros OS.
#endif

#define INVALIDO (-1)
#define ZERO 0
#define UM 1
#define DOIS 2
#define TRES 3
#define CINCO 5
#define DEZ 10
#define CEM 100
#define DELIMITADORES "<>"
#define MINIMO_ARQ_VALIDO 4
#define NULL_TERMINATOR 1
#define SEPARADOR "\n\n\n"
#define INICIO_ARG "quicksort"
#define ERRO_ORDENACAO "\nERRO: O array não está ordenado.\n"
#define ARGS_ESPERADO "\nErro: %d argumentos esperados.\n"
#define ERRO_ARQUIVO_SAIDA "\nErro: Não foi possível gerar o arquivo de saída.\n"
#define ERRO_FORMATO "\nErro: O formato do argumento deve ser 'quicksort<semente1>saida_semente1.txt'.\n"


// Protótipos de funções.

double inicializaClock(void);

double calculaTempo(double tempoInicial);

void escreveNoArquivo(FILE *pArquivo, Tupla *valores, int tamanho, char *string);

void quickSortsSemK(FILE *pArquivo, Tupla *valores, void (*quickSortTipo)(int *, int, int, Tupla *), int *arr, int tamanho, char *string);

void quickSortsComK(FILE *pArquivo, Tupla *valores, void (*quickSortTipo)(int *, int, int, int, Tupla *), int *arr, int k, int tamanho, char *string);

void geraNumerosAleatorios(int *arr, int tamanho, unsigned int seed);

void checaOrdenacao(const int *arr, int tamanho);


/*
 * A função principal. Tem como função a leitura de informações de um arquivo e manipulação dos dados.
 *
 * @return          0 se executado com sucesso, -1 se ocorrer algum erro.
 */
int main(int argc, char *argv[])
{
    /// ----------------------------------------------------------- CHECAGEM ARGUMENTOS --------------------------------------------------------------

    // Checando se a quantidade de argumentos é a esperada.
    if (argc != DOIS)
    {
        printf(ARGS_ESPERADO, UM);
        return INVALIDO;
    }

    // Retorna valor antes do delimitadores esperados. Caso contrário retorna nulo.
    char *string = strstr(argv[UM], INICIO_ARG);

    // Checa se encontrou o valor antes dos delimitadores e valor entre os delimitadores.
    if (!string || !strtok(string, DELIMITADORES))
    {
        printf(ERRO_FORMATO);
        return INVALIDO;
    }

    // Retorna valor entre os delimitadores. Caso contrário retorna nulo.
    char *seedString = strtok(NULL, DELIMITADORES);

    // Checa se encontrou valor entre delimitadores.
    if (!seedString)
    {
        printf(ERRO_FORMATO);
        return INVALIDO;
    }

    // Checa se os algarismos entre os delimitadores são digitos.
    for (int i = ZERO; i < strlen(seedString); i++)
    {
        if (!isdigit((int) seedString[i]))
        {
            printf(ERRO_FORMATO);
            return INVALIDO;
        }
    }

    // Retorna valor depois dos delimitadores. Caso contrário retorna nulo.
    char *nomeArquivo = strtok(NULL, DELIMITADORES);

    // Checa se encontrou valor depois dos delimitadores. E se o mesmo tem o mínimo número de caracteres para ser um arquivo válido.
    if (!nomeArquivo || strlen(nomeArquivo) < MINIMO_ARQ_VALIDO)
    {
        printf(ERRO_FORMATO);
        return INVALIDO;
    }

    /// ----------------------------------------------------------- FIM CHECAGEM ARGUMENTOS ----------------------------------------------------------


    /// ----------------------------------------------------------- PROCESSAMENTO DE DADOS -----------------------------------------------------------

    // Declarando variáveis.
    unsigned int seed = ZERO;
    char arquivo[strlen(CAMINHO_ARQUIVO) + strlen(nomeArquivo) + NULL_TERMINATOR];

    // Atribuindo caminho do arquivo.
    strcpy(arquivo, CAMINHO_ARQUIVO);
    // Atribuindo nome do arquivo.
    strcat(arquivo, nomeArquivo);
    // Atribuindo seed.
    sscanf(seedString, "%i", &seed);

    // Criando ponteiro para arquivo de saída.
    FILE *pArquivo = fopen(arquivo, "a+");

    // Checando se o ponteiro para o arquivo de saída é válido.
    if (!pArquivo)
    {
        printf(ERRO_ARQUIVO_SAIDA);
        return INVALIDO;
    }


    // Criando array com os tamanhos para teste.
    int tamanhos[] = {1000,
                      5000,
                      10000,
                      50000,
                      100000,
                      500000,
                      1000000,
                      1500000,
                      2000000,
                      2500000,
                      3000000,
                      4000000,
                      5000000,
                      6000000,
                      7000000,
                      8000000,
                      90000000,
                      10000000};

    // Realizando os testes com os valores de n diferentes.
    for (int i = ZERO; i < (sizeof(tamanhos) / sizeof(tamanhos[ZERO])); i++)
    {
        // Inicializa array com o tamanho desejado.
        int *arrRandomNum = malloc(tamanhos[i] * sizeof(int));

        // Gerando números aleatórios.
        geraNumerosAleatorios(arrRandomNum, tamanhos[i], seed);

        // Criando uma variável da estrutura Tupla para juntar valores de análise.
        Tupla valores;

        // Quick Sort Recursivo
        quickSortsSemK(pArquivo,
                       &valores,
                       quickSortRecursivo,
                       arrRandomNum,
                       tamanhos[i],
                       "Quick Sort Recursivo - %d elementos\n");

        checaOrdenacao(arrRandomNum, tamanhos[i]);
        geraNumerosAleatorios(arrRandomNum, tamanhos[i], seed);


        // Quick Sort Mediana (k = 3)
        quickSortsComK(pArquivo,
                       &valores,
                       quickSortMediana,
                       arrRandomNum,
                       TRES,
                       tamanhos[i],
                       "\nQuick Sort Mediana (k = 3) - %d elementos\n");

        checaOrdenacao(arrRandomNum, tamanhos[i]);
        geraNumerosAleatorios(arrRandomNum, tamanhos[i], seed);


        // Quick Sort Mediana (k = 5)
        quickSortsComK(pArquivo,
                       &valores,
                       quickSortMediana,
                       arrRandomNum,
                       CINCO,
                       tamanhos[i],
                       "\nQuick Sort Mediana (k = 5) - %d elementos\n");

        checaOrdenacao(arrRandomNum, tamanhos[i]);
        geraNumerosAleatorios(arrRandomNum, tamanhos[i], seed);


        // Quick Sort Inserção (m = 10)
        quickSortsComK(pArquivo,
                       &valores,
                       quickSortInsercao,
                       arrRandomNum,
                       DEZ,
                       tamanhos[i],
                       "\nQuick Sort Inserção (m = 10) - %d elementos\n");

        checaOrdenacao(arrRandomNum, tamanhos[i]);
        geraNumerosAleatorios(arrRandomNum, tamanhos[i], seed);


        // Quick Sort Inserção (m = 100)
        quickSortsComK(pArquivo,
                       &valores,
                       quickSortInsercao,
                       arrRandomNum,
                       CEM,
                       tamanhos[i],
                       "\nQuick Sort Inserção (m = 100) - %d elementos\n");

        checaOrdenacao(arrRandomNum, tamanhos[i]);
        geraNumerosAleatorios(arrRandomNum, tamanhos[i], seed);


        // Quick Sort Empilha Inteligente
        quickSortsSemK(pArquivo,
                       &valores,
                       quickSortEmpilha,
                       arrRandomNum,
                       tamanhos[i],
                       "\nQuick Sort Empilha Inteligente - %d elementos\n");

        checaOrdenacao(arrRandomNum, tamanhos[i]);
        geraNumerosAleatorios(arrRandomNum, tamanhos[i], seed);


        // Quick Sort Iterativo
        quickSortsSemK(pArquivo,
                       &valores,
                       quickSortIterativo,
                       arrRandomNum,
                       tamanhos[i],
                       "\nQuick Sort Iterativo - %d elementos\n");

        checaOrdenacao(arrRandomNum, tamanhos[i]);

        // Adiciona espaçamento no arquivo para diferentes tamanhos de arrays.
        fprintf(pArquivo, SEPARADOR);

        // Liberando array na memória.
        free(arrRandomNum);
    }

    // Fechando arquivo.
    fclose(pArquivo);
    pArquivo = NULL;

    /// --------------------------------------------------------- FIM PROCESSAMENTO DE DADOS ---------------------------------------------------------


    return ZERO;
}


/*
 * Executa função quick sort (sem constante k) e salva detalhes.
 *
 * @param    pArquivo           ponteiro para o arquivo.
 * @param    valores            ponteiro para a estrutura Tupla.
 * @param    quickSortTipo      ponteiro para função quick sort para ser executada.
 * @param    arr                ponteiro para array com números aleatórios.
 * @param    tamanho            tamanho do array.
 * @param    string             contém informações sobre a função quick sort sendo executada.
 */
void quickSortsSemK(FILE *pArquivo, Tupla *valores, void (*quickSortTipo)(int *, int, int, Tupla *), int *arr, int tamanho, char *string)
{
    double clock = inicializaClock();
    inicializaTupla(valores);

    (*quickSortTipo)(arr, ZERO, tamanho - UM, valores);
    valores->tempo = calculaTempo(clock);

    escreveNoArquivo(pArquivo, valores, tamanho, string);
}


/*
 * Executa função quick sort (com constante k) passada como parâmetro e salva detalhes.
 *
 * @param    pArquivo           ponteiro para o arquivo.
 * @param    valores            ponteiro para a estrutura Tupla.
 * @param    quickSortTipo      ponteiro para função quick sort para ser executada.
 * @param    arr                ponteiro para array com números aleatórios.
 * @param    k                  constante para quick sort algoritmo.
 * @param    tamanho            tamanho do array.
 * @param    string             contém informações sobre a função quick sort sendo executada.
 */
void quickSortsComK(FILE *pArquivo, Tupla *valores, void (*quickSortTipo)(int *, int, int, int, Tupla *), int *arr, int k, int tamanho, char *string)
{
    double clock = inicializaClock();
    inicializaTupla(valores);

    (*quickSortTipo)(arr, k, ZERO, tamanho - UM, valores);
    valores->tempo = calculaTempo(clock);

    escreveNoArquivo(pArquivo, valores, tamanho, string);
}


/*
 * Escreve dados em arquivo.
 *
 * @param    pArquivo    ponteiro para o arquivo.
 * @param    valores     ponteiro para a estrutura Tupla.
 * @param    tamanho     tamanho do array.
 * @param    string      contém informações sobre a função quick sort sendo executada.
 */
void escreveNoArquivo(FILE *pArquivo, Tupla *valores, int tamanho, char *string)
{
    fprintf(pArquivo, string, tamanho);
    fprintf(pArquivo, "%0.4lf %ld %ld\n", valores->tempo, valores->qtdComparacoes, valores->qtdMovimentacoes);
}


/*
 * Inicia o clock.
 * @return      valor de quando a função "clock()" é chamada.
 */
double inicializaClock(void)
{
    return (double) clock();
}


/*
 * Finaliza o clock e calcula o tempo total.
 *
 * @param    tempoInicial    valor da primeira vez que a função "clock()" foi chamada.
 * @return                   valor calculado referente ao tempo entre o tempo inicial e o tempo final.
 */
double calculaTempo(double tempoInicial)
{
    return (double) (clock() - tempoInicial) / CLOCKS_PER_SEC;
}


/*
 * Gera números aleatórios no array especificado com seed especifica.
 *
 * @param    arr         ponteiro para array com números aleatórios.
 * @param    tamanho     tamanho do array.
 */
void geraNumerosAleatorios(int *arr, int tamanho, unsigned int seed)
{
    // Definindo a sequência aleatória.
    srand(seed);

    for (int j = ZERO; j < tamanho; j++)
    {
        arr[j] = rand();
    }
}


/*
 * Checa se o array está completamente ordenado.
 *
 * @param    arr                ponteiro para array.
 * @param    tamanho            tamanho do array.
 */
void checaOrdenacao(const int *arr, int tamanho)
{
    for (int j = ZERO; j < tamanho - UM; j++)
    {
        if (arr[j] > arr[j + UM])
        {
            printf(ERRO_ORDENACAO);
        }
    }
}