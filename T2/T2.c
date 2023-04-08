#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool verificaPrimo(int numero){
   int i, contador=0;

    for(i = 1; i <= numero; i++){
        if(numero % i == 0){
            contador++;
        }
        if(contador > 2)
            break;
    }

    if(contador==2){
        return true;
    }
    else{
        return false;
    }
}

void verificaIntervalo(int tamanho, int qntdIntervalo){
    int i, j, tam = tamanho, cont=0, contadorPrimos=0, contadorPrimos2=9999999, guardaIntervalo=0;

    for(i=0; i<qntdIntervalo; i++){
        for(j = tam - tamanho+1; j <= tam; j++){
            if(verificaPrimo(j) == true){
                contadorPrimos++;                               
            }
        }

        if(contadorPrimos <= contadorPrimos2){
            contadorPrimos2 = contadorPrimos;
            contadorPrimos = 0;
            guardaIntervalo=i;
        }
        tam = tam + tamanho;
    }

    mostrarTela(guardaIntervalo, tamanho, qntdIntervalo, contadorPrimos2);
}

void mostrarTela(int guardaIntervalo, int tamanho, int qntdIntervalo, int contadorPrimos){
    int i, j, tam = tamanho;

    printf("-----------------------\n");

    for(i=0; i<qntdIntervalo; i++){
        for(j = tam - tamanho+1; j <= tam; j++){
            if(guardaIntervalo == i){
                printf("%i\n", j);
            }                     
        }
        tam = tam + tamanho;
    }

    printf("-----------------------\n");
    printf("Este eh um (dos) intervalo(s) com menos primos:\nQuantidade:%i primo(s).\n", contadorPrimos);
}

/// não implementou as funções pedidas
int main(){

    int tamanho, qntdIntervalo;

    do{
        printf("Digite o tamanho dos intervalos que deseja, e a quantidade deles:\n");
        scanf("%i %i", &tamanho, &qntdIntervalo);
    }while(tamanho <= 0 || qntdIntervalo <= 0);

    verificaIntervalo(tamanho, qntdIntervalo);
}
