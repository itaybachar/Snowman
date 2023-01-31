function [stick_cond,p]=AreWeNear(x,y,A,mu,sigma,w,Scr,c)

% Checks to see if we've moved to an adjacent site in order to determine
% the sticking probability used by the FrostDLA function

stick_cond=0;p=0;

% D_l is drawn from a normal distribution
D_l=normrnd(mu,sigma);

if ((A(x+1,y)==1)||(A(x,y+1)==1)||(A(x-1,y)==1)||(A(x,y-1)==1))

    %Halt the walker and assign a probability of sticking. (model-specific)
    stick_cond=1;
    S=(1-w/2)/D_l;
    p=Scr/S;
    
end

if ((A(x-1,y-1)==1)||(A(x-1,y+1)==1)||(A(x+1,y+1)==1)||(A(x+1,y-1)==1))
    
    %Halt the walker with corner probability (again, model-specific)
    stick_cond=1;
    S=sqrt(2)*(1-w/2)/D_l;
    p=Scr/S;
    
end

% makes sure p is in the interval [c,1]. (probabilities can't be >1)
% c small but >0 so the simulation keeps moving along (this is optional)
if p>1
    p=1;
end
if p<c
    p=c;
end