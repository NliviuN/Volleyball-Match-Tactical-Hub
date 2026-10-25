#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "Jucator.h"
#include "Echipa.h"
#include "Set.h"

static std::vector<Jucator> citesteJucatoriDinFisier(const std::string& numeFisier) {
    std::vector<Jucator> listaJucatori;
    std::ifstream fisier(numeFisier);

    if (!fisier.is_open()) {
        std::cout<< "Nu s-a putut deschide fisierul!"<<std::endl;
        return listaJucatori;
    }

    std::string linie;
    while (std::getline(fisier, linie)) {
        if (linie.empty()) continue;
        std::stringstream ss(linie);
        std::string nume, postStr;
        double inaltime;
        int serva, preluare, pasa, atac, block, stamina;
        char virgula;
        std::getline(ss, nume, ',');
        std::getline(ss, postStr, ',');
        ss >> inaltime >> virgula >> serva >> virgula >> preluare >>virgula >> pasa >> virgula>> atac >> virgula >> block >> virgula >> stamina;
        while (!postStr.empty() && postStr[0] == ' ') {
            postStr.erase(0, 1);
        }
        PostJucator post = PostJucator::OutsideHitter;
        if (postStr == "Setter") post = PostJucator::Setter;
        else if (postStr == "MiddleBlocker") post = PostJucator::MiddleBlocker;
        else if (postStr == "Opposite") post = PostJucator::Opposite;
        else if (postStr == "Libero") post = PostJucator::Libero;

        StatisticiJucator stats(inaltime, serva, preluare, pasa, atac, block, stamina );
        Jucator jucator(nume, post, stats);

        listaJucatori.push_back(jucator);

    }
    return listaJucatori;
}

static void afisareJucatori(const std::vector<Jucator> &listaJucatori) {
    int nr = 1;
    for (const auto& jucator : listaJucatori) {
        std::cout << nr << ". " << jucator << std::endl;
        nr++;
    }
}

static void executaComanda(std::vector<Jucator> &listaJucatori, Echipa & echipa, const std::string& postStr, int& contorPost, PostJucator post) {
        if (contorPost > 0) {
            int numar;
            while (true) {
                std::cout << "Alege jucatorul in functie de numarul din lista: "<< std::endl;
                std::cin >> numar;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout<<"Introdu un numar valod!"<<std::endl;
                    continue;
                }
                if (numar < 1 || numar > static_cast<int> (listaJucatori.size())) {
                    std::cout << "Numarul nu exista in lista! Alege alt numar!";
                    continue;
                }

                if (listaJucatori[numar-1].getPost() != post) {
                    std::cout << "Acest jucator nu este un " << postStr <<"! Alege al jucator!"<<std::endl;
                    continue;
                }
                break;
            }

            echipa.adaugaJucator(listaJucatori[numar -1]);
            listaJucatori.erase(listaJucatori.begin()+(numar-1));
            contorPost --;
        }
        else std::cout << "Ai deja numarul maxim de "<< postStr <<"s! Alege alt post!"<<std::endl;
}

static Echipa creeazaEchipa(const std::string& nume, std::vector<Jucator> &listaJucatori) {
    Echipa echipa(nume);
    afisareJucatori(listaJucatori);
    int middleBlocker = 3, outsideHitter = 3, libero = 1, setter = 2, opposite = 1;
    std::cout <<"Alege jucatori: "  << std::endl;
    while (middleBlocker !=0 || libero !=0 || setter !=0 || opposite !=0 || outsideHitter != 0) {
        std::cout << "Alege post: "<< std::endl;
        std::string comanda;
        std::cin >> comanda;
        if (comanda == "OutsideHitter") {
            executaComanda(listaJucatori, echipa, comanda, outsideHitter, PostJucator::OutsideHitter);
        } else if (comanda == "Libero") {
            executaComanda(listaJucatori, echipa, comanda, libero, PostJucator::Libero);
        } else if (comanda == "MiddleBlocker") {
            executaComanda(listaJucatori, echipa, comanda, middleBlocker, PostJucator::MiddleBlocker);
        } else if (comanda == "Opposite") {
            executaComanda(listaJucatori, echipa, comanda, opposite, PostJucator::Opposite);
        } else if (comanda == "Setter") {
            executaComanda(listaJucatori, echipa, comanda, setter, PostJucator::Setter);
        }
        else std::cout << "Comanda Gresita!"<< std::endl;
        if (middleBlocker !=0 || libero !=0 || setter !=0 || opposite !=0 || outsideHitter != 0) {
            afisareJucatori(listaJucatori);
        }
        if (middleBlocker !=0 || libero !=0 || setter !=0 || opposite !=0 || outsideHitter != 0) {
            std::cout<< "Mai ai de ales " << (libero + opposite + outsideHitter + middleBlocker + setter) <<" jucatori! ";
            std::cout<<"(Middle Blocker: " << middleBlocker << ", ";
            std::cout<<"Opposite: " << opposite << ", ";
            std::cout<<"Setter: " << setter << ", ";
            std::cout<<"Outside Hitter: " << outsideHitter << ", ";
            std::cout<<"Libero: " << libero << ")"<<std::endl;
        }
    }
    return echipa;
}

