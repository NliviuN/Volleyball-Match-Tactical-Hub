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

std::ostream& operator<<(std::ostream& os, const Meci& meci) {
    os<<"Rezumat meci: "<<std::endl;
    os<< "Total seturi disputate: " << meci.seturiJucate.size() << std::endl;
    os<< "Scor general seturi: " << meci.seturiCastigateGazda << " - " << meci.seturiCastigateOaspete << std::endl;
    os<< "Status: " << meci.determinaStareMeci() << std::endl;
    return os;
}
