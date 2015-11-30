function [ currentlinkmatrix, failurelinkindexmatrix] = generatelinkfailure( linkfailurematrix, linkfailurerangeprob_matrix )
%UNTITLED11 Summary of this function goes here
%   Detailed explanation goes here
[row_link_m, column_link_m] = size(linkfailurematrix);
currentlinkmatrix = zeros(row_link_m, 1);
row_link_index = 1;
failure_link_total = 0;

rand_failure = rand();

while row_link_index <= row_link_m
    if linkfailurerangeprob_matrix(row_link_index) > rand_failure
        break
    end
    row_link_index = row_link_index + 1;
end

selected_link_index = row_link_index;

row_link_index = 1;


while row_link_index <= row_link_m
    if (row_link_index == selected_link_index)
        row_link_index = row_link_index + 1;
        continue
    else
        rand_failure = rand();
        if rand_failure <= linkfailurematrix(row_link_index)
            currentlinkmatrix(row_link_index) = 0;
            failure_link_total = failure_link_total + 1;
        else
            currentlinkmatrix(row_link_index) = 1;
        end
            %linkfailurematrix(row_link_index) = rand();
        row_link_index = row_link_index + 1;
    end
end


failurelinkindexmatrix = zeros(1,failure_link_total);
row_link_index = 1;
failure_link_index = 1;
while row_link_index <= row_link_m
    if currentlinkmatrix(row_link_index) == 0
        failurelinkindexmatrix(failure_link_index) = row_link_index;
        failure_link_index = failure_link_index + 1;
    end
    %linkfailurematrix(row_link_index) = rand();
    row_link_index = row_link_index + 1;
end


end

