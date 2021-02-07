/***********************
BaseWindows.h
Autor: Bruno Trojahn Bolzan
Colaboradores: ----
Descrição: Esta é a biblioteca base para Windows da biblioteca gráfica GUIndaste.
************************/




///Inclusão de bibliotecas
#include <windows.h> //Biblioteca do Windows
#include <CommCtrl.h> //Biblioteca do Windows com definições para controles
#include <string.h> //Biblioteca para manipulação de strings
#include <stdio.h> //Biblioteca para funções de formatação




///Definições
#define JANELA_TEXTO 0 //Macro para janela de texto estático
#define JANELA_BOTAO 1 //Macro para janela botão
#define JANELA_ENTRADA 2 //Macro para janela de texto editável
#define JANELA_GENERICA 3 //Macro para janela genérica
#define JANELA_BARRA_PROGRESSO 4 //Macro para barra de progresso
#define EV_BOTAO_PRESSIONADO 1 //Macro para evento de botão pressionado
#define EV_ROLAGEM 2 //Macro para evento de rolagem
#define ROL_ABAIXO_VERTICAL 0 //Macro para evento de rolagem abaixo vertical
#define ROL_ACIMA_VERTICAL 1 //Macro para evento de rolagem acima vertical
#define ROL_ARRASTADA_VERTICAL 2 //Macro para evento de rolagem arrastada vertical
#define ROL_ABAIXO_HORIZONTAL 3 //Macro para evento de rolagem abaixo horizontal
#define ROL_ACIMA_HORIZONTAL 4 //Macro para evento de rolagem acima horizontal
#define ROL_ARRASTADA_HORIZONTAL 5 //Macro para evento de rolagem arrastada horizontal
#define ROLAGEM_VERTICAL 0x00000001 //Opção para rolagem vertical
#define ROLAGEM_HORIZONTAL 0x00000002 //Opção para rolagem horizontal



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
    unsigned int cor_de_fundo;
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
int _iniciar_janela(wchar_t *titulo, int x, int y, int largura, int altura, COLORREF cor, unsigned long int opcoes); //Protótipo da função interna inicializadora de janela
EVENTO _ler_evento(); //Protótipo da função interna leitora de eventos
JANELA _criar_janela(int tipo, wchar_t *texto, int x, int y, int largura, int altura, int comando, COLORREF cor, COLORREF cor_de_fundo, JANELA pai, unsigned long int opcoes); //Protótipo da função interna criadora de janelas
int _obter_texto_janela(wchar_t *destino, JANELA janela); //Protótipo da função interna que obtém o texto de uma janela
int _obter_janela_x(JANELA janela); //Protótipo da função interna que retorna a posição horizontal de uma janela
int _obter_janela_y(JANELA janela); //Protótipo da função interna que retorna a posição vertical de uma janela
int _modificar_janela_xy(JANELA janela, int x, int y); //Protótipo da função interna que modifica a posição de uma janela
int _modificar_posicao_barra_progresso(JANELA janela, int nova_posicao); //Protótipo da função interna que modifica a posição de uma barra de progresso
int _obter_posicao_barra_rol(JANELA janela, int opcao); //Protótipo da função interna que retorna a posição de uma barra de rolagem
int _obter_tamanho_barra_rol(JANELA janela, int opcao); //Protótipo da função interna que retorna o tamanho do tambor de uma barra de rolagem
int _modificar_barra_rol(JANELA janela, int posicao, int tamanho, int minimo, int maximo, int opcao); //Protótipo da função interna que modifica a posição e o tamanho do tambor de uma barra de rolagem
int _rolar_janela(JANELA janela, int quantidade, int direcao); //Protótipo da função interna que faz a rolagem de uma janela
int _caixa_dialogo(wchar_t *titulo, wchar_t *texto, unsigned int opcoes, JANELA janela); //Protótipo da função interna que cria uma caixa de diálogo
int _destruir_janela(JANELA janela); //Protótipo da função interna que destrói uma janela
void _sair(); //Protótipo da função interna que sai do programa
static JANELA procurar_janela(HWND hwnd); //Protótipo da função que procura a janela associada a determinado HWND




///Variáveis globais
HINSTANCE hInstance;
HWND janela_hwnd;
JANELA lista_janelas = NULL;
COLORREF _cor_de_fundo = 0; //Variável para armazenar a cor de fundo da janela
HBRUSH pincel; //Variável para armazenar o pincel que preenche a cor de fundo do texto estático
EVENTO _evento = {(JANELA) -1, -1, -1, -1}; //Variável para armazenar o evento
int ultimo_evento_processado = 1;
WNDPROC WinProcPadrao;




LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){

    JANELA auxiliar;

    if(_evento.evento != 0 && hwnd == janela_hwnd && ultimo_evento_processado == 1){
        _evento.evento = -1;
        _evento.parametro1 = 0;
        _evento.parametro2 = 0;
    }


    switch(msg){
        case WM_CREATE:
            break;

        case WM_DESTROY:
            _evento.evento = 0;
            PostQuitMessage(wp);
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
                    _evento.evento = EV_ROLAGEM;
                    _evento.parametro1 = ROL_ABAIXO_VERTICAL;
                    break;

                case SB_LINEUP:
                    _evento.janela = 0;
                    _evento.evento = EV_ROLAGEM;
                    _evento.parametro1 = ROL_ACIMA_VERTICAL;
                    break;

                case SB_THUMBTRACK:
                    _evento.janela = 0;
                    _evento.evento = EV_ROLAGEM;
                    _evento.parametro1 = ROL_ARRASTADA_VERTICAL;
                    break;
                default:
                    break;
            }
            break;

        case WM_HSCROLL:
            switch(LOWORD(wp)){

                case SB_LINELEFT:
                    _evento.janela = procurar_janela(hwnd);
                    if(_evento.janela != (JANELA) -2)
                        ultimo_evento_processado = 0;
                    else
                        _evento.janela = 0;
                    _evento.evento = EV_ROLAGEM;
                    _evento.parametro1 = ROL_ABAIXO_HORIZONTAL;

                    break;

                case SB_LINERIGHT:
                    _evento.janela = procurar_janela(hwnd);
                    if(_evento.janela != (JANELA) -2)
                        ultimo_evento_processado = 0;
                    else
                        _evento.janela = 0;
                    _evento.evento = EV_ROLAGEM;
                    _evento.parametro1 = ROL_ACIMA_HORIZONTAL;
                    break;

                case SB_THUMBTRACK:
                    _evento.janela = procurar_janela(hwnd);
                    if(_evento.janela != (JANELA) -2)
                        ultimo_evento_processado = 0;
                    else
                        _evento.janela = 0;
                    _evento.evento = EV_ROLAGEM;
                    _evento.parametro1 = ROL_ARRASTADA_HORIZONTAL;
                    break;
                default:
                    break;
            }
            break;

        case WM_CTLCOLORSTATIC:

            auxiliar = lista_janelas;

            do{
                if(auxiliar->tipo == 0 && auxiliar->pintada == 0){

                    SetTextColor((HDC) wp, auxiliar->cor);
                    SetBkColor((HDC) wp, auxiliar->cor_de_fundo);
                    auxiliar->pintada = 1;
                    pincel = CreateSolidBrush(auxiliar->cor_de_fundo);
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

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);

            JANELA janela = procurar_janela(hwnd);

            HBRUSH brush = CreateSolidBrush(janela->cor_de_fundo);
            HPEN pen = CreatePen(PS_SOLID, 0, janela->cor_de_fundo);

            SelectObject(ps.hdc, (HGDIOBJ) brush);
            SelectObject(ps.hdc, (HGDIOBJ) pen);
            Rectangle(ps.hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);

            DeleteObject((HGDIOBJ) brush);
            DeleteObject((HGDIOBJ) pen);

            EndPaint(hwnd, &ps);
        }
            break;
        default:
            DefWindowProcW(hwnd, msg, wp, lp);
    }

}




LRESULT WINAPI WinProcNovo(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){

    switch(msg){

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);

            JANELA janela = procurar_janela(hwnd);

            HBRUSH brush = CreateSolidBrush(janela->cor_de_fundo);
            HPEN pen = CreatePen(PS_SOLID, 0, janela->cor_de_fundo);

            SelectObject(ps.hdc, (HGDIOBJ) brush);
            SelectObject(ps.hdc, (HGDIOBJ) pen);
            Rectangle(ps.hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
            SetTextColor(ps.hdc, janela->cor);
            SetBkColor(ps.hdc, janela->cor_de_fundo);
            RECT area;
            area.left = 0;
            area.right = janela->largura;
            area.top = 0;
            area.bottom = janela->altura;
            SetTextAlign(ps.hdc, TA_TOP | TA_LEFT);
            DrawTextW(ps.hdc, janela->texto, -1, &area, 0);

            DeleteObject((HGDIOBJ) pen);
            DeleteObject((HGDIOBJ) brush);

            EndPaint(hwnd, &ps);
        }
        break;

        default:
            CallWindowProcW(WinProcPadrao, hwnd, msg, wp, lp);
            break;
    }

}




int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow){

    hInstance = hThisInstance; //Globaliza a instância para acesso pela função _iniciar_janela

    return main_biblioteca(); //Chama a função principal da GUIndaste e retorna o valor que o usuário desejar

}




