/* 1 PROJETO ASA */
/* Rodrigo Lima - 83559 */
/* 2018 */

#include<stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define min(a, b) (a < b ? a : b)

/* Estruturas */
typedef struct node{
  int index; //identificador do ponto de distribuicao
  struct node *next;
} node;

typedef struct vert{
  int index;
  struct vert *next;
} vert;

typedef struct scc{
  int min;
  struct connection *c_first;
  struct vert *v_first;
  struct scc *next;
} scc;

typedef struct connection{
  int org;
  int dst;
  struct connection *next;
} connection;

/* Variaveis Globais */
node** graph; 		//lista de adjacencias
scc* head = NULL; //cabeça da estrutura que guarda scc's
int nr_points; 	    //numero de pontos de distribuicao
int nr_connections; //numero de ligacoes entre pontos
int nr_visited;	    //numero de vertices visitados
int stackIndex;	    //indicador da cabeca de leitura/escrita da pilha
int nr_scc;         //contador do numero de scc presentes no grafo
int nr_scc_connections; //numero de ligacoes entre scc's

/* Funcoes */
node** init_graph();
void add_scc();
void add_vert(int i);
void add_connection(scc* ptr, int org, int dst);
void SCC_Tarjan();
void Tarjan_Visit(int i, int disc[], int low[], int stack[], bool stackMember[], int result[]);

void Solve_SCCconnections(int result[], int low[]);
bool exists_connection(scc* ptr, int org, int dst);

/* Funcao Main */
int main(int argc, const char * argv[]){
  /* Leitura do input */
  scanf("%d", &nr_points);
  scanf("%d", &nr_connections);
  
  /* Inicializacao e preenchimento do grafo */
  graph = init_graph();
  
  /* Algoritmo Tarjan */
  SCC_Tarjan();
  
  return 0;
}

node** init_graph(){
  int i;
  
  /* Inicializacao do grafo */
  node** graph_aux = (node**) malloc(sizeof(node*) * nr_points);
  for(i=0; i < nr_points; i++){
    graph_aux[i] = NULL;
  }
  
  /* Preenchimento da lista de adjacencias */
  int orig = 0, dest = 0;
  for(i=0; i < nr_connections; i++){
    /* Leitura de input */
    scanf("%d %d", &orig, &dest);
    /* Cria no */
    node* new = (node*) malloc(sizeof(node)); //ponteiro para node
    new->index = dest;
    new->next = NULL;
    
    /* Associa o no na lista de adjacencias */
    if(graph_aux[orig-1] == NULL){
      graph_aux[orig-1] = new;
    }
    else{
      new->next = graph_aux[orig-1];
      graph_aux[orig-1] = new;
    }
  }
  
  return graph_aux;
}

int compare(const void * s1, const void * s2){
  connection* elem1 = *(connection**)s1;
  connection* elem2 = *(connection**)s2;
  
  if(elem1->org > elem2->org){
    return 1;
  }
  else if(elem1->org < elem2->org){
    return -1;
  }
  else if(elem1->dst > elem2->dst){
    return 1;
  }
  else{
    return -1;
  }
}

void add_scc(){
  scc* new =(scc*) malloc(sizeof(scc));
  new->v_first = NULL;
  new->c_first = NULL;
  new->next = NULL;
  
  if(head == NULL){ //primeira scc a entrar
    head = new;
  }
  else{
    new->next = head;
    head = new;
  }
}

void add_vert(int i){
  vert* new =(vert*) malloc(sizeof(vert));
  new->index = i;
  new->next = NULL;
  
  if(head->v_first == NULL){
    head->v_first = new;
  }
  else{
    new->next = head->v_first;
    head->v_first = new;
  }
}

void add_connection(scc* ptr, int org, int dst){
  connection* new = (connection*) malloc(sizeof(connection));
  new->org = org;
  new->dst = dst;
  new->next = NULL;
  
  if(ptr->c_first == NULL){
    ptr->c_first = new;
  }
  else{
    new->next = ptr->c_first; 
    ptr->c_first = new;
  }
}

bool exists_connection(scc* ptr, int org, int dst){
  connection* aux = ptr->c_first;
  
  for(aux; aux!=NULL; aux = aux->next){
    if(aux->org == org && aux->dst == dst){
      return true;
    }
  }
  return false;
}

