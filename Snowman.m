%Grid Variables
gridSize = 100;
global grid
grid = 2*ones(gridSize,gridSize);

global originX
originX = gridSize/2;

global originY
originY = gridSize/2;

%Drying Probability Variables
R = 2^(0.5);
C = 1


%Set seed
grid(originX,originY) = 1;

walkerRadius = 10;
walkerTheta = 0;
walkerdTheta = 2*pi/360;

%Main Loop
while walkerTheta<4*pi
    %Get Initial Walker Point
    walkerX = floor(cos(walkerTheta)*walkerRadius);
    walkerY = floor(sin(walkerTheta)*walkerRadius);
    
    %Walk Walker
    walk(walkerX,walkerY);
    
    %Advanace Theta
    walkerTheta = walkerTheta + walkerdTheta;
end


function out = walk(x,y)
    
    %While not attached Loop
    while true
        %Move Walker randomly
        direction = rand();
        if direction < 0.25
            x = x + 1;
        elseif direction < 0.5
            x = x - 1;
        elseif direction < 0.75
            y = y + 1;
        else
            y = y -1;
        end

        %Check if walker is in boundry
        if ((x-originX)^2 + (y-originY)^2) > walkerRadius^2
            %Ignore Walker, Move on to next
            out = -1;
            return
        end

        %Walker is in boundary, Check for frosting
        didFrost = frostProbability(x,y);
        if didFrost
            grid(x,y) = 1;
        end

        %Drying in grid
        calculateDrying(x,y);

        %Return if frosted over
        if didFrost
            out = 1;
            return;
        end
    end
end

function out = calculateDrying(x,y)
    out = 1;
end

function frostProb = frostProbability(x,y)
    %Check for neightbor frost
    % TODO
    % Check that neighbors are in array
    hasNeighbor = grid(x-1,y +1) == 1 || grid(x,y +1) == 1 ||... 
    grid(x+1,y +1) == 1 || grid(x-1,y) == 1 ||grid(x+1,y) == 1 ||... 
    grid(x-1,y -1) == 1 || grid(x,y-1) == 1 || grid(x+1,y -1) == 1;
    
    if hasNeighbor
        %%have to change
        frostProb = 1;
    else
        frostProb = 0;
    end
end