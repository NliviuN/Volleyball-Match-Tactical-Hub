//
// Created by nlivi on 10/27/2026.
//

#ifndef OOP_MECI_H
#define OOP_MECI_H

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Echipa.h"
#include "Set.h"
#include "FormatMeci.h"

class Meci {
private:
    Echipa echipaGazda;
    Echipa echipaOaspete;
    std::vector<Set> seturiJucate;
    int seturiCastigateGazda;
    int seturiCastigateOaspete;
    std::unique_ptr<FormatMeci> format;
    static int numarTotalMeciuri;

public:

    static int getNumarTotalMeciuri();
    Meci(const Echipa& gazda, const Echipa& oaspete, const FormatMeci& formatMeci = MeciOficial());
    Meci(const Meci& other);
    friend void swap(Meci& a, Meci& b) noexcept;
    Meci& operator=(Meci other);
    ~Meci() = default;

    FormatMeci* getFormat() const;
    void pregatesteEchipeIntreSeturi(Echipa& gazda, Echipa& oaspete) const;
    void inregistreazaSet(const Set& setNou, bool castigatDeGazda);
    bool esteMeciulTerminat() const;
    std::string determinaStareMeci() const;
    void afiseazaScorul() const;

    std::string genereazaRaportDetaliat() const;
    void reseteazaMeci();
    bool meniuRaportStatistic() const;

    int getSeturiCastigateGazda() const;
    int getSeturiCastigateOaspete() const;
    const std::vector<Set>& getSeturiJucate() const;


    friend std::ostream& operator<<(std::ostream& os, const Meci& meci);
};

#endif //OOP_MECI_H
