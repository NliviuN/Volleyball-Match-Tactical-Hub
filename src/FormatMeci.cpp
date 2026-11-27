#include "FormatMeci.h"
#include "ExceptiiVolei.h"
#include <cmath>


FormatMeci::FormatMeci(const std::string& numeFormat, int seturiPentruVictorie, int punctePerSet)
    : numeFormat(numeFormat), seturiPentruVictorie(seturiPentruVictorie), punctePerSet(punctePerSet) {
    if (numeFormat.empty()) {
        throw EroareRegulament("Numele formatului de meci nu poate fi vid!");
    }
    if (seturiPentruVictorie <= 0 || punctePerSet <= 0) {
        throw EroareRegulament("Numarul de seturi si punctele per set trebuie sa fie strict pozitive (> 0)!");
    }
}

void FormatMeci::afiseaza(std::ostream& os) const {
    os << "Format: " << numeFormat
       << " | Primul la " << seturiPentruVictorie << " seturi"
       << " | Set standard: " << punctePerSet << " pct \n";

    afiseazaDetaliiSpecifice(os);

    os << "\n Limita schimbari: " << getLimitaSchimbari()
       << " | Recuperare stamina intre seturi: +" << calculeazaRecuperareStamina() << "%\n";
}

std::ostream& operator<<(std::ostream& os, const FormatMeci& format) {
    format.afiseaza(os);
    return os;
}


MeciOficial::MeciOficial(int seturiPentruVictorie, int punctePerSet, int puncteTieBreak, int challengeDisponibile)
    : FormatMeci("Meci Oficial FIVB", seturiPentruVictorie, punctePerSet),
      puncteTieBreak(puncteTieBreak),
      challengeDisponibile(challengeDisponibile) {
    if (puncteTieBreak <= 0 || challengeDisponibile < 0) {
        throw EroareRegulament("Parametri invalizi pentru MeciOficial (tie-break sau challenge)!");
    }
}

std::unique_ptr<FormatMeci> MeciOficial::clone() const {
    return std::make_unique<MeciOficial>(*this);
}

void MeciOficial::afiseazaDetaliiSpecifice(std::ostream& os) const {
    os << "Tie-Break: " << puncteTieBreak << " pct, Video Challenges: " << challengeDisponibile;
}

bool MeciOficial::esteSetTerminat(int numarSet, int puncteGazda, int puncteOaspete) const {
    int setMaximPosibil = 2 * getSeturiPentruVictorie() - 1;
    int tintaPuncte = (numarSet == setMaximPosibil) ? puncteTieBreak : getPunctePerSet();

    if (puncteGazda >= tintaPuncte || puncteOaspete >= tintaPuncte) {
        return std::abs(puncteGazda - puncteOaspete) >= 2;
    }
    return false;
}

double MeciOficial::calculeazaRecuperareStamina() const {
    return 10.0;
}

int MeciOficial::getLimitaSchimbari() const {
    return 6;
}

bool MeciOficial::solicitaChallengeVideo(const std::string& numeEchipa) {
    if (challengeDisponibile > 0) {
        std::cout << ">>> [VIDEO CHALLENGE] Echipa " << numeEchipa
                  << " solicita verificare video! (Challenge-uri ramase: "
                  << challengeDisponibile << ")\n";
        bool contestatie = (std::rand() % 100) < 38;

        if (contestatie) {
            std::cout << ">>> [DECIZIE SCHIMBATA] Reluarea arata ca " << numeEchipa
                      << " a avut dreptate! Punctul se intoarce, iar echipa isi pastreaza challenge-ul ("
                      << challengeDisponibile << " ramase).\n";
            return true;
        } else {
            challengeDisponibile--;
            std::cout << ">>> [DECIZIE MENTINUTA] Arbitrul confirma decizia initiala. "
                      << numeEchipa << " pierde un challenge ("
                      << challengeDisponibile << " ramase).\n";
            return false;
        }
    }
    std::cout << ">>> [VIDEO CHALLENGE] Echipa " << numeEchipa
              << " nu mai are solicitari Video Challenge disponibile!\n";
    return false;
}


MeciAmical::MeciAmical(int seturiPentruVictorie, int punctePerSet, bool permiteEgalitateLaDouaPuncte)
    : FormatMeci("Meci Amical", seturiPentruVictorie, punctePerSet),
      permiteEgalitateLaDouaPuncte(permiteEgalitateLaDouaPuncte) {}

std::unique_ptr<FormatMeci> MeciAmical::clone() const {
    return std::make_unique<MeciAmical>(*this);
}

void MeciAmical::afiseazaDetaliiSpecifice(std::ostream& os) const {
    os << "Regula 2 pct diferenta: " << (permiteEgalitateLaDouaPuncte ? "Da" : "Nu (Golden Point la tinta)");
}

bool MeciAmical::esteSetTerminat(int /*numarSet*/, int puncteGazda, int puncteOaspete) const {
    if (puncteGazda >= getPunctePerSet() || puncteOaspete >= getPunctePerSet()) {
        if (permiteEgalitateLaDouaPuncte) {
            return std::abs(puncteGazda - puncteOaspete) >= 2;
        }
        return puncteGazda != puncteOaspete;
    }
    return false;
}

double MeciAmical::calculeazaRecuperareStamina() const {
    return 25.0;
}

int MeciAmical::getLimitaSchimbari() const {
    return 12;
}


TurneuRapid::TurneuRapid(int seturiPentruVictorie, int punctePerSet, int pragPunctajMaxim)
    : FormatMeci("Turneu Rapid", seturiPentruVictorie, punctePerSet),
      pragPunctajMaxim(pragPunctajMaxim) {
    if (pragPunctajMaxim < punctePerSet) {
        throw EroareRegulament("Pragul maxim de puncte in TurneuRapid nu poate fi mai mic decat punctele per set!");
    }
}

std::unique_ptr<FormatMeci> TurneuRapid::clone() const {
    return std::make_unique<TurneuRapid>(*this);
}

void TurneuRapid::afiseazaDetaliiSpecifice(std::ostream& os) const {
    os << "Plafon maxim (Sudden Death): " << pragPunctajMaxim << " pct";
}

bool TurneuRapid::esteSetTerminat(int /*numarSet*/, int puncteGazda, int puncteOaspete) const {
    if (puncteGazda >= pragPunctajMaxim || puncteOaspete >= pragPunctajMaxim) {
        return puncteGazda != puncteOaspete;
    }
    if (puncteGazda >= getPunctePerSet() || puncteOaspete >= getPunctePerSet()) {
        return std::abs(puncteGazda - puncteOaspete) >= 2;
    }
    return false;
}

double TurneuRapid::calculeazaRecuperareStamina() const {
    return 15.0;
}

int TurneuRapid::getLimitaSchimbari() const {
    return 4;
}