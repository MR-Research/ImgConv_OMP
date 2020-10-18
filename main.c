#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <regex.h>
#include <time.h>
#include <omp.h>
#include "stb_image.h"
#include "stb_image_write.h"

int main(int argc, char *argv[]){ // parameters: image path, filter path, resulting image path, filter size, thead count, scheduler type, chunk size
if (argc == 8){
    //Variable declaration
    FILE *pf;
	char *imgpath = argv[1];
	char *filterpath = argv[2];
	char *imgrespath = argv[3];
	char *filtsize = argv[4];
	char *threadcount = argv[5];
	char *scheduler = argv[6];
    char *chunksize = argv[7];
	
	//Time and date of the calculation
    time_t now;
    struct tm *local;
    int hours, minutes, seconds, day, month, year;
	
	int w, h, bpp, cc = 3, wc, hc, crop_img_size, j;
	double start, end, *tmp, *a = malloc(atoi(filtsize)*atoi(filtsize)*sizeof(double *));
    unsigned char *image, *gray_img, *new_img_gray;
    size_t img_size, gray_img_size, i;
    int gray_channels = 1;
    
    // Explicitly disable dynamic teams
    omp_set_dynamic(0);
    
    //Reading the filter from a file
    pf = fopen (filterpath, "r");
    if (pf == NULL){
        return 0;
    }   
    for(i = 0, tmp = a; i < atoi(filtsize)*atoi(filtsize); ++i){
            fscanf(pf, "%lf", tmp++);
    }
    fclose (pf); 
    //Image loading
	image = stbi_load(imgpath, &w, &h, &bpp, cc);
	img_size = w * h * cc;
	gray_img_size = w * h * gray_channels;
    wc = w - (atoi(filtsize)-1);
    hc = h - (atoi(filtsize)-1);     
    crop_img_size = wc* hc;
    //New image memory allocation
	gray_img = malloc(gray_img_size);
	new_img_gray = malloc(crop_img_size);
    //Grayscale conversion (serial execution)
	for(i = 0; i < gray_img_size; i++) {
        unsigned char *pt = image+(i*3);
        unsigned char *pgt = gray_img+i;            
		*pgt = (uint8_t)((*pt + *(pt + 1) + *(pt + 2))/3.0);
	}
    //Obtain current time for testbench reports
    time(&now);
    local = localtime(&now);
	hours = local->tm_hour;	  	// Hours since midnight (0-23)
	minutes = local->tm_min;	 	// Minutes passed after the hour (0-59)
	seconds = local->tm_sec;	 	// Seconds passed after minute (0-59)

	day = local->tm_mday;			// Day of month (1 to 31)
	month = local->tm_mon + 1;   	// Month of year (0 to 11)
	year = local->tm_year + 1900;	// Year since 1900

	// Measure initial time for parallel calculations
	start = omp_get_wtime();
	//Convolution in parallel
    //Configuring schedule type
        if (scheduler == "static")
            omp_set_schedule(omp_sched_static, atoi(chunksize));
        else if(scheduler == "dynamic")
            omp_set_schedule(omp_sched_dynamic, atoi(chunksize));
        else if (scheduler ==  "guided")
            omp_set_schedule(omp_sched_guided, atoi(chunksize));
        else if (scheduler = "auto")
            omp_set_schedule(omp_sched_auto, atoi(chunksize));
        
    #pragma omp parallel for shared(gray_img, new_img_gray, a, w, wc, crop_img_size) num_threads(atoi(threadcount))  
	for (j = 0; j < crop_img_size; j++){
        int filts = atoi(filtsize);          
        unsigned char *p = gray_img+j + ((int)(j/wc)*(filts-1));
        unsigned char *pg = new_img_gray+j;               
        unsigned char sum = 0;
        int tmpcont = 0;
        int at;      
        for (at = (filts*filts)-1; at >= 0; at--){
            sum += *(a+at) * *(p + tmpcont + (w*(filts-((at/filts)+1))));
            if (tmpcont == (filts-1)){
                tmpcont = 0;
            } else {
                tmpcont++;
            }
        }             
        *pg = (uint8_t)sum; 
	}	
	//Measure final time of parallel convolution
	end = omp_get_wtime();
    //Write convolution result in file
	stbi_write_jpg(imgrespath, wc, hc, gray_channels, new_img_gray, wc * gray_channels);
    //Free memory
	stbi_image_free(image); 
	free(gray_img);
	free(new_img_gray);
	//Print to stdout with csv format, for testbench
	printf("%02d-%02d-%d_%02d:%02d,ImgName:%s FiltType:%s FiltSize:%s ThreadCount:%s Scheduler:%s,%f,%s,%s,%s,%s\n", day, month, year, minutes, seconds, imgpath, filterpath, filtsize, threadcount, scheduler, end - start, filtsize, threadcount, scheduler, chunksize);
} else {
	printf("Incorrect number of arguments\n");
}
	return 0;
}
