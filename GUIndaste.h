/***********************
GUIndaste.h
Autor: Bruno Trojahn Bolzan
Colaboradores: ----
Descrição: Esta é uma biblioteca que oferece funções para a criação de programas com uma interface gráfica rudimentar.
************************/




///Inclusão de bibliotecas
#include "BaseWindows.h" //Inclusão da biblioteca base para Windows, inclua a equivalente em Linux se necessário




///Definições de tipo




///Macros
#define cor_rgb(r,g,b) ((unsigned int)((unsigned char)(r)|((unsigned char)(g) << 8)|((unsigned char)(b) << 16))) //Macro para criar um inteiro baseado no padrão RGB
#define SEM_ROLAGEM_VERTICAL 0 //Flag para janela sem barra de rolagem vertical
#define SEM_ROLAGEM_HORIZONTAL 0 //Flag para janela sem barra de rolagem horizontal
#define ROLAGEM_VERTICAL 1 //Flag para janela com barra de rolagem vertical
#define ROLAGEM_HORIZONTAL 1 //Flag para janela com barra de rolagem horizontal
#define JANELA_TEXTO 0 //Macro para janela de texto estático
#define JANELA_BOTAO 1 //Macro para janela botão
#define JANELA_ENTRADA 2 //Macro para janela de texto editável
#define BOTAO_PRESSIONADO 0 //Macro para evento de botão pressionado
#define ROLAGEM_ABAIXO_VERTICAL 1 //Macro para evento de rolagem abaixo vertical
#define ROLAGEM_ACIMA_VERTICAL 2 //Macro para evento de rolagem acima vertical
#define ROLAGEM_ARRASTADA_VERTICAL 3 //Macro para evento de rolagem arrastada vertical
#define ROLAGEM_ABAIXO_HORIZONTAL 4 //Macro para evento de rolagem abaixo horizontal
#define ROLAGEM_ACIMA_HORIZONTAL 5 //Macro para evento de rolagem acima horizontal
#define ROLAGEM_ARRASTADA_HORIZONTAL 6 //Macro para evento de rolagem arrastada horizontal




///Protótipos de funções
int main_GUIndaste(); //Protótipo da função principal do usuário
int iniciar_janela(wchar_t *titulo, int x, int y, int largura, int altura, unsigned int cor, void (*callback)(int evento, int parametro), int rolagem_vertical, int rolagem_horizontal); //Protótipo da função criadora de uma janela principal
void sair(); //Protótipo da função para sair do programa
JANELA criar_janela(int tipo, wchar_t *texto, int x, int y, int largura, int altura, int comando, unsigned int cor); //Protótipo da função que cria janelas
int obter_texto_janela(wchar_t *destino, JANELA janela); //Protótipo da função que obtém o texto de uma janela
int obter_janela_x(JANELA janela); //Protótipo da função que retorna a posição horizontal de uma janela
int obter_janela_y(JANELA janela); //Protótipo da função que retorna a posição vertical de uma janela
int modificar_janela_xy(JANELA janela, int x, int y); //Protótipo da função que modifica a posição de uma janela
int destruir_janela(JANELA janela); //Protótipo da função que destroi uma janela




///Variáveis globais




///Função principal da GUIndaste
int main_biblioteca(){

    int retorno = main_GUIndaste(); //Chama a função do usuário e armazena o retorno

    return retorno; //Retorna o valor que o usuário desejar
}




//Função para iniciar janela
int iniciar_janela(wchar_t *titulo, int x, int y, int largura, int altura, unsigned int cor, void (*callback)(int evento, int parametro), int rolagem_vertical, int rolagem_horizontal){

    callback_function = callback; //Guarda o ponteiro para a função gestora de eventos do usuário da biblioteca

    //Chama a função interna equivalente e retorna -1 caso ela falhe
    if(_iniciar_janela(titulo, x, y, largura, altura, cor, callback, rolagem_vertical, rolagem_horizontal))
        return -1;

    return 0; //Retorna 0 se a janela for iniciada
}




//Função que cria janela
JANELA criar_janela(int tipo, wchar_t *texto, int x, int y, int largura, int altura, int comando, unsigned int cor){

    //Chama a função interna de criar janela e armazena o retorno
    JANELA retorno_janela = _criar_janela(tipo, texto, x, y, largura, altura, comando, cor);

    return retorno_janela; //Retorna a janela criada
}




//Função que lê os eventos
int ler_evento(){

    _ler_evento(); //Chama a função interna leitora de eventos

    return flag_sair; //Retorna de acordo com a flag
}




//Função que obtém o texto de uma determinada janela
int obter_texto_janela(wchar_t *destino, JANELA janela){

    return _obter_texto_janela(destino, janela);

}




//Função que obtém a posição horizontal de uma janela
int obter_janela_x(JANELA janela){

    return _obter_janela_x(janela);

}




//Função que obtém a posição horizontal de uma janela
int obter_janela_y(JANELA janela){

    return _obter_janela_y(janela);

}




//Função que modifica a posição de uma janela
int modificar_janela_xy(JANELA janela, int x, int y){

    return _modificar_janela_xy(janela, x, y);

}




//Função que destroi uma janela
int destruir_janela(JANELA janela){

    return _destruir_janela(janela);

}




//Função que termina o programa
void sair(){

    _sair(); //Chama a função interna de sair

    flag_sair = 0; //Modifica a flag para término do programa

}
