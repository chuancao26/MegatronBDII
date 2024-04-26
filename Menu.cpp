#include "Menu.hpp"
Menu::Menu(Megatron* mega)
{
  megatron = mega;
  error = "Query erroneo!";
}
void Menu::setTokens(string query)
{
  tokens = {};
  stringstream ss(query);
  string token;
  while (ss >> token) 
  {
    if (token.front() == '"') 
    {
      string quotedToken = token;
      // Si el token termina con comillas, es un token completo
      if (token.back() == '"') 
      {
        tokens.push_back(quotedToken);
      } 
      else 
      {
        // Buscamos el cierre de las comillas
        while (ss >> token) 
        {
          quotedToken += " " + token;
          if (token.back() == '"') 
          {
            // Quitamos las comillas del principio y del final
            tokens.push_back(quotedToken);
            break;
          }
        }
      }
    } 
    else 
    {
      tokens.push_back(token);
    }
  }
  sizeToken = tokens.size();
}
vector<string> Menu::getTokens(string query) 
{
  stringstream ss(query);
  string token;
  vector<string> words;
  while (ss >> token) 
  {
    if (token.front() == '"') 
    {
      string quotedToken = token;
      // Si el token termina con comillas, es un token completo
      if (token.back() == '"') 
      {
        words.push_back(quotedToken);
      } 
      else 
      {
        // Buscamos el cierre de las comillas
        while (ss >> token) 
        {
          quotedToken += " " + token;
          if (token.back() == '"') 
          {
            // Quitamos las comillas del principio y del final
            words.push_back(quotedToken);
            break;
          }
        }
      }
    } 
    else 
    {
      words.push_back(token);
    }
  }
  return words;
}
void Menu::insertEsquema(string query)
{
  setTokens(query);
  string relacion = tokens[3];
  string particion; 
  if(tokens[4] != "with")
  {
    cout << error << endl;
  }
  size_t posWith = query.find("with");
  if (posWith != string::npos) {
    particion = query.substr(posWith + 5); 
  }
  vector<string> parametros = getTokens(particion);
  if ((parametros.size() % 2 != 0))
  {
    cout << "Query erroneo!" << endl;
    return;
  }
  megatron -> crearEsquema(relacion, parametros);
  cout << "Ok!" << endl;
}
void Menu::mostrarEsquema(string query)
{
  setTokens(query);
  cout << sizeToken << endl;
  cout << tokens[2] << endl;
  if (sizeToken != 3)
  {
    cout << "Query erroneo!" << endl;
    return;
  }
  if (tokens[2] == "*")
  {
    megatron -> mostrarAllEsquema();
    cout << "Ok!" << endl;
  }
  else 
  {
    megatron -> mostrarEsquemaByRelacion(tokens[2]); 
    cout << "Ok!" << endl;
  }
}
void Menu::insertRegistros(string query)
{
  setTokens(query);
  string relacion, particion;
  if (sizeToken > 5)  
  {
    if (tokens[0] == "insert" && tokens[1] == "into" && tokens[3] == "values")
    {
      size_t posWith = query.find("values");
      if (posWith != string::npos) {
        particion = query.substr(posWith + 7); 
      }
      vector<string> parametros = getTokens(particion);
      if(parametros.size() % 2 != 0)
      {
	cout << error << endl;
	return;
      }
      relacion = tokens[2];
      megatron -> insertarRegistro(relacion, parametros);
      cout << "Ok!" << endl;
    }
    if(tokens[0] == "insert" && tokens[1] == "into" && tokens[3] == "from" && tokens[4] == "file")
    {
      relacion = tokens[2];
      if (tokens[5].size() < 4)
      {
	cout << "Query erroneo!" << endl;
	return;
      }
      string ruta = tokens[5].substr(1, tokens[5].size() - 2);
      megatron -> leerArchivo(relacion, ruta);
      cout << "Ok!" << endl;
    }
  }
}
void Menu::selectQueries(string query)
{
  setTokens(query);
  vector<string> columnas; 
  int counter = 0;
  while(tokens[counter] != "from")
  {
    if(tokens[counter] != "select")
    {
      columnas.push_back(tokens[counter]);
    }
    counter++;
  }
  if (columnas.size() == 0)
  {
    cout << error << endl;
    return;
  }
  counter++;
  string relacion = tokens[counter];
  if (columnas[0] == "*")
  {
    megatron -> querySelect(relacion, columnas, "No");
    cout << "Ok!" << endl;
  }
  else
  {
    megatron -> querySelect(relacion, columnas, "No");
    cout << "Ok!" << endl;
  }
}
void Menu::whereQueries(string query)
{
  setTokens(query);
  int isWhere = 0;
  vector<string> condicion = {};
  for(int i = 0; i < tokens.size(); i++)
  {
    if(isWhere)
    {
      condicion.push_back(tokens[i]);
    }
    if(tokens[i] == "where")
    {
      isWhere = 1;
    }
  }
  if(condicion.size() != 3)
  {
    cout << error << endl;
    return;
  }
  vector<string> columnas; 
  int counter = 0;
  while(tokens[counter] != "from")
  {
    if(tokens[counter] != "select")
    {
      columnas.push_back(tokens[counter]);
    }
    counter++;
  }
  if (columnas.size() == 0)
  {
    cout << error << endl;
    return;
  }
  counter++;
  string relacion = tokens[counter];
  if (columnas[0] == "*")
  {
    megatron -> queryWhereSelect(relacion, columnas, condicion, "No");
    cout << "Ok!" << endl;
  }
  else
  {
    megatron -> queryWhereSelect(relacion, columnas, condicion, "No"); 
    cout << "Ok!" << endl;
  }
}
void Menu::whereQueriesWithSave(string query)
{
  setTokens(query);
  int isWhere = 0;
  int counter = 0;
  vector<string> condicion = {};
  while(tokens[counter] != "|")
  {
    if (isWhere)
    {
      condicion.push_back(tokens[counter]);
    }
    if(tokens[counter] == "where")
    {
      isWhere = 1;
    }
    counter++;
  }
  if(condicion.size() != 3)
  {
    cout << condicion.size() << endl;
    cout << error << endl;
    return;
  }
  vector<string> columnas; 
  counter = 0;
  while(tokens[counter] != "from")
  {
    if(tokens[counter] != "select")
    {
      columnas.push_back(tokens[counter]);
    }
    counter++;
  }
  if (columnas.size() == 0)
  {
    cout << error << endl;
    return;
  }
  counter++;
  string relacion = tokens[counter];
  megatron -> queryWhereSelect(relacion, columnas, condicion, tokens[tokens.size() - 1]); 
  cout << "Ok!" << endl;
}

