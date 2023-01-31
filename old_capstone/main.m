%% Runs the FrostDLA function
clear,close all

% Size of the domain
n=300;

% Number of particles
num_walkers=1000;

% Pillar distance and width
delta=1;w=delta/4;

% Gaussian mean and width for D_l
mu=.5;sigma=.1;

% Plot configuration
plots.mod=50;plots.flag=true;

% Critical S
Scr=1;

% Aggregate stored as matrix A
A=FrostDLA(n,num_walkers,Scr,mu,sigma,w,plots);

%% Functions Used
dbtype FrostDLA
dbtype moveparticle
dbtype MaybeMoveBack
dbtype AreWeNear