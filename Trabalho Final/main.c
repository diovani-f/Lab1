#define _CRT_SECURE_NO_WARNINGS
#include "tela.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#define LARGTELA 850
#define ALTTELA 900


typedef struct {
    int pontos;
    int bonus;
    int posCerta[9];
    int posErrada[9];
    int qntdClickNasCores;
    int corPraDesenhar[4][9];
    int coresSorteadas[4];
    int linhasJogadas;
    char meunome[30];
}estado;

//Soma os pontos do jogador
void pontuador(estado* Estado) {
    Estado->pontos += (Estado->posCerta[Estado->linhasJogadas]*5);
    Estado->pontos += (Estado->posErrada[Estado->linhasJogadas] * 3);

    if (Estado->posCerta[Estado->linhasJogadas] != 4 && Estado->bonus == 1) {
        Estado->pontos = 0;
    }
    else if (Estado->posCerta[Estado->linhasJogadas] != 4) {
        Estado->bonus--;
    }
    else if (Estado->posCerta[Estado->linhasJogadas] == 4) {
        Estado->pontos = Estado->pontos * Estado->bonus;
    }
}

//Sorteia os numeros aleatorios
void sorteador(estado* Estado) {

    for (int i = 0; i < 4; i++) {
        int corSorteada = 1 + rand() % 7;
        for (int j = 0; j < i; j++) {
            if (corSorteada == Estado->coresSorteadas[j]) {
                i--;
                j--;
            }
        }
        Estado->coresSorteadas[i] = corSorteada;
    }
}

//Imprime os nomes e pontos da lista.
void imprimirArq(char nomes[5][30], int pntArq[]) {
    int var = 0;
    char pontos[12];

    for (int i = 0; i < 5; i++) {
        tela_texto_dir(350, 400+var, 20, 8, nomes[i]);
        sprintf(pontos, "%d", pntArq[i]);
        tela_texto(550, 400 + var, 20, 8, pontos);
        var += 60;
    }
}

//cria um arquivo novo, caso não exista um.
void criaArquivo() {
    FILE* pontos;
    pontos = fopen("dados.txt", "w");

    for (int j = 0; j < 5; j++) {
        fprintf(pontos, "%s\n", "x");
        fprintf(pontos, "%s\n", "0");
    }
    fclose(pontos);
}

//apenas tira as informações do arquivo, e salva em vetores.
void salvaArqVetor(char nomes[5][30], int pntArq[]) {
    char strBruta[500];
    int cont = 0, contChar = 0;

    FILE* pontos;
    pontos = fopen("dados.txt", "r");
    if (pontos == NULL) {
        criaArquivo();
        pontos = fopen("dados.txt", "r");
    }



    for (int i = 1; fgets(strBruta, 500, pontos) != NULL; i++) {
        if (i % 2 == 0) {
            pntArq[cont] = atoi(strBruta);
            cont++;
        }
        else {
            strcpy(nomes[contChar], strBruta);
            nomes[contChar][strlen(nomes[contChar]) - 1] = '\0';
            contChar++;
        }
    }
    fclose(pontos);
}

//Insere o nome e os pontos da pessoa no top 5.
void inserirPontos(estado*Estado, char nomes[5][30], int pntArq[]) {
    FILE* pontos;
    int meuPonto = Estado->pontos, copia;
    char copiaChar[30];

    for (int j = 0; j < 5; j++) {
        if (pntArq[j] < meuPonto) {
           
            copia = pntArq[j];
            pntArq[j] = meuPonto;
            meuPonto = copia;
            //---------------------
            strcpy(copiaChar, nomes[j]);
            strcpy(nomes[j], Estado->meunome);
            strcpy(Estado->meunome, copiaChar);
        }
        else if (j == 4 && pntArq[j] > meuPonto) {
            return;
        }
    }

    pontos = fopen("dados.txt", "w");

    for (int j = 0; j < 5; j++) {
        fprintf(pontos, "%s\n", nomes[j]);
        fprintf(pontos, "%d\n", pntArq[j]);
    }
    fclose(pontos);
}

//Verifica se a pessoa tem pontos suficientes para entrar na lista.
bool verificaPontos(estado *Estado, int pntArq[]) {    
    for (int j = 0; j < 5; j++) {
        if (pntArq[j] < Estado->pontos) {
            return true;
        }
    }
    return false;
}

