/***********************
BaseWindows.h
Autor: Bruno Trojahn Bolzan
Colaboradores: ----
Descrição: Esta é a biblioteca base para Windows da biblioteca gráfica GUIndaste.
************************/




///Inclusão de bibliotecas
#include <windows.h> //Biblioteca do Windows
#include <CommCtrl.h>
#include <string.h> //Biblioteca para manipulação de strings
#include <stdio.h> //Biblioteca para funções de formatação




///Definições
#define JANELA_TEXTO 0 //Macro para janela de texto estático
#define JANELA_BOTAO 1 //Macro para janela botão
#define JANELA_ENTRADA 2 //Macro para janela de texto editável
#define JANELA_GENERICA 3 //Macro para janela genérica
#define JANELA_BARRA_PROGRESSO 4 //Macro para barra de progresso
#define EV_BOTAO_PRESSIONADO 1 //Macro para evento de botão pressionado
#define EV_ROLAGEM_ABAIXO_VERTICAL 2 //Macro para evento de rolagem abaixo vertical
#define EV_ROLAGEM_ACIMA_VERTICAL 3 //Macro para evento de rolagem acima vertical
#define EV_ROLAGEM_ARRASTADA_VERTICAL 4 //Macro para evento de rolagem arrastada vertical
#define EV_ROLAGEM_ABAIXO_HORIZONTAL 5 //Macro para evento de rolagem abaixo horizontal
#define EV_ROLAGEM_ACIMA_HORIZONTAL 6 //Macro para evento de rolagem acima horizontal
#define EV_ROLAGEM_ARRASTADA_HORIZONTAL 7 //Macro para evento de rolagem arrastada horizontal




///Definições de tipos

//Estrutura de janela
typedef struct janela{

    HWND hWnd;
    HWND hWnd_pai;
    int tipo;
    wchar_t texto[2000];
    int x;
    int y;
    int largura;
    int altura;
    int comando;
    unsigned int cor;
    int pintada;
    struct janela *ant;
    struct janela *prox;

}*JANELA;

//Estrutura de evento
typedef struct evento{

    JANELA janela;
    int evento;
    int parametro1;
    int parametro2;

}EVENTO;




///Protótipos de funções
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp); //Protótipo da função que administra as mensagens
int main_biblioteca(); //Protótipo da função principal da GUIndaste
int _iniciar_janela(wchar_t *titulo, int x, int y, int largura, int altura, COLORREF cor, int rolagem_vertical, int rolagem_horizontal); //Protótipo da função interna inicializadora de janela
EVENTO _ler_evento(); //Protótipo da função inteira leitora de eventos
JANELA _criar_janela(int tipo, wchar_t *texto, int x, int y, int largura, int altura, int comando, COLORREF cor, JANELA pai); //Protótipo da função interna criadora de janelas
int _obter_texto_janela(wchar_t *destino, JANELA janela); //Protótipo da função interna que obtém o texto de uma janela
int _obter_janela_x(JANELA janela); //Protótipo da função interna que retorna a posição horizontal de uma janela
int _obter_janela_y(JANELA janela); //Protótipo da função interna que retorna a posição vertical de uma janela
int _modificar_janela_xy(JANELA janela, int x, int y); //Protótipo da função interna que modifica a posição de uma janela
int _modificar_posicao_barra_progresso(JANELA janela, int nova_posicao); //Protótipo da função interna que modifica a posição de uma barra de progresso
int _destruir_janela(JANELA janela); //Protótipo da função interna que destrói uma janela
void _sair(); //Protótipo da função interna que sai do programa
static JANELA procurar_janela(HWND hwnd); ////Protótipo da função que procura a janela associada a determinado HWND




///Variáveis globais
HINSTANCE hInstance;
HWND janela_hwnd;
int flag_sair = -1;
JANELA lista_janelas = NULL;
COLORREF cor_de_fundo = 0; //Variável para armazenar a cor de fundo da janela
HBRUSH pincel; //Variável para armazenar o pincel que preenche a cor de fundo do texto estático
EVENTO _evento; //Variável para armazenar o evento




LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){

    JANELA auxiliar;

    if(flag_sair)
        _evento.evento = -1;

    _evento.parametro1 = 0;
    _evento.parametro2 = 0;

    switch(msg){
        case WM_CREATE:
            break;

        case WM_DESTROY:
            _evento.evento = 0;
            flag_sair = 0;
            PostQuitMessage(0);
            break;

        case WM_COMMAND: //Gerencia as mensagens de botão

            if(lista_janelas){

                JANELA auxiliar = lista_janelas;

                do{

                    if(auxiliar->comando == wp){
                        _evento.janela = auxiliar;
                        _evento.evento = EV_BOTAO_PRESSIONADO;
                        _evento.parametro1 = wp;

                        break;
                    }

                    auxiliar = auxiliar->prox;

                }while(auxiliar != lista_janelas);

            }
            break;

        case WM_VSCROLL:

            switch(LOWORD(wp)){

                case SB_LINEDOWN:

                    _evento.janela = 0;
                    _evento.evento = EV_ROLAGEM_ABAIXO_VERTICAL;
                    break;

                case SB_LINEUP:
                    _evento.janela = 0;
                    _evento.evento = EV_ROLAGEM_ACIMA_VERTICAL;
                    break;

                case SB_THUMBTRACK:
                    _evento.janela = 0;
                    _evento.evento = EV_ROLAGEM_ARRASTADA_VERTICAL;
                    break;
            }
            break;

        case WM_HSCROLL:
            switch(LOWORD(wp)){

                case SB_LINELEFT:
                    _evento.janela = 0;
                    _evento.evento = EV_ROLAGEM_ABAIXO_HORIZONTAL;
                    break;

                case SB_LINERIGHT:
                    _evento.janela = 0;
                    _evento.evento = EV_ROLAGEM_ACIMA_HORIZONTAL;
                    break;

                case SB_THUMBTRACK:
                    _evento.janela = 0;
                    _evento.evento = EV_ROLAGEM_ARRASTADA_HORIZONTAL;
                    break;
            }
            break;

        case WM_CTLCOLORSTATIC:

            auxiliar = lista_janelas;

            do{
                if(auxiliar->tipo == 0 && auxiliar->pintada == 0){

                    SetTextColor((HDC) wp, auxiliar->cor);
                    SetBkColor((HDC) wp, cor_de_fundo);
                    auxiliar->pintada = 1;
                    pincel = CreateSolidBrush(cor_de_fundo);
                    return (LRESULT) pincel;
                }

                auxiliar = auxiliar->prox;

            }while(lista_janelas != auxiliar);

            break;

        case WM_CTLCOLOREDIT:

            auxiliar = lista_janelas;

            do{
                if(auxiliar->tipo == JANELA_ENTRADA && auxiliar->pintada == 0){

                    SetTextColor((HDC) wp, auxiliar->cor);
                    auxiliar->pintada = 1;
                    pincel = CreateSolidBrush(RGB(255, 255, 255));
                    return (LRESULT) pincel;
                }

                auxiliar = auxiliar->prox;

            }while(lista_janelas != auxiliar);

            break;

        default:
            DefWindowProcW(hwnd, msg, wp, lp);
    }

}




LRESULT WINAPI WinProcFilho(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){

    JANELA auxiliar, auxiliar2;

    if(flag_sair)
        _evento.evento = -1;

    _evento.parametro1 = 0;
    _evento.parametro2 = 0;

    switch(msg){
        case WM_CREATE:
            break;

        case WM_DESTROY:
            _evento.evento = 0;
            flag_sair = 0;
            PostQuitMessage(0);
            break;

        case WM_COMMAND: //Gerencia as mensagens de botão

            if(lista_janelas){

                JANELA auxiliar = lista_janelas;

                do{

                    if(auxiliar->comando == wp){
                        _evento.janela = auxiliar;
                        _evento.evento = EV_BOTAO_PRESSIONADO;
                        _evento.parametro1 = wp;

                        break;
                    }

                    auxiliar = auxiliar->prox;

                }while(auxiliar != lista_janelas);

            }
            break;

        case WM_VSCROLL:

            switch(LOWORD(wp)){

                case SB_LINEDOWN:
                    _evento.janela = procurar_janela(hwnd);
                    _evento.evento = EV_ROLAGEM_ABAIXO_VERTICAL;
                    break;

                case SB_LINEUP:
                    _evento.janela = procurar_janela(hwnd);
                    _evento.evento = EV_ROLAGEM_ACIMA_VERTICAL;
                    break;

                case SB_THUMBTRACK:
                    _evento.janela = procurar_janela(hwnd);
                    _evento.evento = EV_ROLAGEM_ARRASTADA_VERTICAL;
                    break;
            }
            break;

        case WM_HSCROLL:
            switch(LOWORD(wp)){

                case SB_LINELEFT:
                    _evento.janela = procurar_janela(hwnd);
                    _evento.evento = EV_ROLAGEM_ABAIXO_HORIZONTAL;
                    break;

                case SB_LINERIGHT:
                    _evento.janela = procurar_janela(hwnd);
                    _evento.evento = EV_ROLAGEM_ACIMA_HORIZONTAL;
                    break;

                case SB_THUMBTRACK:
                    _evento.janela = procurar_janela(hwnd);
                    _evento.evento = EV_ROLAGEM_ARRASTADA_HORIZONTAL;
                    break;
            }
            break;

        case WM_CTLCOLORSTATIC:

            auxiliar = lista_janelas;

            do{
                if(auxiliar->tipo == 0 && auxiliar->pintada == 0){

                    SetTextColor((HDC) wp, auxiliar->cor);

                    auxiliar2 = lista_janelas;
                    do{
                        if(auxiliar->hWnd_pai == auxiliar2->hWnd){
                            SetBkColor((HDC) wp, auxiliar2->cor);
                            break;
                        }

                        auxiliar2 = auxiliar2->prox;

                    }while(auxiliar2 != lista_janelas);

                    auxiliar->pintada = 1;
                    pincel = CreateSolidBrush(auxiliar2->cor);
                    return (LRESULT) pincel;
                }

                auxiliar = auxiliar->prox;

            }while(lista_janelas != auxiliar);

            break;

        case WM_CTLCOLOREDIT:

            auxiliar = lista_janelas;

            do{
                if(auxiliar->tipo == JANELA_ENTRADA && auxiliar->pintada == 0){

                    SetTextColor((HDC) wp, auxiliar->cor);

                    auxiliar2 = lista_janelas;
                    do{
                        if(auxiliar->hWnd_pai == auxiliar2->hWnd){
                            SetBkColor((HDC) wp, auxiliar2->cor);
                            break;
                        }

                        auxiliar2 = auxiliar2->prox;

                    }while(auxiliar2 != lista_janelas);

                    auxiliar->pintada = 1;
                    pincel = CreateSolidBrush(auxiliar2->cor);
                    return (LRESULT) pincel;
                }

                auxiliar = auxiliar->prox;

            }while(lista_janelas != auxiliar);

            break;
        default:
            DefWindowProcW(hwnd, msg, wp, lp);
    }

}




