
#ifndef OOP_ECHIPA_H
#define OOP_ECHIPA_H

#include "Jucator.h"
#include<array>
#include<vector>
#include<string>
#include<iostream>


class Echipa {
private:
    std::string numeEchipa;
    std::vector<Jucator> lot;
    std::array<Jucator, 6> teren;
    std::array<Jucator, 6> asezareRotatie;
    bool esteInAsezareFixa;
    Jucator libero;
    Jucator inlocuitDeLibero;
    bool liberoEsteInTeren;
    int zonaLibero;
    int schimbariEfectuate;

    static bool esteZonaValida(int zona);
    static bool esteZonaSpate(int zona);
    void staminaRotatie();
public:
    explicit Echipa(std::string numeEchipa = "Echipa");
    void adaugaJucator(const Jucator& jucator);
    bool seteazaTitularPeZona(int zona, const std::string& numeJucator);
    bool seteazaLibero(const std::string& numeJucator);
    bool efectueazaSchimbare(int zonaTeren, const std::string& numeRezerva);
    void jucatoriPePosturiFixe();
    void revinoLaAsezareaInitiala();
    void roteste();
    bool introduceLibero(int zonaSpate);
    void scoateLibero();
    void timeout();
    const std::string& getNumeEchipa() const;
    Jucator& getJucator(int zona);
    int getSetter() const;
    const std::vector<Jucator>& getLot() const;
    friend std::ostream& operator<<(std::ostream& os, const Echipa& e);

};
#endif //OOP_ECHIPA_H
