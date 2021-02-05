#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<ctype.h>
#include<string.h>

#define COUNT 10
char search_words[COUNT][20] = {"The","around","graphics","from","by","be","a","which","various","mount"};

void read_word(char *temp, FILE *fp){
    int i = 0;
    char ch;
    while((ch = fgetc(fp)) != EOF && isalpha(ch) == 0){

    }
    while((ch != EOF) && isalpha(ch) != 0){
        temp[i++] = ch;
        ch = fgetc(fp);
    }
    temp[i] = '\0';
    // printf("%s",temp);
}

int get_count(char *filename, char *key){
    int word_count = 0;
    FILE *fp = fopen(filename, "r");
    char temp[40];
    while(feof(fp) == 0){
        // printf("H1");
        read_word(temp, fp);
        // printf("%s",temp);
        if(strcasecmp(key,temp) == 0){
            word_count++;
        }
    }
    fclose(fp);
    return word_count;
}

int main(int argc, char **argv){
    char filename[20] = "file2.txt";
    int i;
    double t = omp_get_wtime();
    omp_set_num_threads(4);
    int count[10];
    #pragma omp parallel for
    for(i=0;i<10;i++){
        count[i] = get_count(filename, search_words[i]);
    }
    for(i=0;i<10;i++){
        printf("%d",count[i]);
    }
    printf("%f\n",omp_get_wtime() - t);
}