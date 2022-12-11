#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

class Presente {
  public:
    int peso;
    int  id;
    int id_pai;
    int *restricoes = NULL; //um presente sabe seu id proprio, peso, id pai (treno em que ele esta) e possui um vetor de restricoes
    int npresentes;
    Presente(int i, int p, int n) {
      id = i;
      peso = p;
      id_pai = -1;
      npresentes =n;
      restricoes = new int[npresentes];
      //int* restricoes = (int*) malloc(npresentes*sizeof(int));
      for (int i=0;i<npresentes;i++){
        restricoes[i] = 0;
      }
    }

    int return_peso(){
      return peso;
    }
    void muda_peso(int p){
      peso = p;
    }
    int return_id(){
      return id;
    }
    int return_pai(){
      return id_pai;
    }
    void muda_pai(int pai){
      id_pai = pai;
    }
    void adiciona_restricao(int i){ //recebe um id de outro presente, e indica que a posicao no vetor (referente ao id) possui restricao
      restricoes[i] = 1;
    }

    int return_restricao(int id_presente){
      return restricoes[id_presente]; //retorna 1 ou 0, indicando se ha ou nao restricao daquele presente com aquele id de outro presente
    }

    void print_restricoes(){
      //printf("Printando restricoes de %i \n",id);
      for (int i =0;i<npresentes;i++){
        if(restricoes[i]==1){
          printf("Restricao de %i com %i: %i\n",id,i,restricoes[i]);
        }
      }
  }

    void print_presente(){
      printf("Presente de id: %i e peso: %i \n",id,peso);
    }

};



class Treno {
  public:
    int capacidade;
    int  id;
    int peso_atual;
    vector<Presente*> presentes; //um treno possui id, capacidade, peso atual (somatorio dos presentes) e vetor de presentes
    Treno(int i, int c) {
      id = i;
      capacidade = c;
      peso_atual = 0;
    }

    void adiciona_presente(Presente* presente){ //adiciona um presente no vetor do treno, muda o pai daquele presente, soma ao peso atual do treno
      if ((peso_atual + presente->return_peso()) <= capacidade){
        presentes.push_back(presente);
        presente->muda_pai(id);
        peso_atual = peso_atual + presente->return_peso();
      }
    }

    void printa_presentes(){
      for (int i =0;i<(presentes.size());i++){
        printf("Presente de id %i e peso %i \n",presentes[i]->return_id(),presentes[i]->return_peso());
      }
    }

    void print_treno(){
      printf("Treno %i com presentes:\n",id);
      this->printa_presentes();
      printf("Espaco sobrando: %i \n",(capacidade-peso_atual));
    }

    int return_peso(){
      return peso_atual;
    }

    int verifica_compatibilidade(int index){
      // funcao q retorna se existe uma incompatibilidade entre um presente de certo indice (id) e os presentes ja presentes no treno
      //basicamente, se retornar 0, eh compativel
      int j1=0;
      int npresentes = presentes.size();
      for (int i =0;i<npresentes;i++){
        j1 =j1+presentes[i]->return_restricao(index);
        if (j1==1){
          i =npresentes +2;
        }
      }
      return j1;
    }

    int npresentes(){
      int npresentes = presentes.size();
      return npresentes;
    }
    //metodo q LIMPA o treno. Usa APENAS, depois que fazer uma copia dos presentes
    void esvazia_treno(){
      presentes.clear();
      peso_atual = 0;
    }
};



//fonte do extract:
//https://www.geeksforgeeks.org/extract-integers-string-c/
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

std::tuple<int, int> extractRestricoes(string str, int a, int dividend, int b) { //retorna uma tupla de valores
  //printf("Chegando \n");
  stringstream ss;
  ss << str;
  string temp;
  int found; int j = 0; int presentes[2];
  while (!ss.eof()) {
    ss >> temp;
    if (stringstream(temp) >> found)
        //cout << found << " ";
        presentes[j] = found;
        j = j+1;
  }
  a = presentes[0]-1;
  b = presentes[1]-1;

  return  std::make_tuple(a,b);
}


bool compare_menor( const Presente* l, const Presente* r) {
  return l->peso < r->peso;
}

bool compare_maior( const Presente* l, const Presente* r) {
  return l->peso > r->peso;
}