//Desenha os circulos de navegação
void desenhaCirculosNavegacao() {
    tela_circulo(80, 850, 35, 2, 1, 1);
    tela_circulo(170, 850, 35, 2, 2, 2);
    tela_circulo(260, 850, 35, 2, 3, 3);
    tela_circulo(350, 850, 35, 2, 4, 4);
    tela_circulo(440, 850, 35, 2, 5, 5);
    tela_circulo(530, 850, 35, 2, 6, 6);
    tela_circulo(620, 850, 35, 2, 7, 7);
}

//Calcula a cor, para saber onde o usuario clicou.
bool calculoCor(int xcirc, int ycirc) {
    int dentroCirculo;

    dentroCirculo = sqrt(pow((xcirc - tela_rato_x_clique()), 2) + pow((ycirc - tela_rato_y_clique()), 2));

    if (dentroCirculo <= 35) {
        return true;
    }
    else
        return false;
}

//Verifdica se o usuario clicou em um dos circulos de cores.
int verificaClickCirculo(estado* Estado) {
    if (calculoCor(80, 850)) {
        return 1;
    }
    else if (calculoCor(170, 850)) {
        return 2;
    }
    else if (calculoCor(260, 850)) {
        return 3;
    }
    else if (calculoCor(350, 850)) {
        return 4;
    }
    else if (calculoCor(440, 850)) {
        return 5;
    }
    else if (calculoCor(530, 850)) {
        return 6;
    }
    else if (calculoCor(620, 850)) {
        return 7;
    }
    else
        return 0;
}

//Verifica se o usuario nao clicou em uma cor ja clicada.
void verificaCorIgual(estado* Estado) {
    int cont = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (Estado->corPraDesenhar[i][Estado->linhasJogadas] == Estado->corPraDesenhar[j][Estado->linhasJogadas] && Estado->corPraDesenhar[i][Estado->linhasJogadas] != 0) {
                cont++;
            }
        }
    }

    if (cont >= 1) {
        Estado->corPraDesenhar[Estado->qntdClickNasCores- 1][Estado->linhasJogadas] = 0;
        Estado->qntdClickNasCores--;
    }
}

//Imprime as cores que o usuario clicou.
void desenharNavegacao(estado* Estado) {

    for (int j = 0; j < 9; j++) {
        for (int i = 0; i < 4; i++) {
            tela_circulo(75 * (i + 1), 80 * (j + 1), 35, 2, Estado->corPraDesenhar[i][j], Estado->corPraDesenhar[i][j]);
        }
    }

}

//Desenha a primeira tela.
void desenhaTela() {
    int fucsia = tela_cria_cor(1, 0.2, 0.8);

    tela_retangulo(15, 15, 690, 890, 5, fucsia, transparente);

    tela_retangulo(700, 15, 843, 63, 5, fucsia, transparente);
    tela_texto(771, 39, 23, 8, "Testar");

    tela_retangulo(700, 70, 843, 118, 5, fucsia, transparente);
    tela_texto(771, 94, 23, 8, "Apagar");

    tela_retangulo(700, 125, 843, 173, 5, fucsia, transparente);
    tela_texto(771, 149, 23, 8, "Encerrar");

    tela_texto(771, 200, 20, 8, "Tente adivinhar");
    tela_texto(771, 230, 20, 8, "as 4 cores");
    tela_texto(771, 260, 20, 8, "sorteadas");
    tela_texto(771, 290, 20, 8, "pelo");
    tela_texto(771, 320, 20, 8, "computador!");


    tela_linha(345, 15, 345, 796, 5, fucsia);
    tela_linha(15, 796, 690, 796, 5, fucsia);


    desenhaCirculosNavegacao();
    tela_circulo(tela_rato_x(), tela_rato_y(), 10, 2, vermelho, branco);
}

