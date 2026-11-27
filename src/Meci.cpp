#include "Meci.h"
#include <tabulate/table.hpp>
#include <sstream>

#include "ExceptiiVolei.h"

int Meci::numarTotalMeciuri = 0;

int Meci::getNumarTotalMeciuri() {
    return numarTotalMeciuri;
}

Meci::Meci(const Echipa& gazda, const Echipa& oaspete, const FormatMeci& formatMeci) :
echipaGazda(gazda), echipaOaspete(oaspete), seturiCastigateGazda(0), seturiCastigateOaspete(0), format(formatMeci.clone()) {

    if (echipaGazda.getNumeEchipa() == echipaOaspete.getNumeEchipa()) {
        throw EroareResurseEchipa("O echipa nu poate juca un meci impotriva ei insesi (" + echipaGazda.getNumeEchipa() + ")!");
    }

    for (int zona = 1; zona <= 6; ++zona) {
        if (echipaGazda.getJucator(zona).getNume().empty() ||
            echipaOaspete.getJucator(zona).getNume().empty()) {
            throw EroareResurseEchipa("Ambele echipe trebuie sa aiba cei 6 titulari configurati pe teren!");
            }
    }
    numarTotalMeciuri++;
}

Meci::Meci(const Meci& other)
    : echipaGazda(other.echipaGazda),
      echipaOaspete(other.echipaOaspete),
      seturiJucate(other.seturiJucate),
      seturiCastigateGazda(other.seturiCastigateGazda),
      seturiCastigateOaspete(other.seturiCastigateOaspete),
      format(other.format ? other.format->clone() : nullptr) {
}

void swap(Meci& a, Meci& b) noexcept {
    using std::swap;
    swap(a.echipaGazda, b.echipaGazda);
    swap(a.echipaOaspete, b.echipaOaspete);
    swap(a.seturiJucate, b.seturiJucate);
    swap(a.seturiCastigateGazda, b.seturiCastigateGazda);
    swap(a.seturiCastigateOaspete, b.seturiCastigateOaspete);
    swap(a.format, b.format);
}

Meci& Meci::operator=(Meci other) {
    swap(*this, other);
    return *this;
}

FormatMeci* Meci::getFormat() const { return format.get();}

void Meci::pregatesteEchipeIntreSeturi(Echipa &gazda, Echipa &oaspete) const {
    int rec = static_cast<int>(format -> calculeazaRecuperareStamina());
    gazda.pregatestePentruSetNou(rec);
    oaspete.pregatestePentruSetNou(rec);
}



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
}


bool Meci::esteMeciulTerminat() const {
    return (seturiCastigateGazda >= format -> getSeturiPentruVictorie() || seturiCastigateOaspete >= format -> getSeturiPentruVictorie());
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
    tabulate::Table tabelRaport;

    tabelRaport.add_row({"=== RAPORT STATISTIC MECI ==="});
    tabelRaport.row(0).format().font_style({tabulate::FontStyle::bold}).font_color(tabulate::Color::yellow);

    std::ostringstream ossInfo;
    ossInfo << "Format utilizat: " << (format ? format->getNumeFormat() : "Nespecificat") << "\n"
            << "Numar total meciuri inregistrate: " << getNumarTotalMeciuri() << "\n"
            << "Echipe: " << echipaGazda.getNumeEchipa() << " vs " << echipaOaspete.getNumeEchipa();

    tabelRaport.add_row({ossInfo.str()});

    int totalSeturi = seturiCastigateGazda + seturiCastigateOaspete;
    std::ostringstream ossStare;
    ossStare << "Total seturi disputate: " << totalSeturi;

    if (totalSeturi == 0) {
        ossStare << "\nMeciul nu a inceput inca.";
    } else if (esteMeciulTerminat()) {
        if (seturiCastigateGazda > seturiCastigateOaspete) {
            ossStare << "\nRezultat: Victorie pentru echipa gazda (" << echipaGazda.getNumeEchipa() << ")!";
        } else {
            ossStare << "\nRezultat: Victorie pentru echipa oaspete (" << echipaOaspete.getNumeEchipa() << ")!";
        }

        int seturiVictorie = format ? format->getSeturiPentruVictorie() : 3;
        int seturiMaximPosibile = 2 * seturiVictorie - 1;
        if (totalSeturi == seturiVictorie) {
            ossStare << "\nVictorie clara in minimum de seturi (" << seturiVictorie << "-0).";
        } else if (totalSeturi == seturiMaximPosibile) {
            ossStare << "\nMeci dramatic decis in setul decisiv / Tie-Break (" << seturiVictorie << "-" << (seturiVictorie - 1) << ")!";
        }
    } else {
        ossStare << "\nMeci in desfasurare.";
        int diferenta = std::abs(seturiCastigateGazda - seturiCastigateOaspete);
        if (diferenta == 0) {
            ossStare << "\nEchilibru perfect pe tabela.";
        } else {
            ossStare << "\nAvantaj minim de un set pe tabela.";
        }
    }

    tabelRaport.add_row({ossStare.str()});

    std::ostringstream ossScorFinal;
    ossScorFinal << "Scor General Seturi: " << seturiCastigateGazda << " - " << seturiCastigateOaspete;
    tabelRaport.add_row({ossScorFinal.str()});

    std::ostringstream ossFinal;
    ossFinal << "\n" << tabelRaport << "\n";
    return ossFinal.str();
}

