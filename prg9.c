#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

void ringio(int , int);

int main(int argc, char **argv){
    int    p, id;
    double time;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if(id == 0){
        time = MPI_Wtime();
        printf("LE RINGIO\n"); 
    }
    ringio(p, id);
    MPI_Finalize();
    if(id == 0){
        printf("%f\n",MPI_Wtime() - time);
        printf("Normal Termination\n");
    }
    return 0;
}

void ringio(int p, int id){
    int i;
    int sizes[5] = {10,100,10,100,1000};
    int iter = 0;
    int tests = 5;
    int j;
    int *x;
    MPI_Status status;
    for(iter=0;iter<5;iter++){
        int n = sizes[iter];
        x = (int *)malloc(n*sizeof(int));
        for(j=0;j<n;j++){
            x[j] = 1;
        }
        if(id == 0){
            for(i=1;i<tests;i++){
                MPI_Send(x,n,MPI_INT,1,1,MPI_COMM_WORLD);
                MPI_Recv(x,n,MPI_INT,p-1,1,MPI_COMM_WORLD,&status);
            }
        }
        else{
            for(i=1;i<tests;i++){
                MPI_Recv(x,n,MPI_INT,id-1,1,MPI_COMM_WORLD,&status);
                MPI_Send(x,n,MPI_INT,(id+1)%p,1,MPI_COMM_WORLD);
            }
        }
        free(x);
    }
}