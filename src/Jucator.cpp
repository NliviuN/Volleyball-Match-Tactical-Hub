#include "Jucator.h"

#include<random>

double Jucator::executaServa(TipServa tip) {

    double putereBaza = statistici.getServePower() * statistici.calculeazaFactorOboseala();

    static std::random_device rd;
    static std::mt19937 gen(rd());

    if (tip == TipServa::TopSpin) {
        statistici.scadeStamina(10);

        std::uniform_int_distribution<int> distEroare(0, 45);
        int procentGreseala = distEroare(gen);

        double putereTopSpin = putereBaza * 1.20;
        return putereTopSpin * (1.0 - procentGreseala / 100.0);


    } else {
        statistici.scadeStamina(5);

        std::uniform_int_distribution<int> distEroare(0, 15);
        int procentGreseala = distEroare(gen);

        return putereBaza * (1.0 - procentGreseala / 100.0);
    }
}

double Jucator::executaPreluare(double punctajServaAdversa) {

    statistici.scadeStamina(5);

    double preluareBaza = statistici.getReceiveAcuracy() * statistici.calculeazaFactorOboseala();

    static std::random_device rd;
    static std::mt19937 gen(rd());

    int procentGreseala = 0;

    if (post == PostJucator::Libero) {
        std::uniform_int_distribution<int> distLibero(0, 10);
        procentGreseala = distLibero(gen);
        preluareBaza *= 1.10;
    } else {
        std::uniform_int_distribution<int> distNormal(0, 30);
        procentGreseala = distNormal(gen);
    }

    double preluare = preluareBaza * (1.0 - procentGreseala / 100.0);
    double calitatePreluare = preluare - (punctajServaAdversa * 0.75);
    return calitatePreluare;
}

double Jucator::executaPasa(double punctajPreluare) {
    statistici.scadeStamina(5);
    double pasaBaza = statistici.getSendAcuracy() * statistici.calculeazaFactorOboseala();
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distEroare(0, 15);
    int procentGreseala = distEroare(gen);
    double pasa = pasaBaza * (1.0 - procentGreseala / 100.0);
    double calitatePasa = pasa + (punctajPreluare * 0.50);
    return calitatePasa;

}

double Jucator::executaAtac(double punctajPasa) {
    statistici.scadeStamina(10);
    double atacBaza = statistici.getAtackPower() * statistici.calculeazaFactorOboseala() * (statistici.getInaltime() / 2.0) ;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distEroare(0, 25);
    int procentGreseala = distEroare(gen);

    std::uniform_int_distribution<int> distAccidentare(0, 100);
    int sansaAccidentare = distAccidentare(gen);

    int pragRisc = statistici.esteEpuizat() ? 10 : 1;
    if (sansaAccidentare <= pragRisc) {
        accidentat = true;
    }
    double atac = atacBaza * (1.0 - procentGreseala / 100.0);
    double calitateAtac = atac + (punctajPasa * 0.40);
    return calitateAtac;
}

double Jucator::executaBlocaj() {
    statistici.scadeStamina(10);
    double blocajBaza = statistici.getBlockAcuracy() * statistici.calculeazaFactorOboseala() * (statistici.getInaltime() / 2.0);
    static std::random_device rd;
    static std::mt19937 gen(rd());
    int procentGreseala = 0;
    if (post == PostJucator::MiddleBlocker) {
        std::uniform_int_distribution<int> distMiddleBlocker(0, 15);
        procentGreseala = distMiddleBlocker(gen);
        blocajBaza *= 1.15;
    } else {
        std::uniform_int_distribution<int> distNormal(0, 30);
        procentGreseala = distNormal(gen);
    }
    double blocaj = blocajBaza * (1.0 - procentGreseala / 100.0);
    return blocaj;
}

bool Jucator::esteIndisponibil() const {
    return accidentat;
}

int Jucator::getPunctajTotal() const {
    return statistici.calculeazaPunctajTotal();
}

std::string Jucator::getNume() const {
    return nume;
}

std:: ostream& operator<<(std:: ostream& os, const Jucator& j) {
    os << "Jucator: " << j.getNume() << ", Post: ";

    switch (j.post) {
        case PostJucator::MiddleBlocker: os << "Middle Blocker"; break;
        case PostJucator::Libero: os << "Libero"; break;
        case PostJucator::Opposite: os << "Opposite"; break;
        case PostJucator::Setter: os << "Setter"; break;
        case PostJucator::OutsideHitter: os << "Outside Hitter"; break;
    }

    os << ", Punctaj total: " << j.getPunctajTotal();
    return os;
}

