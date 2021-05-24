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
#define ROLAGEM_VERTICAL 0x00000001 //Opção para rolagem vertical
#define ROLAGEM_HORIZONTAL 0x00000002 //Opção para rolagem horizontal
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
#define CD_ABORTAR_TENTAR_IGNORAR 0x00000002L
#define CD_CANCELAR_TENTAR_CONTINUAR 0x00000006L
#define CD_AJUDA 0x00004000L
#define CD_OK 0x00000000L
#define CD_OK_CANCELAR 0x00000001L
#define CD_TENTAR_CANCELAR 0x00000005L
#define CD_SIM_NAO 0x00000004L
#define CD_SIM_NAO_CANCELAR 0x00000003L
#define CD_ICONE_AVISO 0x00000030L
#define CD_ICONE_INFORMACAO 0x00000040L
#define CD_ICONE_INTERROGACAO 0x00000020L
#define CD_ICONE_ERRO 0x00000010L
#define CD_RETORNO_ABORTAR 3
#define CD_RETORNO_CANCELAR 2
#define CD_RETORNO_CONTINUAR 11
#define CD_RETORNO_IGNORAR 5
#define CD_RETORNO_NAO 7
#define CD_RETORNO_OK 1
#define CD_RETORNO_TENTAR1 4
#define CD_RETORNO_TENTAR2 10
#define CD_RETORNO_SIM 6




///Protótipos de funções
int main_GUIndaste(); //Protótipo da função principal do usuário
int iniciar_janela(wchar_t *titulo, int x, int y, int largura, int altura, unsigned int cor, unsigned long int opcoes); //Protótipo da função criadora de uma janela principal
void sair(); //Protótipo da função para sair do programa
JANELA criar_janela(int tipo, wchar_t *texto, int x, int y, int largura, int altura, ACAO acao, void *dados, unsigned int cor, unsigned int cor_de_fundo, JANELA pai, unsigned long int opcoes); //Protótipo da função que cria janelas
int obter_texto_janela(wchar_t *destino, JANELA janela); //Protótipo da função que obtém o texto de uma janela
int obter_janela_x(JANELA janela); //Protótipo da função que retorna a posição horizontal de uma janela
int obter_janela_y(JANELA janela); //Protótipo da função que retorna a posição vertical de uma janela
int modificar_texto_janela(JANELA janela, wchar_t *texto); //Protótipo da função que modifica o texto de uma janela
int modificar_janela_xy(JANELA janela, int x, int y); //Protótipo da função que modifica a posição de uma janela
int destruir_janela(JANELA janela); //Protótipo da função que destrói uma janela
int modificar_posicao_barra_progresso(JANELA janela, int nova_posicao); //Protótipo da função que modifica a posição de uma barra de progresso
int caixa_dialogo(wchar_t *titulo, wchar_t *texto, unsigned int opcoes, JANELA janela); //Protótipo da função que cria uma caixa de diálogo
int obter_posicao_barra_rol(JANELA janela, int opcao); //Protótipo da função que retorna a posição de uma barra de rolagem
int obter_tamanho_barra_rol(JANELA janela, int opcao); //Protótipo da função que retorna o tamanho do tambor de uma barra de rolagem
int modificar_barra_rol(JANELA janela, int posicao, int tamanho, int minimo, int maximo, int opcao); //Protótipo da função que modifica a posição e o tamanho do tambor de uma barra de rolagem
int rolar_janela(JANELA janela, int quantidade, int direcao); //Protótipo da função que faz a rolagem de uma janela




///Variáveis globais




///Função principal da GUIndaste
int main_biblioteca(){

    return main_GUIndaste(); //Chama a função do usuário e retorna o valor que o usuário desejar

}




//Função para iniciar janela
int iniciar_janela(wchar_t *titulo, int x, int y, int largura, int altura, unsigned int cor, unsigned long int opcoes){

    //Chama a função interna equivalente e retorna -1 caso ela falhe
    if(_iniciar_janela(titulo, x, y, largura, altura, cor, opcoes))
        return -1;

    return 0; //Retorna 0 se a janela for iniciada
}




//Função que cria janela
JANELA criar_janela(int tipo, wchar_t *texto, int x, int y, int largura, int altura, ACAO acao, void *dados, unsigned int cor, unsigned int cor_de_fundo, JANELA pai, unsigned long int opcoes){

    //Chama a função interna de criar janela e armazena o retorno
    JANELA retorno_janela = _criar_janela(tipo, texto, x, y, largura, altura, acao, dados, cor, cor_de_fundo, pai, opcoes);

    return retorno_janela; //Retorna a janela criada
}




//Função que lê os eventos
EVENTO ler_evento(){

    return _ler_evento(); //Chama a função interna leitora de eventos

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




//Função que modifica o texto de uma janela
int modificar_texto_janela(JANELA janela, wchar_t *texto){

    return _modificar_texto_janela(janela, texto);

}




//Função que modifica a posição de uma barra de progresso
int modificar_posicao_barra_progresso(JANELA janela, int nova_posicao){

    return _modificar_posicao_barra_progresso(janela, nova_posicao);

}




//Função que retorna a posição de uma barra de rolagem
int obter_posicao_barra_rol(JANELA janela, int opcao){

    return _obter_posicao_barra_rol(janela, opcao);

}




//Função que retorna o tamanho do tambor de uma barra de rolagem
int obter_tamanho_barra_rol(JANELA janela, int opcao){

    return _obter_tamanho_barra_rol(janela, opcao);

}




//Função que modifica a posição e o tamanho do tambor de uma barra de rolagem
int modificar_barra_rol(JANELA janela, int posicao, int tamanho, int minimo, int maximo, int opcao){

    return _modificar_barra_rol(janela, posicao, tamanho, minimo, maximo, opcao);

}




//Função que faz a rolagem de uma janela
int rolar_janela(JANELA janela, int quantidade, int direcao){

    return _rolar_janela(janela, quantidade, direcao);

}




//Função que cria uma caixa de diálogo
int caixa_dialogo(wchar_t *titulo, wchar_t *texto, unsigned int opcoes, JANELA janela){

    return _caixa_dialogo(titulo, texto, opcoes, janela);

}




//Função que destroi uma janela
int destruir_janela(JANELA janela){

    return _destruir_janela(janela);

}




//Função que termina o programa
void sair(){

    _sair(); //Chama a função interna de sair

    _evento.evento = 0; //Modifica a flag para término do programa

}
