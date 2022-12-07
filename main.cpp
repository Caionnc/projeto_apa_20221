#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

class Presente
{
public:
  int peso;
  int id;
  int id_pai;
  int *restricoes = NULL;
  int npresentes;
  Presente(int i, int p, int n)
  {
    id = i;
    peso = p;
    id_pai = -1;
    npresentes = n;
    restricoes = new int[npresentes];
    // int* restricoes = (int*) malloc(npresentes*sizeof(int));
    for (int i = 0; i < npresentes; i++)
    {
      restricoes[i] = 0;
    }
  }

  int return_peso()
  {
    return peso;
  }
  void muda_peso(int p)
  {
    peso = p;
  }
  int return_id()
  {
    return id;
  }
  int return_pai()
  {
    return id_pai;
  }
  void muda_pai(int pai)
  {
    id_pai = pai;
  }
  void adiciona_restricao(int i)
  {
    restricoes[i] = 1;
  }

  int return_restricao(int id_presente)
  {
    return restricoes[id_presente];
  }

  void print_restricoes()
  {
    // printf("Printando restricoes de %i \n",id);
    for (int i = 0; i < npresentes; i++)
    {
      if (restricoes[i] == 1)
      {
        printf("Restricao de %i com %i: %i\n", id, i, restricoes[i]);
      }
    }
  }

  void print_presente()
  {
    printf("Presente de id: %i e peso: %i \n", id, peso);
  }
};

class Treno
{
public:
  int capacidade;
  int id;
  int peso_atual;
  vector<Presente *> presentes;
  Treno(int i, int c)
  {
    id = i;
    capacidade = c;
    peso_atual = 0;
  }

  void adiciona_presente(Presente *presente)
  {
    if ((peso_atual + presente->return_peso()) < capacidade)
    {
      presentes.push_back(presente);
      presente->muda_pai(id);
      peso_atual = peso_atual + presente->return_peso();
    }
  }

  void printa_presentes()
  {
    for (int i = 0; i < (presentes.size()); i++)
    {
      printf("Presente de id %i e peso %i \n", presentes[i]->return_id(), presentes[i]->return_peso());
    }
  }

  void print_treno()
  {
    printf("Treno %i com presentes:\n", id);
    this->printa_presentes();
  }

  int return_peso()
  {
    return peso_atual;
  }

  int verifica_compatibilidade(int index)
  {
    // funcao q retorna se existe uma incompatibilidade entre um presente de certo indice e os presentes ja presentes no treno
    // basicamente, se retornar 0, eh compativel
    int j1 = 0;
    int npresentes = presentes.size();
    for (int i = 0; i < npresentes; i++)
    {
      j1 = j1 + presentes[i]->return_restricao(index);
      if (j1 == 1)
      {
        i = npresentes + 2;
      }
    }
    return j1;
  }
};

// fonte do extract:
// https://www.geeksforgeeks.org/extract-integers-string-c/
void extractPesos(string str, int npresentes, int pesos_presentes[])
{
  stringstream ss;

  /* Storing the whole string into string stream */
  ss << str;

  /* Running loop till the end of the stream */
  string temp;
  int found;
  int j = 0;
  while (!ss.eof())
  {

    /* extracting word by word from stream */
    ss >> temp;

    /* Checking the given word is integer or not */
    if (stringstream(temp) >> found)
      // cout << found << " ";
      pesos_presentes[j] = found;
    j = j + 1;

    /* To save from space at the end of string */
    temp = "";
  }

  // return pesos;
}

std::tuple<int, int> extractRestricoes(string str, int a, int dividend, int b)
{
  // printf("Chegando \n");
  stringstream ss;
  ss << str;
  string temp;
  int found;
  int j = 0;
  int presentes[2];
  while (!ss.eof())
  {
    ss >> temp;
    if (stringstream(temp) >> found)
      // cout << found << " ";
      presentes[j] = found;
    j = j + 1;
  }
  a = presentes[0] - 1;
  b = presentes[1] - 1;

  return std::make_tuple(a, b);
}

bool compare_menor(const Presente *l, const Presente *r)
{
  return l->peso < r->peso;
}

bool compare_maior(const Presente *l, const Presente *r)
{
  return l->peso > r->peso;
}

