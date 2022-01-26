#ifndef PESSOA_H
#define PESSOA_H

#include <iostream>
#include "Controlador.h"

class Pessoa
{
private:
    enum Estados{
        parado,
        esperando_chegar,
        decidindo_entrar,   //! decidir se esse estado faz sentido
        dentro_elevador,
        fora_depois
    };

    Estados estado_atual;
public:
    Pessoa(Controlador&, int, int);
    ~Pessoa();
};

//? Como a pessoa vai ter acesso ao controlador? (e consequentemente os botoes)

#endif