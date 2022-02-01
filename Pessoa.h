#ifndef PESSOA_H
#define PESSOA_H

#include <iostream>
#include <unistd.h>
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

    int origem;
    int destino;
    Controlador* elevador_;
public:
    Pessoa(Controlador*, int, int);
    ~Pessoa();

    void run(){
        while(1){
            elevador_->inc();
            sleep(3);
        }
        // for(int i=0; i<5; i++) std::cout << origem <<std::endl;
    }
};

//? Como a pessoa vai ter acesso ao controlador? (e consequentemente os botoes)

#endif