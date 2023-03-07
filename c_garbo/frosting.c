// This is just experimenting with the model Yousef and Nalby thought about

// this macro is for the rand_s function
#define _CRT_RAND_S

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "libbmp.h"
// States of nodes
#define DRY 0
#define WET 1
#define FROZEN 2
#define STOPTEMP 1

// Available Humidity in Wet Pillars
static int AHUM = 0;

// Frosting Node -> Frode
typedef struct Frode{
    unsigned char humidity; // 255 is highest hummidty possible, 0 is lowest relative humidity
    char state;   // current state of node
    char friegh; // frozen horiz or vert neighbors -> freigh, max 4
    char diag; // diagonal frozen neighbors -> max 4
} Frode;

typedef struct Plate{
    Frode ** curr;
    Frode ** prev;
} Plate;

// prints given Frode
void prode(Plate plate, int i, int j){
    Frode temp = plate.curr[i][j];
    printf("Frode (%i,%i)\nHumid:\t%i\nState:\t%i\nFreis:\t%i\n\n", i, j, temp.humidity, temp.state, temp.friegh); 
}

// Produces random double from [0,max)
// Taken from https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/rand-s?view=msvc-170
double genprob(double max){
    unsigned int number;
    errno_t err;
    err = rand_s(&number);
    if (err != 0) {
        printf("The rand_s function failed!\n");
    }
     return (double) number /((double) UINT_MAX + 1) * max;
}


// Allocates square plate of length len with an attribute humidity 
Frode** allocfrodes(int len, unsigned char hum){
    Frode **t;

    t = (Frode **) malloc(sizeof(Frode *) * len);

    for (int i = 0; i < len; i++) {
        t[i] = (Frode *) malloc(sizeof(Frode) * len);
        for (int j = 0; j < len; j++) {
            t[i][j].humidity = hum;
            t[i][j].state = WET;
            t[i][j].friegh = 0;
            t[i][j].diag = 0;
        }
    }

    return t;    
}

// Allocates both plates needed
Plate * newplate(int len, int humidity){
    Plate * p = (Plate *) malloc(sizeof(Plate *));

    p->curr = allocfrodes(len, humidity);
    p->prev = allocfrodes(len, humidity);

    return p;
}

// frees a plate of frodes
void deallocfrodes(Frode ** f, int len){
    for (int i = 0; i < len; i++) {
        free(f[i]);
    }
    free(f);
}

// frees both frode plates and struct itself
void freeplate(Plate * p, int len){
    deallocfrodes(p->curr, len);
    deallocfrodes(p->prev, len);

    free(p);
}

// independent freezing of nodes
int indep_freeze(int temp){
    double p = 11.6379+temp*0.362419;
    double prob = 1/(1+exp(p));

    double chance = genprob(1.0);

    return (chance < prob);
}

// influenced freezing of nodes, runs on WET nodes
int flu_freeze(Frode node){

    // prob is reduced if there are multiple freighs (multiple ice bridges being made)
    // double prob = ((double)(node.humidity))/(255.0 * node.friegh);

    double weight = 1.0;

    // if(node.friegh) weight*=node.friegh;
    // if(node.diag) weight*=2 * node.diag;

    // Logictic curve prob
    double p = 5.60693+(node.humidity/255.0)*-23.4402;
    double prob = 1/((1+exp(p))*(node.friegh + node.diag));
    // // divide prob by frozen neighbors
    // prob /= node.friegh;
    // printf("Prob:\t%f\n", prob);


    double chance = genprob(1.0);
    // printf("Chance:\t%f\n", chance);

    return (chance < prob);
} 

void switchplates(Plate* plate){
    Frode ** temp = plate->curr;
    plate->curr = plate->prev;
    plate->prev = temp;
}

// add frozen neighbor to neighbors of recently frozen nodes
void addFreigh(Plate * plate, int i, int j, int len){
    for (int k  = i-1; k < i+2; k++){
        if (k<0 || k>=len) continue;

        for (int m = j-1; m < j+2; m++){
            if (m<0 || m>=len) continue;

            if(!(k==i && m==j)) {
                // Jerry rigged way to find diagonal neighbors
                if((k+m)%2 == 0)
                    plate->prev[k][m].diag++;
                else
                    plate->prev[k][m].friegh++;
            }
        }
    }
    
}

