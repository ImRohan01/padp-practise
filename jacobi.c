#include<stdio.h>
#include<stdlib.h>
#include<openacc.h>
#include<omp.h>
#include<math.h>

#define N 1024
float A[N][N];
float Anew[N][N];

int main(int argc, char **argv){
    double tol = 1.0e-6;
    double error = 1;
    int iter = 0;
    int max_iter = 1000;
    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            A[i][j] = 0.0;
            Anew[i][j] = 0.0;
            if(j==0){
                A[i][j] = 1.0;
                Anew[i][j] = 1.0;
            }
        }
    }
    double t = omp_get_wtime();
    #pragma acc data copy(A,Anew)
    while(error>tol && iter<max_iter){
        error = 0;
        #pragma acc parallel loop reduction(max:error)
        for(i=1;i<N-1;i++){
            for(j=1;j<N-1;j++){
                Anew[i][j] = 0.25*(A[i][j-1],A[i][j+1], A[i+1][j], A[i-1][j]);
            }
            error = fmax(error, fabs(Anew[i][j]-A[i][j]));
        }
        #pragma acc parallel loop
        for(i=1;i<N;i++){
            for(j=1;j<N;j++){
                A[i][j] = Anew[i][j];
            }
        }
    iter++;
    printf("%f",omp_get_wtime() - t);
    printf("SUCCESS!");
    }
}