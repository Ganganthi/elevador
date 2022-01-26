#include "Pessoa.h"

Pessoa::Pessoa(Controlador &c, int origem, int destino):estado_atual(Estados::parado)
{
    std::cout << "Criando pessoa no elevador " << &c << ", no andar " << origem << " com destino o andar " << destino << "! " << this << std::endl;
}

Pessoa::~Pessoa()
{
    std::cout << "Destruindo pessoa! " << this << std::endl;
}