int _iniciar_janela(wchar_t *titulo, int x, int y, int largura, int altura, COLORREF cor, unsigned long int opcoes){

    _cor_de_fundo = cor;

    WNDCLASSW janela = {0};

    janela.hbrBackground = CreateSolidBrush(cor);
    janela.hCursor = LoadCursor(NULL, IDC_ARROW);
    janela.hInstance = hInstance;
    janela.lpszClassName = L"janela";
    janela.lpfnWndProc = &WinProc;

    if(!RegisterClassW(&janela))
        return -1;

    DWORD estilo = WS_VISIBLE | WS_OVERLAPPEDWINDOW;

    if(opcoes & 0x1)
        estilo |= WS_VSCROLL;

    if(opcoes & 0x2)
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

    if(GetMessageW(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    ultimo_evento_processado = 1;

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


}




JANELA _criar_janela(int tipo, wchar_t *texto, int x, int y, int largura, int altura, int comando, COLORREF cor, COLORREF cor_de_fundo, JANELA pai, unsigned long int opcoes){

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
            if(opcoes & 0x1)
                estilo |= WS_VSCROLL;

            if(opcoes & 0x2)
                estilo |= WS_HSCROLL;

            snwprintf(tipo_janela, 14, L"janela");
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
        if(tipo == JANELA_TEXTO){
            WinProcPadrao = (WNDPROC) SetWindowLongW(nova->hWnd, GWLP_WNDPROC, (LONG_PTR) WinProcNovo);
        }
        nova->tipo = tipo;
        wcscpy(nova->texto, texto);
        nova->x = x;
        nova->y = y;
        nova->largura = largura;
        nova->altura = altura;
        nova->comando = comando;
        nova->cor = cor;
        nova->cor_de_fundo = cor_de_fundo;
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

            if(janela->hWnd_pai == NULL)
                ScreenToClient(janela_hwnd, &xy);
            else
                ScreenToClient(janela->hWnd_pai, &xy);

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




//Função interna que retorna a posição de uma barra de rolagem
int _obter_posicao_barra_rol(JANELA janela, int opcao){

    JANELA auxiliar = lista_janelas;

    if(janela){
        do{
            if(auxiliar == janela)
                return GetScrollPos(auxiliar->hWnd, opcao);

            auxiliar = auxiliar->prox;

        }while(auxiliar != lista_janelas);
    }else{
        return GetScrollPos(janela_hwnd, opcao);
    }

    return 0;

}




//Função interna que retorna o tamanho do tambor de uma barra de rolagem
int _obter_tamanho_barra_rol(JANELA janela, int opcao){

    JANELA auxiliar = lista_janelas;

    SCROLLINFO info_barra_rol = {0};
    info_barra_rol.fMask = SIF_PAGE;

    if(janela){
        do{
            if(auxiliar == janela){
                GetScrollInfo(auxiliar->hWnd, opcao, &info_barra_rol);
                return info_barra_rol.nPage;
            }

            auxiliar = auxiliar->prox;

        }while(auxiliar != lista_janelas);

    }else{
        GetScrollInfo(janela_hwnd, opcao, &info_barra_rol);
        return info_barra_rol.nPage;
    }

    return 0;

}




//Função interna que modifica a posição e o tamanho do tambor de uma barra de rolagem
int _modificar_barra_rol(JANELA janela, int posicao, int tamanho, int minimo, int maximo, int opcao){

    JANELA auxiliar = lista_janelas;

    SCROLLINFO info_barra_rol = {0};

    info_barra_rol.cbSize = sizeof(info_barra_rol);
    info_barra_rol.nPage = tamanho;
    info_barra_rol.nPos = posicao;
    info_barra_rol.nMin = minimo;
    info_barra_rol.nMax = maximo;
    info_barra_rol.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;

    if(janela){
        do{
            if(auxiliar == janela){
                SetScrollInfo(auxiliar->hWnd, opcao, &info_barra_rol, TRUE);
                break;
            }

            auxiliar = auxiliar->prox;

        }while(auxiliar != lista_janelas);
    }else{
        SetScrollInfo(janela_hwnd, opcao, &info_barra_rol, TRUE);
    }

    return 0;

}




//Função interna que faz a rolagem de uma janela
int _rolar_janela(JANELA janela, int quantidade, int direcao){

    JANELA auxiliar = lista_janelas;

    do{
        if(auxiliar == janela){
            if(direcao == 0)
                ScrollWindow(janela->hWnd, quantidade * -1, 0, NULL, NULL);
            else
                ScrollWindow(janela->hWnd, 0, quantidade * -1, NULL, NULL);
        }

        auxiliar = auxiliar->prox;

    }while(lista_janelas != auxiliar);

    return 0;

}





int _caixa_dialogo(wchar_t *titulo, wchar_t *texto, unsigned int opcoes, JANELA janela){

    HWND hwnd_janela;

    if(janela){
        if(lista_janelas){
            JANELA auxiliar = lista_janelas;
            do{
                if(auxiliar == janela)
                    hwnd_janela = auxiliar->hWnd;

                auxiliar = auxiliar->prox;
            }while(auxiliar != lista_janelas);
        }else
            hwnd_janela = janela_hwnd;
    }else
        hwnd_janela = janela_hwnd;

    return MessageBoxW(hwnd_janela, texto, titulo, opcoes);

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
