function [ linkfailurematrix1, linkfailurematrix2, linkfailurematrix3 ] = linkfailureprob( select_underlay_matrix )
%UNTITLED10 Summary of this function goes here
%   Detailed explanation goes here
%[row_link_m, column_link_m] = size(link_matrix);
[select_underlay_matrix_row, select_underlay_matrix_column] = size(select_underlay_matrix);
linkfailurematrix1 = zeros(select_underlay_matrix_column, 1);
linkfailurematrix2 = zeros(select_underlay_matrix_column, 1);
linkfailurematrix3 = zeros(select_underlay_matrix_column, 1);
row_link_index = 1;
while row_link_index <= select_underlay_matrix_column
    %linkfailurematrix(row_link_index) = rand()*0.3;
    linkfailurematrix1(row_link_index) = normalfailprob();
    linkfailurematrix2(row_link_index) = rand()*0.3;
    row_link_index = row_link_index + 1;
end

select_underlay_matrix_column_70_total = int8(select_underlay_matrix_column*0.7);
select_underlay_matrix_column_20_total = int8(select_underlay_matrix_column*0.2);
select_underlay_matrix_column_10_total = select_underlay_matrix_column - select_underlay_matrix_column_70_total - select_underlay_matrix_column_20_total;

failure_70_percent_prob = 0.01;
failure_20_percent_prob = 0.05;
failure_10_percent_prob = 0.20;

row_link_index = 1;
link_index_matrix = zeros(select_underlay_matrix_column, 1);
link_index = 1;
while link_index <= select_underlay_matrix_column
    link_index_matrix(link_index) = link_index;
    link_index = link_index + 1;
end
total_num = select_underlay_matrix_column;
select_underlay_matrix_column_70_index = 1;
select_underlay_matrix_column_20_index = 1;
select_underlay_matrix_column_10_index = 1;

while row_link_index <= select_underlay_matrix_column
    rand_num = int8(rand()*(total_num-1)) + 1;
    if select_underlay_matrix_column_70_index <= select_underlay_matrix_column_70_total
        linkfailurematrix3(link_index_matrix(rand_num)) = failure_70_percent_prob;
        select_underlay_matrix_column_70_index = select_underlay_matrix_column_70_index + 1;
        total_num = total_num - 1;
    end
    if select_underlay_matrix_column_20_index <= select_underlay_matrix_column_20_total
        linkfailurematrix3(link_index_matrix(rand_num)) = failure_20_percent_prob;
        select_underlay_matrix_column_20_index = select_underlay_matrix_column_20_index + 1;
        total_num = total_num - 1;
    end
    if select_underlay_matrix_column_10_index <= select_underlay_matrix_column_10_total
        linkfailurematrix3(link_index_matrix(rand_num)) = failure_10_percent_prob;
        select_underlay_matrix_column_10_index = select_underlay_matrix_column_10_index + 1;
        total_num = total_num - 1;
    end
    
    row_link_index = row_link_index + 1;
end






end

