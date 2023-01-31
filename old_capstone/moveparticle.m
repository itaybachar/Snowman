function [s,x,y]=moveparticle(x,y)

% updates the position of the drunk walker's lattice position
s=rand();
if (s<=1/4)
    % move left
    y=y+1;
else
    if (1/4<s&&s<=1/2)
        % move up
        x=x+1;
    else
        if (1/2<s&&s<=3/4)
            % move down
            x=x-1;
        else
            if (3/4<s)
                % move right
                y=y-1;
            end
        end
    end
end