
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

    static double aplicaEroare(double valoareBaza, int minEroare, int maxEroare);

public:
    explicit Jucator(std::string nume_ = "Liber", PostJucator post_ = PostJucator::OutsideHitter,const StatisticiJucator& statistici_ = StatisticiJucator(), bool accidentat_ = false):
    nume(std:: move(nume_)), post(post_), statistici(statistici_), accidentat(accidentat_){}
    double executaServa(TipServa tip);
    double executaPreluare(double punctajServaAdversa);
    double executaPasa(double punctajPreluare);
    double executaAtac(double punctajPasa);
    double executaBlocaj();
    bool esteIndisponibil() const;
    int getPunctajTotal() const;
    void cresteStamina(int puncteOdihnire);
    const std::string& getNume() const;
    PostJucator getPost() const;
    int getStamina() const;
    Jucator& operator=(const Jucator &j);
    friend std::ostream& operator<<(std::ostream& os, const Jucator& j);
    ~Jucator();
};
#endif //OOP_JUCATOR_H