//Desenha a tela final
void desenhaSegundaTela(estado *Estado) {
    int fucsia = tela_cria_cor(1, 0.2, 0.8);
    char pontos[30];

    tela_retangulo(15, 15, 840, 890, 5, fucsia, transparente);
    tela_texto(450, 350, 25, 8, "Pontuadores:");

    tela_retangulo(320, 117, 595, 180, 5, fucsia, transparente);
    tela_texto(450, 147, 20, 8, "Jogar Novamente");

    tela_retangulo(320, 215, 595, 280, 5, fucsia, transparente);
    tela_texto(450, 247, 20, 8, "Encerrar Partida");

    tela_texto(400, 85, 20, 8, Estado->meunome);
    sprintf(pontos, "%d", Estado->pontos);
    tela_texto(530, 85, 20, 8, pontos);


    tela_circulo(tela_rato_x(), tela_rato_y(), 10, 2, vermelho, branco);
}

//verifica quantos circulos estão no lugar certo.
void acertouPosicao(estado* Estado) {
    int acertou = 0;
    for (int i = 0; i < 4; i++) {
        if (Estado->coresSorteadas[i] == Estado->corPraDesenhar[i][Estado->linhasJogadas])
            acertou++;
    }
    Estado->posCerta[Estado->linhasJogadas] = acertou;
}

//verifica quantos circulos estão no lugar errado.
void errouPosicao(estado* Estado) {
    int acertou = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (Estado->coresSorteadas[i] == Estado->corPraDesenhar[j][Estado->linhasJogadas])
                acertou++;
        }
    }
    Estado->posErrada[Estado->linhasJogadas] = acertou - Estado->posCerta[Estado->linhasJogadas];
}

//Desenha os circulos pretos e brancos.
void desenhaPeB(estado* Estado) {
    int posx = 400, posy = 80;
    for (int i = 0; i < Estado->linhasJogadas; i++) {
        for (int k = 0; k < Estado->posCerta[i]; k++) {
            tela_circulo(posx, posy, 35, 2, 8, 9);
            posx = posx + 80;
        }

        for (int k = 0; k < Estado->posErrada[i]; k++) {
            tela_circulo(posx, posy, 35, 2, 8, 8);
            posx = posx + 80;
        }

        posx = 400;
        posy += 80;
    }
}

//Adiciona as cores clicadas em um vetor.
void armazenaCores(estado *Estado) {
    int ResVerificaClick = verificaClickCirculo(Estado);
    
    if (ResVerificaClick >= 1 && ResVerificaClick <= 7 && Estado->qntdClickNasCores <= 4) {
        if (Estado->qntdClickNasCores < 4) {
            Estado->qntdClickNasCores++;
        }
        while (Estado->corPraDesenhar[Estado->qntdClickNasCores-1][Estado->linhasJogadas] == 0) {
            int teste = Estado->qntdClickNasCores-1;
            Estado->corPraDesenhar[teste][Estado->linhasJogadas] = ResVerificaClick;
            verificaCorIgual(Estado);
        }
    }
}

//Verifica se o usuario clicou pra jogar de novo ou para sair.
bool verificaClickRet() {
    if (tela_rato_x_clique() > 320 && tela_rato_x_clique() < 595 && tela_rato_y_clique() > 117 && tela_rato_y_clique() < 180) {
        return true;
    }
    else if (tela_rato_x_clique() > 320 && tela_rato_x_clique() < 595 && tela_rato_y_clique() > 215 && tela_rato_y_clique() < 280)
        exit(1);
    return false;
}

//Desenha a tela para inserir o nome.
void desenhaTelaNome(char meuNome[]) {

    int fucsia = tela_cria_cor(1, 0.2, 0.8);
    tela_texto(450, 420, 25, 8, "Fim de jogo!");
    tela_texto(450, 450, 23, 8, "Digite seu nome, e pressione enter para voltar ao menu:");
    tela_texto(450, 500, 23, 8, meuNome);
    tela_retangulo(15, 15, 840, 890, 5, fucsia, transparente);
    tela_circulo(tela_rato_x(), tela_rato_y(), 10, 2, vermelho, branco);
}

