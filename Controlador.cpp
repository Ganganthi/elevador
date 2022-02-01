#include "Controlador.h"

Controlador::Controlador(/* args */)
:alarme_()
,porta_()
,motor_()
{
    std::cout << "Criando controlador! " << this << std::endl;
    cont = 0;
}

Controlador::~Controlador()
{
    std::cout << "Destruindo controlador! " << this << std::endl;
}