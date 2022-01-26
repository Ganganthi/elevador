#include "Alarme.h"

Alarme::Alarme(/* args */):alarme_ativo(false)
{
}

Alarme::~Alarme()
{
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