//agora, funcao que copia um presente e retorna a copia
Presente* copia_presente(Presente* original){
  Presente* copia;
  copia = new Presente(original->return_id(),original->return_peso(),original->npresentes);
  for(int i =0;i<copia->npresentes;i++){
    if (original->return_restricao(i)>0){
      copia->adiciona_restricao(i);
    }
  }
  return copia;
}

//funcao que retorna uma copia do treno original com seus presentes
Treno* copia_treno(Treno* original){
  Treno* copia;
  copia = new Treno(original->id,original->capacidade);
  vector<Presente*> copias_presentes;
  int npresentes = original->npresentes();
  for (int i =0;i<npresentes;i++){
    Presente* novo = copia_presente(original->presentes[i]);
    copia->adiciona_presente(novo);
  }

  return copia;
}

//retornar um vector copiado de trenos copiados
vector<Treno*> copia_trenos(vector<Treno*> original){
  vector<Treno*> copia;
  int ntrenos = original.size();
  for (int i =0;i<ntrenos;i++){
    Treno* novo = copia_treno(original[i]);
    copia.push_back(novo);
  }

  return copia;
}

void print_trenos(vector <Treno*> trenos){
  int resposta = trenos.size();
  printf("Total de trenos: %i \n",resposta);
  for(int t=0;t<trenos.size();t++){
      trenos[t]->print_treno();
   }
}

//funcao q remove treno de um vector por indice e retorna copia do removido
/*
Treno* remove_treno(vector <Treno*> trenos, int index){
  Treno* novo = copia_treno(trenos[index]);
  trenos.erase(trenos.begin() + index);
  return novo;
}
*/

//funcao q remove um treno e retorna o vector de trenos sem o removido
vector <Treno*> remove_treno(vector <Treno*> trenos, int index){
  vector <Treno*> novo = copia_trenos (trenos);
  novo.erase (novo.begin()+index);
  return novo;
}

//remove um treno e retorna o vector de trenos. mas diferente do anterior, remove um treno Pelo seu ID, nao posicao no vector
vector <Treno*> remove_via_id(vector <Treno*> trenos, int index){
  vector <Treno*> novo = copia_trenos (trenos);
  for (int i=0;i<novo.size();i++){
    if (novo[i]->id == index){
      novo.erase(novo.begin()+i);
      break;
    };
  }
  return novo;
}

//o mesmo da funcao anterior, mas c 2 trenos
vector <Treno*> remove2_via_id(vector <Treno*> trenos, int index, int index2){
  vector <Treno*> novo = copia_trenos (trenos);
  for (int i=0;i<novo.size();i++){
    if (novo[i]->id == index){
      novo.erase(novo.begin()+i);
    };
  }
  for (int i=0;i<novo.size();i++){
    if (novo[i]->id == index2){
      novo.erase(novo.begin()+i);
    };
  }
  return novo;
}


//o mesma da funcao anterior, mas com 3 trenos
vector <Treno*> remove3_via_id(vector <Treno*> trenos, int index, int index2, int index3){
  vector <Treno*> novo = copia_trenos (trenos);
  for (int i=0;i<novo.size();i++){
    if (novo[i]->id == index){
      novo.erase(novo.begin()+i);
      break;
    };
  }
  for (int i=0;i<novo.size();i++){
    if (novo[i]->id == index2){
      novo.erase(novo.begin()+i);
      break;
    };
  }
  for (int i=0;i<novo.size();i++){
    if (novo[i]->id == index3){
      novo.erase(novo.begin()+i);
      break;
    };
  }
  return novo;
}


