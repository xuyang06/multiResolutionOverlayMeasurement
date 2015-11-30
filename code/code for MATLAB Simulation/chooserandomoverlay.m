function [ chosenoverlay ] = chooserandomoverlay( dimension_m, overlay_m )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
i = 0;
chosenoverlay = [];
while i < overlay_m
    rand_i = int8(rand*(dimension_m-1)) + 1;
    if ismember(rand_i, chosenoverlay)
        continue
    end
    chosenoverlay(i+1) = rand_i;
    i = i + 1;
end

end


