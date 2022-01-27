#include <iostream>
#include <thread>

#include "Alarme.h"
#include "Controlador.h"
#include "Motor.h"
#include "Pessoa.h"
#include "Porta.h"

// void run(int x){
//     while(1) std::cout << "2" << std::endl;
// }

int main(int, char**) {
    Controlador elevador;
    // Pessoa p(elevador, 1, 5);

    // std::thread th1(&Pessoa::run, Pessoa(&elevador, 2, 3));
    // std::thread th2(&Pessoa::run, Pessoa(&elevador, 5, 8));

    std::thread (&Pessoa::run, Pessoa(&elevador, -5, 3)).detach();
    while(1);   // Mantem o codigo funcionando

    return 0;
}
