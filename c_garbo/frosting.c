// This is just experimenting with the model Yousef and Nalby thought about
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
// States of nodes
#define DRY 0
#define WET 1
#define FROZEN 2

// Frosting Node -> Frode
typedef struct Frode{
    char humidity; // 255 is highest hummidty possible, 0 is lowest relative humidity
    char state;   // current state of node
    char friegh; // frozen neighbors -> freigh, max 8
} Frode;

// prints given Frode
void prode(Frode** plate, int i, int j){
    Frode temp = plate[i][j];
    printf("Frode (%i,%i)\nHumid:\t%i\nState:\t%i\nFreis:\t%i\n\n", i, j, temp.humidity, temp.state, temp.friegh); 
}

// Allocates square plate of length len with an attribute humidity 
Frode** allocplate(int len, char hum){
    Frode **t;

    t = (Frode **) malloc(sizeof(Frode *) * len);

    for (int i = 0; i < len; i++) {
        t[i] = (Frode *) malloc(sizeof(Frode) * len);
        for (int j = 0; j < len; j++) {
            t[i][j].humidity = hum;
            t[i][j].state = WET;
            t[i][j].friegh = 0;
        }
    }

    return t;    
}

// independent freezing of nodes
int indep_freeze(int temp){
    time_t t;
    srand((unsigned) time(&t));

    double p = 11.6379+temp*0.362419;
    double prob = 1/(1+exp(p));

    double chance = (double)rand()/(double)(RAND_MAX);

    return (chance < prob);
}

// influenced freezing of nodes, runs on WET nodes
int flu_freeze(Frode node){
    time_t t;
    srand((unsigned) time(&t));

    // prob is reduced if there are multiple freighs (multiple ice bridges being made)
    double prob = ((double)(node.humidity))/(255.0 * node.friegh);
    printf("Prob:\t%f\n", prob);

    // this shit fucked, gotta fix (biased)
    double chance = ((double)rand())/((double)(RAND_MAX));
    printf("Chance:\t%f\n", chance);

    return (chance < prob);
} 


int main(int argc, char**argv){
    // inputs are temp and humidity
    char temp = atoi(argv[1]);
    char humidity = atoi(argv[2]);
    int side_len = atoi(argv[3]);

    Frode** plate = allocplate(side_len, humidity);

    prode(plate, 10, 15);

    Frode* p = &plate[22][25];
    p->friegh = 1;
    printf("Succ:\t%i\n", flu_freeze(*p));
}

