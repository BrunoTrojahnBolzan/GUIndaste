#include <graphics.h>
#include <conio.h>
#include <stdio.h>

//Tipos de janela
#define JANELA_TEXTO 0
#define JANELA_BOTAO 1
#define JANELA_ENTRADA 2

//Eventos (Equivalente a mensagens na Win32 API)
#define BOTAO_PRESSIONADO 0
#define DESLOCA_ABAIXO 1
#define DESLOCA_ACIMA 2

//Flags em geral
#define SAIR 0
#define SEM_BARRA_ROLAGEM 0
#define BARRA_ROLAGEM 1

//Definições internas
#define ALOCAR_JANELA (JANELA) malloc(sizeof(struct janela))

typedef struct janela{

    struct janela *id;
    int tipo;
    char texto[2000];
    int x;
    int y;
    int largura;
    int altura;
    int tamanho_fonte;
    int comando;
    struct janela *ant;
    struct janela *prox;

}*JANELA;

struct barra_rolagem{

    int tamanho_tambor;
    int posicao;
    int existencia;
    int x;
    int y;
    int altura;
    int largura;

};

JANELA lista_janelas = NULL;

void (*callback_func)(int, int);

int flag = -1;

struct barra_rolagem barra = {0, 0, 0};

int cor_de_fundo = 0;

HWND esta_janela;

void iniciar_janela(int largura, int altura, char *titulo, int cor, void (*_callback_func)(int, int), int barra_rolagem){

    initwindow(largura, altura, titulo);
    esta_janela = GetForegroundWindow();
    setbkcolor(cor);
    cor_de_fundo = cor;
    cleardevice();

    if(barra_rolagem == 1){

        barra.existencia = 1;
        barra.x = largura - 20;
        barra.y = 26;
        barra.largura = 20;
        barra.altura = altura;

        setfillstyle(1, LIGHTGRAY);
        bar(barra.x, 0, barra.x + barra.largura, barra.altura);
        setfillstyle(1, BLACK);
        bar(barra.x, 0, barra.x + barra.largura, 20);
        bar(barra.x, barra.altura - 20, barra.x + barra.largura, barra.altura);

        setfillstyle(1, WHITE);
        bar(barra.x, barra.posicao, barra.x + barra.largura, barra.posicao + barra.tamanho_tambor);

    }

    callback_func = (*_callback_func);

}

JANELA criar_janela(int tipo, char *texto, int x, int y, int largura, int altura, int tamanho_letra, int comando){

    if(tipo >=0 && tipo < 3){

        JANELA nova = ALOCAR_JANELA;

        if(nova == NULL) //Se a alocação falhar, retorna um valor inválido
            return (JANELA) -2;
        nova->id = nova;
        nova->tipo = tipo;
        strcpy(nova->texto, texto);
        nova->x = x;
        nova->y = y;
        nova->largura = largura;
        nova->altura = altura;
        nova->tamanho_fonte = tamanho_letra;
        nova->comando = comando;

        if(lista_janelas){
            lista_janelas->ant->prox = nova;
            nova->ant = lista_janelas->ant;
            nova->prox = lista_janelas;
            lista_janelas->ant = nova;
        }else{
            nova->ant = nova;
            nova->prox = nova;
            lista_janelas = nova;
        }

        return nova;

    }else
        return (JANELA) -1;
    return (JANELA) 0;

}

void destruir_janela(JANELA janela){

    if(lista_janelas){
        if(lista_janelas != lista_janelas->ant){
            JANELA auxiliar = lista_janelas;

            do{
                auxiliar = auxiliar->prox;
            }while(lista_janelas != auxiliar && auxiliar != janela);

            if(auxiliar == janela){

                auxiliar->prox->ant = auxiliar->ant;
                auxiliar->ant->prox = auxiliar->prox;

                if(lista_janelas == auxiliar)
                    lista_janelas = lista_janelas->prox;

                free(auxiliar);
            }


        }else{
            if(janela == lista_janelas){
                printf("%s\n", janela->texto);
                free(janela);
                lista_janelas = NULL;
            }
        }
    }

}

void atualizar_tela(){

    cleardevice();

    JANELA auxiliar = lista_janelas;

    do{
        switch(auxiliar->tipo){
        case 0:
            settextstyle(3, 0, auxiliar->tamanho_fonte);
            outtextxy(auxiliar->x, auxiliar->y, auxiliar->texto);
            break;
        case 1:
            settextstyle(3, 0, auxiliar->tamanho_fonte);
            setbkcolor(DARKGRAY);
            outtextxy(auxiliar->x, auxiliar->y, auxiliar->texto);
            setbkcolor(cor_de_fundo);
            break;
        case 2:
            settextstyle(3, 0, auxiliar->tamanho_fonte);
            outtextxy(auxiliar->x, auxiliar->y, auxiliar->texto);
            break;
        default:
            break;
        }

        auxiliar = auxiliar->prox;

    }while(auxiliar != lista_janelas);

    if(barra.existencia == 1){

        setfillstyle(1, LIGHTGRAY);
        bar(barra.x, 0, barra.x + barra.largura, barra.altura);
        setfillstyle(1, BLACK);
        bar(barra.x, 0, barra.x + barra.largura, 20);
        bar(barra.x, barra.altura - 20, barra.x + barra.largura, barra.altura);

        setfillstyle(1, WHITE);
        bar(barra.x, barra.posicao, barra.x + barra.largura, barra.posicao + barra.tamanho_tambor);
    }
}

