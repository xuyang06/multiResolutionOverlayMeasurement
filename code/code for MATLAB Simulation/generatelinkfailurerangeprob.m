function [ linkfailurerangeprob_matrix] = generatelinkfailurerangeprob( linkfailurematrix )
%UNTITLED11 Summary of this function goes here
%   Detailed explanation goes here
[row_link_m, column_link_m] = size(linkfailurematrix);
linkfailurerangeprob_matrix = zeros(row_link_m, 1);
normal_linkfailurerangeprob_matrix = linkfailurematrix/sum(linkfailurematrix);
row_link_index = 1;
pre_val = 0.0;
while row_link_index <= row_link_m
    linkfailurerangeprob_matrix(row_link_index) = normal_linkfailurerangeprob_matrix(row_link_index) + pre_val;
    pre_val = linkfailurerangeprob_matrix(row_link_index);
    row_link_index = row_link_index + 1;
end

end

