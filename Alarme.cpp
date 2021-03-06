#include "Alarme.h"

Alarme::Alarme(/* args */):alarme_ativo(false)
{
    std::cout << "Criando alarme! " << this << std::endl;
}

Alarme::~Alarme()
{
    std::cout << "Destruindo alarme! " << this << std::endl;
}

void Alarme::AtivarAlarme(){
    alarme_ativo = true;
}

void Alarme::DesativarAlarme(){
    alarme_ativo = false;
}

bool Alarme::GetAlarme(){
    return alarme_ativo;
}