#include "StatisticiJucator.h"
#include <iostream>

void StatisticiJucator::scadeStamina(int puncteEfort) {
    stamina -= puncteEfort;
    if (stamina < 0) stamina = 0;
}

void StatisticiJucator::cresteStamina(int puncteOdihna) {
    stamina += puncteOdihna;
    if (stamina > 100) stamina = 100;
}

bool StatisticiJucator::esteEpuizat() const {
    if (stamina == 0) return true;
    return false;
}

int StatisticiJucator::calculeazaPunctajTotal() const {
    int puncteInaltime = static_cast<int>(inaltime * 100.0);
    return puncteInaltime + servePower + receiveAcuracy + sendAcuracy +
        atackPower + blockAcuracy;
}

double StatisticiJucator:: calculeazaFactorOboseala() const {
    return 0.5 + 0.5 * (static_cast<double>(stamina) / 100.0);
}

double StatisticiJucator::getInaltime() const { return inaltime;}
int StatisticiJucator::getServePower() const { return servePower;}
int StatisticiJucator::getReceiveAcuracy() const { return receiveAcuracy;}
int StatisticiJucator::getSendAcuracy() const { return sendAcuracy;}
int StatisticiJucator::getBlockAcuracy() const { return blockAcuracy;}
int StatisticiJucator::getAtackPower() const { return atackPower;}
int StatisticiJucator::getStamina() const {return stamina;}

std::ostream& operator<<(std::ostream& os, const StatisticiJucator& s) {
    os << "H: " << s.inaltime << " m | SRV: " << s.servePower << " | REC: "
    << s.receiveAcuracy << " | SET: " << s.sendAcuracy << " | ATK: " << s.atackPower
    << " |BLK: " << s.blockAcuracy << " | Total" << s.calculeazaPunctajTotal();
    return os;
}