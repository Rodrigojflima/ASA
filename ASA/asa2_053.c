/* 2 PROJETO ASA */
/* Rodrigo Lima - 83559 */
/* AL053 */
/* 2018 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#define min(a, b) (a < b ? a : b)

/* Structures */
typedef struct node{
  int index; /* vertice identifier */
  struct node *next;
  int capacity;
} node;

/* Global Variables */
node** graph; /* adjacency list */
int m; /* number of rows of nodes */
int n; /* number of nodes per row */
int nr_nodes; /* total number of nodes, source(s) and sink(t) not considered */
int maxFlow;

/* Funcoes */
node** init_graph();
void edmonds_karp();

/* Funcao Main */
int main(int argc, const char * argv[]){
  int i;
  
  /* Input scanning */
  scanf("%d %d/n", &m, &n);
  nr_nodes = m*n;
  getchar(); /* Skips the blank line */
  
  /* Inicialization and filling of the graph */
  graph = init_graph();
  
  /* Algorithm execution and output printing */
  edmonds_karp();
  
  return 0;
}

/* Other Functions */

node** init_graph(){ /* Reads the input info and generates the graph as an adjacency list */
  int i, j, capacity;
  
  /* Inicialization of the graph */
  node** graph_aux = (node**) malloc(sizeof(node*) *nr_nodes+2);
  for(i=0; i < nr_nodes+2; i++){
    node* new = (node*) malloc(sizeof(node)); 
    new->index = i;
    new->capacity = -1;
    new->next = NULL;
    graph_aux[i] = new;
  }
  
  /* Filling of the adjacency list */
  /* Creating edges from source(s) to vertices - Relation lp */
  for(i=0; i < m; i++){
    for(j=0; j < n; j++){
      if(j < n-1){
	scanf("%d ", &capacity);
	node* temp = (node*) malloc(sizeof(node));
	temp->index = (n*i)+(j+1);
	temp->capacity = capacity;
	temp->next = graph_aux[0]->next;
	graph_aux[0]->next = temp;
      }else{
	scanf("%d/n", &capacity);
	node* temp = (node*) malloc(sizeof(node));
	temp->index = (n*i)+(j+1);
	temp->capacity = capacity;
	temp->next = graph_aux[0]->next;
	graph_aux[0]->next = temp;
      }
    }
  }
  
  getchar(); /* Skips the blank line */
  
  /* Creating edges from vertices to sink(t) - Relation cp */
  for(i=0; i < m; i++){
    for(j=0; j < n; j++){
      if(j < n-1){
	scanf("%d ", &capacity);
	node* temp = (node*) malloc(sizeof(node));
	temp->index = nr_nodes+1;
	temp->capacity = capacity;
	temp->next = graph_aux[(n*i)+(j+1)]->next;
	graph_aux[(n*i)+(j+1)]->next = temp;
      }else{
	scanf("%d/n", &capacity);
	node* temp = (node*) malloc(sizeof(node));
	temp->index = nr_nodes+1;
	temp->capacity = capacity;
	temp->next = graph_aux[(n*i)+(j+1)]->next;
	graph_aux[(n*i)+(j+1)]->next = temp;
      }
    }
  }
  
  getchar(); /* Skips the blank line */
  
  /* Creating edges between vertices - Relation hp */
  for(i=0; i < m; i++){
    for(j=0; j < n-1 ; j++){
      if(j < n-2){
	scanf("%d ", &capacity);
	node* temp = (node*) malloc(sizeof(node));
	temp->index = (n*i)+(j+2);
	temp->capacity = capacity;
	temp->next = graph_aux[(n*i)+(j+1)]->next;
	graph_aux[(n*i)+(j+1)]->next = temp;
	
	node* temp2 = (node*) malloc(sizeof(node));
	temp2->index = (n*i)+(j+1);
	temp2->capacity = capacity;
	temp2->next = graph_aux[(n*i)+(j+2)]->next;
	graph_aux[(n*i)+(j+2)]->next = temp2;
      }else{
	scanf("%d/n", &capacity);
	node* temp = (node*) malloc(sizeof(node));
	temp->index = (n*i)+(j+2);
	temp->capacity = capacity;
	temp->next = graph_aux[(n*i)+(j+1)]->next;
	graph_aux[(n*i)+(j+1)]->next = temp;
	
	node* temp2 = (node*) malloc(sizeof(node));
	temp2->index = (n*i)+(j+1);
	temp2->capacity = capacity;
	temp2->next = graph_aux[(n*i)+(j+2)]->next;
	graph_aux[(n*i)+(j+2)]->next = temp2;
      }
    }
  }
  
  getchar(); /* Skips the blank line */
  
  /* Creating edges between vertices - Relation vp */
  for(i=0; i < m-1; i++){
    for(j=0; j < n; j++){
      if(j < n-1){
	scanf("%d ", &capacity);
	node* temp = (node*) malloc(sizeof(node));
	temp->index = (n*i)+(j+1)+n;
	temp->capacity = capacity;
	temp->next = graph_aux[(n*i)+(j+1)]->next;
	graph_aux[(n*i)+(j+1)]->next = temp;
	
	node* temp2 = (node*) malloc(sizeof(node));
	temp2->index = (n*i)+(j+1);
	temp2->capacity = capacity;
	temp2->next = graph_aux[(n*i)+(j+1)+n]->next;
	graph_aux[(n*i)+(j+1)+n]->next = temp2;
	
      }else{
	scanf("%d/n", &capacity);
	node* temp = (node*) malloc(sizeof(node));
	temp->index = (n*i)+(j+1)+n;
	temp->capacity = capacity;
	temp->next = graph_aux[(n*i)+(j+1)]->next;
	graph_aux[(n*i)+(j+1)]->next = temp;
	
	node* temp2 = (node*) malloc(sizeof(node));
	temp2->index = (n*i)+(j+1);
	temp2->capacity = capacity;
	temp2->next = graph_aux[(n*i)+(j+1)+n]->next;
	graph_aux[(n*i)+(j+1)+n]->next = temp2;
      }
    }
  }
  
  return graph_aux;
}

