function [ normal_underlay_list ] = optimalinferunderlaywithprob( new_link_matrix, overlay_underlay_matrix, overlay_cost_matrix, linkfailurematrix, overlaylinkmatrix, chosen_overlay_link_index_list)
%UNTITLED15 Summary of this function goes here
%   Detailed explanation goes here

[overlay_underlay_matrix_row, overlay_underlay_matrix_column] = size(overlay_underlay_matrix);
[new_link_matrix_row, new_link_matrix_column] = size(new_link_matrix);


%underlay_consider_list = zeros(1,overlay_underlay_matrix_column);

normal_underlay_list = zeros(overlay_underlay_matrix_column, 1);

overlaylink_index = 1;
[overlaylinkmatrix_row, overlaylinkmatrix_column] = size(overlaylinkmatrix);
underlayconsiderlist = [];
underlayconsiderindex = 1;
abnormaloverlaylist = [];
abnormaloverlaynumlist = [];
abnormaloverlayindex = 1;
while overlaylink_index <= overlaylinkmatrix_column
    if overlaylinkmatrix(overlaylink_index) == 1
        overlay_underlay_matrix_index = 1;
        while overlay_underlay_matrix_index <= overlay_underlay_matrix_column
            if overlay_underlay_matrix(chosen_overlay_link_index_list(overlaylink_index), overlay_underlay_matrix_index) == 1
                normal_underlay_list(overlay_underlay_matrix_index) = 1;
                if ismember(overlay_underlay_matrix_index, underlayconsiderlist) == 0
                    underlayconsiderlist(underlayconsiderindex) = overlay_underlay_matrix_index;
                    underlayconsiderindex = underlayconsiderindex + 1;
                end
            end
            overlay_underlay_matrix_index = overlay_underlay_matrix_index + 1;
        end
    else
        abnormaloverlaylist(abnormaloverlayindex) = overlaylink_index;
        abnormaloverlaynumlist(abnormaloverlayindex) = chosen_overlay_link_index_list(overlaylink_index);
        abnormaloverlayindex = abnormaloverlayindex + 1;
    end
    overlaylink_index = overlaylink_index + 1;
end

%normal_underlay_list
%underlayconsiderlist
%abnormaloverlaylist

underlaynotconsidersize= overlay_underlay_matrix_column - (underlayconsiderindex-1);

underlaynotconsiderlist = zeros(underlaynotconsidersize,1);
underlaynotconsiderindex = 1;
underlayindex = 1;
while underlayindex <= overlay_underlay_matrix_column
    if ismember(underlayindex, underlayconsiderlist) == 0
        underlaynotconsiderlist(underlaynotconsiderindex) = underlayindex;
        underlaynotconsiderindex = underlaynotconsiderindex + 1;
    end
    underlayindex = underlayindex + 1;
end

%underlaynotconsiderlist

%underlaynotconsidersize = underlaynotconsiderindex - 1;

f_matrix = zeros(1, underlaynotconsidersize);

underlaynotconsiderindex = 1;
while underlaynotconsiderindex <= underlaynotconsidersize
    f_matrix(underlaynotconsiderindex) = -log((1/linkfailurematrix(underlaynotconsiderlist(underlaynotconsiderindex))) - 1);
    if f_matrix(underlaynotconsiderindex) < -1000
        f_matrix(underlaynotconsiderindex) = -1000;
    end
    underlaynotconsiderindex = underlaynotconsiderindex + 1;
end

abnormaloverlaysize = abnormaloverlayindex - 1;

A_matrix = zeros(abnormaloverlaysize, underlaynotconsidersize);
b_matrix = zeros(abnormaloverlaysize, 1);

abnormaloverlayindex = 1;
while abnormaloverlayindex <= abnormaloverlaysize
    underlaynotconsiderindex = 1;
    b_abnormal_total = 0;
    while underlaynotconsiderindex <= underlaynotconsidersize
        if overlay_underlay_matrix(abnormaloverlaynumlist(abnormaloverlayindex), underlaynotconsiderlist(underlaynotconsiderindex)) == 1
            A_matrix(abnormaloverlayindex, underlaynotconsiderindex) = 1;
            b_abnormal_total = b_abnormal_total + 1;
        end
        underlaynotconsiderindex = underlaynotconsiderindex + 1;
    end
    b_matrix(abnormaloverlayindex) = b_abnormal_total - 1;
    abnormaloverlayindex = abnormaloverlayindex + 1;
end

%underlaynotconsiderlist
%f_matrix
%A_matrix
%b_matrix

x_got_value = [];
[b_matrix_row, b_matrix_column] = size(b_matrix);
if b_matrix_row > 0 && b_matrix_column > 0
    %b_matrix
    %A_matrix
    %f_matrix
    x_got_value = bintprog(f_matrix, A_matrix, b_matrix);

underlaynotconsiderindex = 1;
while underlaynotconsiderindex <= underlaynotconsidersize
    if x_got_value(underlaynotconsiderindex) == 1
        normal_underlay_list(underlaynotconsiderlist(underlaynotconsiderindex)) = 1;
    end
    underlaynotconsiderindex = underlaynotconsiderindex + 1;
end








end

