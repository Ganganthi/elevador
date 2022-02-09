#include <iostream>
#include <thread>
#include <unistd.h>
#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>

const int andar_max = 6;            // num max de andares
const int andar_min = 0;
bool working = true;
const uint64_t period_ms = 200;
const uint64_t t_ms_andar = 2000;   // 2 segundos
const uint64_t t_ms_porta_aberto = 5000;
const uint64_t t_ms_porta_fechando = 2000;
const auto period = std::chrono::microseconds(period_ms * 1000);
std::mutex m_cout;

uint64_t getTime(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Print(std::string str){
    std::lock_guard<std::mutex> guard(m_cout);
    std::cout << str << std::endl;
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
        std::string str("Descendo Novo Andar -> ");
        str.append(std::to_string(andar_atual-1));
        Print(str);
        std::lock_guard<std::mutex> guard(mut);
        if(andar_atual > andar_min){
            andar_atual--;
            return true;
        }else{
            return false;
        }
    }

    bool SubirAndar(){
        std::string str("Subindo Novo Andar -> ");
        str.append(std::to_string(andar_atual+1));
        Print(str);
        std::lock_guard<std::mutex> guard(mut);
        if(andar_atual < andar_max){
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
        std::string str("SetEstadoPorta -> ");
        switch (est)
        {
        case EstadosPorta::aberto:
            str.append("aberto");
            break;
        case EstadosPorta::fechando:
            str.append("fechando");
            break;
        case EstadosPorta::fechado:
            str.append("fechado");
            break;
        default:
            break;
        }
        Print(str);
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
        switch (est)
        {
        case desligado:
            Print(std::string("SetEstadoMotor -> desligado"));
            break;
        case descendo:
            Print(std::string("SetEstadoMotor -> descendo"));
            break;
        case subindo:
            Print(std::string("SetEstadoMotor -> subindo"));
            break;
        default:
            break;
        }
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


void Elevador(ListaAndares &fila, AndarAtual &andar_atual, EstadosMotor &m_estado, EstadosPorta &p_estado, ComandoAbrir &cmd_abrir)
{
    // Se porta aberta, esperar fechar
    // Se ha elementos na fila, mover
    // Checar o andar, quando chegar, enviar comando de parar motor e abrir porta
    int andar = andar_atual.GetAndarAtual();
    int destino = 0;
    bool movimentando = false;
    std::mutex lock_period;
    std::condition_variable cond_var;
    auto start_time = std::chrono::system_clock::now();
    while(working){
        start_time = std::chrono::system_clock::now();
        if(p_estado.GetEstadoPorta()==EstadosPorta::fechado){
            if(!movimentando){
                if(fila.GetProxAndar(destino)){
                    // tem algo na fila
                    std::string str("Novo destino -> ");
                    str.append(std::to_string(destino));
                    Print(str);
                    if(destino > andar){
                        m_estado.SetEstadoMotor(EstadosMotor::subindo);
                        movimentando = true;
                    }else if(destino < andar){
                        m_estado.SetEstadoMotor(EstadosMotor::descendo);
                        movimentando = true;
                    }else{
                        // cmd_abrir.AtivarComando();
                        p_estado.SetEstadoPorta(EstadosPorta::aberto);
                    }
                }else{
                    // fila vazia
                }
            }else{  // movimentando
                andar = andar_atual.GetAndarAtual();
                if(andar == destino){
                    // chegou no destino
                    m_estado.SetEstadoMotor(EstadosMotor::desligado);
                    // cmd_abrir.AtivarComando();
                    p_estado.SetEstadoPorta(EstadosPorta::aberto);
                    movimentando = false;
                }
            }
        }
        std::unique_lock<std::mutex> lock(lock_period);
        cond_var.wait_until(lock, start_time + period);
    }
}

void Pessoa(ListaAndares &fila, ComandoAbrir &cmd_abrir){
    // Apertar botao
    // Esperar elevador chegar e porta abrir
    // Ativar sensor de presenca e entrar
    // Apertar botao dentro
    // Esperar elevador chegar e abrir a porta
    // Sair do elevador, ativando sensor
    
}

/**
 * @brief Thread do motor, se o estado do motor for subindo ou descendo, 
 *          espera t_ms_andar segundos para trocar de andar
 * 
 * @param andar_atual 
 * @param m_estado 
 */
void Motor(AndarAtual &andar_atual, EstadosMotor &m_estado){
    EstadosMotor::Estados estado_anterior = m_estado.GetEstadoMotor();
    EstadosMotor::Estados estado_atual;
    uint64_t tempo=0, tempo_atual;
    std::mutex lock_period;
    std::condition_variable cond_var;
    auto start_time = std::chrono::system_clock::now();
    while(working){
        start_time = std::chrono::system_clock::now();
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
        std::unique_lock<std::mutex> lock(lock_period);
        cond_var.wait_until(lock, start_time + period);
    }
}

void Porta(EstadosPorta &p_estado, ComandoAbrir &cmd_abrir, EstadosMotor &m_estado){
    // Esperar comando de abrir
    // Trocar estado para aberto
    // Ativar timer para fechar a porta
    // Quando alguem passar pela porta, resetar o timer e abrir a porta
    // Dps de x segundos, trocar para fechando, pessoa ainda pode passar nesse estado
    // Dps de x segundos, trocar para fechado
    uint64_t tempo=0, tempo_atual;
    EstadosPorta::Estados estado_anterior = p_estado.GetEstadoPorta();
    EstadosPorta::Estados estado_atual;
    std::mutex lock_period;
    std::condition_variable cond_var;
    auto start_time = std::chrono::system_clock::now();
    while(working){
        start_time = std::chrono::system_clock::now();
        if(m_estado.GetEstadoMotor()==EstadosMotor::desligado){
            if(cmd_abrir.QueryComando()){
                tempo = getTime();
                p_estado.SetEstadoPorta(EstadosPorta::aberto);
            }else{
                estado_atual = p_estado.GetEstadoPorta();
                switch (estado_atual)
                {
                case EstadosPorta::aberto:
                    if(estado_anterior != EstadosPorta::aberto){
                        tempo = getTime();
                    }
                    tempo_atual = getTime();
                    if(tempo_atual - tempo > t_ms_porta_aberto){
                        p_estado.SetEstadoPorta(EstadosPorta::fechando);
                        tempo = getTime();
                    }
                    break;
                case EstadosPorta::fechando:
                    tempo_atual = getTime();
                    if(tempo_atual - tempo > t_ms_porta_fechando){
                        p_estado.SetEstadoPorta(EstadosPorta::fechado);
                        tempo = getTime();
                    }
                    break;
                case EstadosPorta::fechado:
                    /* code */
                    break;
                default:
                    break;
                }
                estado_anterior = estado_atual;
            }
        }
        std::unique_lock<std::mutex> lock(lock_period);
        cond_var.wait_until(lock, start_time + period);
    }
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

    std::thread elevador(Elevador, std::ref(fila_andares), std::ref(andar_atual), std::ref(estado_motor), std::ref(estado_porta), std::ref(cmd_abrir));
    std::thread motor(Motor, std::ref(andar_atual), std::ref(estado_motor));
    std::thread porta(Porta, std::ref(estado_porta), std::ref(cmd_abrir), std::ref(estado_motor));

    sleep(2);
    fila_andares.InserirExterno(3);
    fila_andares.InserirExterno(5);
    sleep(30);
    fila_andares.InserirExterno(0);
    
    sleep(30);
    // working = false;
    sleep(5);
    elevador.join();

    return 0;
}
