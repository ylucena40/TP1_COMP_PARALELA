#include <stdio.h>
#include <stdlib.h>

void verifica_primo(int vetor[],int sizeVet){
    for(int i = 0; i < sizeVet; i++){
        if( vetor[i] != 1){
            int cont = 0;
            for(int j = 2; j < vetor[i] / 2; j++){
                if(vetor[i] % j == 0){
                    cont++;
                    break;
                }
            }
            if(cont == 0){
                printf("O numero: %d eh primo.\n",vetor[i]);
            }
            else{
                printf("O numero: %d nao eh primo.\n",vetor[i]);
            }
        }
    }
}


int main(int argc, char* argv[]){
    FILE* entryfile;
    if ((entryfile = fopen(argv[1],"r")) == NULL)
    {
      printf("Error on file opening! \n");
      exit(1);
    }
    
    int a, cont, sizeVet;
    int *vetor;

    cont = 0;

    while(!feof(entryfile)){
        fscanf(entryfile,"%d",&a);
        cont++;
    }

    fclose(entryfile);
    fopen(argv[1],"r");
    
    sizeVet = cont++;
    cont = 0;

    vetor = (int*)malloc(sizeVet*sizeof(int));

    while(!feof(entryfile)){
        fscanf(entryfile,"%d",&a);    
        vetor[cont] = a;
        cont++;
    }

    verifica_primo(vetor,sizeVet);

    free(vetor);
    fclose(entryfile);
    return 0;
}