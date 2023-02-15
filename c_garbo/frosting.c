// This is just experimenting with the model Yousef and Nalby thought about
#include <stdlib.h>
#include <stdio.h>
// States of nodes
#define WET 1
#define FROZEN 2

// Frosting Node -> Frode
typedef struct Frode{
    char humidity; // 255 is highest hummidty possible, 0 is lowest relative humidity
    char state;   // current state of node
    char friegh; // frozen neighbors -> freigh, max 8
} Frode;

// To be implemented
int indep_freeze(int tempp);
int flu_freeze(Frode* node);


int main(int argc, char**argv){
    // inputs are temp and humidity
    int temp = atoi(argv[1]);
    int humidity = atoi(argv[2]);


}

