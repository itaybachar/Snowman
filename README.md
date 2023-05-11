# Snowman
This repository contains the code for both approaches taken to simulate frost over the Spring 2023 math capstone.

DLA approach:
The source code for this is contained under DLA/cpp.
To run, open the FrostGrowthDLa folder under DLA in visual studios and build the dll(shared library). Once that is created, copy the dll(or so) over to the CPP folder where you will find the FrostDla.py
Edit the frost DLA python file to adjust for parameters and then you just run the python program.

Iterative Monte Carlo:
this is the second approach taken for the modeling, and is found under the c_garbo folder. Similar the the DLA, you need to compile the files in the folder into a dll or shared object using a compiler of your choice.
Then the gifmaker.py is the python driver that allows you to change parameters and finally run the simulation.
