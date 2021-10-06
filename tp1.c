#include <stdio.h>
#include <stdlib.h>



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

    for(int i = 0; i < sizeVet; i++){
        printf("pos %d:  -> %d\n",i+1,vetor[i]);
    }

    printf("%d\n",vetor[0]);

    free(vetor);
    fclose(entryfile);
    return 0;
}