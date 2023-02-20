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

// prints given Frode
void prode(Frode** plate, int i, int j){
    Frode temp = plate[i][j];
    printf("Frode (%i,%i)\nHumid:\t%i\nState:\t%i\nFreis:\t%i\n", i, j, temp.humidity, temp.state, temp.friegh); 
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

// To be implemented
int indep_freeze(int tempp); // independent freezing of nodes
int flu_freeze(Frode* node); // influenced freezing of nodes


int main(int argc, char**argv){
    // inputs are temp and humidity
    char temp = atoi(argv[1]);
    char humidity = atoi(argv[2]);
    int side_len = atoi(argv[3]);

    Frode** plate = allocplate(side_len, humidity);

    prode(plate, 10, 15);

}

