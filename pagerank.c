#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time
#include "utils.c"

//Exercise 1 - PageRank (directed graph)

unsigned long* write_degrees_out(char* input, unsigned long n){
	FILE *file=fopen(input,"r");
	unsigned long s,t;
	unsigned long *degrees_out= calloc(n,sizeof(unsigned long));
	while (fscanf(file,"%lu %lu", &s, &t)==2) {
		if(s!=t){
        degrees_out[s]++;
      }
	}
	fclose(file);
	// writing list to file
	FILE *fp = fopen("degrees_out.txt","w");
	for(int i = 0; i < n;i++){
		fprintf (fp, "%u %lu\n", i, degrees_out[i]);
	}
	fclose (fp);
	return degrees_out;
}

unsigned long* write_degrees_in(char* input, unsigned long n){
	FILE *file=fopen(input,"r");
	unsigned long s,t;
	unsigned long *degrees_in= calloc(n,sizeof(unsigned long));
	while (fscanf(file,"%lu %lu", &s, &t)==2) {
		if(s!=t){
        degrees_in[t]++;
      }
	}
	fclose(file);
	// writing list to file
	FILE *fp = fopen("degrees_in.txt","w");
	for(int i = 0; i < n;i++){
		fprintf (fp, "%u %lu\n", i, degrees_in[i]);
	}
	fclose(fp);
	return degrees_in;
}

// Matrix T (text list of edges format) and Vector P multiplication
double *matvectprod(char *input, double *P, unsigned long n, unsigned long *degrees_out){
  double *B = calloc(n, sizeof(double));
  unsigned long t,s;  //Initialization of vertices
  FILE* file=fopen(input,"r");
  while (fscanf(file,"%lu %lu", &s, &t)==2){
    if (degrees_out[s] > 0){
      B[t] += 1/(double)degrees_out[s] * P[s];
    }
    else{
      B[t] += 1/(double)n * P[s];
    }
  }
  return B;
}

double *pagerank(char *input, double alpha, int t){
  unsigned long n = countgraph(input);
	printf("Size of graph: %lu\n", n);
  double *P = calloc(n, sizeof(double));
	double s;
	//Random Initialization of the pagerank vector
  for (int i = 0; i < n; i++){
    P[i] = 1./(double)n;
  }
	unsigned long *degrees_in = write_degrees_in(input,n);
	unsigned long *degrees_out = write_degrees_out(input,n);
  for (int j = 0; j < t; j++){
		s = 0;
		printf("Computing pagerank for t = %u\n", j);
		//printf("Starting vectorial product\n");
    P = matvectprod(input,P,n,degrees_out);
		//printf("Updating P\n");
    for (int k = 0; k < n; k++){
      P[k] = (1-alpha)*P[k] + alpha/(double)n;
			s += P[k];
    }
		printf("%le\n",s);
		s = (1-s)/((double)n);
		for (int l=0; l<n;l++){
			P[l]+=s;
		}
  }
	FILE *fp = fopen("results/pagerank09.txt","w");
	for(int l = 0; l < n;l++){
		fprintf (fp, "%u %e\n", l, P[l]);
	}
	fclose (fp);
  return P;
}

int main(int argc,char** argv){
  time_t t1,t2;
	t1=time(NULL);

  double alpha = 0.15;
  int t = 20;
  double *PR;
  //unsigned long n = countgraph(argv[1]);
  //printf("Size of graph: %lu\n", n);
  PR = pagerank(argv[1], alpha, t);

	t2=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

	return 0;
}