//o algoritmo guloso
vector <Treno*> algoritmo_guloso(int npresentes, int ntrenos, int capacidade, int nrestricoes, vector <Presente*> presentes, vector <Treno*> trenos){
  //int resposta = 0;
  printf("N presentes : %i \n",npresentes);
  printf("N trenos : %i \n",ntrenos);
  printf("Capacidade : %i \n",capacidade);
  printf("N restricoes : %i \n",nrestricoes);

  for (int i = 0;i<npresentes;i++){
    presentes[i]->print_presente();
  }
  vector <Presente*> candidatos;
  int selecionados=0; int id_treno = 0;
    //cria um vector de candidatos
    //na pratica, nao precisava, criei so pra evidenciar o guloso mesmo
  for (int i =0;i<npresentes;i++){
      if(presentes[i]->id_pai ==-1){
        candidatos.push_back(presentes[i]);
      }
  }

  std::sort(candidatos.begin(),candidatos.end(),compare_maior);
    //ordena os presentes por peso
    //criterior guloso adotado:
    //cada treno ira adicionar SEMPRE o maior presente possivel (cabe no treno, nao tem restricoes com outros presentes que estao no treno, nao esta em nehum treno)

  while(selecionados<npresentes){ //enquanto todos os presentes nao forem selecionados
    Treno* treno = new Treno(id_treno,capacidade);
    id_treno ++;
    //cria um treno e da um id unico
    int restando = capacidade - treno-> return_peso();
    //variavel que calcula quanto que ainda cabe no treno (inicialmente, como n tem nada, corresponde a capacidade)

    //agora:
    //criterio guloso: adicionar ao treno o maior presente possivel (que ainda caiba nele, quanto ao peso)
    //desde que tal presente tambem respeite as restricoes
    for (int i=0;i<(candidatos.size());i++){ //va de presente em presente
      int index = candidatos[i]->return_id();
      //if((candidatos[i]->return_peso()<restando) &&((treno->verifica_compatibilidade(index))==0))
      //if((candidatos[i]->return_peso()<restando))
      if((candidatos[i]->return_peso()<=restando) &&((treno->verifica_compatibilidade(index))==0)&&(candidatos[i]->return_pai()==-1))
        {
          treno->adiciona_presente(candidatos[i]);
          selecionados++;
          restando = capacidade - treno-> return_peso();
          //candidatos.erase(candidatos.begin()+i);
        }
    }


    trenos.push_back(treno); //adiciona treno no vector de trenos
   // fim do while
  }

  //print_trenos(trenos);


  return trenos; //retorna o vector de trenos, eh a solucao
}

//funcao que remove um presente contido num vetor, a partir do indice, e retorna (uma copia de) ele
Presente remove_presente(vector <Presente*> presentes,int index){
  int id = presentes[index]->return_id();
  int peso = presentes[index]->return_peso();
  int npresentes = presentes[index]->npresentes;
  int id_pai = presentes[index]->npresentes;
  int* restricoes = new int [npresentes];
  Presente presente(id,peso,npresentes);
  for (int i=0;i<npresentes;i++){
    restricoes[i] = (presentes[index]->restricoes[i]);
  }
  presentes.erase(presentes.begin() + index);
  return presente;
}
//aqui sera a vizinhanca 1
//ideia: sera pego UM treno qualquer (o for fara isso, garantindo que todo treno sera pego)
//tal treno sera pego, seus presentes removidos e ele sera apagado
// em seguida, tentaremos distribuir esses presentes
//se der certo, a solucao sera melhorada
//se os presentes nao couberem em nenhum outro treno, serao criados outros novos trenos pra por esses presentes
//os novos trenos serao adicionados no vector de trenos
//no fim, se o vector de trenos estiver menor doq antes, eh pq a solucao foi melhorada


