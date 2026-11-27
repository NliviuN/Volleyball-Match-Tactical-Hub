//
// Created by nlivi on 11/15/2026.
//

#ifndef OOP_FORMATMECI_H
#define OOP_FORMATMECI_H

#include <iostream>
#include <memory>
#include <string>

class FormatMeci {
private:
    std::string numeFormat;
    int seturiPentruVictorie;
    int punctePerSet;
    virtual void afiseazaDetaliiSpecifice(std::ostream& os) const = 0;

public:
    FormatMeci(const std::string& numeFormat, int seturiPentruVictorie, int punctePerSet);
    virtual ~FormatMeci() = default;
    virtual std::unique_ptr<FormatMeci> clone() const = 0;

    void afiseaza(std::ostream& os) const;
    friend std::ostream& operator<< (std::ostream& os, const FormatMeci& formatMeci);

    const std::string& getNumeFormat() const { return numeFormat; }
    int getSeturiPentruVictorie() const { return seturiPentruVictorie; }
    int getPunctePerSet() const { return punctePerSet; }


    virtual bool esteSetTerminat(int numarSet, int puncteGazda, int puncteOaspete) const = 0;
    virtual double calculeazaRecuperareStamina() const = 0;
    virtual int getLimitaSchimbari() const = 0;
};

class MeciOficial: public FormatMeci{
private:
    int puncteTieBreak;
    int challengeDisponibile;

protected:
    void afiseazaDetaliiSpecifice(std::ostream& os) const override;

public:
    explicit MeciOficial(int seturiPentruVictorie = 3, int punctePerSet = 25, int puncteTieBreak = 15, int challengeDisponibile = 2);

    std::unique_ptr<FormatMeci> clone() const override;

    bool esteSetTerminat(int numarSet, int puncteGazda, int puncteOaspete) const override;
    double calculeazaRecuperareStamina() const override;
    int getLimitaSchimbari() const override;

    bool solicitaChallengeVideo(const std::string& numeEchipa);
};

class MeciAmical : public FormatMeci {
private:
    bool permiteEgalitateLaDouaPuncte;

protected:
    void afiseazaDetaliiSpecifice(std::ostream& os) const override;

public:
    explicit MeciAmical(int seturiPentruVictorie = 2, int punctePerSet = 21, bool permiteEgalitateLaDouaPuncte = false);

    std::unique_ptr<FormatMeci> clone() const override;

    bool esteSetTerminat(int numarSet, int puncteGazda, int puncteOaspete) const override;
    double calculeazaRecuperareStamina() const override;
    int getLimitaSchimbari() const override;
};

class TurneuRapid : public FormatMeci {
private:
    int pragPunctajMaxim;

protected:
    void afiseazaDetaliiSpecifice(std::ostream& os) const override;

public:
    explicit TurneuRapid(int seturiPentruVictorie = 2, int punctePerSet = 15, int pragPunctajMaxim = 17);

    std::unique_ptr<FormatMeci> clone() const override;

    bool esteSetTerminat(int numarSet, int puncteGazda, int puncteOaspete) const override;
    double calculeazaRecuperareStamina() const override;
    int getLimitaSchimbari() const override;
};

class GoldenSet : public FormatMeci {

protected:

    void afiseazaDetaliiSpecifice(std::ostream &os) const override;

public:
    explicit GoldenSet(int seturiPentruVictorie = 1, int punctePerSet = 15);

    std::unique_ptr<FormatMeci> clone() const override;

    bool esteSetTerminat(int numarSet, int puncteGazda, int puncteOaspete) const override;
    double calculeazaRecuperareStamina() const override;
    int getLimitaSchimbari() const override;
};
#endif //OOP_FORMATMECI_H
