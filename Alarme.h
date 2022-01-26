#ifndef ALARME_H
#define ALARME_H

#include <iostream>

class Alarme
{
private:
    bool alarme_ativo;
public:
    Alarme(/* args */);
    ~Alarme();

    void AtivarAlarme();
    void DesativarAlarme();

    bool GetAlarme();
};

#endif