int ler_evento(){

    if(flag != SAIR){

        if(GetForegroundWindow() == esta_janela){ //Se o usuário está usando a janela do programa e não outra

            //Lê a posição do mouse na tela
            POINT mouse; //Estrutura que armazena as coordenadas do ponteiro do mouse
            GetCursorPos(&mouse); //Lê as coordenadas do mouse na tela

            ScreenToClient(esta_janela, &mouse); //Transforma as coordenadas do mouse em relação à tela
                                                 //em coordenadas do mouse em relação ao topo da janela

            if(lista_janelas){ //Se houver alguma janela criada

                JANELA auxiliar = lista_janelas; //Variável auxiliar para percorrer a lista de janelas

                do{
                    //Administra botões
                    if(auxiliar->tipo == JANELA_BOTAO && GetAsyncKeyState(VK_LBUTTON) && mouse.x > auxiliar->x && mouse.x < auxiliar->x + auxiliar->largura &&
                       mouse.y > auxiliar->y && mouse.y < auxiliar->y + auxiliar->altura){

                        (*callback_func)(BOTAO_PRESSIONADO, auxiliar->comando);
                        break;

                    }

                    //Administra texto editável
                    if(auxiliar->tipo == JANELA_ENTRADA && GetAsyncKeyState(VK_LBUTTON) && mouse.x > auxiliar->x && mouse.x < auxiliar->x + auxiliar->largura &&
                       mouse.y > auxiliar->y && mouse.y < auxiliar->y + auxiliar->altura){
                        char buff[2000] = {'\0'};
                        int pos = 0;
                        strcpy(buff, auxiliar->texto);
                        pos = strlen(buff);

                        while(1){
                            strcpy(auxiliar->texto, buff);
                            atualizar_tela();
                            outtextxy(auxiliar->x, auxiliar->y, buff);
                            buff[pos] = getch();
                            if(buff[pos] == '\b'){
                                if(pos != 0){
                                    buff[pos] = '\0';
                                    pos--;
                                    buff[pos] = '\0';
                                }else
                                    buff[pos] = '\0';
                                continue;
                            }
                            pos++;
                            if(buff[pos - 1] == '\r')
                                break;
                        }
                        buff[pos] = '\0';
                        buff[pos - 1] = '\0';
                        strcpy(auxiliar->texto, buff);
                        auxiliar->largura = strlen(buff) * 9;
                        break;
                    }

                    auxiliar = auxiliar->prox;

                }while(lista_janelas != auxiliar);
            }

            if(barra.existencia == 1){

                if(mouse.x > barra.x && mouse.x < barra.x + barra.largura && mouse.y > 0 && mouse.y < 20 && GetAsyncKeyState(VK_LBUTTON)){
                    (*callback_func)(DESLOCA_ACIMA, -1);
                    delay(100);
                }

                if(mouse.x > barra.x && mouse.x < barra.x + barra.largura && mouse.y > barra.altura - 20 && mouse.y < barra.altura && GetAsyncKeyState(VK_LBUTTON)){
                    (*callback_func)(DESLOCA_ABAIXO, -1);
                    delay(100);
                }
            }
        }

    }else{
        //futuramente aqui eu teria que desalocar a memória alocada em tempo de execução e etc
        return SAIR;
    }
    return -1;
}

void ler_texto_janela(JANELA janela, char *destino){

    JANELA auxiliar = lista_janelas;

    while(lista_janelas != auxiliar && auxiliar != janela)
        auxiliar = auxiliar->prox;

    if(auxiliar == janela)
        snprintf(destino, strlen(janela->texto) + 2, "%s", janela->texto);
}

void modificar_barra_rolagem(int tamanho_tambor, int posicao){

    barra.tamanho_tambor = tamanho_tambor;
    barra.posicao = posicao;

}

int obter_tamanho_tambor_barra_rol(){

    return barra.tamanho_tambor;

}

int obter_posicao_barra_rol(){

    return barra.posicao;

}

void modificar_janela_x(JANELA janela, int x){

    JANELA auxiliar = lista_janelas;

    while(lista_janelas != auxiliar && auxiliar != janela)
        auxiliar = auxiliar->prox;

    if(auxiliar == janela)
        janela->x = x;

}

void modificar_janela_y(JANELA janela, int y){

    JANELA auxiliar = lista_janelas;

    while(lista_janelas != auxiliar && auxiliar != janela)
        auxiliar = auxiliar->prox;

    if(auxiliar == janela)
        janela->y = y;

}

int obter_janela_x(JANELA janela){

    JANELA auxiliar = lista_janelas;

    while(lista_janelas != auxiliar && auxiliar != janela)
        auxiliar = auxiliar->prox;

    if(auxiliar == janela)
        return janela->x;

    return 0;

}

int obter_janela_y(JANELA janela){

    JANELA auxiliar = lista_janelas;

    while(lista_janelas != auxiliar && auxiliar != janela)
        auxiliar = auxiliar->prox;

    if(auxiliar == janela)
        return janela->y;

    return 0;

}

void sair(){
    flag = SAIR;
}
