#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "Pacient.h"


using namespace std;
#ifndef FILEIO_H
#define FILEIO_H

// functii de scriere in csv
void writeToCSV_Pacient(string filePath, vector<string> &date_pacient);
void writeToCSV_Medic(string filePath, const vector<string> &date_medic);
void writeToCSV_FisaMedicala(const string& filename, FisaMedicala* fisa);

Pacient *readFromCSV_Pacient(string filePath, string email);
Medic *readFromCSV_Medic(string filePath, string email);
FisaMedicala* readFromCSV_Fisa(string filename, string emailPacient);

vector<Medic*> readMediciFrom_CSV(string filePath);
vector<Pacient*> readPacientiFrom_CSV(string filePath);

void writeMediciTo_CSV(string filePath, vector<Medic*> &medici);
void writePacientiTo_CSV(string filePath, vector<Pacient*> &pacienti);







#endif //FILEIO_H
