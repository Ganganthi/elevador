#include <Porta.h>

Porta::Porta():estado_atual(Estados::fechado)
{
    std::cout << "Criando porta! " << this << std::endl;
}

Porta::~Porta()
{
    std::cout << "Destruindo porta! " << this << std::endl;
}

void AbrirPorta(){
    
}

void FecharPorta(){

}