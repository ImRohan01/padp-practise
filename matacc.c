#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#define MAX 1000

int SIZE;

double a[MAX][MAX];
double b[MAX][MAX];
double c[MAX][MAX];
double d[MAX][MAX];

int main(int argc, char **argv){
    int i,j,k;
    double tmp = 0.0;
    SIZE = 100;
    for(i=0;i<SIZE;i++)
        for(j=0;j<SIZE;j++){
            a[i][j] = (double)(i+j);
            b[i][j] = (double)(i-j);
            c[i][j] = 0.0;
            d[i][j] = 0.0;
        }
    //exit(0);
    double t = omp_get_wtime();
    for(i=0;i<SIZE;i++)
        for(j=0;j<SIZE;j++){
            tmp = 0.0;
            for(k=0;k<SIZE;k++)
                tmp += a[i][k] + b[k][j];
            d[i][j] = tmp;
        }
    printf("SEQUENTIAL %f", omp_get_wtime() - t);
    #pragma acc data copyin(a,b) copy(c)
    #pragma acc kernels
    #pragma acc loop tile(32,32)
    t = omp_get_wtime();
    for(i=0;i<SIZE;i++)
        for(j=0;j<SIZE;j++){
            tmp = 0.0;
            #pragma acc reduction(+:tmp)
            for(k=0;k<SIZE;k++)
                tmp += a[i][k] + b[k][j];
            c[i][j] = tmp;
        }
    printf("PARALLEL %f", omp_get_wtime() - t);
    for(i=0;i<SIZE;i++){
        if(d[i][j]!=c[i][j]){
            printf("AIYO\n");
        }
    }
    printf("SUCCESS!");
}