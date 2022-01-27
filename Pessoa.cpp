#include "Pessoa.h"

Pessoa::Pessoa(Controlador *c, int ori, int dest)
:estado_atual(Estados::parado)
,origem(ori)
,destino(dest)
,elevador_(c)
{
    std::cout << "Criando pessoa no elevador " << c << ", no andar " << origem << " com destino o andar " << destino << "! " << this << std::endl;
}

Pessoa::~Pessoa()
{
    std::cout << "Destruindo pessoa! " << this << std::endl;
}