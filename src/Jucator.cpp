#include "Jucator.h"

#include<random>

double Jucator::aplicaEroare(double valoareBaza, int minEroare, int maxEroare) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distEroare(minEroare, maxEroare);
    int procentGreseala = distEroare(gen);
    return valoareBaza * (1.0 - (procentGreseala / 100.0));
}

double Jucator::executaServa(TipServa tip) {

    double putereBaza = statistici.getServePower() * statistici.calculeazaFactorOboseala();

    if (tip == TipServa::TopSpin) {
        statistici.scadeStamina(10);
        putereBaza = putereBaza * 1.20;
        return aplicaEroare(putereBaza,0,45);
    } else {
        statistici.scadeStamina(5);
        return aplicaEroare(putereBaza,0,15);
    }
}

double Jucator::executaPreluare(double punctajServaAdversa) {
    statistici.scadeStamina(5);
    double preluareBaza = statistici.getReceiveAcuracy() * statistici.calculeazaFactorOboseala();
    int maxEroare = 30;
    if (post == PostJucator::Libero) {
        preluareBaza *= 1.10;
        maxEroare = 10;
    }
    double preluare = aplicaEroare(preluareBaza, 0, maxEroare);
    double calitatePreluare = preluare - (punctajServaAdversa * 0.75);
    return calitatePreluare;
}

double Jucator::executaPasa(double punctajPreluare) {
    statistici.scadeStamina(5);
    double pasaBaza = statistici.getSendAcuracy() * statistici.calculeazaFactorOboseala();
    double pasa = aplicaEroare(pasaBaza, 0, 15);
    double calitatePasa = pasa + (punctajPreluare * 0.50);
    return calitatePasa;

}

double Jucator::executaAtac(double punctajPasa) {
    statistici.scadeStamina(10);
    double atacBaza = statistici.getAtackPower() * statistici.calculeazaFactorOboseala() * (statistici.getInaltime() / 2.0) ;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distAccidentare(0, 100);
    int sansaAccidentare = distAccidentare(gen);

    int pragRisc = statistici.esteEpuizat() ? 10 : 1;
    if (sansaAccidentare <= pragRisc) {
        accidentat = true;
    }
    double atac = aplicaEroare(atacBaza, 0, 25);
    double calitateAtac = atac + (punctajPasa * 0.40);
    return calitateAtac;
}

double Jucator::executaBlocaj() {
    statistici.scadeStamina(10);
    double blocajBaza = statistici.getBlockAcuracy() * statistici.calculeazaFactorOboseala() * (statistici.getInaltime() / 2.0);
    int maxEroare = 30;
    if (post == PostJucator::MiddleBlocker) {
        maxEroare = 15;
        blocajBaza *= 1.15;
    }
    return aplicaEroare(blocajBaza, 0, maxEroare);
}

bool Jucator::esteIndisponibil() const {
    return accidentat;
}

int Jucator::getPunctajTotal() const {
    return statistici.calculeazaPunctajTotal();
}

void Jucator::cresteStamina(int puncteOdihnire) {
    statistici.cresteStamina(puncteOdihnire);
}

const std::string& Jucator::getNume() const {
    return nume;
}

Jucator& Jucator::operator=(const Jucator& j) {
    if (this != &j) {
        nume = j.nume;
        post = j.post;
        statistici = j.statistici;
        accidentat = j.accidentat;
    }
    return *this;
}

const StatisticiJucator& Jucator::getStatistici() const {
    return statistici;
};

PostJucator Jucator::getPost() const {
    return post;
}

int Jucator::getStamina() const {
    return statistici.getStamina();
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
    os << ", Inaltime: "<< j.getStatistici().getInaltime();
    os << "m, Putere serva: "<< j.getStatistici().getServePower();
    os<< ", Acuratete preluare: "<<j.getStatistici().getReceiveAcuracy();
    os<< ", Acuratete pase: " << j.getStatistici().getSendAcuracy();
    os<< ", Putere Atac: "<< j.getStatistici().getAtackPower();
    os<<", Acuratete blocaj: " << j.getStatistici().getBlockAcuracy();
    os << ", Punctaj total: " << j.getPunctajTotal()<<std::endl;
    return os;
}