vector <Treno*> Vizinhanca1(vector <Treno*> original){ //vizinhanca 1
  vector <Treno*> nova_solucao = copia_trenos(original);
  int t_original = original.size();
  vector <Treno*> melhor_encontrado = copia_trenos(original);

  for (int i =0;i<t_original;i++){

    //cria solucao temporaria, inicialmente igual a original. ela sera usada para comparar e checar se ha melhora
    vector <Treno*> temporario = copia_trenos(original);
    vector<Presente*> presentes; //vetor que ira armazenar os presentes removidos do treno escolhido
    int npresentes_treno = original[i]->npresentes() ;

    //para um treno, ponha os presentes dele num vetor de presentes, delete esse treno do vector de trenos (temporario)
    for (int j=0;j<npresentes_treno;j++){
      presentes.push_back(original[i]->presentes[j]);
      presentes[j]->muda_pai(-1);
      //presentes[j]->print_presente();
    }
    //Treno* novo_treno = copia_treno(temporario[i]);
    temporario = remove_treno(temporario,i);




    //aqui, distribuir presentes nos trenos q couberem
    //para cada treno no vector temporario
    //para cada presente no vector de presentes
    //se o presente der no treno, ponha ele
    for (int j =0;j<temporario.size();j++){
      int restando = temporario[j]->capacidade - temporario[j]-> return_peso();
      for(int k=0;k<presentes.size();k++){
        int index = presentes[k]->return_id();
        if((presentes[k]->return_peso()<=restando) &&((temporario[j]->verifica_compatibilidade(index))==0)&&(presentes[k]->return_pai()==-1)){
            temporario[j]->adiciona_presente(presentes[k]);
          restando = temporario[j]->capacidade - temporario[j]-> return_peso();
          }

      }

      }
    //se houverem presentes sem pai
    //ponha eles num treno novo, de volta
    int sempai = 0;
    for (int b=0;b<presentes.size();b++){
      if (presentes[b]->return_pai() ==-1){sempai++;} //verifica quantos presentes estao sem pai (sem treno)
    }
    if (sempai>0){ //se houver um presente sequer sem pai
    int selecionados = 0;
    int novo_id = t_original +1;
    int capacidade = original[0]->capacidade;
    while(selecionados<sempai){  //ate os presentes estarem todos com pai (treno)
      Treno* novo_treno = new Treno(novo_id,capacidade); //cria um treno
      novo_id = novo_id +1;

      int restando = capacidade - novo_treno-> return_peso();
      for (int ii=0;ii<(presentes.size());ii++){ //vai de presente em presente
        int index = presentes[ii]->return_id(); //tente por o presente
        if((presentes[ii]->return_peso()<=restando) &&((novo_treno->verifica_compatibilidade(index))==0)&&(presentes[ii]->return_pai()==-1))
        {
          novo_treno->adiciona_presente(presentes[ii]);
          selecionados++;
          restando = capacidade - novo_treno-> return_peso();
        }
      }
      temporario.push_back(novo_treno);//adiciona o treno no vector de trenos
    }
    }

    //printf("\n Nova solucao: \n");
    //print_trenos(temporario);

    //verifica se esse foi a melhor solucao ate agora (ou seja, existem menos trenos do que antes)
      if (temporario.size()<melhor_encontrado.size()){
        melhor_encontrado = temporario;
      }

     //fim da exploracao da vizinhanca
    }



  if(melhor_encontrado.size()<nova_solucao.size()){
    nova_solucao = melhor_encontrado;
    //printf("Foi encontrado uma solucao melhor \n");
  }
  //printf("\n Nova solucao: \n");
  //print_trenos(nova_solucao);
  return nova_solucao;
}


vector <Treno*> Vizinhanca2(vector <Treno*> original){ // a vizinhanca 2 funciona exatamente igual a vizinhanca 1, mas pegando 2 trenos por vez em vez de 1
  vector <Treno*> nova_solucao = copia_trenos(original);
  int t_original = original.size();
  vector <Treno*> melhor_encontrado = copia_trenos(original);

  //aqui, adaptar a vizinhanca anterior pra 2 trenos
  //o for garante que faremos 2 trenos, e serao diferentes
  //na pratica, a combinacao de i e j ira se repetir
  //contudo, TALVEZ isso afete a ordem na qual os presentes serao adicionados aos outros trenos, o que pode resultar em solucoes diferentes
  //novamente, pega 2 trenos, tira os presentes, poe os presentes no vectir de presentes sem pai, remove os trenos
  //tenta adicionar os presentes a outros trenos
  //se nao funcionar, cria novos trenos para esses presentes
  for (int i=0;i<t_original;i++){
    for (int j=0;j<t_original;j++){
      if (i!=j){

      //printf("Valores de i e j %i %i \n",i,j);

      vector <Treno*> temporario = copia_trenos(original); //cria vector temporario, igual ao original
      vector<Presente*> presentes;
      int npresentes_treno1 = original[i]->npresentes() ;
      int npresentes_treno2 = original[j]->npresentes() ;
      int npresentes = npresentes_treno1 + npresentes_treno2;

      for (int k=0;k<npresentes_treno1;k++){
        presentes.push_back(temporario[i]->presentes[k]);
        presentes[k]->muda_pai(-1);
      }
      for (int k=0;k<npresentes_treno2;k++){
        presentes.push_back(temporario[j]->presentes[k]);
        //temporario[j]->presentes[k]->muda_pai(-1);
        presentes[k+npresentes_treno1]->muda_pai(-1);
      }
        //poe presentes dos trenos escolhidos (i e j) no vector de presentes sem pai, remove esses trenos

      temporario = remove2_via_id(temporario,i,j);

      //printf("\n\n\n");
      //print_trenos(temporario);
      //printf("Tamanho de temporario: %i \n", temporario.size());
      //printf("Cheguei aqui \n");
      for (int k =0;k<temporario.size();k++){
        //temporario[k]->print_treno();
        int restando = temporario[k]->capacidade - temporario[k]-> return_peso();
        //printf("Cheguei aqui \n");
        for (int a=0;a<presentes.size();a++){
          int index = presentes[a]->return_id();
          if((presentes[a]->return_peso()<=restando) &&((temporario[k]->verifica_compatibilidade(index))==0)&&(presentes[a]->return_pai()==-1)){

            temporario[k]->adiciona_presente(presentes[a]);
            restando = temporario[k]->capacidade - temporario[k]-> return_peso();
          }
        }
      }
        int sempai = 0;
        for (int b=0;b<presentes.size();b++){
          if (presentes[b]->return_pai() ==-1){sempai++;}
          }
        if (sempai>0){
        int selecionados = 0;
        int novo_id = t_original +1;
        int capacidade = original[0]->capacidade;
        while(selecionados<sempai){
          Treno* novo_treno = new Treno(novo_id,capacidade);
          novo_id = novo_id +1;
          int restando = capacidade - novo_treno-> return_peso();
          for (int ii=0;ii<(presentes.size());ii++){
            int index = presentes[ii]->return_id();
            if((presentes[ii]->return_peso()<=restando) &&((novo_treno->verifica_compatibilidade(index))==0)&&(presentes[ii]->return_pai()==-1))
            {
              novo_treno->adiciona_presente(presentes[ii]);
              selecionados++;
              restando = capacidade - novo_treno-> return_peso();
            }
          }
            temporario.push_back(novo_treno);
        }
        }
        //printf("\n\n");
        //print_trenos(temporario);
        if (temporario.size()<melhor_encontrado.size()){
          melhor_encontrado = temporario;
        }
        }
    }
  }


  //printf("\n Nova solucao: \n");
  //print_trenos(melhor_encontrado);
  if(melhor_encontrado.size()<nova_solucao.size()){
    nova_solucao = melhor_encontrado;
    //printf("Foi encontrado uma solucao melhor \n");
  }

  //printf("\n Nova solucao: \n");
  //print_trenos(nova_solucao);
  return nova_solucao;
}