static void adaugaJucatoriAutomat(std::vector<Jucator> &listaJucatori, Echipa &echipa, PostJucator postjucator, int contorPost) {
    for (auto it = listaJucatori.begin(); it != listaJucatori.end() && contorPost > 0;) {
        if (it->getPost() == postjucator) {
            echipa.adaugaJucator(*it);
            it = listaJucatori.erase(it);
            contorPost--;
        } else {
            ++it;
        }
    }
}

static Echipa creeazaEchipaAutomata(const std::string& nume, std::vector<Jucator> &listaJucatori) {
    Echipa echipa(nume);
    adaugaJucatoriAutomat(listaJucatori, echipa, PostJucator::MiddleBlocker,3);
    adaugaJucatoriAutomat(listaJucatori, echipa, PostJucator::OutsideHitter,3);
    adaugaJucatoriAutomat(listaJucatori, echipa, PostJucator::Libero,1);
    adaugaJucatoriAutomat(listaJucatori, echipa, PostJucator::Setter,2);
    adaugaJucatoriAutomat(listaJucatori, echipa, PostJucator::Opposite,1);

    for (const auto &j : echipa.getLot()) {
        if (j.getPost() == PostJucator::Libero) {
            echipa.seteazaLibero(j.getNume());
            break;
        }
    }

    PostJucator posturiCerute[6] = {
        PostJucator::OutsideHitter,
        PostJucator::Setter,
        PostJucator::MiddleBlocker,
        PostJucator::OutsideHitter,
        PostJucator::Opposite,
        PostJucator::MiddleBlocker
    };

    for (int zona =1 ; zona <= 6 ; ++zona) {
        for (const auto &j : echipa.getLot()) {
            if (j.getPost() == posturiCerute[zona-1]) {
                echipa.seteazaTitularPeZona(zona, j.getNume());
                break;
            }
        }
    }
    return echipa;
}


static void configureazaTeren(Echipa &echipa) {
    std::cout << "CONFIGURARE TITULARI: " << echipa.getNumeEchipa() << std::endl;
    for (const auto& j : echipa.getLot()) {
        if (j.getPost() == PostJucator::Libero) {
            echipa.seteazaLibero(j.getNume());
            break;
        }
    }



    for (int zona = 1 ; zona <= 6; ++zona) {

        static constexpr  PostJucator posturiCerute[6] = {
            PostJucator::OutsideHitter,
            PostJucator::Setter,
            PostJucator::MiddleBlocker,
            PostJucator::OutsideHitter,
            PostJucator::Opposite,
            PostJucator::MiddleBlocker
        };

        static constexpr std::string numePosturi[6] = {
            "OutsideHitter",
            "Setter",
            "MiddleBlocker",
            "OutsideHitter",
            "Opposite",
            "MiddleBlocker"
        };

        PostJucator postCautat = posturiCerute[zona-1];
        std::string numePost = numePosturi[zona-1];

        std::cout<< "Pentru zona " << zona << " trebuie sa alegi un " << numePost << std::endl;
        std::vector<Jucator> lotCurent = echipa.getLot();
        afisareJucatori(lotCurent);

        int numar;
        while (true) {
            std::cout << "Alege numarul jucatorului pentru Zona " << zona << "(" << numePost << "): ";
            std::cin >> numar;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Introdu un numar valid" << std::endl;
                continue;
            }

            if (numar < 1 || numar > static_cast<int>(lotCurent.size())) {
                std::cout << "Numarul nu exista in lista! Alege intre 1 si " << lotCurent.size() << "!" << std::endl;
                continue;
            }

            if (lotCurent[numar-1].getPost() != postCautat) {
                std::cout << "Acest jucator nu este " << numePost << "!" << std::endl;
                continue;
            }
            std::string numeAles = lotCurent[numar-1].getNume();
            if (echipa.seteazaTitularPeZona(zona, numeAles)) {
                break;
            }
        }
    }
    std::cout << std::endl << "Echipa de start a fost asezata pe teren!" << std::endl;
}

int main() {
    std::vector<Jucator> listaJucatori;
    listaJucatori = citesteJucatoriDinFisier("jucatori.txt");
    Echipa echipa = creeazaEchipa("Steaua",listaJucatori);
    configureazaTeren(echipa);
    Echipa echipa2 = creeazaEchipaAutomata("Dinamo",listaJucatori);
    Set primulSet(echipa, echipa2);
    primulSet.joacaSet();
    if (primulSet.AmCastigat()) {
        std::cout << "Am castigat primul set!" << std::endl;
    }
}