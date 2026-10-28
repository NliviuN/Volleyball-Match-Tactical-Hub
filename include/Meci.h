//
// Created by nlivi on 10/27/2026.
//

#ifndef OOP_MECI_H
#define OOP_MECI_H

#include <vector>
#include <string>
#include <iostream>
#include "Echipa.h"
#include "Set.h"

class Meci {
private:
    Echipa echipaGazda;
    Echipa echipaOaspete;
    std::vector<Set> seturiJucate;
    int seturiCastigateGazda;
    int seturiCastigateOaspete;

public:

    Meci(const Echipa& gazda, const Echipa& oaspete);
    void inregistreazaSet(const Set& setNou, bool castigatDeGazda);
    bool esteMeciulTerminat() const;
    std::string determinaStareMeci() const;
    void afiseazaScorul() const;

    std::string genereazaRaportDetaliat() const;
    int calculeazaPuncteClasament(bool pentruGazda) const;
    void reseteazaMeci();
    void meniuRaportStatistic() const;

    int getSeturiCastigateGazda() const;
    int getSeturiCastigateOaspete() const;
    const std::vector<Set>& getSeturiJucate() const;


    friend std::ostream& operator<<(std::ostream& os, const Meci& meci);
};

#endif //OOP_MECI_H
