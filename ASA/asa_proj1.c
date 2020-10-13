/* 1 Projeto - ASA */
// Grupo 78
// Rodrigo Lima - 83559

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BRANCO 1
#define CINZENTO 2
#define PRETO 3

int nr_fotos;
int nr_pares_conhecidos;
int incoerente; // 1 - sem ciclos; 0 - com ciclos
int verificacoes; // verificacoes sao iguais ao [numero de vertices - 1] para grafos com topologica unica

int* vetor; //vetor que ira guardar a ordenacao topologica
int contador_restante; //conta quantas fotos ainda faltam inserir no vetor topologico

struct AdjListNode{
	int vertex; //identificador da fotografia
	int color;  //identificador da cor (branco, cinzento ou preto)
	struct AdjListNode* next;
};

struct AdjList{
  struct AdjListNode *head;
};

struct Graph{
	struct AdjList* array;
};



struct AdjListNode* newAdjListNode(int vertex);
void create_graph();
void insert_adjlist(struct Graph* graph, int vu, int vv);
void dfs(struct Graph* graph);
void dfs_visit(struct Graph* graph, struct AdjListNode* node);
void verify_order(struct Graph* graph);


struct AdjListNode* newAdjListNode(int vertex){
	struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
	newNode->vertex = vertex - 1;
	newNode->next = NULL;
	return newNode;
}

int main(){
  incoerente = 0;
	create_graph();
	return 0;
}

void create_graph(){
	int vu, vv, i;
	scanf("%d %d/n", &nr_fotos, &nr_pares_conhecidos);
	contador_restante = nr_fotos;
	verificacoes = 0;
	// Cria um novo grafo
	struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
	//Cria o vetor que ira guardar a organizacao topologica
	vetor = (int*) malloc(sizeof(int)*nr_fotos);
	// Cria o array de listas de adjacencia. Tamanho igual ao numero de vertices/fotos
	graph->array = (struct AdjList*) malloc(nr_fotos * sizeof(struct AdjList));
	for(i = 0; i < nr_fotos; i++){
	  graph->array[i].head = newAdjListNode(i+1);
	}
	for(i = 0; i < nr_pares_conhecidos; i++){
		scanf("%d %d/n", &vu, &vv);
		insert_adjlist(graph, vu, vv);
	}
	dfs(graph);
}

void insert_adjlist(struct Graph* graph, int vu, int vv){
	struct AdjListNode* newNode = newAdjListNode(vv);
	struct AdjListNode* tmp;
	if(graph->array[vu-1].head->next == NULL){
	  graph->array[vu-1].head->next = newNode;
	}
	else{
	  tmp = graph->array[vu-1].head->next;
	  while(tmp->next != NULL){
	    tmp = tmp->next;
	  }
	  tmp->next = newNode;
	}
}

void dfs(struct Graph* graph){
  int u;
  for(u = 0; u < nr_fotos; u++){
    graph->array[u].head->color = BRANCO; //todos os vertices marcados como nao visitados
  }
  for(u = 0; u < nr_fotos; u++){
    if(incoerente){
      break;
    }
    if(graph->array[u].head->color == BRANCO){
      dfs_visit(graph, graph->array[u].head);
    }
  }
  if(!incoerente){
    verify_order(graph);
    if(verificacoes == nr_fotos - 1){
      for(u = 0; u < nr_fotos; u++){
	printf("%d ", vetor[u]);
      }
      printf("\n");
    }
    else{
      printf("Insuficiente\n");
    }
  }
}

void dfs_visit(struct Graph* graph, struct AdjListNode* node){
  graph->array[node->vertex].head->color = CINZENTO;
  struct AdjListNode* nbr = node->next;
  while(nbr){
    if(graph->array[nbr->vertex].head->color == BRANCO){
	dfs_visit(graph, graph->array[nbr->vertex].head);
    }
    if(graph->array[nbr->vertex].head->color == CINZENTO){
      	printf("Incoerente\n");
	incoerente = 1;
	break;
    }
    nbr=nbr->next;
  }
  graph->array[node->vertex].head->color = PRETO;
  vetor[contador_restante-1]=node->vertex + 1;
  contador_restante -=1;
}

void verify_order(struct Graph* graph){
   int i;
   struct AdjListNode* nbr;
   for (i = 1; i < nr_fotos; i++){
     nbr = graph->array[i-1].head->next;
     while(nbr){
      if(nbr->vertex == vetor[i]-1){
	verificacoes += 1;
      }
      nbr=nbr->next;
    }
  }
 }