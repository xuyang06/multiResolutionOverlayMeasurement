function [ overlay_underlay_matrix, select_underlay_matrix ] = exhaustpath( link_matrix, graph_matrix, chosenoverlay )
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
allcombns = combntns(chosenoverlay,2);
[row_comb_m, column_comb_m] = size(allcombns);
[row_link_m, column_link_m] = size(link_matrix);
row_comb_index = 1;
overlay_underlay_matrix = zeros(row_comb_m, row_link_m);
select_underlay_matrix = [];
select_underlay_index = 1;
while row_comb_index <= row_comb_m
    src_index = allcombns(row_comb_index,1);
    dst_index = allcombns(row_comb_index,2);
    [e L] = dijkstra(graph_matrix, graph_matrix, src_index, dst_index);
    L_unique = unique(L);
    [L_unique_row, L_unique_column] = size(L_unique);
    %[L_row, L_column] = size(L_unique);
    [L_row, L_column] = size(L);
    if L_unique_column ~= L_column
        %e
        %L
        %disp('difference')
        overlay_underlay_matrix(row_comb_m,:) = [];
        row_comb_m = row_comb_m - 1;
        row_comb_index = row_comb_index + 1;
        continue
    end
    %src_index
    %dst_index
    %e
    %L
    if L_column >= 2
        L_index = 1;
        while L_index <= L_column - 1
            src_index = L(L_index);
            dst_index = L(L_index + 1);
            if src_index ~= dst_index
                [ found_link_index ] = findlinkindex( link_matrix, src_index, dst_index);
                %src_index
                %dst_index
                %link_matrix
                %found_link_index
                overlay_underlay_matrix(row_comb_index, found_link_index) = 1;
            end
            L_index = L_index + 1;
        end
    end
    row_comb_index = row_comb_index + 1;
end

[overlay_underlay_matrix_row, overlay_underlay_matrix_column] = size(overlay_underlay_matrix);
underlay_index = 1;
real_underlay_index = 1;
while real_underlay_index <= overlay_underlay_matrix_column
    %underlay_index
    %overlay_underlay_matrix_column
    if (sum(overlay_underlay_matrix(:,underlay_index)) == 0)
        overlay_underlay_matrix(:,underlay_index) = [];
    else
        select_underlay_matrix(select_underlay_index) = real_underlay_index;
        select_underlay_index = select_underlay_index + 1;
        underlay_index = underlay_index + 1;
    end
    real_underlay_index = real_underlay_index + 1;
end




end

