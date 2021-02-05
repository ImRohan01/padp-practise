#include<stdio.h>
#include<string.h>
#include<omp.h>
#include<stdlib.h>
#include<ctype.h>

#define WORDS 5
char search[WORDS][20] = {"The", "man", "from", "India", "hello"};

void read_word(char *temp, FILE *fp){
    int i=0;
    char ch;
    while((ch = fgetc(fp))!=EOF && isalpha(ch)==0){

    }
    while((ch!=EOF) && isalpha(ch)!=0){
        // printf("%c",ch);
        temp[i++] = ch;
        ch = fgetc(fp);
    }
    temp[i] = '\0';
}

int get_count(char *filename, char *key){
    printf("%s",filename);
    FILE *fp = fopen(filename,"r");
    int count = 0;
    char temp[20];
    while(feof(fp) == 0){
        printf("Hi");
        read_word(temp,fp);
        if(strcasecmp(temp,key) == 0){
            count++;
        }
    }
    fclose(fp);
    return count;
}

int main(int argc, char **argv){
    char filename[20] = "file2.txt";
    int i;
    int count[WORDS];
    omp_set_num_threads(2);
    #pragma omp parallel for
    for(i=0;i<WORDS;i++){
        count[i] = get_count(filename, search[i]);
    }
    for(i=0;i<WORDS;i++){
        printf("%d ",count[i]);
    }
}