int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow){

    hInstance = hThisInstance; //Globaliza a instância para acesso pela função _iniciar_janela

    return main_biblioteca(); //Chama a função principal da GUIndaste e retorna o valor que o usuário desejar

}




int _iniciar_janela(wchar_t *titulo, int x, int y, int largura, int altura, COLORREF cor, int rolagem_vertical, int rolagem_horizontal){

    cor_de_fundo = cor;

    WNDCLASSW janela = {0};

    janela.hbrBackground = CreateSolidBrush(cor);
    janela.hCursor = LoadCursor(NULL, IDC_ARROW);
    janela.hInstance = hInstance;
    janela.lpszClassName = L"janela";
    janela.lpfnWndProc = &WinProc;

    if(!RegisterClassW(&janela))
        return -1;

    DWORD estilo = WS_VISIBLE | WS_OVERLAPPEDWINDOW;

    if(rolagem_vertical)
        estilo |= WS_VSCROLL;

    if(rolagem_horizontal)
        estilo |= WS_HSCROLL;

    janela_hwnd = CreateWindowW(L"janela", titulo, estilo, x, y, largura, altura, NULL, NULL, NULL, NULL);

    MSG msg;

    GetMessageW(&msg, 0, 0, 0);
    TranslateMessage(&msg);
    DispatchMessageW(&msg);

    return 0;
}




EVENTO _ler_evento(){

    MSG msg;

    if(GetMessageW(&msg, janela_hwnd, 0, 0)){

        TranslateMessage(&msg);
        DispatchMessageW(&msg);

    }

    return _evento;
}




void _sair(){

    //Destroi todas as janelas
    if(lista_janelas){

        JANELA auxiliar = lista_janelas;

        do{
            DestroyWindow(auxiliar->hWnd);
            auxiliar = auxiliar->prox;

        }while(auxiliar != lista_janelas);

    }

    //Desaloca memória das janelas
    if(lista_janelas){

        JANELA auxiliar = lista_janelas;
        JANELA excluir;

        do{
            excluir = auxiliar;
            auxiliar = auxiliar->prox;
            free(excluir);

        }while(auxiliar != lista_janelas);

    }

    flag_sair = 0;

}




