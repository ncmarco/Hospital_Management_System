#include "FileIO.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "Pacient.h"
#include "Medic.h"
#include "FisaMedicala.h"


using namespace std;



// functii de scriere in fisiere
void writeToCSV_Pacient(string filePath, vector<string> &date_pacient) {
    fstream fout(filePath,ios::app);
    if (!fout.is_open()) {
        cout<<"Eroare la deschiderea fisierului: "<<filePath<<endl;
        return;
    }
    for(int i=0;i<date_pacient.size();++i) {
        fout<<date_pacient[i];
        if(i!=date_pacient.size()-1) fout<<",";
    }
    fout<<"\n";
    fout.flush();
    fout.close();
}
void writeToCSV_Medic(string filePath, const vector<string> &date_medic) {
    string specializare;
    float nota;
    string grad;
    fstream fout(filePath,ios::app);
    if (!fout.is_open()) {
        cout<<"Eroare la deschiderea fisierului: "<<filePath<<endl;
        return;
    }
    for(int i=0;i<date_medic.size();++i) {
        fout<<date_medic[i];
        if(i!=date_medic.size()-1) fout<<",";
    }
    fout<<"\n";
    fout.flush();
    fout.close();
}

void writeToCSV_FisaMedicala(const string& filename, FisaMedicala* fisa) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << fisa->getEmailPacient() << ",";

        for (const auto& entry : fisa->getDiagnostice()) {
            file << entry.first << ";" << entry.second->getEmail() << ";";
        }

        file << "\n";
        file.close();
    } else {
        cout << "Eroare la deschiderea fisierului: " << filename << endl;
    }
}


// functii de citire din fisiere

Pacient *readFromCSV_Pacient(string filePath, string email) {
    ifstream fin(filePath);
    if (!fin.is_open()) {
        cout<<"Eroare la deschiderea fisierului: "<<filePath<<endl;
        return nullptr;
    }
    string line;
    getline(fin,line);
    while(getline(fin,line)) {
        stringstream ss(line);
        string numePacient, prenumePacient, emailPacient, varsta_str, asigurat_str, programariStr;
        getline(ss, numePacient, ',');
        getline(ss, prenumePacient, ',');
        getline(ss, emailPacient, ',');
        if(emailPacient==email) {
            int varsta;
            bool asigurat;
            getline(ss, varsta_str, ',');
            getline(ss, asigurat_str, ',');
            varsta=stoi(varsta_str);
            asigurat=(asigurat_str=="Da");
            Pacient *pacient=new Pacient(numePacient,prenumePacient,emailPacient,varsta,asigurat);
            getline(ss, programariStr, ',');
            stringstream programariStream(programariStr);
            string programare;
            map<int, string> programari;
            while (getline(programariStream, programare, ';')) {
                stringstream progDetails(programare);
                string oraStr, emailMedic;
                getline(progDetails, oraStr, '-');
                getline(progDetails, emailMedic, '-');
                if (!oraStr.empty() && !emailMedic.empty()) {
                    programari[stoi(oraStr)]=emailMedic;
                }
            }

            pacient->setProgramari(programari);
            fin.close();
            return pacient;
        }
    }
    fin.close();
    return nullptr;
}

Medic *readFromCSV_Medic(string filePath, string email) {
    ifstream fin(filePath);
    if (!fin.is_open()) {
        cout<<"Eroare la deschiderea fisierului: "<<filePath<<endl;
        return nullptr;
    }
    string line;
    getline(fin,line);
    while(getline(fin,line)) {
        stringstream ss(line);
        string numeMedic, prenumeMedic, emailMedic, specializare, nota_str, grad, programariStr;
        getline(ss, numeMedic, ',');
        getline(ss, prenumeMedic, ',');
        getline(ss, emailMedic, ',');
        if(emailMedic==email) {
            float nota;
            getline(ss, specializare, ',');
            getline(ss, nota_str, ',');
            nota=stof(nota_str);
            getline(ss,grad,',');
            Medic *medic=new Medic(numeMedic,prenumeMedic,emailMedic,specializare ,nota,grad);
            getline(ss, programariStr, ',');
            stringstream programariStream(programariStr);
            string programare;
            map<int, string> programari;
            while (getline(programariStream, programare, ';')) {
                stringstream progDetails(programare);
                string oraStr, emailPacient;
                getline(progDetails, oraStr, '-');
                getline(progDetails, emailPacient, '-');
                if (!oraStr.empty() && !emailPacient.empty()) {
                    programari[stoi(oraStr)]=emailPacient;
                }
            }

            medic->setProgramari(programari);
            fin.close();
            return medic;
        }
    }
    fin.close();
    return nullptr;
}

