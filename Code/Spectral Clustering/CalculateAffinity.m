function [affinity] = CalculateAffinity(data)

% set the parameters
sigma = 1;

for i=1:size(data,1)    
    for j=1:size(data,1)
        dist = 0;
        for k=1:size(data,2)
        	dist = dist + (data(i,k) - data(j,k))^2; 
        end
        dist = sqrt(dist);
        affinity(i,j) = exp(-dist/(2*sigma^2));
    end
end


