#ifndef MEGATRON_H
#define MEGATRON_H
#include <iostream>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <filesystem>
#include <vector>
#include <iomanip>
using namespace std;
namespace fs = std::filesystem;
class Megatron
{
  private: 
  public:
    Megatron();
    void crearEsquema(string, vector<string>);
    void query();
    void querySelect(string, vector<string>, string);
    void mostrarRelacion(string, int);
    void mostrarEsquemaByRelacion(string);
    void leerArchivo(string, string);
    bool findRelacion(string);
    void insertarRegistro(string,vector<string>);
    void mostrarAllEsquema();
    vector<string> getColumnas(string);
    string getType(string, string);
    bool findAtributo(string, string);
    void queryWhereSelect(string, vector<string>, vector<string>, string);
};
#endif