JANELA _criar_janela(int tipo, wchar_t *texto, int x, int y, int largura, int altura, int comando, COLORREF cor, JANELA pai){

    if(tipo >= 0 && tipo <= 4){
        JANELA nova = malloc(sizeof(struct janela));

        if(nova == NULL) //Se a alocação falhar, retorna um valor inválido
            return (JANELA) -2;

        wchar_t tipo_janela[50] = {0};

        DWORD estilo = WS_VISIBLE | WS_CHILD;

        if(tipo != 3){
            switch(tipo){
                case 0:
                    wcscpy(tipo_janela, L"static");
                    break;
                case 1:
                    wcscpy(tipo_janela, L"button");
                    estilo |= WS_BORDER;
                    break;
                case 2:
                    wcscpy(tipo_janela, L"edit");
                    estilo |= WS_BORDER | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE | WS_VSCROLL;
                    break;
                case 4:
                    wcscpy(tipo_janela, PROGRESS_CLASSW);
                    break;
            }
        }else{
            snwprintf(tipo_janela, 14, L"%ls", pai);
            WNDCLASSW janela_generica = {0};
            janela_generica.hbrBackground = CreateSolidBrush(cor);
            janela_generica.hCursor = LoadCursor(hInstance, IDC_ARROW);
            janela_generica.hInstance = hInstance;
            janela_generica.lpfnWndProc = &WinProcFilho;
            janela_generica.lpszClassName = tipo_janela;

            RegisterClassW(&janela_generica);
        }

        HWND hWnd_final = janela_hwnd;

        if(lista_janelas){
            JANELA auxiliar = lista_janelas;
            do{
                if(auxiliar == pai)
                    hWnd_final = pai->hWnd;

                auxiliar = auxiliar->prox;

            }while(auxiliar != lista_janelas);
        }

        nova->hWnd = CreateWindowW(tipo_janela, texto, estilo, x, y, largura, altura, hWnd_final, (HMENU) comando, NULL, NULL);
        nova->tipo = tipo;
        wcscpy(nova->texto, texto);
        nova->x = x;
        nova->y = y;
        nova->largura = largura;
        nova->altura = altura;
        nova->comando = comando;
        nova->cor = cor;
        nova->pintada = 0;
        nova->hWnd_pai = hWnd_final;
        DeleteObject((HGDIOBJ) pincel);
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




int _obter_texto_janela(wchar_t *destino, JANELA janela){

    JANELA auxiliar = lista_janelas;

    do{
        if(auxiliar == janela){
            GetWindowTextW(auxiliar->hWnd, destino, 5000);
            break;
        }

        auxiliar = auxiliar->prox;

    }while(auxiliar != lista_janelas);


    return 0;

}




int _obter_janela_x(JANELA janela){

    JANELA auxiliar = lista_janelas;

    do{
        if(janela == auxiliar){

            RECT coords;

            GetWindowRect(janela->hWnd, &coords);

            POINT xy = {coords.left, coords.top};

            ScreenToClient(janela_hwnd, &xy);

            return xy.x;
        }

        auxiliar = auxiliar->prox;

    }while(auxiliar != lista_janelas);

    return 0;

}




int _obter_janela_y(JANELA janela){

    JANELA auxiliar = lista_janelas;

    do{
        if(janela == auxiliar){

            RECT coords;

            GetWindowRect(janela->hWnd, &coords);

            POINT xy = {coords.left, coords.top};

            ScreenToClient(janela_hwnd, &xy);

            return xy.y;
        }

        auxiliar = auxiliar->prox;

    }while(auxiliar != lista_janelas);

    return 0;

}




int _modificar_janela_xy(JANELA janela, int x, int y){

    JANELA auxiliar = lista_janelas;

    do{
        if(janela == auxiliar){

            SetWindowPos(janela->hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);

            return 0;

        }

        auxiliar = auxiliar->prox;

    }while(auxiliar != lista_janelas);

    return 0;

}




int _modificar_posicao_barra_progresso(JANELA janela, int nova_posicao){

    JANELA auxiliar = lista_janelas;

    if(auxiliar){
        do{
            if(auxiliar == janela){
                if(nova_posicao >= 0 && nova_posicao <= 100){
                    SendMessageW(auxiliar->hWnd, PBM_SETPOS, nova_posicao, 0);
                    return 0;
                }
                return -1;
            }

            auxiliar = auxiliar->prox;

        }while(auxiliar != lista_janelas);
    }
    return -1;
}




int _destruir_janela(JANELA janela){

    if(janela){

        JANELA auxiliar = lista_janelas;

        if(lista_janelas){

            if(lista_janelas->ant == lista_janelas){
                if(janela == lista_janelas){
                    DestroyWindow(janela->hWnd);
                    free(lista_janelas);
                    lista_janelas = NULL;
                }
            }else{

                do{
                    if(auxiliar == janela){

                        DestroyWindow(auxiliar->hWnd);

                        JANELA excluir = auxiliar;

                        auxiliar->ant->prox = auxiliar->prox;
                        auxiliar->prox->ant = auxiliar->ant;

                        if(auxiliar == lista_janelas)
                            lista_janelas = lista_janelas->prox;
                        break;
                        free(excluir);
                    }

                    auxiliar = auxiliar->prox;

                }while(auxiliar != lista_janelas);
            }

        }

    }

    return 0;
}




//Função que busca a janela associada a determinado HWND
static JANELA procurar_janela(HWND hwnd){

    JANELA auxiliar = lista_janelas;

    if(auxiliar){

        do{
            if(auxiliar->hWnd == hwnd)
                return auxiliar;

            auxiliar = auxiliar->prox;

        }while(auxiliar != lista_janelas);

    }

    return (JANELA) -2;

}
