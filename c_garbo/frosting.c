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
    char dry;
} Frode;

typedef struct Plate{
    Frode ** curr;
    Frode ** prev;
} Plate;

typedef struct Box{
    int side;
    int nonwet; // count of the covers for frozen and dry nodes
    int frozen; // count of the cover for JUST frozen nodes
} Box;

typedef struct BoxCountNode{
    int nonwet;
    int frozen;
} BoxCountNode;

typedef struct LinearReg{
    double a;
    double b;
} LinearReg;

// Takes unnnessary parameters so that there is a general form amongst all humidity reduction functions
double constHumidity(int iter, double reduxPara){
    return 0.0; // literally does nothing but returns zero, do not compile with -Wall
}

double linearCentHumidty(int iter, double reduxPara){
    return -iter/100.0;
}

double linearGenHumidity(int iter, double reduxPara){
    return -(reduxPara*iter)/100.0;
}

// Array of function pointers to the above humidty reduction formulas to simplify fluFreeze function
double (*humReduce[])(int, double) = {&constHumidity, &linearCentHumidty, &linearGenHumidity};


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

void show_status (double percent, const char* desc, int isFinal){
    printf("%s:\t|", desc);
    for(int x = 0; x < 100; x++) {   
       printf("%c", (x < percent)?'#':'-');        
    }
    
    printf("| %.2f%%%c", percent, (isFinal)?'\n':'\r');
    fflush(stdout);
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
            t[i][j].dry = 0;
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
int deallocfrodes(Frode ** f, int len, int bar){
    for (int i = 0; i < len; i++) {
        free(f[i]);
        show_status((++bar/(len/50.0)), "Dealloc Plate", (bar==2*len-1));
    }
    free(f);
    return bar;
}

// frees both frode plates and struct itself
void freeplate(Plate * p, int len){
    int bar = deallocfrodes(p->curr, len, 0);
    deallocfrodes(p->prev, len, bar);

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
int flu_freeze(Frode node, int iter, int reduxInd, double reduxPara){

    double redux = (node.humidity/255.0) + (humReduce[reduxInd])(iter, reduxPara);
    // logistic line fitting
    double p = 10.2598+((redux>0.0)?redux:0.0)*-42.6528; 
    double prob = 1/((1+exp(p))*(node.friegh + 1.41421*node.diag));
    // // divide prob by frozen neighbors
    // prob /= node.friegh;
    // printf("Prob:\t%f\n", prob);


    double chance = genprob(1.0);

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

void addDry(Plate * plate, int i, int j, int len){
    for (int k  = i-1; k < i+2; k++){
        if (k<0 || k>=len) continue;

        for (int m = j-1; m < j+2; m++){
            if (m<0 || m>=len) continue;

            if(!(k==i && m==j)) {
                plate->prev[k][m].dry++;
            }
        }
    }
}

BoxCountNode** makeBoxCounter(int plateLen, int boxWidth){
    int boxCountLen = (plateLen/boxWidth);
    BoxCountNode** p = malloc(sizeof(BoxCountNode*) * boxCountLen);
    for(int i = 0; i < boxCountLen; i++){
        p[i] = malloc(sizeof(BoxCountNode) * boxCountLen);
        for(int j = 0; j < boxCountLen; j++){
            p[i][j].nonwet = 0;
            p[i][j].frozen = 0;
        }
    }
    return p;
}

// this function leaks memory, watch out
void boxCount(Plate* plate, int len, Box* Boxes, int boxNum){
    BoxCountNode*** boxGrids = malloc(sizeof(int**) * boxNum);

    for(int i = 0; i < boxNum; i++){
        boxGrids[i] = makeBoxCounter(len, Boxes[i].side);
    }

    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            Frode cpy = plate->curr[i][j];

            if(cpy.state == WET) continue;

            for(int k = 0; k < boxNum; k++){
                int boxI = i/Boxes[k].side, boxJ = j/Boxes[k].side;
                if(cpy.state == FROZEN){
                    Boxes[k].frozen += !(boxGrids[k][boxI][boxJ].frozen);
                    boxGrids[k][boxI][boxJ].frozen++;
                }
                Boxes[k].nonwet += !(boxGrids[k][boxI][boxJ].nonwet);
                boxGrids[k][boxI][boxJ].nonwet++;
            }
        }
    }
}

void freezing(Plate* plate, char temp, char humidity, int len, int iter, double bias, int reduxInd, double reduxPara){
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

                if(!remHum && (cpy.friegh || cpy.diag) && (genprob(1.0) < bias)){
                    remHum=1;
                    if(flu_freeze(cpy, iter, reduxInd, reduxPara)){
                        plate->prev[i][j].state = FROZEN;
                        addFreigh(plate, i, j, len);
                    }else{
                        plate->prev[i][j].state = DRY;
                        addDry(plate, i, j, len);
                    }
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

void iterfreeze(Plate * plate, char temp, char humidity, int len, int iter, int pwid, double bias, int reduxInd, double reduxPara){
    for (int i = 0; i < iter; i++)
    {
        freezing(plate, temp, humidity, len, i, bias, reduxInd, reduxPara);
        makebitmap(plate, len, i, pwid);
        show_status(((i+1)*100.0)/iter, "Freezing", (i==iter-1));
    }

}

void BoxLinearReg(LinearReg* lireg, int liLen, Box* Boxes, int boxNum){
    double sumX = 0.0, sumX2 = 0.0; // values used for both
    double nwY = 0.0, nwXY = 0.0; // nonwet values
    double frY = 0.0, frXY = 0.0; // frozen values

    for(int i = 0; i < boxNum; i++){
        double logInvSide = log(1.0/Boxes[i].side), logNWcount = log(1.0*Boxes[i].nonwet), logFRcount = log(1.0*Boxes[i].frozen);
        
        sumX += logInvSide; sumX2 += logInvSide*logInvSide; 
        
        nwY += logNWcount; nwXY += logInvSide*logNWcount;

        frY += logFRcount; frXY += logInvSide*logFRcount;
    }

    // nonwet regression values
    double nwB = (boxNum*nwXY - sumX*nwY)/(boxNum*sumX2 - sumX*sumX);
    lireg[0].a = (nwY - nwB*sumX)/boxNum;
    lireg[0].b = nwB;

    //frozen regression values
    double frB = (boxNum*frXY - sumX*frY)/(boxNum*sumX2 - sumX*sumX);
    lireg[1].a = (frY - frB*sumX)/boxNum;
    lireg[1].b = frB;

}

// python hook function
int frost(int temp, int humidity, int len, int iters, int pwid, double bias, int reduxInd, double reduxPara){
    len += 16 - (len%16); // Rounds length up to closest multiple of 16 (this is for box counting) 
    AHUM = len*len*humidity;

    Plate * plate = newplate(len, humidity);

    // Making boxes for widths 2,4,8,16
    int boxNum = 4;
    Box* Boxes = malloc(4*sizeof(Box));
    for(int i = 0; i < boxNum; i++){
        Boxes[i].side = 2 << i;
        Boxes[i].nonwet = 0;
        Boxes[i].frozen = 0;
    }

    // 0 --> nonwet fractal dimension, 1 --> frozen fractal dimension
    // does not need initialization since garbage data will be over written in linear
    // regression function
    LinearReg lireg[2];

    iterfreeze(plate, temp, humidity, len, iters, pwid, bias, reduxInd, reduxPara);
    
    boxCount(plate, len, Boxes, boxNum);
    BoxLinearReg(lireg, 2, Boxes, boxNum);
    printf("\nBox-counting Dimensions:\n\tNonwet: %f\n\tFrozen: %f\n", lireg[0].b, lireg[1].b);
    freeplate(plate, len);
    return 1;
}

int main(int argc, char**argv){
    // inputs are temp and humidity
    // int temp = atoi(argv[1]);
    // int humidity = atoi(argv[2]);
    // int len = atoi(argv[3]);
    // int iters = atoi(argv[4]);

    // AHUM = len*len*humidity;

    // Plate * plate = newplate(len, humidity);
    // Frode** plate = allocplate(side_len, humidity);


    // prode(plate, 10, 15);

    // Frode* p = &plate[22][25];
    // p->friegh = 1;
    // printf("Succ:\t%i\n", flu_freeze(*p));
    // prstate(plate, len);
    // iterfreeze(plate, temp, humidity, len, iters, 1, 1.1, 0, 100.0);
    // prstate(plate, len); 

    // freeplate(plate, len);
    frost(-15, 65, 200, 100, 2, 0.6, 0, 0.3);
    return 0;
}

