#include "Set.h"
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<algorithm>

static std::string numePostText(PostJucator post) {
    switch (post) {
        case PostJucator::MiddleBlocker: return "Middle Blocker";
        case PostJucator::Libero: return "Libero";
        case PostJucator::Opposite: return "Opposite";
        case PostJucator::Setter: return "Setter";
        case PostJucator::OutsideHitter: return "Outside Hitter";
        default: return "Necunoscut";
    }
}

Set::Set(Echipa e1, Echipa e2)
    :echipaMea(e1), echipaAdversa(e2), puncteProprii(0), puncteAdversari(0), esteServiciulMeu(true) {
    std::srand(static_cast<int>(std::time(nullptr)));
}

bool Set::AmCastigat() const {
    return puncteProprii > puncteAdversari;
}

void Set::acordaPunct(bool pentruEchipaMea) {
    echipaMea.revinoLaAsezareaInitiala();
    echipaAdversa.revinoLaAsezareaInitiala();

    if (pentruEchipaMea) {
        puncteProprii++;
        if (!esteServiciulMeu) {
            std::cout << echipaMea.getNumeEchipa()<< " castiga serviciul si face rotatie!"<< std::endl;
            echipaMea.roteste();
            esteServiciulMeu = true;
        }
    } else {
        puncteAdversari++;
        if (esteServiciulMeu) {
            std::cout << echipaAdversa.getNumeEchipa()<< " castiga serviciul si face rotatie!"<< std::endl;
            echipaMea.roteste();
            esteServiciulMeu = false;
        }
    }
}

void Set::pregatesteLiberoInSpate(Echipa &echipa, bool esteLaServiciu) {
    std::vector<int> zonaSpate = esteLaServiciu ? std::vector<int> {6, 5} : std::vector<int>{6, 5, 1};
    for (int z: zonaSpate) {
        if (echipa.getJucator(z).getPost() == PostJucator::MiddleBlocker) {
            echipa.introduceLibero(z);
            break;
        }
    }
}

std::vector<int> Set::obtineZoneValideAtac(Echipa& echipa, int zonaSetter, int nivelPreluare) {
    std::vector<int> zoneValide;

    for (int z = 1; z <= 6; ++z) {
        if (z == zonaSetter) continue;
        PostJucator post = echipa.getJucator(z).getPost();
        if (post == PostJucator::Libero) continue;

        if (nivelPreluare == 1) {
            if (post == PostJucator::OutsideHitter || post == PostJucator::Opposite) {
                zoneValide.push_back(z);
            }

        } else if (nivelPreluare == 2) {
            if (post != PostJucator::MiddleBlocker) {
                zoneValide.push_back(z);
            }
        } else {
          zoneValide.push_back(z);
        }
    }

    if (zoneValide.empty()) {
        for (int z = 1; z <= 6; ++z) {
            if (z != zonaSetter && echipa.getJucator(z).getPost() != PostJucator::Libero) {
                zoneValide.push_back(z);
            }
        }
    }

    return zoneValide;
}

