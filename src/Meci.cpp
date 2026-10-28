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
    if (esteMeciulTerminat()) {
        std::cout<< std::endl << "MECIUL S-A INCHEIAT!" << std::endl;
    } else {
        std::cout << std::endl << "Setul " << seturiJucate.size() << " s-a incheiat!" << std::endl;
    }

    std::cout << "Doresti sa afisezi raportul statistic al meciului? (1 - Da / 0 - Nu): ";
    int optiune = 0;
    if (std::cin >> optiune && optiune == 1) {
        std::cout <<std::endl << genereazaRaportDetaliat() << "\n";
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

std::string Meci::genereazaRaportDetaliat() const {
    std::string raport = "Raport Statistic Meci\n" ;
    int totalSeturi = seturiCastigateGazda + seturiCastigateOaspete;
    raport += "Total seturi disputate: " + std::to_string(totalSeturi) + "\n";

    if (totalSeturi == 0) {
        raport += "Meciul nu a inceput inca.\n";
        return raport;
    }

    if (esteMeciulTerminat()) {
        if (seturiCastigateGazda > seturiCastigateOaspete) {
            raport += "Victorie pentru echipa gazda!\n";
        } else {
            raport += "Victorie pentru echipa oaspete!\n";
        }

        if (totalSeturi == 3) {
            raport += "Victorie clara in minimum de seturi (3-0).\n";
        } else if (totalSeturi == 4) {
            raport += "Meci disputat, decis in 4 seturi (3-1).\n";
        } else if (totalSeturi == 5) {
            raport += "Meci dramatic decis in setul decisiv / Tie-Break (3-2)!\n";
        }
    } else {
        raport += "Meci in desfasurare.\n";
        int diferenta = std::abs(seturiCastigateGazda - seturiCastigateOaspete);
        if (diferenta == 0) {
            raport += "Echilibru perfect la seturi in acest moment.\n";
        } else if (diferenta == 1) {
            raport += "Avantaj minim de un set pe tabela.\n";
        } else {
            raport += "Avantaj de doua seturi.\n";
        }
    }

    int pctGazda = calculeazaPuncteClasament(true);
    int pctOaspete = calculeazaPuncteClasament(false);
    raport += "Puncte clasament (sistem FIVB): Gazda " + std::to_string(pctGazda) +
              " pct | Oaspete " + std::to_string(pctOaspete) + " pct\n";

    return raport;
}

int Meci::calculeazaPuncteClasament(bool pentruGazda) const {
    if (!esteMeciulTerminat()) {
        return 0;
    }

    int seturiProprii = pentruGazda ? seturiCastigateGazda : seturiCastigateOaspete;
    int seturiAdversar = pentruGazda ? seturiCastigateOaspete : seturiCastigateGazda;

    if (seturiProprii == 3) {
        if (seturiAdversar <= 1) {
            return 3;
        }
        return 2;
    }

    if (seturiProprii == 2 && seturiAdversar == 3) {
        return 1;
    }

    return 0;
}

void Meci::reseteazaMeci() {
    seturiJucate.clear();
    seturiCastigateGazda = 0;
    seturiCastigateOaspete = 0;
}

void Meci::meniuRaportStatistic() const {
    std::cout << std::endl << "Doresti sa afisezi raportul statistic? (1 - Da / 0 - Nu): ";
    int optiune = 0;
    if (std::cin >> optiune && optiune == 1) {
        std::cout << std::endl << genereazaRaportDetaliat() << std::endl;
    } else {
        std::cout << "Continuam fara afisarea raportului." << std::endl;
    }
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
