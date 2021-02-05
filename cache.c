#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>

int res = 0;

int min(int a, int b){
    return a<b?a:b;
}

int Striker(int composite[],int start, int stride, int limit){
    int i = start;
    for(;i<=limit;i+=stride){
        if(!composite[i])
            composite[i] = 1;
    }
    return i;
}

void CacheUnfriendly(int composite[], int n){
    int m = (int)sqrt(n);
    printf("%d\n",m);
    int i;
    int count = 0;
    double t = omp_get_wtime();
    for(i=2;i<=m;i++){
        if(!composite[i]){
            count++;
            Striker(composite, 2*i, i, n);
        }
    }
    for(i=m+1;i<=n;i++){
        if(!composite[i]){
            count++;
            Striker(composite, 2*i, i, n);
        }
    }
    res = count;
    printf("Cache Unfriendly: %d\n",count);
    printf("Time: %f\n",omp_get_wtime()-t);
}

void CacheFriendly(int composite[], int n){
    int m = (int)sqrt(n);
    int factor[n+1];
    int strike[n+1];
    int n_factor = 0;
    int i,k;
    int count = 0;
    double t = omp_get_wtime();
    for(i=2;i<=m;i++){
        if(!composite[i]){
            count++;
            strike[n_factor] = Striker(composite, 2*i, i, m);
            factor[n_factor++] = i;
        }
    }
    for(i=m+1;i<=n;i+=m){
        int limit = min(i+m-1,n);
        for(k=0;k<n_factor;k++){
            strike[k] = Striker(composite, strike[k], factor[k],limit);
        }
        for(k=i;k<=limit;k++){
                if(!composite[k]){
                    count++;
                }
            }
    }
    printf("Cache Friendly: %d\n",count);
    printf("Time: %f\n",omp_get_wtime()-t);
}

void Parallel(int composite[], int n){
    int m = (int)sqrt(n);
    int factor[n];
    int strike[n];
    int n_factor = 0;
    int i,k;
    int count = 0;
    double t = omp_get_wtime();
    {
        #pragma omp parallel for num_threads(4)
        for(i=2;i<=m;i++){
            if(!composite[i]){
                count++;
                strike[n_factor] = Striker(composite, 2*i, i, m);
                factor[n_factor++] = i;
            }
        }
    }
    for(i=m+1;i<=n;i+=m){
        int limit = min(i+m-1,n);
        {
        for(k=0;k<n_factor;k++){
            strike[k] = Striker(composite, strike[k], factor[k],limit);
        }
        for(k=i;k<=limit;k++){
                if(!composite[k]){
                    count++;
                }
            }
        }
    }
    printf("Parallel: %d\n",count);
    printf("Time: %f\n",omp_get_wtime()-t);
}

int main(int argc, char **argv){
    int n = 100000;
    int composite[n+1];
    int i;
    for(i=0;i<n+1;i++){
        composite[i] = 0;
    }
    CacheUnfriendly(composite, n);
    for(i=0;i<n+1;i++){
        composite[i] = 0;
    }
    CacheFriendly(composite, n);
    for(i=0;i<n+1;i++){
        composite[i] = 0;
    }
    Parallel(composite, n);
}