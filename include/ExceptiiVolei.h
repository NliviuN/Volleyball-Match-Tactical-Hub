//
// Created by nlivi on 11/15/2026.
//

#ifndef OOP_EXCEPTIIVOLEI_H
#define OOP_EXCEPTIIVOLEI_H

#include <exception>
#include <string>

class EroareVolei : public std::exception {
private:
    std::string msg;

public:
    explicit EroareVolei(const std::string& msg);
    const char* what() const noexcept override;
    ~EroareVolei() noexcept override = default;
};

class EroareDate : public EroareVolei {
public:
    explicit EroareDate(const std::string& detalii);
};

class EroareRegulament : public EroareVolei {
public:
    explicit EroareRegulament(const std::string& detalii);
};

class EroareResurseEchipa : public EroareVolei {
public:
    explicit EroareResurseEchipa(const std::string& detalii);
};

#endif //OOP_EXCEPTIIVOLEI_H