vector <Treno*> Vizinhanca3(vector <Treno*> original){ //literalmente a vizinhanca 2, mas aplicada a 3 trenos por vez em vez de 2
  vector <Treno*> nova_solucao = copia_trenos(original);
  int t_original = original.size();
  vector <Treno*> melhor_encontrado = copia_trenos(original);

  //aqui, adaptar a vizinhanca anterior pra 2 trenos

  for (int i=0;i<t_original;i++){
    for (int j=0;j<t_original;j++){
      for(int y=0; y<t_original;y++){
        if (i!=j && i!=y && j!=y){

      //printf("Valores de i e j %i %i \n",i,j);

          vector <Treno*> temporario = copia_trenos(original);
          vector<Presente*> presentes;
          int npresentes_treno1 = original[i]->npresentes() ;
          int npresentes_treno2 = original[j]->npresentes() ;
          int npresentes_treno3 = original[y]->npresentes();
          int npresentes = npresentes_treno1 + npresentes_treno2+npresentes_treno3;

          for (int k=0;k<npresentes_treno1;k++){
            presentes.push_back(temporario[i]->presentes[k]);
            presentes[k]->muda_pai(-1);
          }
          for (int k=0;k<npresentes_treno2;k++){
            presentes.push_back(temporario[j]->presentes[k]);
            //temporario[j]->presentes[k]->muda_pai(-1);
            presentes[k+npresentes_treno1]->muda_pai(-1);
          }
          for (int k=0;k<npresentes_treno3;k++){
            presentes.push_back(temporario[y]->presentes[k]);
            //temporario[j]->presentes[k]->muda_pai(-1);
            presentes[k+npresentes_treno1+npresentes_treno2]->muda_pai(-1);
          }


          temporario = remove3_via_id(temporario,i,j,y);

          //printf("\n\n\n");
          //print_trenos(temporario);
          //printf("Tamanho de temporario: %i \n", temporario.size());
          //printf("Cheguei aqui \n");
          for (int k =0;k<temporario.size();k++){
            //temporario[k]->print_treno();
            int restando = temporario[k]->capacidade - temporario[k]-> return_peso();

            for (int a=0;a<presentes.size();a++){
              int index = presentes[a]->return_id();
              if((presentes[a]->return_peso()<=restando) &&((temporario[k]->verifica_compatibilidade(index))==0)&&(presentes[a]->return_pai()==-1)){

                temporario[k]->adiciona_presente(presentes[a]);
                restando = temporario[k]->capacidade - temporario[k]-> return_peso();
              }
            }
          }
            int sempai = 0;
            for (int b=0;b<presentes.size();b++){
              if (presentes[b]->return_pai() ==-1){sempai++;}
              }
            if (sempai>0){
            int selecionados = 0;
            int novo_id = t_original +1;
            int capacidade = original[0]->capacidade;
            while(selecionados<sempai){
              Treno* novo_treno = new Treno(novo_id,capacidade);
              novo_id = novo_id +1;
              int restando = capacidade - novo_treno-> return_peso();
              for (int ii=0;ii<(presentes.size());ii++){
                int index = presentes[ii]->return_id();
                if((presentes[ii]->return_peso()<=restando) &&((novo_treno->verifica_compatibilidade(index))==0)&&(presentes[ii]->return_pai()==-1))
                {
                  novo_treno->adiciona_presente(presentes[ii]);
                  selecionados++;
                  restando = capacidade - novo_treno-> return_peso();
                }
              }
                temporario.push_back(novo_treno);
            }
            }
            //printf("\n\n");
            //print_trenos(temporario);
            if (temporario.size()<melhor_encontrado.size()){
              melhor_encontrado = temporario;
            }
          }
    }
  }
  }


  //printf("\n Nova solucao: \n");
  //print_trenos(melhor_encontrado);
  if(melhor_encontrado.size()<nova_solucao.size()){
    nova_solucao = melhor_encontrado;
    //printf("Foi encontrado uma solucao melhor \n");
  }

  //printf("\n Nova solucao: \n");
  //print_trenos(nova_solucao);
  return nova_solucao;
}


