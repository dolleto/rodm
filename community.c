#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time
#include <stdbool.h>
#include "utils.c"


void generate_graph(int n, int nb_clusters, double p, double q, char *output){
  FILE *fp = fopen(output,"w");
  int *cl = calloc(n, sizeof(int));
  for (int i =0; i < n; i++){
    cl[i] = rand() % nb_clusters;
  }
  for (int i = 0; i < n - 1; i++){
    for (int j = i + 1; j < n; j++){
      if (cl[i] == cl[j]){
        if (rand()/(double)RAND_MAX < p){
          fprintf (fp, "%u %u\n", i, j);
          fprintf (fp, "%u %u\n", j, i);
        }
      }
      else {
        if (rand()/(double)RAND_MAX < q){
          fprintf (fp, "%u %u\n", i, j);
          fprintf (fp, "%u %u\n", j, i);
        }
      }
    }
  }
  fclose (fp);
}

int main(int argc,char** argv){
  time_t t1,t2;
	t1=time(NULL);
  int n = 400;
  int nb_clusters = 4;
  double p = 0.6;
  double q = 0.4;
  char *output = "data/random_graph.txt";

  generate_graph(n, nb_clusters, p, q, output);

	t2=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

	return 0;
}
