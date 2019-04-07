/*
Utility functions used in the different implementations
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NLINKS 100000000 //maximum number of edges for memory allocation, will increase if needed

//Computes the maximum of three unsigned long
unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

// struct used for reordering nodes
struct node{
    unsigned long node_nb;
    unsigned int degree;
};

//Exercise 2 - Size of a graph
//Counts the number of nodes
unsigned long countgraph(char* input){
	FILE *file=fopen(input,"r");
	unsigned long n=0;
	unsigned long s,t;
	while (fscanf(file,"%lu %lu", &s, &t)==2) {
		n=max3(n,s,t);
	}
	fclose(file);
	n++;
	return n;
}

//Exercise 3 - Cleaning data
//Writes a new txt file without bidirected edges and self loops
void cleangraph(char* input, char* output){
	char command[500]  = "";
	strcat(command, "awk '{if ($1<$2) print $1\" \"$2;else if ($2<$1) print $2\" \"$1}' ");
	strcat(command, input);
	strcat(command,  " | sort -n | uniq >");
	strcat(command, output);
	int e = system(command);
}

//Exercise 4 - Node degree
//writing the degree of each node from file
unsigned long* write_degrees(char* input, unsigned long n){
	FILE *file=fopen(input,"r");

	unsigned long s;
	unsigned long t;
	unsigned long *degrees= calloc(n,sizeof(unsigned long));
	while (fscanf(file,"%lu %lu", &s, &t)==2) {
		if(s!=t){
            degrees[s]++;
		    degrees[t]++;}
        else{degrees[s]++;}
	}
	fclose(file);
	// writing list to file
	FILE *fp = fopen("degrees.txt","w");
	int i;
	for(i = 0; i < n;i++){
		fprintf (fp, "%u %lu\n", i, degrees[i]);
	}
	fclose (fp);
	return degrees;
}

//Exercise 7 - Three graph datastructures
//1. List of edges
typedef struct {
	unsigned long s;
	unsigned long t;
} edge;

//edge list structure:
typedef struct {
	unsigned long n;//number of nodes
	unsigned long e;//number of edges
	edge *edges;//list of edges
} edgelist;

//reading the edgelist from file and store it in memory as an list of edges
edgelist* readedgelist(char* input){
	unsigned long e1=NLINKS;
	FILE *file=fopen(input,"r");

	edgelist *g=malloc(sizeof(edgelist));
	g->n=0;
	g->e=0;
	g->edges=malloc(e1*sizeof(edge));//allocate some RAM to store edges

	while (fscanf(file,"%lu %lu", &(g->edges[g->e].s), &(g->edges[g->e].t))==2) {
		g->n=max3(g->n,g->edges[g->e].s,g->edges[g->e].t);
		if (g->e++==e1) {//increase allocated RAM if needed
			e1+=NLINKS;
			g->edges=realloc(g->edges,e1*sizeof(edge));
		}
	}
	fclose(file);

	g->n++;

	g->edges=realloc(g->edges,g->e*sizeof(edge));

	return g;
}

//removing edgelist from memory
void free_edgelist(edgelist *g){
	free(g->edges);
	free(g);
}

//2. Adjacency matrix

unsigned long* build_matrix(char* input, unsigned long n) {

    unsigned long* G= calloc(n*n,sizeof(unsigned long));
    unsigned long t;
    unsigned long s;
    //Initialization of vertices
    unsigned long test_sum = 0;

    FILE* file=fopen(input,"r");
    //Reading the edges;
    while (fscanf(file,"%lu %lu", &s, &t)==2) {
        G[s*n + t]=1;
        G[t*n + s]=1;
    }

    for (int i = 0; i < n*n;i++){test_sum+=G[i];}

    printf("Number of edges with G: %lu\n",test_sum);

   return G;
}

//3. Adjacency array

typedef struct {
	unsigned long n; //number of nodes
	unsigned long *cd; //cumulative degree
	unsigned long *adj; //concatenated lists of neighbors
} adjarray;

adjarray* create_adjarray(char* input, unsigned long *degrees, unsigned long n){
	FILE *file=fopen(input,"r");

	unsigned long s,t;
	adjarray *g=malloc(sizeof(adjarray));
	unsigned long sum_degrees = 0;
	unsigned long *current_nb_neighbor = calloc(n,sizeof(unsigned long));
	g->cd=calloc(n+1, sizeof(unsigned long));//allocate some RAM to cumulative degrees
	for (int j=1; j<n+1; j++){
		g->cd[j]+=degrees[j-1] + g->cd[j-1];
		}
	g->n=n;
	for (int i = 0; i<n; i++){
		sum_degrees += degrees[i]; //total number of directed edges
		}
	g -> adj=calloc(sum_degrees, sizeof(unsigned long));
	while (fscanf(file,"%lu %lu", &s, &t)==2) {
			g->adj[g->cd[s] + current_nb_neighbor[s]] = t;
      g->adj[g->cd[t] + current_nb_neighbor[t]] = s;
			current_nb_neighbor[s]++;
      current_nb_neighbor[t]++;
		}
	fclose(file);

	return g;
}

//removing adjacency array from memory
void free_adjarray(adjarray *g){
	free(g->cd);
	free(g->adj);
	free(g);
}

unsigned long countgraph_v2(char* input){
	unsigned long n = countgraph(input);
	unsigned long *dejavu= calloc(n+1,sizeof(unsigned long)); //un 1 a la ieme place indique qu'on a deja vu le node i
	unsigned long compteur = 0;
	unsigned long s;
	unsigned long t;
	int a=1;
	FILE *file=fopen(input,"r");
	while (fscanf(file,"%lu %lu", &s, &t)==2) {
		if(dejavu[s]==0){
			dejavu[s]=1;
			compteur++;
		}
		if(dejavu[t]==0){
			dejavu[t]=1;
			compteur++;
		}
	}
return compteur;
}


void renumber(char* input, char* output){
	unsigned long n = countgraph(input);
	unsigned long *dejavu= calloc(n+1,sizeof(unsigned long)); //un 1 a la ieme place indique qu'on a deja vu le node i
	unsigned long *correspondance= calloc(n+1,sizeof(unsigned long)); //a la ieme place se trouve le nouveau nom du noeud i
	unsigned long compteur = 0;
	unsigned long s;
	unsigned long t;
	int a=1;
	FILE *file=fopen(input,"r");
	while (fscanf(file,"%lu %lu", &s, &t)==2) {
		if(dejavu[s]==0){
			correspondance[s]=compteur;
			dejavu[s]=1;
			compteur++;
		}
		if(dejavu[t]==0){
			correspondance[t]=compteur;
			dejavu[t]=1;
			compteur++;
		}
	}
	fclose(file);
	FILE *file2=fopen(input,"r");
	FILE *fp=fopen(output,"w");
    while (fscanf(file2,"%lu %lu", &s, &t)==2) {
			fprintf (fp, "%lu %lu\n", correspondance[s], correspondance[t]);
					}
	fclose (fp);
	fclose(file);
}
