function [ overlaylinkmatrix ] = generateoverlaycondition( currentlinkmatrix, chosen_overlay_link_index_list, overlay_underlay_matrix )
%UNTITLED12 Summary of this function goes here
%   Detailed explanation goes here
[row_link_m, column_link_m] = size(chosen_overlay_link_index_list);
[overlay_underlay_matrix_row, overlay_underlay_matrix_column] = size(overlay_underlay_matrix);
overlaylinkmatrix = zeros(row_link_m, 1);
row_link_index = 1;
while row_link_index <= column_link_m
    overlaylinkcondition = 1;
    overlaylinkindex = 1;
    while overlaylinkindex <= overlay_underlay_matrix_column
        if overlay_underlay_matrix(chosen_overlay_link_index_list(row_link_index), overlaylinkindex) == 1
            if currentlinkmatrix(overlaylinkindex) == 0
                %row_link_index
                %chosen_overlay_link_index_list(row_link_index)
                %overlaylinkindex
                overlaylinkcondition = 0;
                break
            end
        end
        overlaylinkindex = overlaylinkindex + 1;
    end
    overlaylinkmatrix(row_link_index) = overlaylinkcondition;
    %linkfailurematrix(row_link_index) = rand();
    row_link_index = row_link_index + 1;
end




end

