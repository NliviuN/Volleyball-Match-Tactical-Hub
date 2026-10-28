
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
    PostJucator post = PostJucator::Libero;
    StatisticiJucator statistici;
    bool accidentat = false;

    static double aplicaEroare(double valoareBaza, int minEroare, int maxEroare);

public:
    Jucator() = default;
    explicit Jucator(const std::string & nume_, PostJucator post_ = PostJucator::Libero ,const StatisticiJucator& statistici_ = StatisticiJucator(2.0), bool accidentat_ = false);
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
    const StatisticiJucator& getStatistici() const;
    friend std::ostream& operator<<(std::ostream& os, const Jucator& j);
    Jucator(const Jucator &j) = default;
    ~Jucator() = default;
    Jucator& operator=(const Jucator &j);
};
#endif //OOP_JUCATOR_H
