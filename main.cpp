#include <iostream>
#include "Megatron.hpp"
#include "Menu.hpp"
using namespace std;
int main()
{
  Megatron mega;
  Menu menu(&mega);
  cout << "Bienvenido a Megatron!!" << endl;
  string query;
  for(;;)
  {
    getline(cin, query);
    vector<string> tokens = menu.getTokens(query);
    if(tokens.size() > 3)
    {
      int isWhere = 0;
      int isGuardar = 0;
      if (tokens[0] == "create" && tokens[1] == "esquema" && tokens[2] == "for")
      {
        menu.insertEsquema(query);
	continue;
      }
      for(int i = 0; i < tokens.size(); i++)
      {
        if(tokens[i] == "where")
        {
          isWhere = 1;
        }
        if(tokens[i] == "|")
        {
          isGuardar = 1;
	  break;
        }
      }
      if (isGuardar)
      {
	menu.whereQueriesWithSave(query);
      }
      if(isWhere && !isGuardar)
      {
	menu.whereQueries(query);
      }
      if (tokens[0] == "select" && !isWhere) 
      {
	menu.selectQueries(query);
      }
    }
    if (tokens.size() == 3)
    {
      if (tokens[0] == "show" && tokens[1] == "esquema")
      { 
	menu.mostrarEsquema(query);
	continue;
      }
    }
    if (tokens.size() > 4)
    {
      if (tokens[0] == "insert" && tokens[1] == "into")
      {
	menu.insertRegistros(query);
	continue;
      }
    }
    if(query == "clear") 
    {
      system("clear"); 
      continue;
    }
    if(query == "exit")
    {
      cout << "Adios!!" << endl;
      break;
    }
 }
  return 0;
} 