//Pega as informações da tela do nome e da tela final.
bool telaMenu(estado* Estado) {
    char nomesArq[5][30], c, meuNome[30];
    int pntArq[5], contador=0, cont1=0;
    meuNome[0] = '\0';

    salvaArqVetor(nomesArq, pntArq);


    //Tela insere nome, so consegui fazer funcionar aqui dentro :(.
    while (true) {
        c = tela_tecla();
        if (c == '\n') {
            break;
        }
        else if (c == '\b' && contador > 0) {
            meuNome[contador-1] = '\0';
            contador--;
        }
        else if(c != '\0' && c != '\b') {
            meuNome[contador] = c;
            meuNome[contador+1] = '\0';
            contador++;
        }
        desenhaTelaNome(meuNome);
        tela_atualiza();
    }
    contador = 0;
    strcpy(Estado->meunome, meuNome);


    while (true) {
        desenhaSegundaTela(Estado);
        imprimirArq(nomesArq, pntArq);   
            
        
        if (Estado->posCerta[Estado->linhasJogadas-1] == 4) {
            tela_texto(450, 50, 40, 8, "Você Ganhou!!!");
            if (verificaPontos(Estado, pntArq) == true && contador == 0) {
                inserirPontos(Estado, nomesArq, pntArq);
                strcpy(Estado->meunome, meuNome);
                contador = 1;
            }
        }
        else {
            tela_texto(450, 50, 40, 8, "Você Perdeu!!!");
        }


        if (tela_rato_clicado()) {
            if (verificaClickRet() == true)
                return true;
        }
        tela_atualiza();
    }
}

//pegas as informações do usuario clicar em teste, apagar e encerrar.
void barraLateral(estado *Estado) {
    if (tela_rato_x_clique() > 700 && tela_rato_x_clique() < 843 && tela_rato_y_clique() > 15 && tela_rato_y_clique() < 63) {
        if (Estado->qntdClickNasCores >= 4) {
            acertouPosicao(Estado);
            errouPosicao(Estado);
            pontuador(Estado);
            Estado->linhasJogadas++;
            Estado->qntdClickNasCores = 0;
        }
    }
    else if (tela_rato_x_clique() > 700 && tela_rato_x_clique() < 843 && tela_rato_y_clique() > 70 && tela_rato_y_clique() < 118) {
        if (Estado->qntdClickNasCores > 0) {
            Estado->corPraDesenhar[Estado->qntdClickNasCores-1][Estado->linhasJogadas] = 0;
            Estado->qntdClickNasCores--;
        }
    }
    else if (tela_rato_x_clique() > 700 && tela_rato_x_clique() < 843 && tela_rato_y_clique() > 125 && tela_rato_y_clique() < 173) {
        exit(1);
    }
}

//Zera todas as informações para uma nova jogada.
void zerador(estado* Estado) {
    for (int i = 0; i < 4; i++) {
        printf("%d-", Estado->coresSorteadas[i]);
    }

    for (int k = 0; k < 9; k++) {
        for (int i = 0; i < 4; i++) {
            Estado->corPraDesenhar[i][k] = 0;
        }
        Estado->posCerta[k] = 0;
        Estado->posErrada[k] = 0;
    }
    Estado->linhasJogadas = 0;
    Estado->qntdClickNasCores = 0;
    Estado->pontos = 0;
    Estado->bonus = 9;
}

//Pega as informações da primeira tela, e chama o resto das funções.
void primeiraTela(estado* Estado) {
    bool jogarNovamente=true;
    while (jogarNovamente == true)
    {
        sorteador(Estado);
        zerador(Estado);
        while (true) {
            if (tela_rato_clicado()) {
                armazenaCores(Estado);
                barraLateral(Estado);
            }


            desenhaTela();
            desenhaPeB(Estado);
            desenharNavegacao(Estado);


            tela_atualiza();


            if (Estado->linhasJogadas > 0) {
                if (Estado->linhasJogadas == 9 || Estado->posCerta[Estado->linhasJogadas - 1] == 4) {
                    if (telaMenu(Estado) == true) {
                        break;
                    }

                }
            }
        }
    }

}


// A função principal
int main(void)
{
    srand(time(NULL));
    
    estado Estado;

    tela_inicio(LARGTELA, ALTTELA, "Senha");

    primeiraTela(&Estado);
    // encerra a tela gráfica
    tela_fim();

    return 0;
}


/*
#define transparente 0
#define azul 2
#define vermelho 1
#define verde 3
#define amarelo 4
#define preto 9
#define laranja 5
#define rosa 6
#define branco 8
#define marrom 7
*/