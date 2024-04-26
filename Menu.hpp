#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include <string>
#include "Megatron.hpp"
class Menu
{
  private:
    Megatron* megatron;
    vector<string> tokens;
    string error;
    int sizeToken;
  public: 
    Menu(Megatron*);
    void setTokens(string word);
    vector<string> getTokens(string word);
    void insertEsquema(string);
    void mostrarEsquema(string);
    void insertRegistros(string);
    void selectQueries(string);
    void whereQueries(string);
    void whereQueriesWithSave(string);
};
#endif
