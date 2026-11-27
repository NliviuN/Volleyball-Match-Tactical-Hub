#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "Jucator.h"
#include "Echipa.h"
#include "Set.h"
// cppcheck-suppress missingInclude
#include  <nlohmann/json.hpp> // v3.12.0
#include <tabulate/table.hpp>
#include <sstream>

#include "ExceptiiVolei.h"
#include "Meci.h"

struct EchipaSalvata {
    Echipa echipa;
    std::vector<Jucator> jucatoriRamasi;
};

static void afisareEchipaCompleta(const Echipa& echipa) {
    std::cout << "\n========================================\n";
    std::cout << " ECHIPA: " << echipa.getNumeEchipa() << "\n";
    std::cout << "========================================\n";

    std::cout << "[LIBERO ACTIV]\n";
    std::ostringstream ossLibero;
    ossLibero << echipa.getLibero();
    std::cout << "  - " << ossLibero.str() << "\n\n";

    std::cout << "[TITULARI PE TEREN]\n";
    tabulate::Table tabelTitulari;
    tabelTitulari.format().locale("C");
    tabelTitulari.add_row({"Zona", "Jucator"});
    for (int zona = 1; zona <= 6; ++zona) {
        std::ostringstream ossZona, ossJucator;
        ossZona << "Zona " << zona;
        ossJucator << echipa.getTitularZona(zona);
        tabelTitulari.add_row({ossZona.str(), ossJucator.str()});
    }
    std::cout << tabelTitulari << "\n\n";

    std::cout << "[REZERVE PE BANCA]\n";
    const auto& rezerve = echipa.getLot();
    if (rezerve.empty()) {
        std::cout << "  Nicio rezerva disponibila.\n";
    } else {
        tabulate::Table tabelRezerve;
        tabelRezerve.format().locale("C");
        tabelRezerve.add_row({"Nr.", "Jucator"});
        for (size_t i = 0; i < rezerve.size(); ++i) {
            std::ostringstream ossNr, ossJucator;
            ossNr << (i + 1);
            ossJucator << rezerve[i];
            tabelRezerve.add_row({ossNr.str(), ossJucator.str()});
        }
        std::cout << tabelRezerve << "\n";
    }
    std::cout << "========================================\n";
}

static std::vector<Jucator> citesteJucatoriDinFisier(const std::string& numeFisier) {
    std::vector<Jucator> listaJucatori;
    std::ifstream fisier(numeFisier);

    if (!fisier.is_open()) {
        throw EroareDate("Nu s-a putut deschide fisierul: " + numeFisier);
    }

    nlohmann::json jData;
    try {
        fisier >> jData;
    } catch (const nlohmann::json::exception& e) {
        throw EroareDate("Fisierul JSON este corupt sau invalid: " + std::string(e.what()));
    }

    if (!jData.is_array() || jData.empty()) {
        throw EroareDate("Fisierul " + numeFisier + " nu contine o lista valida de jucatori!");
    }

    try {
        for (const auto& elem : jData) {
            std::string nume = elem.at("nume");
            std::string postStr = elem.at("post");
            double inaltime = elem.at("inaltime");
            int serva = elem.at("serva");
            int preluare = elem.at("preluare");
            int pasa = elem.at("pasa");
            int atac = elem.at("atac");
            int block = elem.at("block");
            int stamina = elem.at("stamina");

            PostJucator post;
            if (postStr == "Outside Hitter") post = PostJucator::Outside_Hitter;
            else if (postStr == "Setter") post = PostJucator::Setter;
            else if (postStr == "Middle Blocker") post = PostJucator::Middle_Blocker;
            else if (postStr == "Opposite") post = PostJucator::Opposite;
            else if (postStr == "Libero") post = PostJucator::Libero;
            else {
                throw EroareDate("Post necunoscut '" + postStr + "' pentru jucatorul " + nume);
            }

            StatisticiJucator stats(inaltime, serva, preluare, pasa, atac, block, stamina);
            listaJucatori.emplace_back(nume, post, stats);
        }
    } catch (const nlohmann::json::exception& e) {
        throw EroareDate("Structura JSON invalida pentru un jucator: " + std::string(e.what()));
    }

    return listaJucatori;
}