void SCC_Tarjan(){
  nr_visited = 0;
  stackIndex = 0;
  nr_scc = 0;
  int i;
  int disc[nr_points], low[nr_points], stack[nr_points], result[nr_points];
  bool stackMember[nr_points];
  
  for(i=0; i<nr_points; i++){
    disc[i] = -1;
    low[i] = -1;
    stack[i] = 0;
    result[i] = 0;
    stackMember[i] = false;
  }
  
  for(i=0; i<nr_points; i++){
    if(disc[i] == -1){
      Tarjan_Visit(i, disc, low, stack, stackMember, result);
    }
  }
  
  Solve_SCCconnections(result, low);
}

void Tarjan_Visit(int i, int disc[], int low[], int stack[], bool stackMember[], int result[]){
  nr_visited++;
  disc[i]=nr_visited;
  low[i]=nr_visited;
  
  stack[stackIndex]=i+1;
  stackMember[i]=true;
  stackIndex++;
  
  /* Associacao dos vertices da lista de adj */
  node* ptr = graph[i];	
  while(ptr != NULL){
    int v = ptr->index-1;
    if(disc[v] == -1){
      Tarjan_Visit(v, disc, low, stack, stackMember, result);
      low[i] = min(low[i], low[v]);
    }
    else if(stackMember[v]){
      low[i] = min(low[i], disc[v]);
    }
    ptr = ptr->next; //passar ao no seguinte
  }
  
  if(disc[i] == low[i]){
    int first_out = 0;
    nr_scc++;
    add_scc();
    while(stack[stackIndex] != i+1){
      stackIndex--;
      stackMember[stack[stackIndex]-1]=false;
      add_vert(stack[stackIndex]);
      result[stack[stackIndex]-1]=nr_scc;
      
      if(first_out == 0){ //primeiro da scc a sair
	head->min = stack[stackIndex];
	first_out++;
      }
      else if(stack[stackIndex] < head->min){
	head->min = stack[stackIndex];
      }
    }
  }
}

void Solve_SCCconnections(int result[], int low[]){
  int v, i, k;
  /* Atualizar a low - ira guardar em cada vertice o valor minimo do vertice da scc a que pertence */
  scc* ptr = head;
  for(ptr; ptr!=NULL; ptr = ptr->next){
    vert* ptr2 = ptr->v_first;
    while(ptr2 != NULL){
      v = ptr2->index-1;
      low[v] = ptr->min;
      ptr2 = ptr2->next;
    }
  }
  
  /* Procura de ligacoes */
  for(ptr = head; ptr!=NULL; ptr = ptr->next){
    vert* ptr2 = ptr->v_first;
    while(ptr2 != NULL){
      v = ptr2->index-1;
      node* ptr3 = graph[v];
      while(ptr3 != NULL){
	k = ptr3->index-1;
	if(result[v] != result[k]){
	  if(exists_connection(ptr, low[v], low[k])){ //retorna true se existir ligacao, false caso contrario
	  //ignora e passa ao proximo
	  }
	  else{
	    nr_scc_connections++;
	    add_connection(ptr, low[v], low[k]);
	  }
	}
	ptr3 = ptr3->next;
      }
      ptr2 = ptr2->next;
    }
  }
  
  /* Array de ligacoes */
   connection* array[nr_scc_connections];
   
  /* Passa as ligacoes para o array */
  int count = 0;
  for(ptr=head; ptr!=NULL; ptr = ptr->next){
    connection* c_aux = ptr->c_first;
    while(c_aux != NULL){
      array[count]=c_aux;
      count++;
      c_aux = c_aux->next;
    }
  }
   
  /* Q-Sort */
  qsort(array, nr_scc_connections, sizeof(connection*), compare);
  
  /* Impressao de resultados */
  printf("%d\n", nr_scc);
  printf("%d\n", nr_scc_connections);
  if(nr_scc_connections){ //se tem ligacoes, printa
    for(i=0; i<nr_scc_connections; i++){
      printf("%d %d\n", array[i]->org, array[i]->dst);
    }
  }
}