void Set::afisareMeniuTactic() {
    int optiune;
    do {
        std::cout << "Scor set: " << echipaMea.getNumeEchipa() << "(" << puncteProprii << ") - " << echipaAdversa.getNumeEchipa() << "(" << puncteAdversari << ")" << std::endl;
        std::cout << " La serviciu: " << (esteServiciulMeu ? echipaMea.getNumeEchipa() : echipaAdversa.getNumeEchipa()) << std::endl;        std::cout << "___________________________"<<std::endl;
        std::cout << "1. Joaca urmatorul punct!" << std::endl;
        std::cout << "2. Efectueaza o schimbare!" << std::endl;
        std::cout << "3. Timeout (+10 stamina)"<<std::endl;
        std::cout << "4. Afiseaza rezervele"<<std::endl;
        std::cout << "5. Afiseaza informatii despre teren (titulari/stamina)" <<std::endl;
        std::cout << "Alege o optiune: ";
        std::cin >> optiune;

        if (std::cin.eof()) {
            return;
        }

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000,'\n');
            std::cout << "Optiune invalda!"<<std::endl;
            continue;
        }

        if (optiune == 2) {
            int zona;
            std::string numeRezerva;
            std::cout << "Introdu numarul zonei(1-6) pentru schimbare: ";
            std::cin >> zona;
            std::cout<< "Introdu numele rezervei: ";
            std::cin.ignore();
            std::getline(std::cin,numeRezerva);
            echipaMea.efectueazaSchimbare(zona,numeRezerva);
        } else if (optiune == 3) {
            echipaMea.timeout();
        } else if (optiune == 4) {
            std::cout<< "Banca de rezerve:"<<std::endl;
            for (const auto& j : echipaMea.getLot()) {
                std::cout<<j<<std::endl;
            }
        } else if (optiune == 5) {
            std::cout<< "Titulari"<<std::endl;
            for (int i = 1; i<=6; i++) {
                std::cout<< "Zona " << i << echipaMea.getJucator(i).getNume() << " (Stamina: " << echipaMea.getJucator(i).getStamina() << ", Post: " << numePostText(echipaMea.getJucator(i).getPost())<< ")" << std::endl;
            }
        }
    }while (optiune != 1);
}

