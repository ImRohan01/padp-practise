#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char **argv){
    int id, p;
    double t, pi;
    int count = 0;
    int total_count = 0;
    int n = 10000;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    if(id == 0){
        t = MPI_Wtime();
        printf("Estimating PI\n");
    }
    else{
        double x,y,z;
        int i;
        count = 0;
        srand(1234);
        for(i=0;i<n;i++){
            x = (double)rand()/RAND_MAX;
            y = (double)rand()/RAND_MAX;
            z = x*x + y*y;
            // if(z>1)
            // printf("%f",z);
            if(z<1) count++;
        }
    }
    MPI_Reduce(&count,&total_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    if(id==0){
        printf("%d",total_count);
        printf("PI Value %f\n",((double)(total_count)/((p-1)*n)*4));
        printf("Total Time %f\n",MPI_Wtime()-t);
    }
    MPI_Finalize();
}