static int afiseazaJucatoriDupaPost(const std::vector<Jucator> &listaJucatori, const PostJucator post, int bugetDisponibil = 10000) {
    std::string postStr;
    if (post == PostJucator::Libero) postStr = "Libero";
    else if (post == PostJucator::Middle_Blocker) postStr = "Middle Blocker";
    else if (post == PostJucator::Outside_Hitter) postStr = "Outside Hitter";
    else if (post == PostJucator::Opposite) postStr = "Opposite";
    else if (post == PostJucator::Setter) postStr = "Setter";

    std::cout << "\n *** Lista " << postStr << "s";
    if (bugetDisponibil < 10000) {
        std::cout << " (Buget disponibil: " << bugetDisponibil << ") *** \n";
    } else { std::cout << " *** \n"; }

    tabulate::Table tabelJucatoriPost;
    tabelJucatoriPost.format().locale("C");
    tabelJucatoriPost.add_row({"Nr.", "Jucator si Statistici", "Punctaj Total"});

    int contor = 1;
    for (const Jucator& j: listaJucatori) {
        if (j.getPost() == post && j.getPunctajTotal() <= bugetDisponibil) {
            std::ostringstream ossNr, ossDetalii, ossPunctaj;
            ossNr << contor;
            ossDetalii << j;
            ossPunctaj << j.getPunctajTotal();

            tabelJucatoriPost.add_row({ossNr.str(), ossDetalii.str(), ossPunctaj.str()});
            contor++;
        }
    }

    if (contor == 1) {
        std::cout << "Nu există jucători disponibili pentru acest post în bugetul curent.\n";
    } else {
        tabelJucatoriPost.column(1).format().width(55);
        std::cout << tabelJucatoriPost << "\n";
    }

    std::cout << "----------------------------------------------------------------\n";
    return contor - 1;
}

static Jucator gasesteInListaDupaNumarSiPost(const std::vector<Jucator> &listaJucatori, PostJucator post, int numar, int bugetDisponibil = 10000) {
    int contor = 1;
    for (const Jucator& j: listaJucatori) {
        if (j.getPost() == post && j.getPunctajTotal() <= bugetDisponibil) {
            if (contor == numar) {
                return j;
            }
            contor++;
        }
    }
    return listaJucatori.front();
}

static bool alegeJucator(int &numar, int numarPost, const std::vector<int> &jucatoriAlesi) {
    if (!(std::cin >> numar)) {
        if (std::cin.eof()) {
            return false;
        }
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Introdu un numar valid!\n" << std::endl;
        return false;
    }

    if (numar < 1 || numar > numarPost) {
        std::cout << "Numarul nu se afla in lista! Alege intre 1 si " << numarPost << ".\n";
        return false;
    }

    if (std::ranges::find(jucatoriAlesi, numar) != jucatoriAlesi.end()) {
        std::cout << "Acest jucator a fost deja ales!\n";
        return false;
    }

    return true;
}

