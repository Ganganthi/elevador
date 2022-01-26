#ifndef MOTOR_H
#define MOTOR_H

#include <iostream>

class Motor
{
private:
    enum Estados{
        parado,
        descendo,
        subindo
    };

    Estados estado_atual;
public:
    Motor(/* args */);
    ~Motor();

    //? Controle do andar atual fica no motor ou controlador?
    //? Quem define quando chegou em um novo andar? Motor ou controlador?
};


#endif