void edmonds_karp(){ /* Edmonds-Karp Algorithm */
  int i, j;
  int segmentationWeight = 0;
  int result[nr_nodes+2]; /* 0 means C pixel, 1 means P pixel */
  
  node** parentsList= (node**) malloc(sizeof(node*) *nr_nodes+2);
  node** queue= (node**) malloc(sizeof(node*) *nr_nodes+2);
  
  /* Sets all vertices to classification C(0) */
  for(i=0; i<nr_nodes+2; i++){
    result[i]=0;
  }
  
  do{
    int front = 0;
    int rear = -1;
    int itemCount = 0;
    int flow = 0;
    node* currentNode;
    node* aux;
    
    for(i=0; i<nr_nodes+2; i++){ /* Cleaning of the parents array */
      parentsList[i]=NULL;
      queue[i]=NULL;
    }
    
    queue[++rear]=graph[0];
    itemCount++;
    
    do{
      currentNode = queue[front++];
      itemCount--;
      aux = graph[currentNode->index];
      for(;aux!=NULL; aux = aux->next){
	if(aux->capacity > 0 && parentsList[aux->index]==NULL){ /* There is available capacity and x is not seen before in search */
	  parentsList[aux->index]=currentNode;
	  queue[++rear]=aux;
	  itemCount++;
	  if(aux->index == nr_nodes+1){
	    itemCount=0;
	    break;
	  }
	}
      }
    }while(itemCount!=0);
    
    if(parentsList[nr_nodes+1]!=NULL){ /* We have found a path to source (t) */
      node* e = aux;
      flow = 999;
            
      for(e = aux; parentsList[e->index]!=NULL ; e=parentsList[e->index]){
	flow = min(flow, e->capacity);
      }
      e = aux;
      
      for(e = aux; parentsList[e->index]!=NULL ; e=parentsList[e->index]){
	e->capacity -= flow;
      }
      
      segmentationWeight += flow;
    }
    
  }while(parentsList[nr_nodes+1]!=NULL);
  
  /* No paths are left to find */
  /* Vertices with parents are the ones visited, we mark them with 1 (P Classification) */
  for(i=0; i<nr_nodes+2; i++){
    if(parentsList[i]!=NULL){
      result[i]=1;
    }
  }
  
  /* Output Printing */
  /* Total segmentation weight */
  printf("%d\n\n", segmentationWeight);
  
  /* Pixel classification matrix print */
  for(i=0; i<m ;i++){
    for(j=0; j<n; j++){
      if(result[(n*i)+(j+1)]==1){
	printf("C ");
      }else{
	printf("P ");
      }
    }
    printf("\n");
  }
}