static Echipa creeazaEchipa(const std::string& nume, std::vector<Jucator>& listaJucatori) {
    Echipa echipa(nume);

    int bugetMiddle = 1765;
    int bugetOutside = 1835;
    int bugetLibero = 500;
    int bugetOpposite = 1175;
    int bugetSetter = 595;

    int middleBlocker = 0, outsideHitter = 0, libero = 0, opposite = 0, setter = 0;
    int numar = 0;

    std::cout << "----------------------------------------\n";
    std::cout << "Alege jucatori (Sistem Bugetar & Strategie): " << std::endl;
    std::cout << "----------------------------------------\n";

    while (middleBlocker < 3 && !std::cin.eof()) {
        int maxAcum = bugetMiddle - (2 - middleBlocker) * 575;

        int numarMiddle = afiseazaJucatoriDupaPost(listaJucatori, PostJucator::Middle_Blocker, maxAcum);
        if (numarMiddle == 0) {
            std::cout << "Nu mai ai jucatori in acest buget!\n";
            break;
        }

        std::cout << "\n[Buget MB curent: " << bugetMiddle << " | Plafon selectie: " << maxAcum << "]\n";
        std::cout << "Alege un Middle Blocker dupa numarul din lista: ";

        std::vector<int> dummy;
        if (alegeJucator(numar, numarMiddle, dummy)) {
            Jucator jucator = gasesteInListaDupaNumarSiPost(listaJucatori, PostJucator::Middle_Blocker, numar, maxAcum);

            bugetMiddle -= static_cast<int>(jucator.getPunctajTotal());
            echipa.adaugaJucator(jucator);

            std::erase_if(listaJucatori, [&jucator](const Jucator& j) {
                return j.getNume() == jucator.getNume();
            });
            middleBlocker++;
        }
    }
    if (std::cin.eof()) return echipa;

    bugetOutside += bugetMiddle;

    while (outsideHitter < 3 && !std::cin.eof()) {
        int maxAcum = bugetOutside - (2 - outsideHitter) * 596;

        int numarOutside = afiseazaJucatoriDupaPost(listaJucatori, PostJucator::Outside_Hitter, maxAcum);
        if (numarOutside == 0) {
            std::cout << "Nu mai ai jucatori in acest buget!\n";
            break;
        }

        std::cout << "\n[Buget OH curent: " << bugetOutside << " | Plafon selectie: " << maxAcum << "]\n";
        std::cout << "Alege un Outside Hitter dupa numarul din lista: ";

        std::vector<int> dummy;
        if (alegeJucator(numar, numarOutside, dummy)) {
            Jucator jucator = gasesteInListaDupaNumarSiPost(listaJucatori, PostJucator::Outside_Hitter, numar, maxAcum);

            bugetOutside -= static_cast<int>(jucator.getPunctajTotal());
            echipa.adaugaJucator(jucator);

            std::erase_if(listaJucatori, [&jucator](const Jucator& j) {
                return j.getNume() == jucator.getNume();
            });
            outsideHitter++;
        }
    }
    if (std::cin.eof()) return echipa;

    bugetLibero += bugetOutside;

    while (libero < 1 && !std::cin.eof()) {
        int numarLibero = afiseazaJucatoriDupaPost(listaJucatori, PostJucator::Libero, bugetLibero);
        if (numarLibero == 0) {
            std::cout << "Nu mai ai jucatori in acest buget!\n";
            break;
        }

        std::cout << "\n[Buget Libero disponibil: " << bugetLibero << "]\n";
        std::cout << "Alege Libero-ul dupa numarul din lista: ";

        std::vector<int> dummy;
        if (alegeJucator(numar, numarLibero, dummy)) {
            Jucator jucator = gasesteInListaDupaNumarSiPost(listaJucatori, PostJucator::Libero, numar, bugetLibero);

            bugetLibero -= static_cast<int>(jucator.getPunctajTotal());
            echipa.adaugaJucator(jucator);

            std::erase_if(listaJucatori, [&jucator](const Jucator& j) {
                return j.getNume() == jucator.getNume();
            });
            libero++;
        }
    }
    if (std::cin.eof()) return echipa;

    bugetOpposite += bugetLibero;

    while (opposite < 2 && !std::cin.eof()) {
        int maxAcum = bugetOpposite - (1 - opposite) * 582;

        int numarOpposite = afiseazaJucatoriDupaPost(listaJucatori, PostJucator::Opposite, maxAcum);
        if (numarOpposite == 0) {
            std::cout << "Nu mai ai jucatori in acest buget!\n";
            break;
        }

        std::cout << "\n[Buget Opposite curent: " << bugetOpposite << " | Plafon selectie: " << maxAcum << "]\n";
        std::cout << "Alege un Opposite dupa numarul din lista: ";

        std::vector<int> dummy;
        if (alegeJucator(numar, numarOpposite, dummy)) {
            Jucator jucator = gasesteInListaDupaNumarSiPost(listaJucatori, PostJucator::Opposite, numar, maxAcum);

            bugetOpposite -= static_cast<int>(jucator.getPunctajTotal());
            echipa.adaugaJucator(jucator);

            std::erase_if(listaJucatori, [&jucator](const Jucator& j) {
                return j.getNume() == jucator.getNume();
            });
            opposite++;
        }
    }
    if (std::cin.eof()) return echipa;

    bugetSetter += bugetOpposite;

    // --- 5. SETTER (1 jucător) ---
    while (setter < 1 && !std::cin.eof()) {
        int numarSetter = afiseazaJucatoriDupaPost(listaJucatori, PostJucator::Setter, bugetSetter);
        if (numarSetter == 0) {
            std::cout << "Nu mai ai jucatori in acest buget!\n";
            break;
        }

        std::cout << "\n[Buget Setter disponibil: " << bugetSetter << "]\n";
        std::cout << "Alege Setter-ul dupa numarul din lista: ";

        std::vector<int> dummy;
        if (alegeJucator(numar, numarSetter, dummy)) {
            Jucator jucator = gasesteInListaDupaNumarSiPost(listaJucatori, PostJucator::Setter, numar, bugetSetter);

            bugetSetter -= static_cast<int>(jucator.getPunctajTotal());
            echipa.adaugaJucator(jucator);

            std::erase_if(listaJucatori, [&jucator](const Jucator& j) {
                return j.getNume() == jucator.getNume();
            });
            setter++;
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
    adaugaJucatoriAutomat(listaJucatori, echipa, PostJucator::Middle_Blocker,3);
    adaugaJucatoriAutomat(listaJucatori, echipa, PostJucator::Outside_Hitter,3);
    adaugaJucatoriAutomat(listaJucatori, echipa, PostJucator::Libero,1);
    adaugaJucatoriAutomat(listaJucatori, echipa, PostJucator::Setter,2);
    adaugaJucatoriAutomat(listaJucatori, echipa, PostJucator::Opposite,1);

    for (const auto &j : echipa.getLot()) {
        if (j.getPost() == PostJucator::Libero) {
            echipa.seteazaLibero(j.getNume(), false);
            break;
        }
    }

    const PostJucator posturiCerute[6] = {
        PostJucator::Outside_Hitter,
        PostJucator::Setter,
        PostJucator::Middle_Blocker,
        PostJucator::Outside_Hitter,
        PostJucator::Opposite,
        PostJucator::Middle_Blocker
    };

    for (int zona =1 ; zona <= 6 ; ++zona) {
        for (const auto &j : echipa.getLot()) {
            if (j.getPost() == posturiCerute[zona-1]) {
                echipa.seteazaTitularPeZona(zona, j.getNume(), false);
                break;
            }
        }
    }
    return echipa;
}


static void configureazaTeren(Echipa& echipa) {
    if (std::cin.eof()) return;

    std::cout << "-----------------------" << std::endl;
    std::cout << "CONFIGURARE TITULARI: " << echipa.getNumeEchipa() << std::endl;

    for (const auto& j : echipa.getLot()) {
        if (j.getPost() == PostJucator::Libero) {
            std::string numeLibero = j.getNume();
            echipa.seteazaLibero(numeLibero, true);
            break;
        }
    }

    std::vector<Jucator> lotInitial = echipa.getLot();
    int numar = 0;
    std::vector<int> jucatoriAlesi;

    std::cout << "-------------------------------------\n";
    std::cout << "Seteaza Middle Blockerii in teren\n";
    int numarMiddleBlockeri = afiseazaJucatoriDupaPost(lotInitial, PostJucator::Middle_Blocker);
    const int zoneMiddle[2] = {3, 6};
    for (int zona : zoneMiddle) {
        while (!std::cin.eof()) {
            std::cout << "\nSeteaza in pozitia " << zona << ": ";
            if (alegeJucator(numar, numarMiddleBlockeri, jucatoriAlesi)) {
                Jucator j = gasesteInListaDupaNumarSiPost(lotInitial, PostJucator::Middle_Blocker, numar);
                if (echipa.seteazaTitularPeZona(zona, j.getNume(), true)) {
                    jucatoriAlesi.push_back(numar);
                    break;
                }
            }
        }
        if (std::cin.eof()) return;
    }

    jucatoriAlesi.clear();
    std::cout << "-------------------------------------\n";
    std::cout << "Seteaza Outside Hitterii in teren\n";
    int numarOutsideHitteri = afiseazaJucatoriDupaPost(lotInitial, PostJucator::Outside_Hitter);
    const int zoneOutside[2] = {1, 4};
    for (int zona : zoneOutside) {
        while (!std::cin.eof()) {
            std::cout << "\nSeteaza in pozitia " << zona << ": ";
            if (alegeJucator(numar, numarOutsideHitteri, jucatoriAlesi)) {
                Jucator j = gasesteInListaDupaNumarSiPost(lotInitial, PostJucator::Outside_Hitter, numar);
                if (echipa.seteazaTitularPeZona(zona, j.getNume(), true)) {
                    jucatoriAlesi.push_back(numar);
                    break;
                }
            }
        }
        if (std::cin.eof()) return;
    }

    jucatoriAlesi.clear();
    std::cout << "-------------------------------------\n";
    std::cout << "Seteaza Setterul in teren\n";
    int numarSetteri = afiseazaJucatoriDupaPost(lotInitial, PostJucator::Setter);
    while (!std::cin.eof()) {
        std::cout << "\nSeteaza in pozitia 2: ";
        if (alegeJucator(numar, numarSetteri, jucatoriAlesi)) {
            Jucator j = gasesteInListaDupaNumarSiPost(lotInitial, PostJucator::Setter, numar);
            if (echipa.seteazaTitularPeZona(2, j.getNume(), true)) {
                jucatoriAlesi.push_back(numar);
                break;
            }
        }
    }
    if (std::cin.eof()) return;

    jucatoriAlesi.clear();
    std::cout << "-------------------------------------\n";
    std::cout << "Seteaza Opposite-ul in teren\n";
    int numarOpposite = afiseazaJucatoriDupaPost(lotInitial, PostJucator::Opposite);
    while (!std::cin.eof()) {
        std::cout << "\nSeteaza in pozitia 5: ";
        if (alegeJucator(numar, numarOpposite, jucatoriAlesi)) {
            Jucator j = gasesteInListaDupaNumarSiPost(lotInitial, PostJucator::Opposite, numar);
            if (echipa.seteazaTitularPeZona(5, j.getNume(), true)) {
                jucatoriAlesi.push_back(numar);
                break;
            }
        }
    }
    if (std::cin.eof()) return;

    std::cout << "\nEchipa de start a fost asezata pe teren!" << std::endl;
}

static bool gasesteJucatorDupaNume(const std::vector<Jucator>& totiJucatorii,
                                   const std::string& numeCautat,
                                   Jucator& jucatorGasit) {
    for (const auto& j : totiJucatorii) {
        if (j.getNume() == numeCautat) {
            jucatorGasit = j;
            return true;
        }
    }
    return false;
}

static void salveazaEchipeInFisier(const std::string& numeFisier,
                                   const std::vector<EchipaSalvata>& echipeCreate) {
    nlohmann::json jArray = nlohmann::json::array();

    for (const auto& item : echipeCreate) {
        nlohmann::json jEchipa;
        jEchipa["nume_echipa"] = item.echipa.getNumeEchipa();
        jEchipa["libero"] = item.echipa.getLibero().getNume();

        nlohmann::json jTitulari = nlohmann::json::array();
        for (int zona = 1; zona <= 6; ++zona) {
            jTitulari.push_back(item.echipa.getTitularZona(zona).getNume());
        }
        jEchipa["titulari"] = jTitulari;

        nlohmann::json jRezerve = nlohmann::json::array();
        for (const auto& rez : item.echipa.getLot()) {
            jRezerve.push_back(rez.getNume());
        }
        jEchipa["rezerve"] = jRezerve;

        jArray.push_back(jEchipa);
    }

    std::ofstream out(numeFisier);
    if (out.is_open()) {
        out << jArray.dump(4);
    }
}

static std::vector<EchipaSalvata> incarcaEchipeDinFisier(const std::string& numeFisier,
                                                         const std::vector<Jucator>& totiJucatorii) {
    std::vector<EchipaSalvata> echipeIncarcate;
    std::ifstream in(numeFisier);
    if (!in.is_open()) {
        return echipeIncarcate; // Normal dacă nu s-a salvat încă nicio echipă
    }

    if (in.peek() == std::ifstream::traits_type::eof()) {
        return echipeIncarcate; // Fișier gol, nimic de încărcat
    }

    nlohmann::json jArray;
    try {
        in >> jArray;
    } catch (const nlohmann::json::exception& e) {
        throw EroareDate("Fisierul cu echipe salvate (" + numeFisier + ") este corupt: " + std::string(e.what()));
    }

    if (!jArray.is_array()) {
        throw EroareDate("Format invalid in " + numeFisier + ": se astepta o lista JSON de echipe.");
    }

    try {
        for (const auto& jEchipa : jArray) {
            std::string numeEchipa = jEchipa.at("nume_echipa");
            std::string numeLibero = jEchipa.at("libero");
            std::vector<std::string> numeTitulari = jEchipa.at("titulari").get<std::vector<std::string>>();
            std::vector<std::string> numeRezerve = jEchipa.at("rezerve").get<std::vector<std::string>>();

            if (numeTitulari.size() < 6) {
                throw EroareDate("Echipa salvata '" + numeEchipa + "' are mai putin de 6 titulari!");
            }

            Echipa echipaReconstruita(numeEchipa);
            std::vector<std::string> jucatoriFolositi;

            Jucator jTemp;
            if (gasesteJucatorDupaNume(totiJucatorii, numeLibero, jTemp)) {
                echipaReconstruita.adaugaJucator(jTemp);
                jucatoriFolositi.push_back(numeLibero);
            }
            for (const auto& numeT : numeTitulari) {
                if (gasesteJucatorDupaNume(totiJucatorii, numeT, jTemp)) {
                    echipaReconstruita.adaugaJucator(jTemp);
                    jucatoriFolositi.push_back(numeT);
                }
            }
            for (const auto& numeR : numeRezerve) {
                if (gasesteJucatorDupaNume(totiJucatorii, numeR, jTemp)) {
                    echipaReconstruita.adaugaJucator(jTemp);
                    jucatoriFolositi.push_back(numeR);
                }
            }

            echipaReconstruita.seteazaLibero(numeLibero, false);
            for (int zona = 1; zona <= 6; ++zona) {
                echipaReconstruita.seteazaTitularPeZona(zona, numeTitulari[zona - 1], false);
            }

            std::vector<Jucator> ramasi;
            for (const auto& j : totiJucatorii) {
                if (std::find(jucatoriFolositi.begin(), jucatoriFolositi.end(), j.getNume()) == jucatoriFolositi.end()) {
                    ramasi.push_back(j);
                }
            }

            echipeIncarcate.push_back({echipaReconstruita, ramasi});
        }
    } catch (const nlohmann::json::exception& e) {
        throw EroareDate("Structura JSON invalida in " + numeFisier + ": " + std::string(e.what()));
    }

    return echipeIncarcate;
}

static void joacaMeciComplet(Echipa &echipa1, Echipa &echipa2) {
    std::vector<std::unique_ptr<FormatMeci>> formateDisponibile;
    formateDisponibile.push_back(std::make_unique<MeciOficial>());
    formateDisponibile.push_back(std::make_unique<MeciAmical>());
    formateDisponibile.push_back(std::make_unique<TurneuRapid>());
    formateDisponibile.push_back(std::make_unique<GoldenSet>());

    int optiuneFormat;
    while (true) {
        std::cout << "\n========================================\n";
        std::cout << "      ALEGE FORMATUL MECIULUI DE VOLEI  \n";
        std::cout << "========================================\n";

        for (size_t i = 0; i < formateDisponibile.size(); ++i) {
            std::cout << (i + 1) << ". " << formateDisponibile[i]->getNumeFormat() << "\n";
        }
        std::cout << static_cast<int>(formateDisponibile.size())+1 << ". Inapoi la meniul principal"<< "\n";
        std::cout << "========================================\n";

        std::cout << "Introdu numarul formatului dorit (1-" << formateDisponibile.size()<< " sau 0 pentru detalii): ";

        optiuneFormat= -1;
        if (!(std::cin >> optiuneFormat) || optiuneFormat < 0 || optiuneFormat > static_cast<int>(formateDisponibile.size())+1) {
            if (std::cin.eof()) {
                return;
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << ">>> Input invalid sau optiune inexistenta!\n";
        }

        if (optiuneFormat > 0 && optiuneFormat <= static_cast<int>(formateDisponibile.size())) {
            break;
        }

        if (optiuneFormat == static_cast<int>(formateDisponibile.size())+1) {
            return;
        }

        if (optiuneFormat == 0) {
            std::cout << "\n=== AFISARE DETALII FORMATE ===\n";
            tabulate::Table tabelDetalii;
            tabelDetalii.format().locale("C");
            tabelDetalii.add_row({"Nr.", "Detalii Format"});

            for (size_t i = 0; i < formateDisponibile.size(); ++i) {
                std::ostringstream oss;
                oss << *formateDisponibile[i];
                tabelDetalii.add_row({std::to_string(i + 1), oss.str()});
            }

            std::cout << tabelDetalii << "\n";
            std::cout << "========================================\n";
        }
    }
    const FormatMeci* formatAles = formateDisponibile[optiuneFormat - 1].get();

    Meci meci(echipa1, echipa2, *formatAles);

    while (!meci.esteMeciulTerminat() && !std::cin.eof()) {
        int numarSetCurent = static_cast<int>(meci.getSeturiJucate().size()) + 1;

        Set setCurent(echipa1, echipa2, meci.getFormat(), numarSetCurent);
        setCurent.joacaSet();
        if (setCurent.esteAbandonat()) return;

        if (std::cin.eof()) {
            break;
        }

        echipa1 = setCurent.getEchipaMea();
        echipa2 = setCurent.getEchipaAdversa();
        meci.inregistreazaSet(setCurent, setCurent.AmCastigat());
        std::cout << meci.determinaStareMeci() << std::endl;
        meci.afiseazaScorul();
        if (!meci.meniuRaportStatistic()) return;

        if (!meci.esteMeciulTerminat()) {
            std::cout << ">>> Pauza intre seturi: jucatorii recupereaza stamina ("
                      << meci.getFormat()->calculeazaRecuperareStamina() << "%)...\n";
            meci.pregatesteEchipeIntreSeturi(echipa1, echipa2);
        }
    }
    meci.reseteazaMeci();
}

static bool existaNumeEchipa(const std::vector<EchipaSalvata>& echipeCreate, const std::string& numeCautat) {
    for (const auto& item : echipeCreate) {
        if (item.echipa.getNumeEchipa() == numeCautat) {
            return true;
        }
    }
    return false;
}

int main() {
    try {
        std::vector<Jucator> listaJucatori;
        listaJucatori = citesteJucatoriDinFisier("jucatori.json");
        std::vector<EchipaSalvata> echipeCreate = incarcaEchipeDinFisier("echipe_salvate.json", listaJucatori);
        int optiuneMeniu = 0;
        while (true) {
            std::cout << "\n===============================" << std::endl;
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
                std::string numeEchipa;

                while (true) {
                    std::cout << "Introdu numele echipei (sau 0 pentru Inapoi): ";
                    if (!(std::cin >> numeEchipa)) {
                        return 0;
                    }

                    if (existaNumeEchipa(echipeCreate, numeEchipa)) {
                        std::cout << "Eroare: Echipa '" << numeEchipa
                                  << "' exista deja! Alege un nume diferit.\n";
                        continue;
                    }
                    break;
                }
                if (numeEchipa == "0") {
                    continue;
                }
                std::vector<Jucator> jucatoriDisponibili = listaJucatori;
                Echipa echipaNoua = creeazaEchipa(numeEchipa, jucatoriDisponibili);
                configureazaTeren(echipaNoua);

                echipeCreate.push_back({echipaNoua, jucatoriDisponibili});
                salveazaEchipeInFisier("echipe_salvate.json", echipeCreate);
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
                std::cout << "Alege numarul echipei (sau 0 pentru inapoi): ";

                int indexEchipa = 0;
                if (!(std::cin >> indexEchipa)) {
                    break;
                }

                if (indexEchipa < 0 || indexEchipa > static_cast<int>(echipeCreate.size())) {
                    std::cout << "Numar invalid! Revenire la meniul principal." << std::endl;
                    continue;
                }

                if (indexEchipa == 0) {
                    continue;
                }

                EchipaSalvata selectie = echipeCreate[indexEchipa - 1];
                std::cout << "Ai ales echipa: " << selectie.echipa.getNumeEchipa() << std::endl;

                while (true) {
                    std::cout << "\n--- MENIU ECHIPA: " << selectie.echipa.getNumeEchipa() << " ---\n";
                    std::cout << "1. Joaca" << std::endl;
                    std::cout << "2. Afiseaza jucatorii si statisticile" << std::endl;
                    std::cout << "3. Sterge aceasta echipa" << std::endl;
                    std::cout << "4. Inapoi la meniul principal" << std::endl;
                    std::cout << "Alege o optiune: ";

                    int optiuneJoc = 0;
                    if (!(std::cin >> optiuneJoc)) {
                        break;
                    }

                    if (optiuneJoc == 1) {
                        std::vector<Jucator> ramasiPentruAdversar = selectie.jucatoriRamasi;
                        Echipa echipaAdversa = creeazaEchipaAutomata("Adversar_" + selectie.echipa.getNumeEchipa(), ramasiPentruAdversar);
                        joacaMeciComplet(selectie.echipa, echipaAdversa);
                        break;

                    } else if (optiuneJoc == 2) {
                        std::cout << "\n=== LOTUL SI STATISTICILE ECHIPEI " << selectie.echipa.getNumeEchipa() << " ===\n";
                        afisareEchipaCompleta(selectie.echipa);

                    } else if (optiuneJoc == 3) {
                        std::string numeSters = selectie.echipa.getNumeEchipa();
                        echipeCreate.erase(echipeCreate.begin() + (indexEchipa - 1));
                        salveazaEchipeInFisier("echipe_salvate.json", echipeCreate);
                        std::cout << "Echipa '" << numeSters << "' a fost stearsa definitiv!\n";
                        break;

                    } else if (optiuneJoc == 4) {
                        std::cout << "Revenire la meniul principal..." << std::endl;
                        break;

                    } else {
                        std::cout << "Optiune invalida! Alege intre 1 si 4." << std::endl;
                    }
                }
            } else if (optiuneMeniu == 3) {
                std::cout << "Exit!" << std::endl;
                break;
            } else {
                std::cout << "Optiune gresita! Alege 1, 2 sau 3." << std::endl;
            }
        }
    }
    catch (const EroareDate & e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Programul se opreste din cauza fisierului de date."<<std::endl;
        return 1;
    }
    catch (const EroareVolei& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}