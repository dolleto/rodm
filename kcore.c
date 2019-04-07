#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time
#include "utils.c"

//Exercise 3 - K-core

int *cores(adjarray *g){
	int d,md,i,j,start,num,v,u,w,du,pu,pw;
	md = 0;
	//Output: core number of each vertex
	int *deg = calloc(g->n, sizeof(int));
	//Set of vertices sorted by degree
	int *vert = calloc(g->n, sizeof(int));
	//For each degree, position of first vertex in array vert
	int *bin = calloc(g->n, sizeof(int));
	//Positions of vertices in vert
	int *pos = calloc(g->n, sizeof(int));
	printf("Computing kcore for graph of %lu nodes\n", g->n);

	//Computing the degree of each vertex and storing the maximum degree
	for (v = 0; v<g->n; v++){
		d = g->cd[v+1] - g->cd[v];
		deg[v] = d;
		//printf("Degree of node %u: %u\n", v, deg[v]);
		if (d > md){md = d;};
	}
	//Sorting the vertices in increasing degree order (bin sort)
	for (v=0; v<g->n; v++){bin[deg[v]]++;}
	start = 1;
	for (d=0; d < md; d++){
		num = bin[d];
		bin[d] = start;
		start += num;
	}
	for (v=0; v<g->n; v++){
		pos[v] = bin[deg[v]];
		vert[pos[v]] = v;
		bin[deg[v]]++;
	}
	//Recover the starting positions of the bins
	for (d=md; d>0; d--){
		bin[d] = bin[d-1];
	}
	bin[0] = 1;
	//Kcore algorithm
	for (i=0; i<g->n; i++){
		v = vert[i];
		//printf("Computing kcore for node %u of degree %u\n", v, deg[v]);
		for (j = g->cd[v]; j < g->cd[v+1]; j++){
			u = g->adj[j];
			//printf("Considering neighbour %u of degree %u\n", u, deg[u]);
			if (deg[u] > deg[v]){
				du = deg[u];
				pu = pos[u];
				pw = bin[du];
				w = vert[pw];
				if (u != w){
					pos[u] = pw;
					pos[w] = pu;
					vert[pu] = w;
					vert[pw] = u;
				}
				bin[du]++;
				deg[u]--;
			}
		}
	}
	return deg;
}

int main(int argc,char** argv){
  time_t t1,t2;
	adjarray *g;
	cleangraph(argv[1], argv[2]);
	renumber(argv[2], argv[3]);
	// fonction qui supprime les duplicats
	// et les self loops

	unsigned long n = countgraph(argv[3]);
  unsigned long *degrees = write_degrees(argv[3], n);

	g=create_adjarray(argv[3], degrees, n);

	t1=time(NULL);

	int *kcores = cores(g);
	int kcore = 0;
	FILE *fp = fopen("cores.txt","w");

	for (int i = 0; i<g->n; i++){
		//printf("Core number of node %u: %u\n", i, kcores[i]);
		fprintf (fp, "%u %u\n", i, kcores[i]);
		if (kcores[i] > kcore){
			kcore = kcores[i];
		}
	}
	fclose (fp);

	printf("Core value of the graph : %u\n", kcore);

	t2=time(NULL);


	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

	return 0;
}
