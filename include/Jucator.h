
#ifndef OOP_JUCATOR_H
#define OOP_JUCATOR_H
#include "StatisticiJucator.h"
#include <iostream>

enum class PostJucator {
    Setter,
    OutsideHitter,
    Opposite,
    MiddleBlocker,
    Libero

};

enum class TipServa {
    TopSpin,
    Float
};

class Jucator {
private:
    std::string nume;
    PostJucator post;
    StatisticiJucator statistici;
    bool accidentat;

public:
    double executaServa(TipServa tip);
    double executaPreluare(double punctajServaAdversa);
    double executaPasa(double punctajPreluare);
    double executaAtac(double punctajPasa);
    double executaBlocaj();
    bool esteIndisponibil() const;
    int getPunctajTotal() const;
    std::string getNume() const;

    friend std::ostream& operator<<(std::ostream& os, const Jucator& j);

};
#endif //OOP_JUCATOR_H