FisaMedicala* readFromCSV_Fisa(string filename, string emailPacient) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Eroare la deschiderea fișierului " << filename << endl;
        return nullptr;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string email, diagnosticeStr;

        getline(ss, email, ',');

        if (email == emailPacient) {
            getline(ss, diagnosticeStr, ',');

            stringstream ss_diag(diagnosticeStr);
            string diagnostic;
            map<string, Medic*> diagnosticCuMedic;

            while (getline(ss_diag, diagnostic, ';')) {
                stringstream diagDetails(diagnostic);
                string diag, emailMedic;
                getline(diagDetails, diag, '-');
                getline(diagDetails, emailMedic, '-');

                if (!emailMedic.empty()) {
                    Medic* medic = readFromCSV_Medic("medici.csv", emailMedic);
                    if (medic != nullptr) {
                        diagnosticCuMedic[diag] = medic;
                    } else {
                        cout << "Eroare la citirea medicului cu email " << emailMedic << endl;
                    }
                }
            }

            file.close();
            return new FisaMedicala(emailPacient, diagnosticCuMedic);
        }
    }

    file.close();
    return nullptr;
}

vector<Medic*> readMediciFrom_CSV(string filePath) {
    vector<Medic*> medici;
    ifstream file(filePath);
    string line, email, nume, prenume, specializareStr, nota, grad, programari;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, nume, ',');
        getline(ss, prenume, ',');
        getline(ss, email, ',');
        getline(ss, specializareStr, ',');
        getline(ss, nota, ',');
        getline(ss, grad, ',');
        getline(ss, programari, ',');

        Medic* medic=new Medic( nume, prenume,email, specializareStr, stof(nota), grad);
        map<int, string> progMap;
        stringstream programariStream(programari);
        string programare;
        while (getline(programariStream, programare, ';')) {
            stringstream progDetails(programare);
            string oraStr, emailPacient;
            getline(progDetails, oraStr, '-');
            getline(progDetails, emailPacient, '-');
            if (!oraStr.empty() && !emailPacient.empty()) {
                progMap[stoi(oraStr)]=emailPacient;
            }
        }
        medic->setProgramari(progMap);
        medici.push_back(medic);
    }
    file.close();
    return medici;
}

vector<Pacient*> readPacientiFrom_CSV(string filePath) {
    vector<Pacient*> pacienti;
    ifstream file(filePath);
    string line, email, nume, prenume, varstaStr, asigurare, programari;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, nume, ',');
        getline(ss, prenume, ',');
        getline(ss, email, ',');
        getline(ss, varstaStr, ',');
        getline(ss, asigurare, ',');
        getline(ss, programari, ',');

        bool asigurareBool=(asigurare=="Da");
        Pacient* pacient=new Pacient(nume, prenume,email,  stoi(varstaStr), asigurareBool);
        map<int, string> progMap;
        stringstream programariStream(programari);
        string programare;
        while (getline(programariStream, programare, ';')) {
            stringstream progDetails(programare);
            string oraStr, emailMedic;
            getline(progDetails, oraStr, '-');
            getline(progDetails, emailMedic, '-');
            if (!oraStr.empty() && !emailMedic.empty()) {
                progMap[stoi(oraStr)]=emailMedic;
            }
        }
        pacient->setProgramari(progMap);
        pacienti.push_back(pacient);
    }
    file.close();
    return pacienti;
}


void writeMediciTo_CSV(string filePath, vector<Medic*> &medici) {
    ofstream fout(filePath);
    if (!fout.is_open()) {
        cout<<"Eroare la deschiderea fisierului: "<<filePath<<endl;
        return;
    }
    fout<<"Nume,Prenume,Email,Specializare,Nota,Grad,Programari\n";
    for(const auto& medic : medici) {
        fout<<medic->getNume()<<","<<medic->getPrenume()<<","<<medic->getEmail()<< ","<<medic->getSpecializare()<<","<<medic->getNota()<<","<<medic->getGrad() << ",";
        const auto& programari=medic->getProgramari();
        for(const auto& prog:programari) {
            fout<<prog.first<<"-"<<prog.second<<";";
        }
        fout<<"\n";
    }
    fout.close();
}

void writePacientiTo_CSV(string filePath, vector<Pacient*> &pacienti) {
    ofstream fout(filePath);
    if (!fout.is_open()) {
        cout << "Eroare la deschiderea fisierului: "<<filePath<<endl;
        return;
    }
    fout<<"Nume,Prenume,Email,Varsta,Asigurat,Programari\n";
    for(const auto& pacient:pacienti) {
        fout<<pacient->getNume()<<","<<pacient->getPrenume()<<","<<pacient->getEmail()<< ","<< pacient->getVarsta()<<","<<(pacient->getAsigurat() ? "Da":"Nu")<<",";
        const auto& programari=pacient->getProgramari();
        for (const auto& prog:programari) {
            fout<<prog.first<<"-"<<prog.second<<";";
        }
        fout<<"\n";
    }
    fout.close();
}