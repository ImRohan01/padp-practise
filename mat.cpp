#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

int main(){
	int it=1;
	while(it<=4){
		int r = it*500, c = it*500;
		int **A = (int **)malloc(r*sizeof(int *));
		for(int i=0;i<r;i++){
                        A[i] = (int *)malloc(c*sizeof(int));
                }
		int **B = (int **)malloc(r*sizeof(int *)); 
	        for(int i=0;i<r;i++){
                        B[i] = (int *)malloc(c*sizeof(int));                        
		}
		int **C = (int **)malloc(r*sizeof(int *));
		for(int i=0;i<r;i++){
                        C[i] = (int *)malloc(c*sizeof(int));
                }
		
		printf("%d %d %d", it, r, c);
		for(int i=0;i<r;i++){
			for(int j=0; j<c; j++){
				A[i][j] = rand()/r;
				B[i][j] = rand()/r;
			}

		}
		double  s = omp_get_wtime();
		for(int i=0;i<r;i++){
			for(int j=0;j<c;j++){
				for(int k=0;k<r;k++){
					C[i][j] += A[i][k] + B[k][j];
				}
			}
		}
		printf("Time taken for sequential: %f\n", omp_get_wtime() - s);
		for(int t=2; t<=8; t*=2){
			double  s = omp_get_wtime();
			omp_set_num_threads(t);
			int j,k;
			#pragma omp parallel for private(j,k)
                	for(int i=0;i<r;i++){
                        	for(j=0;j<c;j++){
                                	C[i][j] = 0;
                                	for(k=0;k<r;k++){
                                        	C[i][j] += A[i][k] + B[k][j];
                               		 }
                       		 }
               		 }
               		 printf("Time taken for %d threads: %f\n", t, omp_get_wtime() - s);
			
		}
		free(A);
		free(B);
		free(C);
		it++;
	}
	return 0;
}
