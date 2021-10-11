#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>
#define STD_TAG 0

void verificaPrimo(int particao[], int resposta[], int sizeVet){
    for(int i = 0; i < sizeVet; i++){
        if(particao[i] != 1){
            int cont = 0;
            for(int j = 2; j < (particao[i]/2)+1; j++){
                if(particao[i] % j == 0){
                    cont++;
                }
            }
            if(cont == 0){
                resposta[i] = 2;
            }
            else{
                resposta[i] = cont+2;
            }
        }
        else{
            resposta[i] = 1;
        }
    }
}

int nLinhasArquivoEntrada(){
    FILE* entryfile;
    
    if ((entryfile = fopen("testeentrada.txt","r")) == NULL)
    {
      printf("Error on file opening! \n");
      exit(1);
    }
    
    int valorLinha, verificaLinha, cont;

    cont = 0;

    while(!feof(entryfile)){
        verificaLinha = fscanf(entryfile, "%d", &valorLinha);
        if(verificaLinha == 1){
            cont++;
        }
    }

    return cont;
}

void arquivoEntrada(int vetor[]){
    
    FILE* entryfile;
    
    if ((entryfile = fopen("testeentrada.txt","r")) == NULL)
    {
      printf("Error on file opening! \n");
      exit(1);
    }
    
    int valorLinha, verificaLinha, cont;
    cont = 0;
    while(!feof(entryfile)){
        verificaLinha = fscanf(entryfile,"%d",&valorLinha);    
        if(verificaLinha == 1){
            vetor[cont] = valorLinha;
            cont++;
        }
    }
    
    fclose(entryfile);

}

void arquivoSaida(int sizeVet, int resultado[]){

    FILE* exitfile;
    exitfile = fopen("testesaida.txt", "w");

    for (int i = 0; i < sizeVet; i++){
        fprintf(exitfile, "%d\n", resultado[i]);
    }

    fclose(exitfile);
}

void agrupaVetor(int resultado[], int resposta[],int rank, int tamParticao, int size, int sizeVet){

    int fim;
    if (rank != size-1){
        fim = tamParticao*rank;
    }
    else{
        fim = sizeVet;
    }

    int contResposta = 0;
    for (int i = fim-tamParticao; i < fim ; i++){
        resultado[i] = resposta[contResposta];
        contResposta++;
    }
    
}


void setParticao(int vetor[], int particao[], int inicio, int fim){
    int cont = 0;
    for(int i = inicio; i < fim; i++){
        particao[cont] = vetor[i];
        cont++;
    }
}

void calculaInicioFim(int *inicio, int *fim, int proc, int size, int tamParticao, int sizeVet){
    if (proc != size-1){
        *fim = tamParticao*proc;
    }
    else{
        *fim = sizeVet;
    }
            
    *inicio = *fim - tamParticao;
}

void calculaTamPart(int *tamParticao, int sizeVet, int proc, int size){
    if (proc != size-1){ 
        *tamParticao = sizeVet/(size-1);
    }
    else {
        *tamParticao = (sizeVet/(size-1))+(sizeVet%(size-1));
    }
}

int main(int argc, char **argv){

    int rank, size;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank != 0) {
        int tamParticao;
        int *particao;
        int *resposta;
        
        clock_t t;

        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &tamParticao);

        particao = (int*)malloc(tamParticao*sizeof(int));
        resposta = (int*)malloc(tamParticao*sizeof(int));

        MPI_Recv(particao, tamParticao, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        t = clock();
        verificaPrimo(particao,resposta,tamParticao);
        t = clock() - t;

        printf("Tempo de execucao em milisegundos: %lf", ((double)t)/((CLOCKS_PER_SEC/1000)));

        MPI_Send(resposta, tamParticao, MPI_INT, 0, STD_TAG, MPI_COMM_WORLD);
        
        free(particao);
        free(resposta);
    }

    else {
        
        int *vetor;
        int *particao;
        int *resposta;
        int *resultado;
        int sizeVet, tamParticao;

        sizeVet = nLinhasArquivoEntrada();
        
        vetor = (int*)malloc(sizeVet*sizeof(int));
        resultado = (int*)malloc(sizeVet*sizeof(int));

        arquivoEntrada(vetor);

        for(int i = 1; i < size; i++){
            calculaTamPart(&tamParticao,sizeVet,i,size);
            
            particao = (int*)malloc(tamParticao*sizeof(int));

            int inicio, fim;
            calculaInicioFim(&inicio, &fim, i, size, tamParticao, sizeVet);
            
            setParticao(vetor,particao,inicio,fim);
            MPI_Send(particao, tamParticao, MPI_INT, i, STD_TAG, MPI_COMM_WORLD);

            free(particao);
        }

        for (int i = 1; i < size; i++){
            
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &tamParticao);

            resposta = (int*)malloc(tamParticao*sizeof(int));

            MPI_Recv(resposta, tamParticao, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            agrupaVetor(resultado,resposta,status.MPI_SOURCE, tamParticao, size, sizeVet);
            
            free(resposta);
        }
        free(vetor);
        arquivoSaida(sizeVet,resultado);
        free(resultado);
    }

    MPI_Finalize();
    
    

    return 0;
}