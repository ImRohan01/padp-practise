#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#define SEED 123456

int main(){
	int n, count=0, i, t;
	printf("Enter the number of iterations\n");
	scanf("%d", &n);
	srand(SEED);
	for(int t=1; t<=8; t++){
		double x,y,z;
		count = 0;
		omp_set_num_threads(t);
		double s = omp_get_wtime();
	        #pragma omp parallel for private(x,y,z) reduction(+: count)
	        for(i=0;i<n;i++){
			x = (double)rand()/RAND_MAX;
			y = (double)rand()/RAND_MAX;
			z = x*x + y*y;
			if(z<=1) count++;
		}
		printf("%d\n",count);
		double pi = (double)count/n*4;
		printf("%f\n", omp_get_wtime() - s);
		printf("%f\n", pi);
	}	
	return 0;	
}
