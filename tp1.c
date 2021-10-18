#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void verifica_primo(int vetor[], int resposta[],int sizeVet){
    
    for(int i = 0; i < sizeVet; i++){
        printf("%d\n",vetor[i]);
        if( vetor[i] != 1){
            int cont = 0;
            for(int j = 2; j < (vetor[i]/2)+1; j++){
                if(vetor[i] % j == 0){
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
        else {
            resposta[i] = 1;
        }
    }
}


int main(int argc, char* argv[]){
    FILE* entryfile;
    FILE* exitfile;
    if ((entryfile = fopen(argv[1],"r")) == NULL)
    {
      printf("Error on file opening! \n");
      exit(1);
    }
    
    int a,b, cont, sizeVet;
    int *vetor;
    int *resposta;

    cont = 0;

    while(!feof(entryfile)){
        b = fscanf(entryfile,"%i",&a);
        if(b == 1){
            cont++;
        }
    }

    fclose(entryfile);
    entryfile = fopen(argv[1],"r");
    exitfile = fopen("saida.txt","w");
    
    sizeVet = cont;
    cont = 0;

    vetor = (int*)malloc(sizeVet*sizeof(int));
    resposta = (int*)malloc(sizeVet*sizeof(int));

    while(!feof(entryfile)){
        b = fscanf(entryfile,"%d",&a);    
        if(b == 1){
            vetor[cont] = a;
            cont++;
        }
    }
    
    t = clock();
    verifica_primo(vetor,resposta,sizeVet);
    t = clock() - t;

    printf("Tempo de execucao em segundos: %lf \n", ((double)t)/((CLOCKS_PER_SEC)));


    for(int i = 0; i < sizeVet; i++){
        fprintf(exitfile,"%d\n",resposta[i]);
    }

    free(vetor);
    fclose(entryfile);
    fclose(exitfile);
    return 0;
}