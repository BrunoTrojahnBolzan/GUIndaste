#include "GUIndaste.h"
#include <stdio.h>
#include <stdlib.h>

#define TAM 200

typedef struct info{

    int codigo;
    wchar_t nome[50];

}INFO;

typedef struct aluno{

    INFO informacao;
    struct aluno *ant;
    struct aluno *prox;
    JANELA janela;

}ALUNO;

void criar_menu(void *dados_genericos);
void criar_inserir(void *insercoes);
void inserir(void *dados);
void destruir_inserir(JANELA *insercoes);
void criar_excluir(void *exclusoes);
void excluir(void *dados);
void destruir_excluir(JANELA *exclusoes);
void criar_alterar(void *alteracoes);
void alterar(void *dados);
void destruir_alterar(JANELA *alterar);
void criar_buscar(void *buscas);
void buscar(void *dados);
void destruir_buscar(JANELA *buscas);
void destruir_tudo(JANELA **dados_gen);
void sobre(void *dados);
void sai(void *dados);
int inserir_aluno(ALUNO **l, INFO informacao, JANELA lista);
int excluir_aluno(ALUNO **l, int codigo, JANELA lista);
void alterar_aluno(ALUNO *aluno, INFO info);
ALUNO *buscar_aluno(ALUNO **l, int codigo);

int criadas[5] = {0};

int min_rol = 0;
int max_rol = 0;
int num_alunos = 0;
int tamanho_rol = 0;
int posicao_rol = 0;

