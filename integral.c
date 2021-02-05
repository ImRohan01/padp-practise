#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>

double f(double x){
    double pi = 3.14;
    return (double)(50.0/(pi*(2500*x*x+1)));
}

int main(int argc, char **argv){
    double exact = 0.49936;
    int n = 999999;
    int i;
    int id, p, a=0, b=10, my_n;
    double my_total=0, total=0;
    double my_a, my_b;
    double t = MPI_Wtime();
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    double x = 10.0/(p-1);

    if(id == 0){
        printf("Computing the value of integral f(x)\n");
        my_n = n/p;
    }
    MPI_Bcast(&my_n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // printf("Hi");
    if(id == 0){
        for(i=1;i<=p-1;i++){
            my_a = x*i - x;
            my_b = x*i;
            MPI_Send(&my_a, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
            MPI_Send(&my_b, 1, MPI_DOUBLE, i, 2, MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Status status;
        MPI_Recv(&my_a, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&my_b, 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &status);
        printf("%d %f %f",id,my_a,my_b);
        for(i=0;i<my_n;i++){
        double x = ((my_n - i)*my_a + (i-1)*my_b )/ (my_n - 1);
        my_total += f(x);
        }
        my_total = (my_b-my_a)*my_total/(double)my_n;
    }

    MPI_Reduce(&my_total, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(id == 0){
        printf("The error is %f\n", exact - total);
        printf("%f",MPI_Wtime() - t);
        printf("Success!\n");

    }
    MPI_Finalize();
    return 0;
}
