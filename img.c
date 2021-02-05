#include<stdio.h>
#include<stdlib.h>
#include<gd.h>
#include<omp.h>

int main(int argc, char **argv){
    char iname[15], oname[15];
    omp_sched_t def_sched;
    int def_chunk_size;
    omp_get_schedule(&def_sched,&def_chunk_size);
    int i;
    for(i=1;i<=4;i++){
        sprintf(iname,"in%d.png",i);
        FILE *fp1, *fp2;
        gdImagePtr img;
        int sched,w,h,x,y,red,blue,green,color,tmp,tid;
        fp1 = fopen(iname, "r");
        img = gdImageCreateFromPng(fp1);
        w = gdImageSX(img);
        h = gdImageSY(img);
        for(sched=0x0;sched<=0x3;sched++){
            if(sched == 0x0) omp_set_schedule(def_sched,def_chunk_size);
            else omp_set_schedule(sched,1);
            sprintf(oname,"out%d%d.png",i,sched);
            #pragma omp parallel for private(y,red,green,blue,color,tmp,tid)
            for(x=0;x<w;x++){
                for(y=0;y<h;y++){
                    tid = omp_get_thread_num();
                    color = gdImageGetPixel(img,x,y);
                    red = gdImageRed(img,color);
                    green = gdImageBlue(img,color);
                    blue = gdImageGreen(img,color);
                    tmp = (red+blue+green)/3;
                    red=green=blue=tmp;
                    color = gdImageColorAllocate(img,red,green,blue);
                    gdImageSetPixel(img,x,y,color);
                }
            }
            fp2 = fopen(oname,"w");
            gdImagePng(img,fp2);
            fclose(fp2);
        }
        gdImageDestroy(img);
        fclose(fp1);
    }
}