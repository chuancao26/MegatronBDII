#include "Megatron.hpp"
Megatron::Megatron()
{
  cout << "se instancio Megatron" << endl;  
}
bool Megatron::findRelacion(string relacion)
{
  ifstream esquema("usr/db/esquema");
  string linea;
  while(getline(esquema, linea))
  {
    stringstream ss(linea);
    string sublinea;
    while(getline(ss, sublinea, '#'))
    {
      if (relacion == sublinea)
	return true;
      else 
	break;
    }
  }
  esquema.close();
  return false;
}
//devuelve un vector con todos las columnas de una relacion
vector<string> Megatron::getColumnas(string relacion)
{
  vector<string> columnas = {};
  ifstream archivo("usr/db/esquema");
  string linea;
  while(getline(archivo, linea))
  {
    stringstream ss(linea);
    string sublinea;
    int found = 0;
    int contador = 0;
    while(getline(ss, sublinea, '#'))
    {
      if(relacion == sublinea)
      {
	found = 1;
      }
      else if (!found)
      {
	break;
      }
      if(found && contador % 2 != 0)
      {
	columnas.push_back(sublinea);
      }
      contador++;
    }
    if (found)
      break;
    contador = 0;
  }
  archivo.close();
  return columnas; 
}
void Megatron::crearEsquema(string relacion, vector<string> parametros)
{
  if(findRelacion(relacion))
  {
    cout << "relacion ya existente!" << endl;
    return;
  }
  ofstream archivo("usr/db/esquema", ios::app);
  string tipo, atributo; 
  archivo << relacion << '#';
  for(int i = 0; i < parametros.size(); i = i + 2)
  {
    tipo = parametros[i + 1];
    atributo = parametros[i];
    
    if (i >= parametros.size() - 2)
    {
      archivo << atributo << '#' << tipo << endl;
    }
    else
    {
      archivo << atributo << '#' << tipo << '#';
    }
  }
  archivo.close();
}
bool Megatron::findAtributo(string relacion, string atributo)
{
  if(!findRelacion(relacion))
  {
    return false;
  }
  vector<string> columnas = getColumnas(relacion);
  for (int i = 0; i < columnas.size(); i++)
  {
    if (columnas[i] == atributo)
      return true;
  }
  return false;
}
string Megatron::getType(string relacion, string atributo)
{
  if(!findRelacion(relacion))
  {
    return "not found";
  }
  ifstream esquema("usr/db/esquema");
  string linea;
  int found = 0, atributoFound = 0;
  while(getline(esquema, linea))
  {
    stringstream ss(linea);
    string sublinea;
    while(getline(ss, sublinea, '#'))
    {
      if (relacion == sublinea)
	found = 1;
      if (found)
      {
        if(atributoFound)
        {
          return sublinea;
        }
        if(sublinea == atributo)
        {
          atributoFound = 1;
        }
      }
    }
  }
  esquema.close();
  return "Not found";
}

