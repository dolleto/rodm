#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time
#include <stdbool.h>
#include "file.c"
#include "utils.c"

//Exercise 8: Breadth first search algorithm

unsigned long maxvect(unsigned long n, unsigned* vect){
	unsigned long i,imax;
	unsigned vmax=0;
	for (i=0;i<n;i++){
		if ((vect[i]!=-1) && (vect[i]>=vmax)){
			imax=i;
			vmax=vect[i];
		}
	}
	return imax;
}
//Computing connected components
typedef struct {
	unsigned long *csizes;
	unsigned long *components;
} co_components;

co_components* find_co_components(adjarray *g){
	co_components *c = malloc(sizeof(co_components));
	c->components = malloc(g->n*sizeof(unsigned long));
	int* marks = calloc(g->n, sizeof(int));
	c->csizes = malloc(sizeof(unsigned long));
	printf("Size of g: %lu\n", g->n);
	//Pour chaque noeud
	int nb_co_components = 1;
	for (unsigned long s=0; s<g->n; s++){
		//Si le noeud n'a jamais été visité, initialiser une pile avec le noeud
		if (marks[s] == 0){
			int curr_nb_components = 1;
			//printf("Bfs from node %lu\n", s);
			File* bfsFile = initialiser();
			enfiler(bfsFile, s);
			marks[s] = 1;
			if (nb_co_components==1){
				c->csizes[0] = 1;
				c->components[0] = s;
			}
			else{
				//printf("Nb connected components: %u\n", nb_co_components);
				c->csizes = realloc(c->csizes, nb_co_components*sizeof(unsigned long));
				c->csizes[nb_co_components-1] = c->csizes[nb_co_components-2] + 1;
				c->components[c->csizes[nb_co_components-2]] = s;
			}
			while (bfsFile->size > 0){
				int u = defiler(bfsFile);
				for (unsigned long v = g->cd[u]; v < g->cd[u+1]; v++){
					if (marks[g->adj[v]] == 0){
						enfiler(bfsFile, g->adj[v]);
						marks[g->adj[v]] = 1;
						curr_nb_components++;
						if (nb_co_components == 1){
							c->components[curr_nb_components - 1] = g->adj[v];
						}
						else{
							c->components[c->csizes[nb_co_components-2] + curr_nb_components - 1]  = g->adj[v];
						}
						c->csizes[nb_co_components-1]++;
					}
				}
			}
			nb_co_components++;
		}
	}
	nb_co_components--;
	printf("Number of connected components: %u\n", nb_co_components);
	for (int k = 0; k<nb_co_components+1; k++){
		c->csizes[nb_co_components - k] = c->csizes[nb_co_components - k - 1];
	}
	c->csizes[0] = 0;
	for (int j = 0; j<nb_co_components; j++){
		printf("Cumulative size of connected components: %lu\n", c->csizes[j]);
		printf("Root node of component number %u: %lu\n", j + 1, c->components[c->csizes[j]]);
	}
	/*printf("Connected components:\n");
	for (int l = 0; l<g->n; l++){
		printf("%lu ", c->components[l]);
	}*/
	return c;
}
//Computing diameter of graph

int diameter(adjarray *g){
	int diameter = 0;
	long *pred = calloc(g->n, sizeof(long));
	int marked;
	//FOr every node s, run BFS and store all predecessors.
	unsigned long seuil = 0;
	for (unsigned long s=0; s<g->n; s++){
		marked = 0;
		int* marks = calloc(g->n, sizeof(int));
		for (int i = 0; i < g->n; i++){
			pred[i] = -1;
		}
		File* bfsFile = initialiser();
		enfiler(bfsFile, s);
		printf("Marking node %lu\n", s);
		marks[s] = 1;
		marked++;
		printf("Computing predecessors list from node %lu ...\n", s);
		while (bfsFile->size > 0 && (double)marked/(double)g->n * 100 <= 100.0){
			int u = defiler(bfsFile);
			for (unsigned long v = g->cd[u]; v < g->cd[u+1]; v++){
				if (marks[g->adj[v]] == 0){
					if (marked % 10000 == 0){printf("\rNodes marked : %f %%\n", (double)marked/(double)g->n * 100);};
					enfiler(bfsFile, g->adj[v]);
					marks[g->adj[v]] = 1;
					marked++;
					pred[g->adj[v]] = u;
				}
			}
		}
		int shortest_path = 0;
		long backtrack;
		for (int j = 0; j<g->n; j++){
			backtrack = pred[j];
			int local_shortest_path = g->n;
			if (backtrack != -1){
				local_shortest_path = 1;
				while (backtrack != s){
					local_shortest_path++;
					backtrack = pred[backtrack];
				}
				if (local_shortest_path >= shortest_path){
					shortest_path = local_shortest_path;
				}
			}
		}
		if (shortest_path >= diameter){
		 	diameter = shortest_path;
		}
		if (s % 1 == 0){
			printf("Current best diameter: %u\n", diameter);
		}
	}
	printf("Diameter of graph of size %lu is %u\n", g->n, diameter);
	return diameter;
}

unsigned *bfs(adjarray *g, unsigned long u) {
	unsigned long n=g->n,l=1,i,j,v;

	static unsigned *dist=NULL,*list=NULL;

	if (dist==NULL){
		dist=malloc(n*sizeof(unsigned));
		list=malloc(n*sizeof(unsigned));//this will not be freed
	}

	for (i=0;i<n;i++) {
		dist[i]=-1;
	}

	list[0]=u;

	dist[u]=0;

	for (i=0;i<l;i++) {
		v=list[i];
		for (j=g->cd[v];j<g->cd[v+1];j++) {
			if (dist[g->adj[j]]==-1) {
				list[l++]=g->adj[j];
				dist[g->adj[j]]=dist[v]+1;
			}
		}
	}

	return dist;
}

void lower_bound(adjarray *g){
	unsigned *dist;
	unsigned dmax=0;
	bool *chosen;
	unsigned long i,u=0;

	chosen=calloc(g->n, sizeof(bool));
	for (i=0;i<g->n;i++) {
		if (chosen[u]==0)
			i--;
		else {
			u=i;
			if (chosen[u]==1)
				continue;
		}
		chosen[u]=1;
		printf("Computing distances from node %lu\n",u);
		dist=bfs(g,u);

		u=maxvect(g->n,dist);
		dmax=(dmax>dist[u])?dmax:dist[u];
		printf("Lower bound on diameter = %u\n",dmax);
	}
	printf("Diameter = %u\n",dmax);
}

//Main script
int main(int argc,char** argv){
	adjarray* a;
	time_t t1,t2;
	co_components *f;
	int d;

	t1=time(NULL);
	// fonction qui supprime les trous dans la numerotation
	renumber(argv[1], argv[2]);
	printf("Renumbered graph\n");
	// fonction qui supprime les duplicats
	// et les self loops
	cleangraph(argv[2], argv[3]);
	printf("Cleaned graph\n");

	unsigned long n = countgraph(argv[3]);
	printf("Count OK\n");
  unsigned long *degrees = write_degrees(argv[3], n);
	printf("Degrees OK\n");

	a=create_adjarray(argv[3], degrees, n);

	printf("Finished preliminary computations\n");
	//f=find_co_components(a);
	t1=time(NULL);
	d = diameter(a);

	//lower_bound(a);
	t2=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

	return 0;
}
