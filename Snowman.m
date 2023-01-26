%Grid Variables
gridSize = 11;

global frostGrid
frostGrid = 2*ones(gridSize,gridSize);

originX = ceil(gridSize/2);

originY = ceil(gridSize/2);

%Drying Probability Variables
R = 2^(0.5);
C = 1;


%Set seed
frostGrid(originX,originY) = 1;

walkerRadius = 3;
walkerTheta = 0;
walkerdTheta = 2*pi/360;

%Main Loop
while walkerTheta<4*pi
    %Get Initial Walker Point
    walkerX = floor(cos(walkerTheta)*walkerRadius) + originX;
    walkerY = floor(sin(walkerTheta)*walkerRadius) + originY;
    
    %Walk Walker
    walk(walkerX,walkerY,originX,originY,walkerRadius);
    
    %Advanace Theta
    walkerTheta = walkerTheta + walkerdTheta;
end

function out = walk(x,y,ox,oy,walkerRadius)
    global frostGrid

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
        if ((x-ox)^2 + (y-oy)^2) > walkerRadius^2
            %Ignore Walker, Move on to next
            out = -1;
            fprintf("OUT OF BOUND\n");
            return
        end

        %Walker is in boundary, Check for frosting
        didFrost = frostProbability(x,y);
        
        
        if didFrost
            fprintf("Frost" + didFrost+ "\n");
            
            frostGrid(x,y) = 1;
            fprintf("Current: " + frostGrid(x,y)+"\n");
        end

        %Drying in grid
        calculateDrying(x,y);

        %Return if frosted over
        if didFrost
            out = 1;
            break;
        end
    end
end

function out = calculateDrying(x,y)
    out = 1;
end

function frostProb = frostProbability(x,y)
	global frostGrid

    %Check for neightbor frost
    % TODO
    % Check that neighbors are in array
    hasNeighbor = frostGrid(x-1,y +1) == 1 || frostGrid(x,y +1) == 1 ||... 
    frostGrid(x+1,y +1) == 1 || frostGrid(x-1,y) == 1 ||frostGrid(x+1,y) == 1 ||... 
    frostGrid(x-1,y -1) == 1 || frostGrid(x,y-1) == 1 || frostGrid(x+1,y -1) == 1;
    
    if hasNeighbor
        %%have to change
        frostProb = true;
    else
        frostProb = false;
    end
end