#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#define STD_TAG 0


int *vetor;
int resposta[];
int vetorFinal[];

void verificaPrimo(int inicio, int fim, int rank){
    
    int contadorResposta = 0;
    
    printf("----- Proc = %d -----\n", rank);
    printf("--------------------------\n");

    for(int i = inicio; i < fim; i++){
        printf("%d\n", vetor[i]);
        if(vetor[i] != 1){
            int cont = 0;
            for(int j = 2; j < (vetor[i]/2)+1; j++){
                if(vetor[i] % j == 0){
                    cont++;
                }
            }
            if(cont == 0){
                resposta[contadorResposta] = 2;
                //printf("Valor = %d -- Divisores = %d\n", vetor[i], 2);
            }
            else{
                resposta[contadorResposta] = cont+2;
                //printf("Valor = %d -- Divisores = %d\n", vetor[i], cont+2);
            }
            contadorResposta++;
        }
        else{
            resposta[contadorResposta] = 1;
            contadorResposta++;
        }
    }
    printf("--------------------------\n");
}

int arquivoEntrada(){
    
    FILE* entryfile;
    
    if ((entryfile = fopen("testeentrada.txt","r")) == NULL)
    {
      printf("Error on file opening! \n");
      exit(1);
    }
    
    int valorLinha, verificaLinha, cont, sizeVet;

    cont = 0;

    while(!feof(entryfile)){
        verificaLinha = fscanf(entryfile, "%d", &valorLinha);
        if(verificaLinha == 1){
            cont++;
        }
    }

    fclose(entryfile);
    entryfile = fopen("testeentrada.txt","r");
    
    sizeVet = cont;
    cont = 0;

    vetor = (int*)malloc(sizeVet*sizeof(int));
    vetorFinal[sizeVet];

    while(!feof(entryfile)){
        verificaLinha = fscanf(entryfile,"%d",&valorLinha);    
        if(verificaLinha == 1){
            vetor[cont] = valorLinha;
            cont++;
        }
    }
    
    fclose(entryfile);
    
    return sizeVet;

}

void arquivoSaida(int vetorFinal[], int sizeVet){

    FILE* exitfile;
    exitfile = fopen("testesaida.txt", "w");

    for (int i = 0; i < sizeVet; i++){
        fprintf(exitfile, "%d\n", vetorFinal[i]);
    }

    fclose(exitfile);
}

void agrupaVetor(int vetorFinal[], int resposta[], int rank, int tamResposta, int size, int sizeVet){

    int fim;

    if (rank != size-1){
        fim = tamResposta*rank;
    }
    else{
        fim = sizeVet;
    }

    //printf("Processador = %d -- InicioVetor = %d -- FimVetor = %d\n", rank, fim-tamResposta, fim);

    int contResposta = 0;
    for (int i = fim-tamResposta; i < fim ; i++){
        vetorFinal[i] = resposta[contResposta];
        contResposta++;
        //printf("VetorFinal[%d] = %d\n", i, vetorFinal[i]);
    }


}

int main(int argc, char **argv){

    int rank, size;

    int sizeVet;

    int tamResposta;

    sizeVet = arquivoEntrada();

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank != 0) {
        
        if (rank != size-1){ 
            tamResposta = sizeVet/(size-1);
        }
        else {
            tamResposta = (sizeVet/(size-1))+(sizeVet%(size-1));
        }

        resposta[tamResposta];

        int inicio, fim;

        if (rank != size-1){
            fim = tamResposta*rank;
        }
        else{
            fim = sizeVet;
        }
        
        inicio = fim - tamResposta;

        printf("Rank = %d - Inicio = %d - Fim = %d\n", rank, inicio, fim-1);

        verificaPrimo(inicio, fim, rank);
        MPI_Ssend(resposta, tamResposta, MPI_INT, 0, STD_TAG, MPI_COMM_WORLD);
    }

    else {
        

        for (int i = 1; i < size; i++){
            if (i != size-1){ 
                tamResposta = sizeVet/(size-1);
            }
            else {
                tamResposta = (sizeVet/(size-1))+(sizeVet%(size-1));
            }

            MPI_Recv(resposta, tamResposta, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            agrupaVetor(vetorFinal, resposta, i, tamResposta, size, sizeVet);
        }
    }

    MPI_Finalize();

    arquivoSaida(vetorFinal, sizeVet);

    return 0;
}