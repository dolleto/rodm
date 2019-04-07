#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "utils.c"

//Exercise 5 - A special quantity
//computing the quantity from file
unsigned long quantity(char *input, unsigned long *degrees, unsigned long n){
	unsigned long s,t;
	unsigned long q =0;
	FILE *file=fopen(input,"r");
    while (fscanf(file,"%lu %lu", &s, &t)==2) {
			q += degrees[s] * degrees[t];
		}
	printf("Quantity of graph is equal to: %lu\n", q);
	return q;
}

//Exercise 6 - Degree distribution
//writing the degree distribution from file
unsigned long* degree_distrib(unsigned long *degrees, unsigned long n){
	unsigned long *distribution= calloc(n,sizeof(unsigned long));
	for (int i=0; i < n; i++) {
		distribution[degrees[i]]++;
	}

	FILE *fp = fopen("results/distribution.txt","w");
	for(int j = 0; j < n; j++){
		fprintf (fp, "%u %lu\n", j, distribution[j]);
	}
	fclose (fp);

	return distribution;
}

// "plot "distribution.txt" using 1:2 with lines" in gnuplot to plot the distribution

int main(int argc,char** argv){
	time_t t1,t2,t3,t4;
	edgelist *e;
	unsigned long *m;
	adjarray *a;
	// a ajouter : fonction qui supprime les trous dans la numerotation
	renumber(argv[1], argv[2]);
	// fonction qui supprime les duplica&ts
	// et les self loops
	cleangraph(argv[2], argv[3]);

	unsigned long n = countgraph(argv[3]);
	unsigned long *degrees = write_degrees(argv[3], n);

	printf("Finished preliminary steps, number of nodes: %lu\n", n);
	t1=time(NULL);

  /*unsigned long n = countgraph(argv[1]);
  unsigned long *degrees = write_degrees(argv[1], n);

  unsigned long q = quantity(argv[1], degrees, n);
  unsigned long *distrib = degree_distrib(degrees, n);*/


	e = readedgelist(argv[3]);
	printf("Stored graph as edge list, graph has %lu nodes\n", e->n);
	free_edgelist(e);

	t2=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

	m = build_matrix(argv[3], n);
	printf("Stored graph as adjacency matrix\n");
	free(m);

	t3=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t3-t2)/3600,((t3-t2)%3600)/60,((t3-t2)%60));

	a = create_adjarray(argv[3], degrees, n);
	printf("Stored graph as adjacency array\n");
	free_adjarray(a);

	t4=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t4-t2)/3600,((t4-t2)%3600)/60,((t4-t2)%60));


	return 0;
}
