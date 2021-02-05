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
        double t = MPI_Wtime();
        int input1 = 10000;
        int input2 = 10000;
        int output, output1, output2;
        int recvd = 0;
        MPI_Send(&input1,1,MPI_INT,1,1,MPI_COMM_WORLD);
        MPI_Send(&input2,1,MPI_INT,2,2,MPI_COMM_WORLD);
        MPI_Status status;
        while(recvd<2){
            MPI_Recv(&output, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            int source = status.MPI_SOURCE;
            if(source == 1){
                output1 = output;
            }
            if(source == 2){
                output2 = output;
            }
            recvd++;
        }
        printf("output1 %d output2 %d \n",output1, output2);
        printf("Process 0 took %f\n",MPI_Wtime() - t);
        MPI_Finalize();
        printf("Successful end of execution\n");
    }
    else if(id == 1){
        int output,input;
        int i,j, count;
        double time = MPI_Wtime();
        MPI_Status status;
        MPI_Recv(&input,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
        printf("%d\n",input);
        for(i=0;i<input;i++){
            for(j=0;j<input;j++){
                count = (count + 1)%1000;
            }
        }
        output = count;
        printf("%d\n",output);
        MPI_Send(&output,1,MPI_INT,0,3,MPI_COMM_WORLD);
        printf("Process 1 took %f\n", MPI_Wtime() - time);
        MPI_Finalize();
    }
    else if(id == 2){
        int output,input;
        int i,j, count;
        double time = MPI_Wtime();
        MPI_Status status;
        MPI_Recv(&input,1,MPI_INT,0,2,MPI_COMM_WORLD,&status);
        printf("%d\n",input);
        for(i=0;i<input;i++){
            for(j=0;j<input;j++){
                count = (count + 1)%1000;
            }
        }
        output = count;

        printf("%d\n",output);
        MPI_Send(&output,1,MPI_INT,0,4,MPI_COMM_WORLD);
        printf("Process 2 took %f\n", MPI_Wtime() - time);
        MPI_Finalize();
    }
    return 0;
}