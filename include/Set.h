
#ifndef OOP_SET_H
#define OOP_SET_H

#include "Echipa.h"
#include "FormatMeci.h"
class Set {
private:
    Echipa echipaMea;
    Echipa echipaAdversa;
    int puncteProprii;
    int puncteAdversari;
    bool esteServiciulMeu;
    FormatMeci* format;
    int numarSet;
    bool ultimulPunctAdversar;
    bool meciAbandonat;

    void afisareMeniuTactic();
    static bool inlocuiesteJucatorAccidentat(Echipa& echipa, int zona);
    void simuleazaRaliu();
    void acordaPunct(bool pentruEchipaMea);
    static void pregatesteLiberoInSpate(Echipa& echipa, bool esteLaServiciu);
    static std::vector<int> obtineZoneValideAtac(Echipa& echipa, int zonaSetter, int nivelPreluare);

public:
    Set(Echipa e1, Echipa e2, FormatMeci* format = nullptr, int numarSet = 1, bool ultimulPunctAdversar = false, bool meciAbandonat = false);
    void joacaSet();
    bool AmCastigat() const;
    friend std::ostream& operator<< (std::ostream& os, const Set& set);
    bool esteAbandonat() const;

    int getPuncteProprii() const;
    int getPuncteAdversari() const;
    const Echipa& getEchipaMea() const;
    const Echipa& getEchipaAdversa() const;
};
#endif //OOP_SET_H