void Meci::reseteazaMeci() {
    seturiJucate.clear();
    seturiCastigateGazda = 0;
    seturiCastigateOaspete = 0;
}

bool Meci::meniuRaportStatistic() const {
    std::cout << std::endl << "Doresti sa afisezi raportul statistic? (1 - Da | 2 - Nu | 0 - Revino la meniul principal): ";
    int optiune = 0;
    if (!(std::cin >> optiune)) {
        return false;
    }
    if (optiune == 1) {
        std::cout << std::endl << genereazaRaportDetaliat() << std::endl;
        return true;
    } else if (optiune == 0) {
        return false;
    } else {
        std::cout << "Continuam fara afisarea raportului." << std::endl;
        return true;
    }
}

void Meci::afiseazaScorul() const {
    tabulate::Table tabelScor;
    tabelScor.add_row({"Echipa", "Seturi Castigate", "Necesar Victorie"});

    std::ostringstream ossG, ossO, ossNecesar;
    ossG << seturiCastigateGazda;
    ossO << seturiCastigateOaspete;
    if (format) {
        ossNecesar << format->getSeturiPentruVictorie();
    } else {
        ossNecesar << "3";
    }

    tabelScor.add_row({echipaGazda.getNumeEchipa(), ossG.str(), ossNecesar.str()});
    tabelScor.add_row({echipaOaspete.getNumeEchipa(), ossO.str(), ossNecesar.str()});

    std::cout << "\n--- SCOR CURENT MECI ---\n";
    std::cout << tabelScor << std::endl;
}


int Meci::getSeturiCastigateGazda() const { return seturiCastigateGazda;}

int Meci::getSeturiCastigateOaspete() const { return seturiCastigateOaspete; }

const std::vector<Set>& Meci::getSeturiJucate() const { return seturiJucate; }

std::ostream& operator<<(std::ostream& os, const Meci& meci) {
    if (meci.format) {
        os << "_____________________\n" << *meci.format << "\n_____________________\n";
    }
    os << "Rezumat meci: " << std::endl;
    os << "Total seturi disputate: " << meci.getSeturiJucate().size() << std::endl;

    if (!meci.getSeturiJucate().empty()) {
        tabulate::Table tabelSeturi;
        tabelSeturi.add_row({"Set", meci.echipaGazda.getNumeEchipa(), meci.echipaOaspete.getNumeEchipa()});

        for (size_t i = 0; i < meci.getSeturiJucate().size(); ++i) {
            std::ostringstream ossNr, ossPG, ossPO;
            ossNr << (i + 1);
            ossPG << meci.getSeturiJucate()[i].getPuncteProprii();
            ossPO << meci.getSeturiJucate()[i].getPuncteAdversari();
            tabelSeturi.add_row({ossNr.str(), ossPG.str(), ossPO.str()});
        }
        os << tabelSeturi << "\n";
    }

    os << "Scor general seturi: " << meci.getSeturiCastigateGazda() << " - " << meci.getSeturiCastigateOaspete() << std::endl;
    os << "Status: " << meci.determinaStareMeci() << std::endl;
    return os;
}
