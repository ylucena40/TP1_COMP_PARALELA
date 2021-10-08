#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#define STD_TAG 0


int *vetor;
int resposta[];

void verifica_primo(int inicio, int fim){
    
    int contadorResposta = 0;

    for(int i = inicio; i < fim; i++){
        if(vetor[i] != 1){
            int cont = 0;
            for(int j = 2; j < (vetor[i]/2)+1; j++){
                if(vetor[i] % j == 0){
                    cont++;
                }
            }
            if(cont == 0){
                resposta[contadorResposta] = 2;
            }
            else{
                resposta[contadorResposta] = cont+2;
            }
            contadorResposta++;
        }
        else{
            resposta[contadorResposta] = 1;
            contadorResposta++;
        }
    }
}

int arquivo(){
    
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

int main(int argc, char **argv){

    int rank, size;

    int sizeVet;

    int tamResposta;

    sizeVet = arquivo();

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank != 0) {
        
        if (rank != size-1){ 
            tamResposta = sizeVet/size-1;
        }
        else {
            tamResposta = (sizeVet/size-1)+(sizeVet%size-1);
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

        verifica_primo(inicio, fim);
        MPI_Send(resposta, tamResposta, MPI_INT, 0, STD_TAG, MPI_COMM_WORLD);
    }

    else {
        for (int i = 1; i < size; i++){
            MPI_Recv(resposta, 6, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Proc %d -- %d\n", status.MPI_SOURCE, resposta[0]);
        }
    }

    MPI_Finalize();

    return 0;
}