#include<stdio.h>
#include<omp.h>
#include<math.h>
#include<gd.h>

int main(int argc, char **argv){
    FILE *fp1, *fp2;
    gdImagePtr img;
    char iname[15], oname[15];
    int red, green, blue, tid, tmp, color, x, y, w, h;
    omp_sched_t def_sched;
    int def_chunk_size;
    omp_get_schedule(&def_sched, &def_chunk_size);
    for(int i=1; i<=4; i++){
        sprintf(iname,"in%d.png",i);
        fp1 = fopen(iname,"r");
        img = gdImageCreateFromPng(fp1);
        w = gdImageSX(img);
        h = gdImageSY(img);
        for(int sched = 0x0; sched <= 0x3; sched++){
            sprintf(oname,"out%d%d.png",i,sched);
            if(sched == 0x0){
                omp_set_schedule(def_sched, def_chunk_size);
            }
            else{
                omp_set_schedule(sched, 1);
            }
            double t = omp_get_wtime();
            #pragma omp parallel for private(y,red,green,blue,color,tmp,tid)
            for(x=0;x<w;x++){
                for(y=0;y<h;y++){
                    tid = omp_get_thread_num();
                    color = gdImageGetPixel(img,x,y);
                    red   = gdImageRed(img,color);
                    blue  = gdImageBlue(img,color);
                    green = gdImageRed(img,color);
                    tmp = (red+green+blue)/3;
                    red=green=blue=tmp;
                    color = gdImageColorAllocate(img,red,green,blue);
                    gdImageSetPixel(img,x,y,color);
                }
            }
            fp2 = fopen(oname,"w");
            printf("Hello");
            gdImagePng(img,fp2);
            fclose(fp2);
           
            printf("%f",omp_get_wtime() - t);
        }
         gdImageDestroy(img);
    }
    return 0;
}