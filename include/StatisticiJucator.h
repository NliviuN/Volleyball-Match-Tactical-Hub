//
// Created by nlivi on 10/24/2026.
//

#ifndef OOP_STATISTICIJUCATOR_H
#define OOP_STATISTICIJUCATOR_H
#include <iosfwd>

class StatisticiJucator {

private:
    double inaltime;
    int servePower;
    int receiveAcuracy;
    int sendAcuracy;
    int atackPower;
    int blockAcuracy;
    int stamina;

public :
    explicit StatisticiJucator(double inaltime_, int servePower_, int receiveAcuracy_,
        int sendAcuracy_, int atackPower_, int blockAcuracy_, int stamina_ = 100) :
        inaltime(inaltime_), servePower(servePower_), receiveAcuracy(receiveAcuracy_),
    sendAcuracy(sendAcuracy_), atackPower(atackPower_), blockAcuracy(blockAcuracy_), stamina(stamina_) {}

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
