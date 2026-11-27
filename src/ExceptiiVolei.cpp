#include "ExceptiiVolei.h"

EroareVolei::EroareVolei(const std::string& msg) : msg(msg){}

const char* EroareVolei::what() const noexcept {
    return msg.c_str();
}

EroareDate::EroareDate(const std::string& detalii) : EroareVolei("[Eroare Date ] " + detalii) {}

EroareRegulament::EroareRegulament(const std::string& detalii) : EroareVolei("[Eroare Regulament] " + detalii) {}

EroareResurseEchipa::EroareResurseEchipa(const std::string& detalii) : EroareVolei("[Eroare Resurse Echipa] " + detalii) {}