void Set::simuleazaRaliu() {
    pregatesteLiberoInSpate(echipaMea, esteServiciulMeu);
    pregatesteLiberoInSpate(echipaAdversa, !esteServiciulMeu);

    double punctajPreluareCurenta = 0.0;
    bool echipaMeaAtaca = false;

    if (esteServiciulMeu) {
        int tipServaAles = 2;
        std::cout << std::endl << "Serviciu: " << echipaMea.getJucator(1).getNume()<<std::endl;
        std::cout << "Alege tipul de serva: "<< std::endl;
        std::cout << "1. TopSpin (puternica, -10 stamina)" << std::endl;
        std::cout << "2. Float (tactica, -5 stamina)" << std::endl;
        std::cout << "Optiune (1 sau 2): ";
        std::cin >> tipServaAles;

        TipServa servaAleasa = (tipServaAles == 1) ? TipServa::TopSpin : TipServa::Float;
        double punctajServa = echipaMea.getJucator(1).executaServa(servaAleasa);
        echipaMea.jucatoriPePosturiFixe();

        if (punctajServa <40.0) {
            std::cout << "Serva gresita! Punct pentru adversari!"<<std::endl;
            acordaPunct(false);
            return;
        }

        echipaAdversa.jucatoriPePosturiFixe();
        std::vector<int> zoneSpate = {5, 6, 1};
        int zonaPrimitor = zoneSpate[std::rand() % zoneSpate.size()];

        std::cout << "Adversarul preia serva prin " << echipaAdversa.getJucator(zonaPrimitor).getNume() << " (Zona " << zonaPrimitor << ")"<<std::endl;
        punctajPreluareCurenta = echipaAdversa.getJucator(zonaPrimitor).executaPreluare(punctajServa);

        if (punctajPreluareCurenta <= 0.0) {
            std::cout << ">>> ACE! Punct direct pentru echipa ta!" << std::endl;
            acordaPunct(true);
            return;
        }
        echipaMeaAtaca = false;
    } else {
        TipServa servaBot = (std::rand() % 100 < 60) ?TipServa::TopSpin : TipServa::Float;
        std::cout << "Adversarul executa o serva de tip " << (servaBot == TipServa::TopSpin ? "TopSpin" : "Float") << " prin jucatorul " << echipaAdversa.getJucator(1).getNume() << "!" << std::endl;
        double punctajServaAdvers = echipaAdversa.getJucator(1).executaServa(servaBot);
        echipaAdversa.jucatoriPePosturiFixe();

        if (punctajServaAdvers < 40.0) {
            std::cout << "Adversarul a gresit serva! Punct pentru echipa ta!" << std::endl;
            acordaPunct(true);
            return;
        }

        echipaMea.jucatoriPePosturiFixe();
        std::vector<int> zoneSpate = {6, 5, 1};
        int zonaPrimitor = zoneSpate[std::rand() % zoneSpate.size()];

        std::cout << echipaMea.getJucator(zonaPrimitor).getNume() << " (Zona " << zonaPrimitor << ") incearca preluarea"<<std::endl;
        punctajPreluareCurenta = echipaMea.getJucator(zonaPrimitor).executaPreluare(punctajServaAdvers);

        if (punctajPreluareCurenta <= 0.0) {
            std::cout << "ACE Advers! Punct pentru adversari!"<<std::endl;
            acordaPunct(false);
            return;
        }
        echipaMeaAtaca = true;
    }

    bool mingeInJoc = true;
    while (mingeInJoc) {
        int nivelPreluare = 1;
        if (punctajPreluareCurenta >= 30.0) {
            nivelPreluare = 3;
        } else if (punctajPreluareCurenta >= 15.0) {
            nivelPreluare = 2;
        } else {
            nivelPreluare = 1;
        }
        if (!echipaMeaAtaca) {
            std::cout<< "Preluare de nivel " << nivelPreluare <<std::endl;

            int zonaSetterAdvers = echipaAdversa.getSetter();
            double punctajPasaAdvers = echipaAdversa.getJucator(zonaSetterAdvers).executaPasa(punctajPreluareCurenta);
            std::vector<int> zoneValideAtac = obtineZoneValideAtac(echipaAdversa, zonaSetterAdvers, nivelPreluare);
            int zonaAtacAdvers = zoneValideAtac[std::rand() % zoneValideAtac.size()];
            double punctajAtacAdvers = echipaAdversa.getJucator(zonaAtacAdvers).executaAtac(punctajPasaAdvers);
            std::cout << "Setter-ul " << echipaAdversa.getJucator(zonaSetterAdvers).getNume()
                          << " ridica pentru atacul lui " << echipaAdversa.getJucator(zonaAtacAdvers).getNume()
                          << " (Zona " << zonaAtacAdvers << ")!" << std::endl;

            int catiBlocheaza = 0;
            std::cout << "Cati jucatori vrei sa sara la blocaj? (1, 2 sau 3): ";
            std::cin >> catiBlocheaza;
            while (catiBlocheaza < 1 || catiBlocheaza > 3) {
                std::cout << "Alege un numar valid intre 1 si 3: ";
                std::cin >> catiBlocheaza;
            }
            std::vector<int> zoneAleseBlocaj;
            if (catiBlocheaza == 3) {
                zoneAleseBlocaj = {2, 3, 4};
            } else {
                for (int i = 1; i <= catiBlocheaza; ++i) {
                    int zonaBlocant = 0;
                    bool zonaValida = false;
                    while (!zonaValida) {
                        std::cout << "Alege zona jucatorului #" << i << " care sare la block (2, 3 sau 4): ";
                        std::cin >> zonaBlocant;

                        if (zonaBlocant < 2 || zonaBlocant > 4) {
                            std::cout << "Doar jucatorii din prima linie (zonele 2, 3 sau 4) pot bloca!\n";
                            continue;
                        }
                        if (std::ranges::find(zoneAleseBlocaj, zonaBlocant) != zoneAleseBlocaj.end()) {
                            std::cout << "Deja ai ales aceasta zona!\n";
                            continue;
                        }
                        zonaValida = true;
                        zoneAleseBlocaj.push_back(zonaBlocant);
                    }
                }
            }
            std::cout<<"Sar la blocaj: ";
            double maxBlock = 0.0;
            double sumaBlock = 0.0;
            for (int z : zoneAleseBlocaj) {
                std::cout << echipaMea.getJucator(z).getNume() << " (Z" << z << ") " << std::endl;
                double val = echipaMea.getJucator(z).executaBlocaj();
                if (val > maxBlock) maxBlock = val;
                sumaBlock += val;
            }
            double punctajBlocajTotal = maxBlock + (sumaBlock - maxBlock) * 0.35;
            if (punctajBlocajTotal >= punctajAtacAdvers + 10.0) {
                std::cout << ">>> BLOCAJ IMPENETRABIL! Punct direct pentru echipa ta!\n";
                acordaPunct(true);
                mingeInJoc = false;
            } else if (punctajAtacAdvers >= punctajBlocajTotal + 25.0) {
                std::cout << ">>> ATAC DEVASTATOR al adversarului pe langa blocaj! Punct pentru ei!\n";
                acordaPunct(false);
                mingeInJoc = false;
            } else {
                std::cout << "Atacul trece de blocaj! Mingea se duce in terenul tau!" << std::endl;
                int setterMeu = echipaMea.getSetter();
                int zonaPreluareDefensiva = 6;
                do {
                    zonaPreluareDefensiva = (std::rand() % 6) + 1;
                } while (zonaPreluareDefensiva == setterMeu);

                std::cout << echipaMea.getJucator(zonaPreluareDefensiva).getNume()
                          << " (Zona " << zonaPreluareDefensiva << ") plonjeaza dupa minge!" << std::endl;

                double fortaAtacRamasa = std::max(25.0, punctajAtacAdvers - (punctajBlocajTotal * 0.45));
                double rezultatDefensiva = echipaMea.getJucator(zonaPreluareDefensiva).executaPreluare(fortaAtacRamasa);

                if (rezultatDefensiva > 0.0) {
                    std::cout << "Preluare spectaculoasa! Mingea ramane in joc, organizam contraatacul!" <<std::endl;
                    punctajPreluareCurenta = rezultatDefensiva;
                    echipaMeaAtaca = true;
                } else {
                    std::cout << "Preluarea defensiva a esuat! Punct pentru adversari!" << std::endl;
                    acordaPunct(false);
                    mingeInJoc = false;
                }
            }
        } else {
            int setterMeu = echipaMea.getSetter();
            double punctajPasaMea = echipaMea.getJucator(setterMeu).executaPasa(punctajPreluareCurenta);
            std::vector<int> zoneValideAtac = obtineZoneValideAtac(echipaMea, setterMeu, nivelPreluare);
            std::cout << "Setter-ul " << echipaMea.getJucator(setterMeu).getNume()
                      << " este gata sa paseze. Alege cui ridica mingea:" << std::endl;
            for (int z : zoneValideAtac) {
                std::cout << " - Zona " << z << ": " << echipaMea.getJucator(z).getNume()
                          << " (" << numePostText(echipaMea.getJucator(z).getPost()) << ")" << std::endl;
            }

            int zonaAtacAleasa = zoneValideAtac[0];
            bool alegereCorecta = false;
            while (!alegereCorecta) {
                std::cout << "Introdu zona atacantului dorit: ";
                std::cin >> zonaAtacAleasa;
                if (std::ranges::find(zoneValideAtac, zonaAtacAleasa) != zoneValideAtac.end()) {
                    alegereCorecta = true;
                } else {
                    std::cout << "Zona aleasa nu este disponibila pentru aceasta faza de atac! Mai incearca." << std::endl;
                }
            }
            double punctajAtacMeu = echipaMea.getJucator(zonaAtacAleasa).executaAtac(punctajPasaMea);
            std::cout << echipaMea.getJucator(zonaAtacAleasa).getNume() << " ataca din zona " << zonaAtacAleasa << "!" << std::endl;
            int catiBlocheazaBot = 2;
            if (nivelPreluare == 1) {
                catiBlocheazaBot = (std::rand() % 2) + 2;
            } else if (nivelPreluare == 3) {
                catiBlocheazaBot = (std::rand() % 2) + 1;
            } else {
                catiBlocheazaBot = (std::rand() % 3) + 1;
            }

            std::vector<int> frontRowBot = {3, 2, 4};
            double maxBlockBot = 0.0;
            double sumaBlockBot = 0.0;
            for (int i = 0; i < catiBlocheazaBot; ++i) {
                int z = frontRowBot[i];
                double val = echipaAdversa.getJucator(z).executaBlocaj();
                if (val > maxBlockBot) maxBlockBot = val;
                sumaBlockBot += val;
            }
            double punctajBlocajBot = maxBlockBot + (sumaBlockBot - maxBlockBot) * 0.35;
            std::cout << "Adversarul ridica un blocaj cu " << catiBlocheazaBot << " jucatori!" << std::endl;

            if (punctajBlocajBot >= punctajAtacMeu + 10.0) {
                std::cout << ">>> BLOCAJ ADVERS! Punct pentru adversari!" << std::endl;
                acordaPunct(false);
                mingeInJoc = false;
            } else if (punctajAtacMeu >= punctajBlocajBot + 25.0) {
                std::cout << ">>> ATAC IMPARABIL! Punct pentru echipa ta!" << std::endl;;
                acordaPunct(true);
                mingeInJoc = false;
            } else {
                std::cout << "Atacul tau trece de blocaj! Adversarii incearca sa salveze mingea din aparare" << std::endl;
                int setterAdvers = echipaAdversa.getSetter();
                int zonaAparareBot = 6;
                do {
                    zonaAparareBot = (std::rand() % 6) + 1;
                } while (zonaAparareBot == setterAdvers);

                double fortaAtacRamasa = std::max(25.0, punctajAtacMeu - (punctajBlocajBot * 0.45));
                double rezultatDefensivaBot = echipaAdversa.getJucator(zonaAparareBot).executaPreluare(fortaAtacRamasa);

                if (rezultatDefensivaBot > 0.0) {
                    std::cout << " Adversarul salveaza mingea prin "<< echipaAdversa.getJucator(zonaAparareBot).getNume() << "! Raliul continua!" << std::endl;
                    punctajPreluareCurenta = rezultatDefensivaBot;
                    echipaMeaAtaca = false;
                } else {
                    std::cout << " Apararea adversa cedeaza! Punct castigat de echipa ta!" << std::endl;
                    acordaPunct(true);
                    mingeInJoc = false;
                }
            }
        }
    }
}