// retorna la cantidad total de columnas de un esquema
int cantAtributos(string relacion)
{
  ifstream esquema("usr/db/esquema");
  string linea;
  int found = 0, counter = 0;
  while(getline(esquema, linea))
  {
    stringstream ss(linea);
    string sublinea;
    while(getline(ss, sublinea, '#'))
    {
      if (relacion == sublinea)
	found = 1;
      counter++;
    }
    if (found)
      return counter / 2;
    else
      counter = 0;
  }
  esquema.close();
  return 0;
}
void Megatron::leerArchivo(string relacion, string ruta)
{
  if (!findRelacion(relacion))
  {
    cout << "Relacion no encontrada!" << endl;
    return;
  }
  ofstream output("usr/db/" + relacion);
  ifstream archivo(ruta);
  string linea;
  int first = 1, lcounter = 0;
  getline(archivo, linea);
  // Leyendo las columnas
  while(getline(archivo, linea))
  {
    stringstream ss(linea);
    string elemento;
    //primera fila
    while(getline(ss, elemento, ','))
    {
      if (first)
      {
        first = 0;
      }
      else
      {
        output << '#';
      }
      if (elemento[0] == '"')
      {
        string concatenado = elemento;
        while(ss.peek() != EOF)
        {
          getline(ss, elemento, ',');
          concatenado += ", " + elemento;
          if (elemento.back() == '"') break;
        }
        //concatenado = concatenado.substr(1, concatenado.size() - 2);
        output << concatenado;
      }
      else if(elemento.size() == 0)
      {
        output << "";
      }
      else
      {
        output << elemento;
      }
    }
    output << endl;
    first = 1;
  }
  archivo.close();
  output.close();
}
void Megatron::insertarRegistro(string relacionE, vector<string> atributos)
{
  if(!findRelacion(relacionE))
  {
    cout << "relacion no existente.." << endl;
    return;
  }
  if(cantAtributos(relacionE) == 0)
  {
    cout << "Faltan atributos..." << endl;
    return;
  }
  vector<string> columnas = getColumnas(relacionE);
  for (int i = 0; i < atributos.size(); i = i + 2)
  {
    if (!findAtributo(relacionE, atributos[i]))
    {
      cout << "Atributo " << atributos[i] << " no encontrado!" << endl;
      return;
    }
  }
  ofstream relacion("usr/db/" + relacionE, ios::app);
  for(int i = 0; i < atributos.size(); i = i + 2)
  {
    string tipo = getType(relacionE, atributos[i]);
    cout << "el tipo es: " << tipo << endl;
    if(tipo == "float")
    {
      float valor;
      stringstream validacion(atributos[i + 1]);
      if(validacion >> valor)
      {
        if(i >= atributos.size() - 2) 
        {
          relacion << atributos[i + 1] << '\n';
        }
        else
        {
          relacion << atributos[i + 1] << '#';
        }
      }
      else
      {
	cout << atributos[i + 1] << " no es un float" << endl;
	return;
      }
    }
    else if(tipo == "int")
    {
      int valor;
      stringstream validacion(atributos[i + 1]);
      if(validacion >> valor)
      {
        if(i >= atributos.size() - 2) 
        {
          relacion << atributos[i + 1] << '\n';
        }
        else
        {
          relacion << atributos[i + 1] << '#';
        }
      }
      else
      {
	cout << atributos[i + 1] << " no es un int" << endl;
	return;
      }
    }
    else if(tipo != "not found")
    {
      if(i >= atributos.size() - 2) 
      {
        relacion << atributos[i + 1] << '\n';
      }
      else
      {
        relacion << atributos[i + 1] << '#';
      }
    }
  }
  relacion.close();
}

