#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>

#define CLUSTER_SIZE 4
#define POINTS_SIZE 100000

int cluster[CLUSTER_SIZE][2] = {{25,25}, {25,75}, {75,25}, {75,75}};
int points[POINTS_SIZE][2];
long long cluster_count[CLUSTER_SIZE];

void populate_points(){
    long long i;
    
    for (i=0;i<CLUSTER_SIZE;i++){
        cluster_count[i] = 0;
    }
    for(i=0;i<POINTS_SIZE;i++){
        srand(i);
        points[i][0] = rand()%100;
        points[i][1] = rand()%100;
    }
}

double get_distance(int x1, int y1, int x2, int y2){
    int x = x2-x1, y = y2-y1;
    return (double)sqrt(x*x + y*y);
}

int main(int argc, char **argv){
    double t ;
    populate_points();
    long long i;
    int th = 0;
    printf("Enter the number of threads\n");
    scanf("%d",&th);
    t = omp_get_wtime();
    #pragma omp parallel for private(i) shared(cluster, points) reduction(+:cluster_count) num_threads(th)
    for(i=0;i<POINTS_SIZE;i++){
        int index = -1;
        double min_dist = 999;
        double curr_dist = -1;
        int j;
        for(j=0;j<CLUSTER_SIZE;j++){
            curr_dist = get_distance(points[i][0],points[i][1],cluster[j][0], cluster[j][1]);
            if(curr_dist<min_dist){
                min_dist = curr_dist;
                index = j;
            }
        }
        cluster_count[index]++;
    }
    for(i=0;i<CLUSTER_SIZE;i++){
        printf("%lld\n",cluster_count[i]);
    }
    printf("%f",omp_get_wtime() - t);
}