#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <iostream>
#include <vector>

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
public:
    Controlador();
    ~Controlador();
};


#endif