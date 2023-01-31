function A=FrostDLA(n,num_walkers,Scr,mu,sigma,w,plots)

% Runs a Monte Carlo simulation of a preliminary model for frosting.
% All inputs are defined in the main script.
% Output is an nxn matrix, n should be even.

% The simulation makes use of three additional functions: 
% MoveParticle:  updates lattice position through a simple rule
% MaybeMoveBack: checks to make sure we haven't landed on top of the frost
% AreWeNear:     calculates sticking probability, highly model-specific


% DLA-specific execution parameters
buffer=10;inner_buffer=2;
max_distance=20;c=0.01;

% Seed particle location assumed near or at the center of the lattice
seed=round(n/2);

% Current domain of simulation
radius=max_distance;
A=zeros(n);

% Place the seed in the center of the lattice
A(seed,seed)=1;

% Initiate the simulation
k=2;
rewalk=0;
agg_size=1;
agg_size_squared=1;

while (k<=num_walkers)
    
    % Launch Random Walker Far Away from Aggregate
    if(rewalk==0||A(x,y)>1)
        s=rand;
        x=floor(seed+radius*cos(s*2*pi));
        y=floor(seed+radius*sin(s*2*pi));
    end
    
    % Proceed with Random Walk Until Sticking Condition is False.
    stick_cond=0;
    while (stick_cond==0)
        
        % Move the particle.
        [s,x,y]=moveparticle(x,y);
        
        % Calculate how far the walker is from the center.
        distance_squared=((x-seed)^2+(y-seed)^2);
        
        % Check to see if we have moved onto the frost or forbidden zone.
        [x,y]=MaybeMoveBack(A,s,x,y,...
            agg_size_squared,inner_buffer,radius,buffer,distance_squared);
        
        % Determine the sticking probability.
        [stick_cond,stick_prob]=AreWeNear(x,y,A,mu,sigma,w,Scr,c);
        
        % End of Random Walk
    end
    
    % Check to see if walker is likely to stick.
    p=rand;
    if (stick_prob<p)
        rewalk=1;
        % This is now part of the forbidden zone of attachment
        A(x,y)=2;
    else
        rewalk=0;
    end
    
    if (rewalk==0)
        
        % Walker attaches. Stick it and update state space time
        A(x,y)=1;
        k=k+1;
        if (mod(k,plots.mod)==0&&plots.flag)
            pcolor(A),colormap('hot'),colorbar,shading interp
            pause(1e-16)
        end
        
        % Two approaches to growing the domain. Second is commented out
        if ((radius-sqrt(distance_squared))<max_distance)
            radius=radius+1;
            agg_size=agg_size+1;
            agg_size_squared=agg_size^2;
        end
        
        %         D=bwdist(A);
        %         agg_size=sqrt(2)*seed-D(1,1);
        %         agg_size_squared=agg_size^2;
        %         radius=agg_size;
        %         radius_squared=radius^2;
    end
end