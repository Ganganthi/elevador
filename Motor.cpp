#include "Motor.h"

Motor::Motor(/* args */):estado_atual(Estados::parado)
{
    std::cout << "Criando motor! " << this << std::endl;
}

Motor::~Motor()
{
    std::cout << "Destruindo motor! " << this << std::endl;
}