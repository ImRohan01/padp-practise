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
    int n = 10;
    int size = 100;
    int i,j;
    int *x;
    MPI_Status status;
    for(i=1;i<=4;i++){
        x = (int *)malloc(i*sizeof(int));
        for(j=0;j<i;j++){
            x[j] = 1;
        }
        if(id == 0){
            // double t = MPI_Wtime();
            for(j=1;j<n;j++){
                MPI_Send(x,i,MPI_INT,1,0,MPI_COMM_WORLD);
                MPI_Recv(x,i,MPI_INT,p-1,0,MPI_COMM_WORLD,&status);
            }
            // printf("%f",MPI_Wtime()-t);
        }
        else{
            for(j=1;j<n;j++){
                MPI_Recv(x,i,MPI_INT,id-1,0,MPI_COMM_WORLD,&status);
                MPI_Send(x,i,MPI_INT,(id+1)%p,0,MPI_COMM_WORLD);
            }
        }
        free(x);
    }
}