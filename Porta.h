#ifndef PORTA_H
#define PORTA_H

#include <iostream>

class Porta
{
private:
    enum Estados{
        fechado,
        aberto,
        fechando
    };

    Estados estado_atual;
public:
    Porta(/* args */);
    ~Porta();

    void AbrirPorta();
    void FecharPorta();
};

//? Timer para comecar a fechar a porta, e para terminar de fechar
//? Pesssoa passa na porta e reseta o tempo

//? Dessa forma, fechar nao seria usado (talvez com botao de abrir e fechar)

#endif