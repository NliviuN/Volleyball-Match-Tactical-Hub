#include "Echipa.h"

bool Echipa::esteZonaValida(int zona) {
    return zona >= 1 && zona <= 6;
}

bool Echipa::esteZonaSpate(int zona) {
    return zona == 1 || zona == 6 || zona == 5;
}

void Echipa::staminaRotatie() {
    for (auto & jRotatie : asezareRotatie) {
        for (const auto & jTeren : teren) {
            if (jRotatie.getNume() == jTeren.getNume()) {
                jRotatie = jTeren;
                break;
            }
        }
    }
}

Echipa::Echipa(std::string nume) :
numeEchipa(std::move(nume)), teren(), asezareRotatie(), esteInAsezareFixa(false),
libero(), inlocuitDeLibero(), liberoEsteInTeren(false), zonaLibero(-1),
schimbariEfectuate(0){}

void Echipa::adaugaJucator(const Jucator& jucator) {
    lot.push_back(jucator);
}

bool Echipa::seteazaTitularPeZona(int zona, const std::string& numeJucator) {
    if (!esteZonaValida(zona)) {
        std::cout<< "Eroare zona: alege 1-6!"<< std::endl;
        return false;
    }

    for (auto it = lot.begin(); it != lot.end(); ++it) {
        if (it->getNume() == numeJucator) {
            if (it->esteIndisponibil()) {
                std::cout<<"Jucatorul "<< numeJucator << " este indisponibil!" << std::endl;
                return false;
            }
            teren[zona-1] = *it;
            asezareRotatie[zona -1] = *it;
            lot.erase(it);
            std:: cout << numeJucator << " a fost asezat in zona " <<zona << std::endl;
            return true;
        }
    }
    return false;
}

bool Echipa::seteazaLibero(const std::string &numeJucator) {
    for (auto it = lot.begin(); it != lot.end(); ++it) {
        if (it->getNume() == numeJucator) {
            libero = *it;
            lot.erase(it);
            std:: cout << "Libero-ul echipei este " << numeJucator<<std::endl;
            return true;
        }
    }
    return false;
}

bool Echipa::efectueazaSchimbare(int zonaTeren, const std::string &numeRezerva) {
    if (esteInAsezareFixa) {
        revinoLaAsezareaInitiala();
    }
    if (schimbariEfectuate >= 6) {
        std::cout<<"In acest set pentru echipa"<< numeEchipa <<"s-a atins limita de 6 schimbari!"<< std::endl;
        return false;
    }
    if (!esteZonaValida(zonaTeren)) {
        return false;
    }
    if (liberoEsteInTeren && zonaTeren == zonaLibero) {
        std::cout << "Nu poti face schimbare standard pe pozitia ocupata de Libero!" << std::endl;
        return false;
    }

    for (auto& rez : lot) {
        if (rez.getNume() == numeRezerva && !rez.esteIndisponibil()) {
            std::cout << "Iese "<< teren[zonaTeren-1].getNume() << " (Zona " << zonaTeren << ") -> Intra " << rez.getNume() << std::endl;
            std::swap(teren[zonaTeren-1],rez);
            asezareRotatie[zonaTeren - 1] = teren[zonaTeren -1];
            schimbariEfectuate++;
            return true;
        }
    }
    std::cout<<"Rezerva care a fost aleasa nu este disponibila!"<<std::endl;
    return false;
}

void Echipa::jucatoriPePosturiFixe() {
    if (esteInAsezareFixa) {
        return;
    }

    asezareRotatie = teren;
    esteInAsezareFixa = true;

    for (int i=1; i<=3; ++i) {
        if (teren[i].getPost() == PostJucator::MiddleBlocker && i != 2) {
            std::swap(teren[i],teren[2]);
        }
    }

    for (int i=1 ;i<=3; ++i) {
        if (teren[i].getPost() == PostJucator::OutsideHitter && i != 3) {
            std::swap(teren[i],teren[3]);
        }
    }

    for (int i : {0, 4, 5}) {
        if ((teren[i].getPost() == PostJucator::Libero || teren [i].getPost() == PostJucator::MiddleBlocker) && i != 5) {
            std::swap(teren[i],teren[5]);
        }
    }
    for (int i : {0, 4, 5}) {
        if (teren[i].getPost() == PostJucator::OutsideHitter && i != 4) {
            std::swap(teren[i],teren[4]);
        }
    }
}

void Echipa::revinoLaAsezareaInitiala() {
    if (!esteInAsezareFixa) {
        return;
    }
    staminaRotatie();
    teren = asezareRotatie;
    esteInAsezareFixa = false;
}

void Echipa::roteste() {
    if (esteInAsezareFixa) {
        revinoLaAsezareaInitiala();
    }

    if (liberoEsteInTeren && zonaLibero == 5) {
        scoateLibero();
    }

    Jucator tempZona1 = teren[0];
    teren[0] = teren[1];
    teren[1] = teren[2];
    teren[2] = teren[3];
    teren[3] = teren[4];
    teren[4] = teren[5];
    teren[5] = tempZona1;

    if (liberoEsteInTeren) {
        if (zonaLibero ==1) {
            zonaLibero = 6;
        } else if (zonaLibero == 6) {
            zonaLibero = 5;
        }
    }

    asezareRotatie = teren;
}

bool Echipa::introduceLibero(int zonaSpate) {
    if (esteInAsezareFixa) {
        revinoLaAsezareaInitiala();
    }
    if (liberoEsteInTeren || !esteZonaSpate(zonaSpate)) {
        return false;
    }

    inlocuitDeLibero = teren[zonaSpate - 1];
    teren[zonaSpate - 1] = libero;
    asezareRotatie[zonaSpate - 1] = libero;
    liberoEsteInTeren = true;
    zonaLibero = zonaSpate;

    std::cout<< " Intra Libero-ul " << libero.getNume() << "in zona " << zonaSpate << " in locul lui " << inlocuitDeLibero.getNume()<< std::endl;
    return true;
}

void Echipa::scoateLibero() {
    if (!liberoEsteInTeren) {
        return;
    }
    if (esteInAsezareFixa) {
        revinoLaAsezareaInitiala();
    }
    libero = teren[zonaLibero - 1];
    teren[zonaLibero - 1] = inlocuitDeLibero;
    asezareRotatie[zonaLibero - 1] = inlocuitDeLibero;
    liberoEsteInTeren = false;
    zonaLibero = -1;
}

void Echipa::timeout() {
    std::cout << "TIMEOUT" << std::endl;
    for (auto& j: teren) {
        j.cresteStamina(10);
    }
}

const std::string& Echipa::getNumeEchipa() const {
    return numeEchipa;
}

Jucator& Echipa::getJucator(int zona) {
    if (!esteZonaValida(zona)) {
        return teren[0];
    }
    return teren[zona - 1];
}

int Echipa::getSetter() const {
    for (int i = 0; i < 6; ++i) {
        if (teren[i].getPost() == PostJucator::Setter) {
            return i+1;
        }
    }
    return 2;
}

const std::vector<Jucator>& Echipa::getLot() const { return lot; }

const std::array<Jucator, 6>& Echipa::getTeren() const { return teren;}

std::ostream& operator<<(std::ostream& os, const Echipa& e) {
    os << e.getNumeEchipa() << std::endl;
    os << "Titulari: ";
    for (const auto& j : e.getTeren()) {
        os << j << std::endl;
    }
    return os;
}

