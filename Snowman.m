clear;
%Grid Variables
gridSize = 200;

global frostGrid

frostGrid = 2*ones(gridSize,gridSize);

originX = ceil(gridSize/2);
originY = ceil(gridSize/2);

%Drying Probability Variables
R = 2^(0.5);
C = 1;

%Set seed
frostGrid(originX,originY) = 1;

global walkerRadius
walkerRadius = 30;
radiusDelta = 1;
walkerTheta = 0;
walkerdTheta = 2*pi/360;

global running
running = true;


%Main Loop
while running
    %Get Initial Walker Point
    walkerTheta = rand()*4*pi;
    walkerX = floor(cos(walkerTheta)*walkerRadius) + originX;
    walkerY = floor(sin(walkerTheta)*walkerRadius) + originY;

    %Walk Walker
    walk(walkerX,walkerY,originX,originY,gridSize,radiusDelta);
    
    %img = normalize(frostGrid,'range',[0 1]);
   % imshow(img,'InitialMagnification',800)
    plt = pcolor(frostGrid)
    colormap cool
    shading interp
    pause(1e-16)
    plt.LineWidth = 0.1;

    
    %Advanace Theta
  %  walkerTheta = walkerTheta + walkerdTheta;
end

walkerTheta = 0;
while walkerTheta < 2*pi
    x = floor(cos(walkerTheta)*walkerRadius) + originX;
    y = floor(sin(walkerTheta)*walkerRadius) + originY;
    frostGrid(x,y) = 3;
    walkerTheta = walkerTheta + walkerdTheta;
end



function out = walk(x,y,ox,oy,gridSize,radiusDelta,C)
    global frostGrid
    global walkerRadius
    global running
    
    originalX = x;
    originalY = y;
    
    maxRestart = 20;
    restartCount = 0;
    %While not attached Loop
    while true
        %Move Walker randomly
        direction = rand();
        if direction < 0.25
            x = x + 1;
        elseif direction < 0.5
            y = y + 1;
        elseif direction < 0.75
            x = x - 1;
        elseif direction < 1
            y = y -1;
        end

        %Check if walker is in boundry
        if ((x-ox)^2 + (y-oy)^2) > walkerRadius^2 || frostGrid(x,y) == 0 || frostGrid(x,y) == 1
            %Ignore Walker, Move on to next
            out = -1;
            if restartCount < maxRestart
                x = originalX;
                y = originalY;
            else
                break;
            end
            
            restartCount = restartCount + 1;
            %fprintf("OUT OF BOUND\n");
        end

        %Walker is in boundary, Check for frosting
        didFrost = frostProbability(x,y);
        compareProb = rand();
        if didFrost > compareProb           
            frostGrid(x,y) = 1;
            
            distanceToRadius = pdist([ox,oy;x,y],'euclidean');
            
            if distanceToRadius >= 0.8*walkerRadius
                if walkerRadius<(gridSize/2-2)
                    walkerRadius = walkerRadius + radiusDelta;
                   % fprintf("radius: " + walkerRadius +"\n");
                else
                    running = false
                end
            end
%            if distanceToRadius >= gridSize/2 -2
 %               running = false;
  %          end
        end

        %Drying in grid
        calculateDrying(x,y,C);

        %Return if frosted over
        if didFrost
            out = 1;
            break;
        end
    end
end

function out = calculateDrying(x,y,C,c1,c2,R)
    global frostGrid
    
    R = floor(R);
    for i=-R:R
        for j=-R:R
            
        end
    end
    
    
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
        frostProb = 0.5;
    else
        frostProb = 0;
    end
end