void freezing(Plate* plate, char temp, char humidity, int len, int iter){
    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            Frode cpy = plate->curr[i][j];
            if(cpy.state == WET){
                // remove humidity boolean (also acts as a "did node freeze?" bool)
                int remHum = 0;

                // ONLY WRITE TO PREV MATRIX, ONLY READ FROM CURR MATRIX
                if(iter < STOPTEMP && indep_freeze(temp)){
                    plate->prev[i][j].state = FROZEN;
                    addFreigh(plate, i, j, len);
                    remHum = 1;
                }

                if(!remHum && (cpy.friegh || cpy.diag)){
                    if(flu_freeze(cpy)){
                        plate->prev[i][j].state = FROZEN;
                        addFreigh(plate, i, j, len);
                    }else{
                        plate->prev[i][j].state = DRY;
                    }
                    remHum = 1;
                }

                if(remHum){
                    AHUM -= plate->curr[i][j].humidity;
                    plate->prev[i][j].humidity = 0;
                }else{
                    plate->prev[i][j].state = plate->curr[i][j].state;
                }
            }else{
                plate->prev[i][j].state = plate->curr[i][j].state;
            }
        }
    }

    switchplates(plate);
}


// Print current state of plate (curr, prev)
void prstate(Plate* plate, int len){
    for(int i = 0; i < len; i++){
        for(int j = 0; j < 2*len; j++){
            char c;

            if(j == len) printf(" ");
            Frode ** whichPlate = (j < len)?plate->curr:plate->prev;
            int m = (j < len)?j:j-len;

            switch (whichPlate[i][m].state)
            {
                case DRY:
                    c = 'X';
                    break;
                case WET:
                    c = '~';
                    break;
                case FROZEN:
                    c = '#';
                    break;
            }

            printf("%c", c);
        }
        printf("\n");
    }
    printf("\n");
}

void makebitmap(Plate * plate, int len, int iter, int pwid){
    
    struct RGB {
        int r;
        int g;
        int b;
    } color;

    bmp_img img;
    bmp_img_init_df (&img, pwid*len, pwid*len);

    char filename[16] = "fr";

    char num[4];
    sprintf(num, "%03d", iter);

    strcat(filename, num);
    strcat(filename, ".bmp");

    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            switch (plate->curr[i][j].state)
            {
            case DRY:
                color.r = 0; color.g = 0; color.b = 0;
                break;
            case WET:
                color.r = 250; color.g = 250; color.b = 250;
                break;
            case FROZEN:
                color.r = 50; color.g = 178; color.b = 247;
                break;
            }

            for(int k = pwid*i; k < pwid*(i+1); k++){
                for(int l = pwid*j; l < pwid*(j+1); l++)
                    bmp_pixel_init (&img.img_pixels[k][l], color.r, color.g, color.b);
            }
        }
    }

    bmp_img_write (&img, filename);
	bmp_img_free (&img);

    char command[50] = "move ";
    strcat(command, filename);
    strcat(command, " Snowman\\c_garbo\\pictemp >NUL");
    system(command);
}

void iterfreeze(Plate * plate, char temp, char humidity, int len, int iter, int pwid){
    for (int i = 0; i < iter; i++)
    {
        freezing(plate, temp, humidity, len, i);
        makebitmap(plate, len, i, pwid);
    }
}

// python hook function
int frost(int temp, int humidity, int len, int iters, int pwid){
    AHUM = len*len*humidity;

    Plate * plate = newplate(len, humidity);

    iterfreeze(plate, temp, humidity, len, iters, pwid);
    freeplate(plate, len);
    return 1;
}

int main(int argc, char**argv){
    // inputs are temp and humidity
    int temp = atoi(argv[1]);
    int humidity = atoi(argv[2]);
    int len = atoi(argv[3]);
    int iters = atoi(argv[4]);

    AHUM = len*len*humidity;

    Plate * plate = newplate(len, humidity);
    // Frode** plate = allocplate(side_len, humidity);


    // prode(plate, 10, 15);

    // Frode* p = &plate[22][25];
    // p->friegh = 1;
    // printf("Succ:\t%i\n", flu_freeze(*p));
    // prstate(plate, len);
    iterfreeze(plate, temp, humidity, len, iters, 1);
    // prstate(plate, len); 

    freeplate(plate, len);
}