int main_GUIndaste(){
    ALUNO *l = NULL;

    JANELA botoes[6] = {0};
    JANELA insercoes[5] = {0};
    JANELA exclusoes[3] = {0};
    JANELA alteracoes[7] = {0};
    JANELA buscas[3] = {0};
    JANELA texto_sobre = NULL;

    iniciar_janela(L"Programa com lista", 0, 0, 1280, 700, cor_rgb(110, 140, 240), 0);
    EVENTO evento;
    JANELA lista = criar_janela(JANELA_GENERICA, L"", 20, 400, 6 * 150, 100, NULL, NULL, cor_rgb(255, 255, 255), cor_rgb(255, 255, 255), NULL, ROLAGEM_HORIZONTAL);
    void *dados_genericos[] = {(void*) botoes, (void*) insercoes, (void*) exclusoes, (void*) alteracoes, (void*) buscas, (void*) &l, (void*) lista, (void*) &texto_sobre};
    criar_menu(dados_genericos);

    modificar_barra_rol(lista, 0, 901, 0, 900, 0);

    do{
        evento = ler_evento();
        switch(evento.evento){
            case EV_ROLAGEM:
                switch(evento.parametro1){
                    case ROL_ABAIXO_HORIZONTAL:
                        if(posicao_rol > min_rol){
                            if(posicao_rol - 10 >= min_rol)
                                posicao_rol -= 10;
                            else
                                posicao_rol = min_rol;
                            modificar_barra_rol(lista, posicao_rol, tamanho_rol, min_rol, max_rol, 0);
                            rolar_janela(lista, -10, 0);
                        }
                        break;
                    case ROL_ACIMA_HORIZONTAL:
                        if(posicao_rol <= (max_rol - tamanho_rol)){
                            if(posicao_rol + 10 <= max_rol)
                                posicao_rol += 10;
                            else
                                posicao_rol = max_rol;
                            modificar_barra_rol(lista, posicao_rol, tamanho_rol, min_rol, max_rol, 0);
                            rolar_janela(lista, 10, 0);
                        }
                        break;
                    case ROL_ARRASTADA_HORIZONTAL:
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }while(evento.evento);

    return 0;
}

void criar_menu(void *dados_genericos){

    wchar_t texto_botoes[6][50] = {L"Inserir", L"Excluir", L"Alterar", L"Buscar", L"Sair", L"Sobre"};

    ACAO acoes[6] = {&criar_inserir, &criar_excluir, &criar_alterar, &criar_buscar, &sai, &sobre};

    JANELA *botoes = ((JANELA**) dados_genericos)[0];

    for(int i = 0; i < 6; i++)
        botoes[i] = criar_janela(JANELA_BOTAO, texto_botoes[i], (i * 150) + 20, 20, 150, 50, acoes[i], (void*) dados_genericos, 0, 0, NULL, 0);
}

void criar_inserir(void *dados_gen){

    if(!(criadas[0] == 0 && criadas[1] == 0 && criadas[2] == 0 && criadas[3] == 0 && criadas[4] == 0))
        destruir_tudo((JANELA**) dados_gen);
    JANELA *insercoes = ((JANELA**) dados_gen)[1];

    insercoes[0] = criar_janela(JANELA_TEXTO, L"Código:", 20, 80, 100, 50, (ACAO) NULL, (void*) NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    insercoes[1] = criar_janela(JANELA_TEXTO, L"Nome:", 20, 180, 100, 50, (ACAO) NULL, NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    insercoes[2] = criar_janela(JANELA_ENTRADA, L"", 80, 80, 100, 50, (ACAO) NULL, NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    insercoes[3] = criar_janela(JANELA_ENTRADA, L"", 80, 180, 700, 50, (ACAO) NULL, NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    insercoes[4] = criar_janela(JANELA_BOTAO, L"Inserir", 820, 180, 100, 50, (ACAO) &inserir, dados_gen, 0, 0, NULL, 0);

    criadas[0] = 1;

}

void destruir_inserir(JANELA *insercoes){

    for(int i = 0; i < 5; i++)
        destruir_janela(insercoes[i]);

    criadas[0] = 0;

}

void criar_excluir(void *dados_gen){

    if(!(criadas[0] == 0 && criadas[1] == 0 && criadas[2] == 0 && criadas[3] == 0 && criadas[4] == 0))
        destruir_tudo((JANELA**) dados_gen);

    JANELA *exclusoes = ((JANELA**) dados_gen)[2];

    exclusoes[0] = criar_janela(JANELA_TEXTO, L"Código:", 20, 80, 100, 50, NULL, NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    exclusoes[1] = criar_janela(JANELA_ENTRADA, L"", 80, 80, 100, 50, NULL, NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    exclusoes[2] = criar_janela(JANELA_BOTAO, L"Excluir", 220, 80, 100, 50, &excluir, dados_gen, 0, 0, NULL, 0);

    criadas[1] = 1;

}

void destruir_excluir(JANELA *exclusoes){

    for(int i = 0; i < 3; i++)
        destruir_janela(exclusoes[i]);

    criadas[1] = 0;

}

void criar_alterar(void *dados_gen){

    if(!(criadas[0] == 0 && criadas[1] == 0 && criadas[2] == 0 && criadas[3] == 0 && criadas[4] == 0))
        destruir_tudo((JANELA**) dados_gen);

    JANELA *alteracoes = ((JANELA**) dados_gen)[3];

    alteracoes[0] = criar_janela(JANELA_TEXTO, L"Código a\nalterar:", 20, 80, 100, 50, (ACAO) NULL, (void*) NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    alteracoes[1] = criar_janela(JANELA_TEXTO, L"Código:", 20, 180, 100, 50, (ACAO) NULL, (void*) NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    alteracoes[2] = criar_janela(JANELA_TEXTO, L"Nome:", 20, 280, 100, 50, (ACAO) NULL, NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    alteracoes[3] = criar_janela(JANELA_ENTRADA, L"", 80, 80, 100, 50, (ACAO) NULL, NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    alteracoes[4] = criar_janela(JANELA_ENTRADA, L"", 80, 180, 100, 50, (ACAO) NULL, NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    alteracoes[5] = criar_janela(JANELA_ENTRADA, L"", 80, 280, 700, 50, (ACAO) NULL, NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    alteracoes[6] = criar_janela(JANELA_BOTAO, L"Alterar", 820, 280, 100, 50, (ACAO) &alterar, dados_gen, 0, 0, NULL, 0);

    criadas[2] = 1;

}

void destruir_alterar(JANELA *alteracoes){

    for(int i = 0; i < 7; i++)
        destruir_janela(alteracoes[i]);

    criadas[3] = 0;

}

void criar_buscar(void *dados_gen){

    if(!(criadas[0] == 0 && criadas[1] == 0 && criadas[2] == 0 && criadas[3] == 0 && criadas[4] == 0))
        destruir_tudo((JANELA**) dados_gen);

    JANELA *buscas = ((JANELA**) dados_gen)[4];

    buscas[0] = criar_janela(JANELA_TEXTO, L"Código:", 20, 80, 100, 50, NULL, NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    buscas[1] = criar_janela(JANELA_ENTRADA, L"", 80, 80, 100, 50, NULL, NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);
    buscas[2] = criar_janela(JANELA_BOTAO, L"Buscar", 220, 80, 100, 50, &buscar, dados_gen, 0, 0, NULL, 0);

    criadas[4] = 1;

}

void destruir_buscar(JANELA *buscas){

    for(int i = 0; i < 3; i++)
        destruir_janela(buscas[i]);

    criadas[3] = 0;

}

void destruir_tudo(JANELA **dados_gen){

    destruir_inserir(((JANELA**) dados_gen)[1]);
    destruir_excluir(((JANELA**) dados_gen)[2]);
    destruir_alterar(((JANELA**) dados_gen)[3]);
    destruir_buscar(((JANELA**) dados_gen)[4]);
    destruir_janela(((JANELA*) dados_gen)[7]);

}

int inserir_aluno(ALUNO **l, INFO informacao, JANELA lista){
    if(!buscar_aluno(l, informacao.codigo)){
        ALUNO *novo_aluno = malloc(sizeof(ALUNO));

        novo_aluno->informacao = informacao;

        wchar_t buffer[100] = {0};

        snwprintf(buffer, 99, L" Código: %d\n Nome: %ls", informacao.codigo, informacao.nome);

        int x = (TAM * num_alunos) + 10;

        if(num_alunos > 0)
            x += 20 * num_alunos;

        novo_aluno->janela = criar_janela(JANELA_TEXTO, buffer, x, 20, TAM, 50, NULL, NULL, cor_rgb(0, 0, 0), cor_rgb(114, 247, 11), lista, 0);

        num_alunos++;

        max_rol = num_alunos * (TAM + 20);
        float relacao = (float) max_rol / (4 * (TAM + 20));
        if(relacao)
            tamanho_rol = (int) (max_rol / relacao);
        else
            tamanho_rol = 0;

        modificar_barra_rol(lista, 0, tamanho_rol, min_rol, max_rol, 0);

        if(*l){
            novo_aluno->ant = (*l)->ant;
            novo_aluno->prox = *l;
            (*l)->ant->prox = novo_aluno;
            (*l)->ant = novo_aluno;
        }else{
            novo_aluno->ant = novo_aluno;
            novo_aluno->prox = novo_aluno;
            *l = novo_aluno;
        }

        return 1;
    }

    return 0;
}

int excluir_aluno(ALUNO **l, int codigo, JANELA lista){

    ALUNO *excluido = buscar_aluno(l, codigo);

    if(excluido){

        ALUNO *aux = excluido;

        destruir_janela(excluido->janela);

        do{
            modificar_janela_xy(aux->janela, obter_janela_x(aux->janela) - TAM - 20, 20);
            aux = aux->prox;
        }while(*l != aux);

        excluido->ant->prox = excluido->prox;
        excluido->prox->ant = excluido->ant;
        if(*l == excluido)
            *l = excluido->prox;
        if(*l == excluido)
            *l = NULL;

        free(excluido);

        num_alunos--;

        max_rol = num_alunos * TAM + 20 * num_alunos;

        if(num_alunos > 0)
            tamanho_rol = (max_rol / num_alunos) + 300;
        else
            tamanho_rol = 300;

        modificar_barra_rol(lista, 0, tamanho_rol, min_rol, max_rol, 0);

        return 1;
    }else
        caixa_dialogo(L"Aviso", L"O código não corresponde a algum aluno.", CD_OK | CD_ICONE_AVISO, NULL);

    return 0;

}

void alterar_aluno(ALUNO *aluno, INFO info){
    aluno->informacao = info;
    wchar_t buffer[100] = {0};
    snwprintf(buffer, 99, L" Código: %d\n Nome: %ls", info.codigo, info.nome);
    modificar_texto_janela(aluno->janela, buffer);
    caixa_dialogo(L"Sucesso!", L"Informações do aluno alteradas", CD_ICONE_INFORMACAO, CD_OK);
}

void inserir(void *dados){
    wchar_t buffer[50] = {0};
    obter_texto_janela(buffer, ((JANELA**) dados)[1][2]);
    INFO auxiliar = {0};
    swscanf(buffer, L"%d", &auxiliar.codigo);
    obter_texto_janela(auxiliar.nome, ((JANELA**) dados)[1][3]);

    int achou = inserir_aluno(((ALUNO***) dados)[5], auxiliar, ((JANELA*) dados)[6]);

    if(achou)
        caixa_dialogo(L"Aluno inserido", L"O aluno foi inserido.", CD_OK | CD_ICONE_INFORMACAO, NULL);
    else
        caixa_dialogo(L"Aviso", L"O código já corresponde a algum aluno.", CD_OK | CD_ICONE_AVISO, NULL);

    destruir_tudo(dados);
}

void excluir(void *dados){
    wchar_t buffer[50] = {0};
    obter_texto_janela(buffer, ((JANELA**) dados)[2][1]);
    INFO auxiliar = {0};
    swscanf(buffer, L"%d", &auxiliar.codigo);

    int achou = excluir_aluno(((ALUNO***) dados)[5], auxiliar.codigo, ((JANELA*) dados)[6]);

    if(achou)
        caixa_dialogo(L"Aluno excluído", L"O aluno foi excluído.", CD_OK | CD_ICONE_INFORMACAO, NULL);

    destruir_tudo(dados);
}

void alterar(void *dados){
    wchar_t buffer[50] = {0};
    obter_texto_janela(buffer, ((JANELA**) dados)[3][3]);
    int codigo = 0;
    swscanf(buffer, L"%d", &codigo);
    INFO info = {0};
    obter_texto_janela(buffer, ((JANELA**) dados)[3][4]);
    swscanf(buffer, L"%d", &info.codigo);
    obter_texto_janela(info.nome, ((JANELA**) dados)[3][5]);

    ALUNO *aluno = buscar_aluno(((ALUNO***) dados)[5], codigo);

    if(aluno){
        if(buscar_aluno(((ALUNO***) dados)[5], info.codigo) == NULL)
            alterar_aluno(aluno, info);
        else
            caixa_dialogo(L"Erro", L"Código já existente", CD_ICONE_ERRO | CD_OK, NULL);
    }
    else
        caixa_dialogo(L"Erro", L"Aluno não encontrado", CD_ICONE_ERRO | CD_OK, NULL);


    destruir_tudo(dados);
}

void buscar(void *dados){
    wchar_t buffer[50] = {0};
    obter_texto_janela(buffer, ((JANELA**) dados)[4][1]);
    INFO auxiliar = {0};
    swscanf(buffer, L"%d", &auxiliar.codigo);

    ALUNO *aluno_buscado = buscar_aluno(((ALUNO***) dados)[5], auxiliar.codigo);

    if(aluno_buscado){
        snwprintf(buffer, 49, L"Código: %d\nNome: %ls", aluno_buscado->informacao.codigo, aluno_buscado->informacao.nome);
        caixa_dialogo(L"Aluno encontrado", buffer, CD_OK | CD_ICONE_INFORMACAO, NULL);
    }else
        caixa_dialogo(L"Aviso", L"O código não corresponde a algum aluno.", CD_OK | CD_ICONE_AVISO, NULL);

    destruir_tudo(dados);
}

ALUNO *buscar_aluno(ALUNO **l, int codigo){
    if(*l){
        ALUNO *auxiliar = *l;
        do{
            if(auxiliar->informacao.codigo == codigo)
                return auxiliar;
            auxiliar = auxiliar->prox;
        }while(auxiliar != *l);
    }
    return NULL;
}

void sobre(void *dados){
    if(!(criadas[0] == 0 && criadas[1] == 0 && criadas[2] == 0 && criadas[3] == 0))
        destruir_tudo((JANELA**) dados);

    ((JANELA*) dados)[7] = criar_janela(JANELA_TEXTO, L"Adicionar texto aqui.", 20, 90, 900, 300, NULL, NULL, cor_rgb(0, 0, 0), cor_rgb(110, 140, 240), NULL, 0);

    criadas[4] = 1;
}

void sai(void *dados){
    sair();
}
