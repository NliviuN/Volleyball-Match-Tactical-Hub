#include "Meci.h"

Meci::Meci(const Echipa& gazda, const Echipa& oaspete)
: echipaGazda(gazda), echipaOaspete(oaspete), seturiCastigateGazda(0), seturiCastigateOaspete(0) {}

void Meci::inregistreazaSet(const Set &setNou, bool castigatDeGazda) {
    seturiJucate.push_back(setNou);
    if (castigatDeGazda) {
        seturiCastigateGazda++;
    } else {
        seturiCastigateOaspete++;
    }
}

bool Meci::esteMeciulTerminat() const {
    return (seturiCastigateGazda >= 2 || seturiCastigateOaspete >= 2);
}

std::string Meci::determinaStareMeci() const {
    std::string stare;
    if (seturiCastigateGazda > seturiCastigateOaspete) {
        stare = echipaGazda.getNumeEchipa();
    } else if (seturiCastigateOaspete > seturiCastigateGazda) {
        stare = echipaOaspete.getNumeEchipa();
    }

    if (this->esteMeciulTerminat()) {
        stare += " a castigat!";
    } else if (seturiCastigateGazda != seturiCastigateOaspete) {
        stare += " conduce!";
    }

    if (seturiCastigateGazda == seturiCastigateOaspete) {
        stare = "Egalitate!";
    }
    return stare;
}

void Meci::afiseazaScorul() const {
    std::cout << "Scor: " <<echipaGazda.getNumeEchipa() << " (" << seturiCastigateGazda << ") - " << echipaOaspete.getNumeEchipa() << " (" << seturiCastigateOaspete << ")" << std::endl;
}

int Meci::getSeturiCastigateGazda() const { return seturiCastigateGazda;}

int Meci::getSeturiCastigateOaspete() const { return seturiCastigateOaspete; }

const std::vector<Set>& Meci::getSeturiJucate() const { return seturiJucate; }
std::ostream& operator<<(std::ostream& os, const Meci& meci) {
    os<<"Rezumat meci: "<<std::endl;
    os<< "Total seturi disputate: " << meci.getSeturiJucate().size() << std::endl;
    os<< "Scor general seturi: " << meci.getSeturiCastigateGazda() << " - " << meci.getSeturiCastigateOaspete() << std::endl;
    os<< "Status: " << meci.determinaStareMeci() << std::endl;
    return os;
}
