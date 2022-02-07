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
const int andar_min = 0;
bool working = true;
const uint64_t t_ms_andar = 2000;   // 2 segundos

uint64_t getTime(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

class ListaAndares
{
private:
    std::mutex mut;
    std::queue<int> fila_andares;
public:
    ListaAndares(){}
    ~ListaAndares(){}

    bool InserirExterno(int andar){
        if(andar > andar_max) return false;
        if(andar < andar_min) return false;
        std::lock_guard<std::mutex> guard(mut);
        fila_andares.push(andar);
        return true;
    }

    bool InserirInterno(int andar){
        if(andar > andar_max) return false;
        if(andar < andar_min) return false;
        std::lock_guard<std::mutex> guard(mut);
        fila_andares.push(andar);
        return true;
    }

    bool GetProxAndar(int &andar){
        std::lock_guard<std::mutex> guard(mut);
        if(fila_andares.size()>0){
            andar = fila_andares.front();
            fila_andares.pop();
            return true;
        }else{
            return false;
        }
    }
};

class AndarAtual
{
private:
    int andar_atual;
    std::mutex mut;
public:
    AndarAtual():andar_atual(0){}
    ~AndarAtual(){}

    bool DescerAndar(){
        std::lock_guard<std::mutex> guard(mut);
        if(andar_atual <= andar_min){
            andar_atual--;
            return true;
        }else{
            return false;
        }
    }

    bool SubirAndar(){
        std::lock_guard<std::mutex> guard(mut);
        if(andar_atual >= andar_max){
            andar_atual++;
            return true;
        }else{
            return false;
        }
    }

    int GetAndarAtual(){
        std::lock_guard<std::mutex> guard(mut);
        return andar_atual;
    }
};

class EstadosPorta
{
public:
    enum Estados{
        fechado,
        aberto,
        fechando
    };
private:
    EstadosPorta::Estados estado;
    std::mutex mut;
public:
    EstadosPorta():estado(EstadosPorta::Estados::fechado){}
    ~EstadosPorta(){}
    void SetEstadoPorta(EstadosPorta::Estados est){
        std::lock_guard<std::mutex> guard(mut);
        estado = est;
    }
    EstadosPorta::Estados GetEstadoPorta(){
        std::lock_guard<std::mutex> guard(mut);
        return estado;
    }
};

class EstadosPessoa
{
public:
    enum Estados{
        parado,
        esperando_chegar,
        decidindo_entrar,   //! decidir se esse estado faz sentido
        dentro_elevador,
        fora_depois
    };
private:
    EstadosPessoa::Estados estado;
    std::mutex mut;
public:
    EstadosPessoa():estado(EstadosPessoa::Estados::parado){}
    ~EstadosPessoa(){}
    void SetEstadoPessoa(EstadosPessoa::Estados est){
        std::lock_guard<std::mutex> guard(mut);
        estado = est;
    }
    EstadosPessoa::Estados GetEstadoPessoa(){
        std::lock_guard<std::mutex> guard(mut);
        return estado;
    }
};

class EstadosMotor
{
public:
    enum Estados{
        desligado,
        descendo,
        subindo
    };
private:
    EstadosMotor::Estados estado;
    std::mutex mut;
public:
    EstadosMotor():estado(EstadosMotor::Estados::desligado){}
    ~EstadosMotor(){}
    void SetEstadoMotor(EstadosMotor::Estados est){
        std::lock_guard<std::mutex> guard(mut);
        estado = est;
    }
    EstadosMotor::Estados GetEstadoMotor(){
        std::lock_guard<std::mutex> guard(mut);
        return estado;
    }
};

class ComandoAbrir
{
private:
    bool comando_ativo;
    std::mutex mut;
public:
    ComandoAbrir():comando_ativo(false){}
    ~ComandoAbrir(){}
    void AtivarComando(){
        std::lock_guard<std::mutex> guard(mut);
        comando_ativo = true;
    }
    bool QueryComando(){
        std::lock_guard<std::mutex> guard(mut);
        if(comando_ativo){
            comando_ativo = false;
            return true;
        }else{
            return false;
        }
    }
};

class ComandoAlarme
{
private:
    bool alarme_ativo;
    std::mutex mut;
public:
    ComandoAlarme():alarme_ativo(false){}
    ~ComandoAlarme(){}
    void AtivarAlarme(){
        std::lock_guard<std::mutex> guard(mut);
        alarme_ativo = true;
    }
    void DesativarAlarme(){
        std::lock_guard<std::mutex> guard(mut);
        alarme_ativo = false;
    }
    bool QueryAlarme(){
        std::lock_guard<std::mutex> guard(mut);
        if(alarme_ativo){
            return true;
        }else{
            return false;
        }
    }
};


void Elevador(ListaAndares &fila, EstadosMotor &m_estado, EstadosPorta &p_estado)
{
    // Se porta aberta, esperar fechar
    // Se ha elementos na fila, mover
    // Checar o andar, quando chegar, enviar comando de parar motor e abrir porta
}

void Pessoa(ListaAndares &fila, ComandoAbrir &cmd_abrir){
    // Apertar botao
    // Esperar elevador chegar e porta abrir
    // Ativar sensor de presenca e entrar
    // Apertar botao dentro
    // Esperar elevador chegar e abrir a porta
    // Sair do elevador, ativando sensor
    
}

void Motor(AndarAtual &andar_atual, EstadosMotor &m_estado){
    EstadosMotor::Estados estado_anterior = EstadosMotor::Estados::desligado;
    EstadosMotor::Estados estado_atual;
    uint64_t tempo=0, tempo_atual;
    while(working){
        estado_atual = m_estado.GetEstadoMotor();
        switch (estado_atual)
        {
        case EstadosMotor::Estados::desligado:
            /* code */
            break;
        case EstadosMotor::Estados::subindo:
            if(estado_anterior!=EstadosMotor::Estados::subindo){
                tempo = getTime();
            }else{
                tempo_atual = getTime();
                if(tempo_atual - tempo > t_ms_andar){
                    andar_atual.SubirAndar();
                    tempo = getTime();
                }
            }
            break;
        case EstadosMotor::Estados::descendo:
            if(estado_anterior!=EstadosMotor::Estados::descendo){
                tempo = getTime();
            }else{
                tempo_atual = getTime();
                if(tempo_atual - tempo > t_ms_andar){
                    andar_atual.DescerAndar();
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

void Porta(EstadosPorta &p_estado, ComandoAbrir &cmd_abrir){
    // Esperar comando de abrir
    // Trocar estado para aberto
    // Ativar timer para fechar a porta
    // Quando alguem passar pela porta, resetar o timer e abrir a porta
    // Dps de x segundos, trocar para fechando, pessoa ainda pode passar nesse estado
    // Dps de x segundos, trocar para fechado
}

void Alarme(ComandoAlarme &alarme){
    // Ativar alarme quando ativar == true
    // Desativar caso contrário
}

int main(int, char**) {

    AndarAtual andar_atual;
    ListaAndares fila_andares;

    EstadosMotor estado_motor;
    EstadosPessoa estado_pessoa;
    EstadosPorta estado_porta;

    ComandoAbrir cmd_abrir;
    ComandoAlarme cmd_alarme;

    // std::thread elevador(Elevador, std::ref(fila_andares), std::ref(m_fila));
    std::thread motor(Motor, std::ref(andar_atual), std::ref(estado_motor));

    motor.join();

    return 0;
}
