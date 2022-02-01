#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <iostream>
#include <vector>
#include <mutex>
#include <unistd.h>

#include "Alarme.h"
#include "Porta.h"
#include "Motor.h"

class Controlador
{
private:
    Alarme alarme_;
    Porta porta_;
    Motor motor_;

    std::vector<int> fila_externa;
    std::vector<int> fila_interna;

    int cont;
public:
    Controlador();
    ~Controlador();

    void operator()(int &x){
        while(1){
            std::cout << x <<std::endl;
            sleep(1);
            // inc();
        }
    }

    void inc(){
        cont++;
    }
};


#endif