vector <Treno*> VND(vector <Treno*> solucao,int n_estruturas){
  int k=0;
  while (k<n_estruturas){

    switch(k){
      case 0:{
        vector <Treno*> nova_solucao = Vizinhanca1(solucao);
        if (nova_solucao.size() < solucao.size()){
          solucao = nova_solucao;
          k = 0;
        }
        else {k++;}
        break;
      }

      case 1:{
        vector <Treno*> nova_solucao2 = Vizinhanca2(solucao);
        if (nova_solucao2.size() < solucao.size()){
          solucao = nova_solucao2;
          k = 0;
        }
        else {k++;}
        break;
      }

      case 2:{
        vector <Treno*> nova_solucao3 = Vizinhanca3(solucao);
        if (nova_solucao3.size() < solucao.size()){
          solucao = nova_solucao3;
          k = 0;
        }
        else {k++;}
        break;
      }
    }

  }

  return solucao;
}



int main() {
  //std::cout << "Hello World!\n";
  //ifstream Arquivo ("Entrada.txt");
  //ifstream Arquivo ("n30_k150_A.txt");
  //ifstream Arquivo ("n40_k150_C.txt");
  //ifstream Arquivo ("n60_k130_A.txt");
  //ifstream Arquivo ("n80_k130_A.txt");
  ifstream Arquivo ("n120_k150_B.txt");

  //instancias grandes:
  //ifstream Arquivo ("n400_k200_A.txt");
  //ifstream Arquivo ("n400_k200_B.txt");
  //ifstream Arquivo ("n500_k200_A.txt");
  //ifstream Arquivo ("n500_k200_B.txt");
  //ifstream Arquivo ("n1000_k200_A.txt");
  //ifstream Arquivo ("n1000_k200_B.txt");

  string string;
  int i =0; int npresentes;
  int ntrenos;
  int capacidade;
  int nrestricoes;

  //extraindo o numero de presentes, de trenos, a capacidade, e o numero de restricoes
  while (getline (Arquivo, string)) {
  if(i==0){
    sscanf(string.c_str(),"%d",&npresentes);}
  else if(i==1){
    sscanf(string.c_str(),"%d",&ntrenos);}
  else if(i==2){
    sscanf(string.c_str(),"%d",&capacidade);}
  else if(i==3){
    sscanf(string.c_str(),"%d",&nrestricoes);}
  else {break;}
    i = i+1;
  }

  //criando a matriz de restricoes, inicializada com 0
  // leia-se: 0 == sem restricao
  int *pesos_presentes = new int[npresentes];
  int matriz_restricoes [npresentes][npresentes];
  for (i = 0; i<npresentes;i++){
    for (int j = 0;j<npresentes;j++){
      matriz_restricoes[i][j] = 0;
    }
  }

  //reinicia a leitura do arquivo
  i = 0 ; //rewind(Arquivo);
  Arquivo.clear();
  Arquivo.seekg(0);

  //extrair pesos de presentes e as restricoes, lendo linha a linha
  while (getline (Arquivo, string)) {
    if (i==5){
      extractPesos(string,npresentes, pesos_presentes);
    }
    else if (i>6){
      int a,b;
      tie(a,b) = extractRestricoes(string,npresentes, a,b);
      matriz_restricoes[a][b] = 1;
      matriz_restricoes[b][a] = 1;
    }
    i = i+1;
  }

  //vetor de presentes e trenos (ponteiros)
  vector<Presente* > presentes;
  vector<Treno*> trenos;
  //cria presentes e poe no vetor de presentes. cria com um id i, peso do vetor de pesos
  for (i = 0;i<npresentes;i++){
    presentes.push_back(new Presente(i,pesos_presentes[i],npresentes));
  }

  //removendo presentes de peso maior que capacidade ou peso negativo, pondo as restricoes de tais presentes como negativa (serao ignoradas)
  for (i=0;i<npresentes;i++){
    if (presentes[i]->return_peso()>capacidade){
      presentes.erase(presentes.begin() + i);
      npresentes --;
      for (int j=0;j<npresentes;j++){
        matriz_restricoes[i][j] =-1;
        matriz_restricoes[j][i] = -1;
      }
    }

    else if ((presentes[i]->return_peso())<(0)){
      presentes[i]->muda_peso(0);
    }
  }


  //adicionado restricoes da matriz de restricoes no array de restricoes de cada presente, individualmente
  //leia-se: se for == 1, presente i tem restricao com j
  for (i = 0; i<npresentes;i++){
    for (int j = 0;j<npresentes;j++){
      if (matriz_restricoes[i][j]==1 && i!=j){

       presentes[i]->adiciona_restricao(j);
       presentes[j]->adiciona_restricao(i);
      }
    }
  }



 trenos = algoritmo_guloso(npresentes,ntrenos,capacidade,nrestricoes,presentes,trenos);


  printf("\nSolucao apos o algoritmo guloso: %li trenos \n", trenos.size());
  print_trenos(trenos);


    /*
  int somatorio_capacidade = 0;
  for (int i =0;i<trenos.size();i++){
    somatorio_capacidade = trenos[i]->capacidade - trenos[i]->peso_atual + somatorio_capacidade;
  }
  printf("Somatorio do espaco sobrando: %i \n",somatorio_capacidade);


  trenos = Vizinhanca1(trenos);
  printf("\nSolucaoo apas vizinhanca 1: %i trenos \n",trenos.size());
  //print_trenos(trenos);

  somatorio_capacidade = 0;
  for (int i =0;i<trenos.size();i++){
    somatorio_capacidade = trenos[i]->capacidade - trenos[i]->peso_atual + somatorio_capacidade;
  }
  printf("Somatorio do espaco sobrando 1: %i \n",somatorio_capacidade);


  trenos = Vizinhanca2(trenos);
  printf("\nSolucao apos vizinhanca 2: %i trenos \n",trenos.size());
  //print_trenos(trenos);

  somatorio_capacidade = 0;
  for (int i =0;i<trenos.size();i++){
    somatorio_capacidade = trenos[i]->capacidade - trenos[i]->peso_atual + somatorio_capacidade;
  }
  printf("Somatorio do espaco sobrando 2: %i \n",somatorio_capacidade);

  trenos = Vizinhanca3(trenos);
  printf("\nSolucao apos vizinhanca 3: %i trenos \n",trenos.size());
  //print_trenos(trenos);

  somatorio_capacidade = 0;
  for (int i =0;i<trenos.size();i++){
    somatorio_capacidade = trenos[i]->capacidade - trenos[i]->peso_atual + somatorio_capacidade;
  }
  printf("Somatorio do espaco sobrando 3: %i \n",somatorio_capacidade);
    */

  trenos = VND (trenos,3);
  printf("\nSolucao apos VND: \n");
  print_trenos(trenos);


  return 0;
}
