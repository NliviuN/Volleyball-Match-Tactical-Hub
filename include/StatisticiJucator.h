//
// Created by nlivi on 10/24/2026.
//

#ifndef OOP_STATISTICIJUCATOR_H
#define OOP_STATISTICIJUCATOR_H
#include <iosfwd>

class StatisticiJucator {

private:
    double inaltime = 2.0;
    int servePower = 70;
    int receiveAcuracy = 70;
    int sendAcuracy = 70;
    int atackPower = 70;
    int blockAcuracy = 70;
    int stamina = 100;

public :
    StatisticiJucator() = default;

    explicit StatisticiJucator(double inaltime_, int servePower_ = 70, int receiveAcuracy_ = 70,
        int sendAcuracy_ = 70, int atackPower_ = 70, int blockAcuracy_ = 70, int stamina_ = 100);

    void scadeStamina(int puncteEfort);
    void cresteStamina(int puncteOdihna);
    bool esteEpuizat() const;
    int calculeazaPunctajTotal() const;
    double calculeazaFactorOboseala() const;

    double getInaltime() const;
    int getServePower() const;
    int getReceiveAcuracy() const;
    int getSendAcuracy() const;
    int getAtackPower() const;
    int getBlockAcuracy() const;
    int getStamina() const;

    friend std::ostream& operator<<(std::ostream& os, const StatisticiJucator& s);

};
#endif //OOP_STATISTICIJUCATOR_H
