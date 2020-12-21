/***********************
BaseWindows.h
Autor: Bruno Trojahn Bolzan
Colaboradores: ----
Descrição: Esta é a biblioteca base para Windows da biblioteca gráfica GUIndaste.
************************/




///Inclusão de bibliotecas
#include <windows.h> //Biblioteca do Windows
#include <string.h> //Biblioteca para manipulação de strings




///Definições
//Não há nada aqui ainda :v




///Definições de tipos

//Estrutura de janela
typedef struct janela{

    HWND hWnd;
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




///Protótipos de funções
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp); //Protótipo da função que administra as mensagens
int main_biblioteca(); //Protótipo da função principal da GUIndaste
int _iniciar_janela(wchar_t *titulo, int x, int y, int largura, int altura, COLORREF cor, void (*callback)(int evento, int parametro), int rolagem_vertical, int rolagem_horizontal); //Protótipo da função interna inicializadora de janela
int _ler_evento(); //Protótipo da função inteira leitora de eventos
JANELA _criar_janela(int tipo, wchar_t *texto, int x, int y, int largura, int altura, int comando, COLORREF cor); //Protótipo da função interna criadora de janelas
int _obter_texto_janela(wchar_t *destino, JANELA janela); //Protótipo da função interna que obtém o texto de uma janela
int _obter_janela_x(JANELA janela); //Protótipo da função interna que retorna a posição horizontal de uma janela
int _obter_janela_y(JANELA janela); //Protótipo da função interna que retorna a posição vertical de uma janela
int _modificar_janela_xy(JANELA janela, int x, int y); //Protótipo da função interna que modifica a posição de uma janela
int _destruir_janela(JANELA janela); //Protótipo da função interna que destroi uma janela
void _sair(); //Protótipo da função interna que sai do programa




///Variáveis globais
HINSTANCE hInstance;
HWND janela_hwnd;
void (*callback_function)(int evento, int parametro);
JANELA lista_janelas = NULL;
int flag_sair = -1;
COLORREF cor_de_fundo = 0;
HBRUSH pincel;




LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){

    JANELA auxiliar;

    switch(msg){
        case WM_CREATE:
            break;
        case WM_DESTROY:
            flag_sair = 0;
            PostQuitMessage(0);
            break;
        case WM_COMMAND: //Gerencia as mensagens de botão

            if(lista_janelas){

                JANELA auxiliar = lista_janelas;

                do{

                    if(auxiliar->comando == wp){
                        callback_function(0, wp);
                        break;
                    }

                    auxiliar = auxiliar->prox;

                }while(auxiliar != lista_janelas);

            }
            break;

        case WM_VSCROLL:

            switch(LOWORD(wp)){

                case SB_LINEDOWN:
                    callback_function(1, 0);
                    break;

                case SB_LINEUP:
                    callback_function(2, 0);
                    break;

                case SB_THUMBTRACK:
                    callback_function(3, HIWORD(wp));
                    break;
            }
            break;

        case WM_HSCROLL:
            switch(LOWORD(wp)){

                case SB_LINELEFT:
                    callback_function(4, 0);
                    break;

                case SB_LINERIGHT:
                    callback_function(5, 0);
                    break;

                case SB_THUMBTRACK:
                    callback_function(6, HIWORD(wp));
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
                    return (INT_PTR) pincel;
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

    int retorno = main_biblioteca(); //Chama a função principal da GUIndaste e armazena o retorno

    return retorno; //Retorna o valor que o usuário desejar
}




int _iniciar_janela(wchar_t *titulo, int x, int y, int largura, int altura, COLORREF cor, void (*callback)(int evento, int parametro), int rolagem_vertical, int rolagem_horizontal){

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




int _ler_evento(){

    MSG msg;

    if(GetMessageW(&msg, janela_hwnd, 0, 0)){

        TranslateMessage(&msg);
        DispatchMessageW(&msg);

    }
    return 1;
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




JANELA _criar_janela(int tipo, wchar_t *texto, int x, int y, int largura, int altura, int comando, COLORREF cor){

    if(tipo >= 0 && tipo < 3){
        JANELA nova = malloc(sizeof(struct janela));

        if(nova == NULL) //Se a alocação falhar, retorna um valor inválido
            return (JANELA) -2;

        wchar_t tipo_janela[10] = {0};

        DWORD estilo = WS_VISIBLE | WS_CHILD;

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

        }

        nova->hWnd = CreateWindowW(tipo_janela, texto, estilo, x, y, largura, altura, janela_hwnd, (HMENU) comando, NULL, NULL);
        nova->tipo = tipo;
        wcscpy(nova->texto, texto);
        nova->x = x;
        nova->y = y;
        nova->largura = largura;
        nova->altura = altura;
        nova->comando = comando;
        nova->cor = cor;
        nova->pintada = 0;
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
