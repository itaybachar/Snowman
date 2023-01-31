function [x,y]=...
    MaybeMoveBack(A,s,x,y,agg_size_squared,inner_buffer,...
                         radius,buffer,distance_squared)

% This functions checks to see if we've moved onto the aggregate.

% This is done by checking  if the current lattice position within
% the simulation domain is already either frost or forbidden (A(x,y)>0)

% By inputting s generated from the moveparticle function, we know how
% how to "step back" from where we came from.

if (distance_squared<=(agg_size_squared+inner_buffer)&&A(x,y)>0)
    if (s<=1/4)
        y=y-1;
    else
        if (1/4<s&&s<=1/2)
            x=x-1;
        else
            if (1/2<s&&s<=3/4)
                x=x+1;
            else
                if (3/4<s)
                    y=y+1;
                else
                end
            end
        end
    end
end

% If we've stepped outside the allowable domain, respawn on the boundary
if (distance_squared>(radius+buffer)^2)
    x=floor(length(A)/2+radius*cos(s*2*pi));
    y=floor(length(A)/2+radius*sin(s*2*pi));
end