void Set::joacaSet() {
    std::cout << " Incepe setul: " << echipaMea.getNumeEchipa() << " vs " << echipaAdversa.getNumeEchipa() << std::endl;

    while ((!std::cin.eof() && puncteProprii < 5 && puncteAdversari < 5) || std::abs(puncteProprii - puncteAdversari) < 2) {
        afisareMeniuTactic();
        if (std::cin.eof()) {
            break;
        }

        simuleazaRaliu();

        if (std::cin.eof()) {
            break;
        }
    }

    if (AmCastigat()) {
        std::cout << " SET CASTIGAT DE " << echipaMea.getNumeEchipa() << " (" << puncteProprii << " - " << puncteAdversari << ")!" << std::endl;
    } else {
        std::cout << " SET CASTIGAT DE " << echipaAdversa.getNumeEchipa() << " (" << puncteAdversari << " - " << puncteProprii << ")!" <<std::endl;
    }
}

const Echipa& Set::getEchipaAdversa() const { return echipaAdversa; }

const Echipa& Set::getEchipaMea() const { return echipaMea; }

int Set::getPuncteAdversari() const { return puncteAdversari; }

int Set::getPuncteProprii() const { return puncteProprii; }

std::ostream& operator<< (std::ostream& os, const Set& set) {
    os << set.getEchipaMea() << " vs " << set.getEchipaAdversa() << " (" << set.getPuncteProprii() << "-" << set.getPuncteAdversari() << ")" << std::endl;
    return os;
}