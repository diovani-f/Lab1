#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(NULL));
    int tentativa = 0, numGerado, max=101, min=0, teste=0;


    printf("Pense em um numero entre 1 e 100:");

    while(teste == 0){
        
        do{
            numGerado = min + rand() % max;
        }while(numGerado >= max || numGerado <= min);

        printf("\nSeu numero eh: %i\n0 - Acertou\n1 - Meu numero eh maior\n2 - Meu numero eh menor\n", numGerado);
        scanf("%i", &tentativa);

        if(tentativa == 1){
            min = numGerado;
        }

        else if(tentativa == 2){
            max = numGerado;
        }

        else if(tentativa == 0){
            printf("Acertei seu numero!!\n");
            teste = 1;
            break;
        }
        else{
            printf("Voce digitou um valor invalido!!\n");
        }
    }
    printf("FIM!\n");
}
