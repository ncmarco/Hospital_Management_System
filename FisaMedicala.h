#ifndef FISAMEDICALA_H
#define FISAMEDICALA_H

#include <vector>
#include <string>
#include <memory>
#include "Pacient.h"
#include "Medic.h"
using namespace std;

class Pacient;
class Medic;

class FisaMedicala {
    string emailPacient;
    map<string, Medic*> diagnostice;

public:
    FisaMedicala() {}
    FisaMedicala(const string& email, const map<string, Medic*>& diag)
        : emailPacient(email), diagnostice(diag) {}

    void afisare() const;
    void setDiagnostic(const map<string,Medic*> diagnostice) { this->diagnostice=diagnostice; };
    void adaugaDiagnostic(const string& diag, Medic* medic);
    const map<string, Medic*>& getDiagnostice() const { return diagnostice; }
    const string& getEmailPacient() const { return emailPacient;}
};

#endif // FISAMEDICALA_H
