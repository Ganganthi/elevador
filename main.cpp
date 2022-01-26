#include <iostream>

#include "Alarme.h"
#include "Controlador.h"
#include "Motor.h"
#include "Pessoa.h"
#include "Porta.h"


int main(int, char**) {
    Controlador elevador;
    Pessoa p(elevador, 1, 5);
    return 0;
}