int algoritmo_guloso(int npresentes, int ntrenos, int capacidade, int nrestricoes, vector<Presente *> presentes, vector<Treno *> trenos)
{
  int resposta = 0;
  printf("N presentes : %i \n", npresentes);
  printf("N trenos : %i \n", ntrenos);
  printf("Capacidade : %i \n", capacidade);
  printf("N restricoes : %i \n", nrestricoes);

  for (int i = 0; i < npresentes; i++)
  {
    presentes[i]->print_presente();
  }
  vector<Presente *> candidatos;
  int selecionados = 0;
  int id_treno = 0;

  for (int i = 0; i < npresentes; i++)
  {
    if (presentes[i]->id_pai == -1)
    {
      candidatos.push_back(presentes[i]);
    }
  }

  std::sort(candidatos.begin(), candidatos.end(), compare_maior);

  while (selecionados < npresentes)
  {
    Treno *treno = new Treno(id_treno, capacidade);
    id_treno++;
    // cria um treno e da um id unico
    int restando = capacidade - treno->return_peso();
    // variavel que calcula quanto que ainda cabe no treno (inicialmente, como n tem nada, a capacidade)

    // agora:
    // criterio guloso: adicionar ao treno o maior presente possivel (que ainda caiba nele, quanto ao peso)
    // desde que tal presente tambem respeite as restricoes
    for (int i = 0; i < (candidatos.size()); i++)
    {
      int index = candidatos[i]->return_id();
      // if((candidatos[i]->return_peso()<restando) &&((treno->verifica_compatibilidade(index))==0))
      // if((candidatos[i]->return_peso()<restando))
      if ((candidatos[i]->return_peso() < restando) && ((treno->verifica_compatibilidade(index)) == 0) && (candidatos[i]->return_pai() == -1))
      {
        treno->adiciona_presente(candidatos[i]);
        selecionados++;
        restando = capacidade - treno->return_peso();
        // candidatos.erase(candidatos.begin()+i);
      }
    }

    trenos.push_back(treno);
    // fim do while
  }

  resposta = trenos.size();
  printf("Total de trenos: %i \n", resposta);
  for (int t = 0; t < trenos.size(); t++)
  {
    trenos[t]->print_treno();
  }

  return resposta;
}

// funcao que remove um presente contido num vetor, a partir do indice, e retorna (uma copia de) ele
Presente remove_presente(vector<Presente *> presentes, int index)
{
  int id = presentes[index]->return_id();
  int peso = presentes[index]->return_peso();
  int npresentes = presentes[index]->npresentes;
  int id_pai = presentes[index]->npresentes;
  int *restricoes = new int[npresentes];
  Presente presente(id, peso, npresentes);
  for (int i = 0; i < npresentes; i++)
  {
    restricoes[i] = (presentes[index]->restricoes[i]);
  }
  presentes.erase(presentes.begin() + index);
  return presente;
}

int main()
{
  // std::cout << "Hello World!\n";
  // ifstream Arquivo ("Entrada.txt");
  ifstream Arquivo("n30_k150_A.txt");
  // ifstream Arquivo ("n80_k130_A.txt");
  // ifstream Arquivo ("n120_k150_A.txt");

  string string;
  int i = 0;
  int npresentes;
  int ntrenos;
  int capacidade;
  int nrestricoes;

  // extraindo o numero de presentes, de trenos, a capacidade, e o numero de restricoes
  while (getline(Arquivo, string))
  {
    if (i == 0)
    {
      sscanf(string.c_str(), "%d", &npresentes);
    }
    else if (i == 1)
    {
      sscanf(string.c_str(), "%d", &ntrenos);
    }
    else if (i == 2)
    {
      sscanf(string.c_str(), "%d", &capacidade);
    }
    else if (i == 3)
    {
      sscanf(string.c_str(), "%d", &nrestricoes);
    }
    else
    {
      break;
    }
    i = i + 1;
  }

  // criando a matriz de restricoes, inicializada com 0
  //  leia-se: 0 == sem restricao
  int *pesos_presentes = new int[npresentes];
  int matriz_restricoes[npresentes][npresentes];
  for (i = 0; i < npresentes; i++)
  {
    for (int j = 0; j < npresentes; j++)
    {
      matriz_restricoes[i][j] = 0;
    }
  }

  // reinicia a leitura do arquivo
  i = 0; // rewind(Arquivo);
  Arquivo.clear();
  Arquivo.seekg(0);

  // extrair pesos de presentes e as restricoes, lendo linha a linha
  while (getline(Arquivo, string))
  {
    if (i == 5)
    {
      extractPesos(string, npresentes, pesos_presentes);
    }
    else if (i > 6)
    {
      int a, b;
      tie(a, b) = extractRestricoes(string, npresentes, a, b);
      matriz_restricoes[a][b] = 1;
      matriz_restricoes[b][a] = 1;
    }
    i = i + 1;
  }

  // vetor de presentes e trenos (ponteiros)
  vector<Presente *> presentes;
  vector<Treno *> trenos;
  for (i = 0; i < npresentes; i++)
  {
    presentes.push_back(new Presente(i, pesos_presentes[i], npresentes));
  }

  // removendo presentes de peso maior que capacidade ou peso negativo, pondo as restricoes de tais presentes como negativa (serao ignoradas)
  for (i = 0; i < npresentes; i++)
  {
    if (presentes[i]->return_peso() > capacidade)
    {
      presentes.erase(presentes.begin() + i);
      npresentes--;
      for (int j = 0; j < npresentes; j++)
      {
        matriz_restricoes[i][j] = -1;
        matriz_restricoes[j][i] = -1;
      }
    }

    else if ((presentes[i]->return_peso()) < (0))
    {
      presentes[i]->muda_peso(0);
    }
  }

  // adicionado restricoes da matriz de restricoes no array de restricoes de cada presente, individualmente
  // leia-se: se for == 1, presente i tem restricao com j
  for (i = 0; i < npresentes; i++)
  {
    for (int j = 0; j < npresentes; j++)
    {
      if (matriz_restricoes[i][j] == 1 && i != j)
      {

        presentes[i]->adiciona_restricao(j);
        presentes[j]->adiciona_restricao(i);
      }
    }
  }

  int resposta = algoritmo_guloso(npresentes, ntrenos, capacidade, nrestricoes, presentes, trenos);

  return 0;
}