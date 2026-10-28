#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "Jucator.h"
#include "Echipa.h"
#include "Set.h"
// cppcheck-suppress missingInclude
#include  "nlohmann/json.hpp"
#include "Meci.h"

struct EchipaSalvata {
    Echipa echipa;
    std::vector<Jucator> jucatoriRamasi;
};

static std::vector<Jucator> citesteJucatoriDinFisier(const std::string& numeFisier) {
    std::vector<Jucator> listaJucatori;
    std::ifstream fisier(numeFisier);

    if (!fisier.is_open()) {
        std::cout<< "Nu s-a putut deschide fisierul!"<<std::endl;
        return listaJucatori;
    }

    nlohmann::json jData;
    fisier >> jData;

    for (const auto& elem : jData) {
        std::string nume = elem["nume"];
        std::string postStr = elem["post"];
        double inaltime = elem["inaltime"];
        int serva = elem["serva"];
        int preluare = elem["preluare"];
        int pasa = elem["pasa"];
        int atac = elem["atac"];
        int block = elem["block"];
        int stamina = elem["stamina"];

        PostJucator post = PostJucator::OutsideHitter;
        if (postStr == "Setter") post = PostJucator::Setter;
        else if (postStr == "MiddleBlocker") post = PostJucator::MiddleBlocker;
        else if (postStr == "Opposite") post = PostJucator::Opposite;
        else if (postStr == "Libero") post = PostJucator::Libero;

        StatisticiJucator stats(inaltime, serva, preluare, pasa, atac, block, stamina);
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

    const PostJucator posturiCerute[6] = {
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

        static const std::string numePosturi[6] = {
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

static void joacaMeciComplet(const Echipa &echipa1, const Echipa &echipa2) {
    Meci meci(echipa1, echipa2);
    meci.afiseazaScorul();

    while (!meci.esteMeciulTerminat()) {
        Set setCurent(echipa1, echipa2);
        setCurent.joacaSet();
        meci.inregistreazaSet(setCurent, setCurent.AmCastigat());
        std::cout << meci.determinaStareMeci() << std::endl;
        meci.afiseazaScorul();
        meci.meniuRaportStatistic();
    }
    meci.reseteazaMeci();
}

int main() {
    std::vector<Jucator> listaJucatori;
    listaJucatori = citesteJucatoriDinFisier("jucatori.json");
    std::vector<EchipaSalvata> echipeCreate;
    int optiuneMeniu = 0;
    while (true) {
        std::cout << "===============================" << std::endl;
        std::cout << "         MENIU PRINCIPAL       " << std::endl;
        std::cout << "===============================" << std::endl;
        std::cout << "1. Creeaza echipa" << std::endl;
        std::cout << "2. Alege echipa" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Alege o optiune: ";

        if (!(std::cin >> optiuneMeniu)) {
            break;
        }

        if (optiuneMeniu == 1) {
            std::cout << "Introdu numele echipei: ";
            std::string numeEchipa;
            if (!(std::cin >> numeEchipa)) {
                break;
            }

            std::vector<Jucator> jucatoriDisponibili = listaJucatori;
            Echipa echipaNoua = creeazaEchipa(numeEchipa, jucatoriDisponibili);
            configureazaTeren(echipaNoua);

            echipeCreate.push_back({echipaNoua, jucatoriDisponibili});
            std::cout << "Echipa " << numeEchipa << " a fost adaugata automat in 'Alege echipa'!" << std::endl;

        } else if (optiuneMeniu == 2) {
            if (echipeCreate.empty()) {
                std::cout << "Nu exista nicio echipa creata! Creeaza una mai intai." << std::endl;
                continue;
            }

            std::cout << "Alege echipa:" << std::endl;
            for (int i = 0; i < static_cast<int>(echipeCreate.size()); ++i) {
                std::cout << i + 1 << ". " << echipeCreate[i].echipa.getNumeEchipa() << std::endl;
            }
            std::cout << "Alege numarul echipei: ";

            int indexEchipa = 0;
            if (!(std::cin >> indexEchipa)) {
                break;
            }

            if (indexEchipa < 1 || indexEchipa > static_cast<int>(echipeCreate.size())) {
                std::cout << "Numar invalid! Revenire la meniul principal." << std::endl;
                continue;
            }

            EchipaSalvata selectie = echipeCreate[indexEchipa - 1];
            std::cout << "Ai ales echipa: " << selectie.echipa.getNumeEchipa() << std::endl;

            std::cout << "1. Joaca" << std::endl;
            std::cout << "2. Exit" << std::endl;
            std::cout << "Alege o optiune: ";

            int optiuneJoc = 0;
            if (!(std::cin >> optiuneJoc)) {
                break;
            }

            if (optiuneJoc == 1) {
                std::vector<Jucator> ramasiPentruAdversar = selectie.jucatoriRamasi;
                Echipa echipaAdversa = creeazaEchipaAutomata("Adversar_" + selectie.echipa.getNumeEchipa(), ramasiPentruAdversar);

                joacaMeciComplet(selectie.echipa, echipaAdversa);

            } else if (optiuneJoc == 2) {
                std::cout << "Iesire din joc" << std::endl;
                break;
            } else {
                std::cout << "Optiune invalida!" << std::endl;
            }

        } else if (optiuneMeniu == 3) {
            std::cout << "Exit!" << std::endl;
            break;
        } else {
            std::cout << "Optiune gresita! Alege 1, 2 sau 3." << std::endl;
        }
    }

    return 0;
}