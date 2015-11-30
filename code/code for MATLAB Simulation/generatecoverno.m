function [ new_link_matrix, overlay_cost_matrix ] = generatecoverno( link_matrix, overlay_underlay_matrix, select_underlay_matrix, linkfailurematrix)
%UNTITLED8 Summary of this function goes here
%   Detailed explanation goes here
[link_matrix_row, link_matrix_column] = size(link_matrix);
[select_underlay_matrix_row, select_underlay_matrix_column] = size(select_underlay_matrix);
new_link_matrix = zeros(select_underlay_matrix_column,4);
[overlay_underlay_matrix_row, overlay_underlay_matrix_column] = size(overlay_underlay_matrix);
overlay_cost_matrix = zeros(1,overlay_underlay_matrix_row);
link_row_index = 1;
new_link_index = 1;
while link_row_index <= link_matrix_row
    src_index = link_matrix(link_row_index, 1);
    dst_index = link_matrix(link_row_index, 2);
    link_delay = link_matrix(link_row_index, 3);
    %link_cover_num = int8(rand*2) + 1;
    if ismember(link_row_index, select_underlay_matrix) == 1
        link_cover_num = 1;
        if (linkfailurematrix(new_link_index) >= 0.2)
            link_cover_num = 3;
        elseif (linkfailurematrix(new_link_index) >= 0.1)
            link_cover_num = 2;
        end
        new_link_matrix(new_link_index,:) = [src_index, dst_index, link_delay, link_cover_num];
        new_link_index = new_link_index + 1;
    end
    link_row_index = link_row_index + 1;
end
overlay_underlay_matrix_row_index = 1;
while overlay_underlay_matrix_row_index <= overlay_underlay_matrix_row
    overlay_underlay_matrix_column_index = 1;
    overlay_underlay_matrix_row_cost = 0;
    while overlay_underlay_matrix_column_index <= overlay_underlay_matrix_column
        overlay_underlay_matrix_row_cost = overlay_underlay_matrix_row_cost + overlay_underlay_matrix(overlay_underlay_matrix_row_index, overlay_underlay_matrix_column_index)*new_link_matrix(overlay_underlay_matrix_column_index, 3);
        overlay_underlay_matrix_column_index = overlay_underlay_matrix_column_index + 1;
    end
    overlay_cost_matrix(overlay_underlay_matrix_row_index) = overlay_underlay_matrix_row_cost;
    overlay_underlay_matrix_row_index = overlay_underlay_matrix_row_index + 1;
end


end

