#include <iostream>
#include <thread>
#include <unistd.h>
#include <queue>
#include <mutex>
#include <chrono>

#include "Alarme.h"
#include "Controlador.h"
#include "Motor.h"
#include "Pessoa.h"
#include "Porta.h"

const int andar_max = 6;            // num max de andares
bool working = true;
const uint64_t t_ms_andar = 2000;   // 2 segundos

uint64_t getTime(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

enum EstadosPorta{
    fechado,
    aberto,
    fechando
};
enum EstadosPessoa{
    parado,
    esperando_chegar,
    decidindo_entrar,   //! decidir se esse estado faz sentido
    dentro_elevador,
    fora_depois
};
enum EstadosMotor{
    desligado,
    descendo,
    subindo
};

void Elevador(std::queue<int> &fila, std::mutex &m_fila)
{
    // Se porta aberta, esperar fechar
    // Se ha elementos na fila, mover
    // Checar o andar, quando chegar, enviar comando de parar motor e abrir porta
    while(1){
        m_fila.lock();
        sleep(10);
        m_fila.unlock();
        sleep(10);
    }
}

void Pessoa(std::queue<int> &fila, std::mutex &m_fila){
    // Apertar botao
    // Esperar elevador chegar e porta abrir
    // Ativar sensor de presenca e entrar
    // Apertar botao dentro
    // Esperar elevador chegar e abrir a porta
    // Sair do elevador, ativando sensor
    while(1){
        if(m_fila.try_lock()){
            std::cout << "Locked!" << std::endl;
            m_fila.unlock();
        }else{
            std::cout << "Failed!" << std::endl;
        }
        sleep(1);
    }
}

void Motor(int &andar_atual, std::mutex &m_andar, EstadosMotor &estado, std::mutex &m_motor){
    EstadosMotor estado_anterior = desligado;
    EstadosMotor estado_atual;
    uint64_t tempo=0, tempo_atual;
    while(working){
        m_motor.lock();
        estado_atual = estado;
        m_motor.unlock();
        switch (estado_atual)
        {
        case desligado:
            /* code */
            break;
        case subindo:
            if(estado_anterior!=subindo){
                tempo = getTime();
            }else{
                tempo_atual = getTime();
                if(tempo_atual - tempo > t_ms_andar){
                    m_andar.lock();
                    andar_atual--;
                    m_andar.unlock();
                    tempo = getTime();
                }
            }
            break;
        case descendo:
            if(estado_anterior!=descendo){
                tempo = getTime();
            }else{
                tempo_atual = getTime();
                if(tempo_atual - tempo > t_ms_andar){
                    m_andar.lock();
                    andar_atual--;
                    m_andar.unlock();
                    tempo = getTime();
                }
            }
            break;
        default:
            break;
        }
        estado_anterior = estado_atual;
    }
}

void Porta(EstadosPorta &estado, std::mutex &m_porta, bool &comando_abrir, std::mutex &m_cmd_abrir){
    // Esperar comando de abrir
    // Trocar estado para aberto
    // Ativar timer para fechar a porta
    // Quando alguem passar pela porta, resetar o timer e abrir a porta
    // Dps de x segundos, trocar para fechando, pessoa ainda pode passar nesse estado
    // Dps de x segundos, trocar para fechado
}

void Alarme(bool &ativar, std::mutex &m_cmd_ativar){
    // Ativar alarme quando ativar == true
    // Desativar caso contrário
}

int main(int, char**) {
    std::queue<int> fila_andares;
    std::mutex m_fila;

    int andar_atual = 0;
    std::mutex m_andar;

    EstadosMotor estado_motor = desligado;
    std::mutex m_motor;



    // std::thread elevador(Elevador, std::ref(fila_andares), std::ref(m_fila));
    // std::thread p1(Pessoa, std::ref(fila_andares), std::ref(m_fila));
    std::thread motor(Motor, std::ref(andar_atual), std::ref(m_andar), 
                        std::ref(estado_motor), std::ref(m_motor));

    // elevador.join();
    // p1.join();
    motor.join();

    // Controlador elevador;
    // // Pessoa p(elevador, 1, 5);

    // // std::thread th1(&Pessoa::run, Pessoa(&elevador, 2, 3));
    // // std::thread th2(&Pessoa::run, Pessoa(&elevador, 5, 8));

    // int val=0;

    // std::thread elev(elevador, val);
    // // std::thread (&Pessoa::run, Pessoa(&elevador, -5, 3)).detach();
    // while(1){
    //     elevador.inc();
    //     sleep(2);
    // }

    return 0;
}