void Megatron::mostrarAllEsquema()
{
  vector<string> columnas = {};
  ifstream archivo("usr/db/esquema");
  string linea;
  while(getline(archivo, linea))
  {
    stringstream ss(linea);
    string sublinea;
    while(getline(ss, sublinea, '#'))
    {
      cout << sublinea << ' ';
    }
    cout << '\n';
  }
  archivo.close();
}
void Megatron::mostrarRelacion(string relacion, int cantPrint)
{
  if(!findRelacion(relacion))
  {
    cout << "relacion no encontrada.." << endl;
    return;
  }
  vector<string> columnas = getColumnas(relacion);
  for(int i = 0; i < columnas.size(); i++)
  {
    cout << "|" << setw(cantPrint) << left << columnas[i];
  }
  cout << endl;
  ifstream archivo("usr/db/" + relacion);
  string linea;
  while(getline(archivo, linea))
  {
  
    stringstream ss(linea);
    string sublinea;
    while(getline(ss, sublinea, '#'))
    {
      if (sublinea.size() > cantPrint)
      {
	sublinea = sublinea.substr(0,cantPrint);
      }
      cout << "|" << setw(cantPrint) << left << sublinea;
    }
    cout << endl;
  }
}
// Devuelve las posiciones a partir de un grupo de nombre de columnas 
vector<int> posByColumn(string relacion, vector<string> colname)
{
  vector<int> columnas = {};
  ifstream archivo("usr/db/esquema");
  string linea;
  while(getline(archivo, linea))
  {
    stringstream ss(linea);
    string sublinea;
    int found = 0;
    int contador = 0;
    while(getline(ss, sublinea, '#'))
    {
      if(relacion == sublinea || found)
      {
	found = 1;
        for(int i = 0; i < colname.size(); i++)
	{
          if (colname[i] == sublinea)
          {
            columnas.push_back(contador);
          }
        }
      }
      if (!found)
      {
	break;
      }
      contador++;
    }
    if (found)
      break;
    contador = 0;
  }
  archivo.close();
  return columnas; 
}
// Retorna un vector de columnas de acuerdo al orden en el esquema  
vector<string> getColumnasByID(string relacion, vector<string> columnas)
{
  vector<string> salida = {};
  ifstream archivo("usr/db/esquema");
  string linea;
  while(getline(archivo, linea))
  {
    stringstream ss(linea);
    string sublinea;
    int found = 0;
    int contador = 0;
    while(getline(ss, sublinea, '#'))
    {
      if(relacion == sublinea)
      {
	found = 1;
      }
      else if (!found)
      {
	break;
      }
      if(found)
      {
	for(int i = 0; i < columnas.size(); i++)
	{
	  if(sublinea == columnas[i])
	  {
	    salida.push_back(sublinea);
	  }
	}
      }
      contador++;
    }
    if (found)
      break;
    contador = 0;
  }
  archivo.close();
  return salida; 
}
void Megatron::mostrarEsquemaByRelacion(string relacion)
{
  if(!findRelacion(relacion))
  {
    cout << "No existe esta relacion!" << endl;
    return;
  }
  ifstream archivo("usr/db/esquema");
  string linea;
  while(getline(archivo, linea))
  {
    stringstream ss(linea);
    string sublinea;
    int found = 0;
    int contador = 0;
    while(getline(ss, sublinea, '#'))
    {
      if(found)
      {
        cout << "|" << setw(10) << left << sublinea;
      }
      if(contador % 2 == 0 && found)
      {
	cout << endl;
      }
      if(relacion == sublinea)
      {
	found = 1;
      }
      else if (!found)
      {
	break;
      }
      contador++;
    }
    if (found)
    {
      cout << endl;
      break; 
    }
  }
  archivo.close();
}
// guardar : 0 no y 1 si 
void Megatron::querySelect(string relacion, vector<string> columnas, string guardarName)
{
  if (!findRelacion(relacion))
  {
    cout << "No existe relacion.." << endl;
    return;
  }
  ifstream lectura("usr/db/" + relacion);
  string linea;
  if(columnas.size() == 1)
  {
    columnas = getColumnas(relacion);
  }
  vector<int> posiciones = posByColumn(relacion, columnas); 
  if (posiciones.size() != columnas.size())
  {
   cout << "Instruccion no valida" << endl;
   return;
  }
  vector<string> titulos = getColumnasByID(relacion, columnas);
  vector<string> parametrosGuardar = {};
  for (int i = 0; i < titulos.size(); i++)
  {
    string sublinea = titulos[i];
    parametrosGuardar.push_back(titulos[i]);
    parametrosGuardar.push_back(getType(relacion, titulos[i]));
    if (sublinea.size() > 10)
    {
      sublinea = sublinea.substr(0, 10);
    }
    cout << "|" << setw(10) << left << sublinea << ' ';
  }
  cout << '\n';
  if(guardarName != "No")
  {
    crearEsquema(guardarName, parametrosGuardar);
  }
  string rutaGuardado = "usr/db/" + guardarName;
  ofstream archivoGuardado(rutaGuardado);
  while(getline(lectura, linea))
  {
    stringstream ss(linea);
    string sublinea;
    int counter = 0;
    int newline = 0;
    int isFirst = 1; // Cuando es la primera columna se debe imprimir 1 vez 
    while(getline(ss, sublinea, '#'))
    {
      for(int i = 0; i < posiciones.size(); i++)
      {
	archivoGuardado << sublinea << '#';
	if (counter == posiciones[i] / 2)
	{ 
	  if (sublinea.size() > 10)
	  {
	    sublinea = sublinea.substr(0, 10);
	  }
          cout << "|" << setw(10) << left << sublinea << ' ';
	}
      }
      counter++;
    }
    cout << '\n';
    archivoGuardado << endl;
  }
  lectura.close();
  if(guardarName == "No")
  {
    fs::remove(rutaGuardado);
  }
}
//Posicion 0: A 
//Posicion 1: condicion
//Posicion 2: B
bool evaluarCondicion(vector<string> condicion, string valor)
{
  string signo = condicion[1];
  float numero, numeroCondicion;
  stringstream ssValor(valor);
  stringstream ssValorCondicion(condicion[2]);
  if ((ssValor >> numero) && (ssValorCondicion >> numeroCondicion))
  {
    if (condicion[1] == "<")
    {
      return numero < numeroCondicion;
    }
    else if (condicion[1] == ">")
    {
      return numero > numeroCondicion;
    }
    else if (condicion[1] == "<=")
    {
      return numero <= numeroCondicion;
    }
    else if (condicion[1] == ">=")
    {
      return numero >= numeroCondicion;
    }
    else if (condicion[1] == "=" || condicion[1] == "==")
    {
      return numero == numeroCondicion;
    }
    else if (condicion[1] == "!=" || condicion[1] == "<>")
    {
      return numero != numeroCondicion;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}
// Funcion que determinara si una determinada linea se imprimira o no
bool printLine(string relacion, string linea, vector<string> condicion)
{
  string columnaC = condicion[0];
  vector<string> columna = {columnaC};
  vector<int> posiciones = posByColumn(relacion, columna);
  stringstream ss(linea);
  string word;
  int counter = 0;
  while(getline(ss, word, '#'))
  {
    for(int i = 0; i < posiciones.size(); i++)
    {
      if (counter == posiciones[i] / 2)
      {
	if (word.size() > 0)
	{
	  return evaluarCondicion(condicion, word);
	}
      }
    }
    counter++;
  }
  return false;
}
void Megatron::queryWhereSelect(string relacion, vector<string> columnas, vector<string> condicion, string guardarName)
{
  if (!findRelacion(relacion))
  {
    cout << "No existe relacion.." << endl;
    return;
  }
  ifstream lectura("usr/db/" + relacion);
  string linea;
  if(columnas.size() == 1)
  {
    columnas = getColumnas(relacion);
  }
  vector<int> posiciones = posByColumn(relacion, columnas); 
  if (posiciones.size() != columnas.size())
  {
   cout << "Instruccion no valida" << endl;
   return;
  }
  vector<string> titulos = getColumnasByID(relacion, columnas);
  vector<string> parametrosGuardar = {};
  for (int i = 0; i < titulos.size(); i++)
  {
    string sublinea = titulos[i];
    parametrosGuardar.push_back(titulos[i]);
    parametrosGuardar.push_back(getType(relacion, titulos[i]));
    if (sublinea.size() > 10)
    {
      sublinea = sublinea.substr(0, 10);
    }
    cout << "|" << setw(10) << left << sublinea << ' ';
  }
  cout << '\n';
  if(guardarName != "No")
  {
    crearEsquema(guardarName, parametrosGuardar);
  }
  string rutaGuardado = "usr/db/" + guardarName;
  ofstream archivoGuardado(rutaGuardado);
  while(getline(lectura, linea))
  {
    stringstream ss(linea);
    string sublinea;
    int counter = 0;
    int newline = 0;
    int toPrint = 0;
    if(printLine(relacion, linea, condicion))
    {
      toPrint = 1;
      while(getline(ss, sublinea, '#'))
      {
        for(int i = 0; i < posiciones.size(); i++)
        {
          if (counter == posiciones[i] / 2)
          {
	    archivoGuardado << sublinea << '#';
            if (sublinea.size() > 10)
            {
              sublinea = sublinea.substr(0, 10);
            }
            cout << "|" << setw(10) << left << sublinea << ' ';
          }
        }
        counter++;
      }
    }
    if (toPrint)
    {
      cout << '\n';
      archivoGuardado << endl;
    }
  }
  archivoGuardado.close();
  lectura.close();
  if(guardarName == "No")
  {
    fs::remove(rutaGuardado);
  }
}

