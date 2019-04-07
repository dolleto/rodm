#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time
#include "utils.c"

// comparateur custom
int struct_cmp_by_price(const void *a, const void *b)
{
    struct node *ia = (struct node *)a;
    struct node *ib = (struct node *)b;
    return (int)(100.f*ia->degree - 100.f*ib->degree);
    /* float comparison: returns negative if b > a
    and positive if a > b. We multiplied result by 100.0
    to preserve decimal fraction */

}

//Exercise 4 - Node degree
//writing the degree of each node from file
unsigned long* write_degrees_max(char* input){
	FILE *file=fopen(input,"r");

	unsigned long s;
	unsigned long t;
	unsigned long n = countgraph(input);
	unsigned long *degrees= calloc(n,sizeof(unsigned long));
	while (fscanf(file,"%lu %lu", &s, &t)==2) {
		if(s>t){degrees[t]++;}
        else if(s<t){degrees[s]++;}
        else if (s==t){printf("Il ya une self loop alors qu'il ne faut pas");}
	}
	fclose(file);
	// writing list to file
	FILE *fp = fopen("degrees_max.txt","w");
	int i;
	for(int i = 0; i < n;i++){
		fprintf (fp, "%u %lu\n", i, degrees[i]);
	}
	fclose (fp);
	return degrees;
}

//reorder nodes by degree croissant
void rewrite_already_clean_file(char* input){
	unsigned long n = countgraph(input);
	unsigned long s;
	unsigned long t;

	// create a list of nodes from a file
	//struct node arr_node[n];
	struct node* arr_node = malloc(n*sizeof(struct node));
	unsigned long *degrees = write_degrees(input, n); // it bugs in write degrees for sure
	for(int i=0; i<n; i++){
	arr_node[i].node_nb = i;
	arr_node[i].degree = degrees[i];
	}

	size_t structs_len = sizeof(arr_node) / sizeof(struct node);

	//on retrie les noeuds
	qsort(arr_node, structs_len, sizeof(struct node), struct_cmp_by_price);

	// on créé la liste suivante : a l'index i se trouve le nouvel id du node ancienemment i
	unsigned long *correspondance= calloc(n,sizeof(unsigned long));
	for(int i=0; i<n; i++){
		correspondance[arr_node[i].node_nb] = i;
	}

	//on ecrit la file avec les noeuds réordonnés
	FILE *fp = fopen("nodes_reordered.txt","w");
	FILE *file=fopen(input,"r");
    while (fscanf(file,"%lu %lu", &s, &t)==2) {
			fprintf (fp, "%lu %lu\n", correspondance[s], correspondance[t]);
					}
	fclose(file);
	fclose (fp);
}

// fonction qui créé un adjarray
adjarray* create_adjarray_max(char* input){
	FILE *file=fopen(input,"r");

	unsigned long s;
	unsigned long t;
	adjarray *g=malloc(sizeof(adjarray));
	unsigned long n = countgraph(input);
	unsigned long *degrees = write_degrees_max	(input);
	unsigned long sum_degrees = 0;
	unsigned long *current_nb_neighbor = calloc(n,sizeof(unsigned long));
	g->cd=calloc(n+1, sizeof(unsigned long));//allocate some RAM to cumulative degrees
	g->cd[0]=0;
	for (int j=1; j<n+1; j++){
		g->cd[j]+=degrees[j-1] + g->cd[j-1];
		}
	g->n=n;
	for (int i = 0; i<n; i++){

		sum_degrees += degrees[i]; //total number of directed edges
		}
	g -> adj=calloc(sum_degrees, sizeof(unsigned long));
	while (fscanf(file,"%lu %lu", &s, &t)==2) {
			if(s>t){
				g->adj[g->cd[t] + current_nb_neighbor[t]] = s;
				current_nb_neighbor[t]++;
			}
            if(t>s){
            	g->adj[g->cd[s] + current_nb_neighbor[s]] = t;
           		current_nb_neighbor[s]++;
			}
		}
	fclose(file);

	return g;
}


// fonction qui compte le nombre d'éléments communs d'une liste (nulle car en n^2 donc a changer)
int compute_sub_triangles(int s, int t, adjarray* adjarr){
	int nb_triangles_sub=0;
	for(unsigned long i=adjarr->cd[s]; i<adjarr->cd[s+1]; i++){
		for(unsigned long j=adjarr->cd[t]; j<adjarr->cd[t+1]; j++){
			if(adjarr->adj[j]==adjarr->adj[i]){
				nb_triangles_sub++;
			}
		}
	}

return nb_triangles_sub;
}


int count_triangles(char* input, adjarray* adjarr){
	int nb_triangles = 0;
	unsigned long s;
	unsigned long t;

	FILE *file=fopen(input,"r");
	while (fscanf(file,"%lu %lu", &s, &t)==2) {
		nb_triangles += compute_sub_triangles(s, t, adjarr);
	}
	fclose(file);
return nb_triangles;
}


int main(int argc,char** argv){
	time_t t1,t2;
	int nb_triangles;
	adjarray* a;
	t1=time(NULL);
	// a ajouter : fonction qui supprime les trous dans la numerotation
	renumber(argv[1], argv[2]);
	// fonction qui supprime les duplicats
	// et les self loops
	cleangraph(argv[2], argv[3]);
	//fonction qui renumerote les noeuds en fonction de leur degré et l'ecrit dans "nodes_reordered.txt"
	rewrite_already_clean_file(argv[3]);

	// fonction qui créé l'adjarray modifié ne contenant que les voisins de degrés superieurs
	a=create_adjarray_max("nodes_reordered.txt");

	// fonction qui compte les triangles
	nb_triangles = count_triangles("nodes_reordered.txt", a);
	printf("Nombre de triangles: %d \n", nb_triangles);
	t2=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

	return 0;
}
