#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char **argv){
    double t;
    int p, id;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(id == 0){
        int input1 = 10000, input2 = 1000;
        int output, output1, output2, recvd=0;
        t = MPI_Wtime();
        MPI_Send(&input1, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        MPI_Send(&input2, 1, MPI_INT, 2, 2, MPI_COMM_WORLD);
        while(recvd<2){
            int source;
            MPI_Status status;
            MPI_Recv(&output,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            source = status.MPI_SOURCE;
            if(source==1){
                output1 = output;
                printf("Process 1 returned %d in time %f\n",output1,MPI_Wtime()-t);
            }
            if(source==2){
                output2 = output;
                 printf("Process 2 returned %d in time %f\n",output2,MPI_Wtime()-t);
            }
            recvd++;
        }

    }
    if(id == 1){
        int input, output, i, j;
        MPI_Status status;
        MPI_Recv(&input,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
        for(i=0;i<input;i++){
            for(j=0;j<input;j++){
                output += 0;
            }
        }
        MPI_Send(&output,1,MPI_INT,0,3,MPI_COMM_WORLD);
    }
    if(id == 2){
        int input, output, i, j;
        MPI_Status status;
        MPI_Recv(&input,1,MPI_INT,0,2,MPI_COMM_WORLD,&status);
        for(i=0;i<input;i++){
            for(j=0;j<input;j++){
                output += 0;
            }
        }
        MPI_Send(&output,1,MPI_INT,0,4,MPI_COMM_WORLD);
    }

    MPI_Finalize();
    
    if(id==0){
        printf("Successful Execution\n");
        printf("Total time taken %f\n",MPI_Wtime() - t);
    }
}