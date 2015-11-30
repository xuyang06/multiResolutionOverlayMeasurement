function [ normal_underlay_list ] = greedyinferunderlaywithprob( new_link_matrix, overlay_underlay_matrix, overlay_cost_matrix, linkfailurematrix, overlaylinkmatrix, chosen_overlay_link_index_list)
%UNTITLED9 Summary of this function goes here
%   Detailed explanation goes here
%underlay_cover_link_index_list = new_link_matrix(:,4)
%chosen_overlay_link_index_list = []

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
normal_underlay_list
underlayconsiderlist
abnormaloverlaylist
abnormaloverlaynumlist

underlaylink_index = 1;
while underlaylink_index <= overlay_underlay_matrix_column
    if ismember(underlaylink_index, underlayconsiderlist) == 0
        if linkfailurematrix(underlaylink_index) >= 0.5
            
            underlayconsiderlist(underlayconsiderindex) = underlaylink_index;
            underlayconsiderindex = underlayconsiderindex + 1;
            underlayconditions = overlay_underlay_matrix(:,underlaylink_index);
            overlayconditions_index = 1;
            while overlayconditions_index <= overlay_underlay_matrix_row
                if underlayconditions(overlayconditions_index) == 1 
                    if ismember(overlayconditions_index, chosen_overlay_link_index_list) == 1
                        if ismember(overlayconditions_index, abnormaloverlaynumlist) == 1
                            %underlayconditions
                            %overlayconditions_index
                            %abnormaloverlaynumlist
                            [ abnormal_index ] = findindexinmatrix( overlayconditions_index, abnormaloverlaynumlist);
                            abnormaloverlaylist(abnormal_index) = [];
                            abnormaloverlaynumlist(abnormal_index) = [];
                            abnormaloverlayindex = abnormaloverlayindex - 1;
                        end
                    end
                end
                overlayconditions_index = overlayconditions_index + 1;
            end
        end
    end
    %(underlaylink_index)
    underlaylink_index = underlaylink_index + 1;
end
normal_underlay_list
underlayconsiderlist
abnormaloverlaylist
abnormaloverlaynumlist


while 1
    if abnormaloverlayindex <= 0
        abnormaloverlayindex
        if underlayconsiderindex < overlay_underlay_matrix_column
            underlay_index = 1;
            while underlay_index <= overlay_underlay_matrix_column
                if ismember(underlay_index, underlayconsiderlist) == 0
                    normal_underlay_list(underlay_index) = 1
                end
                underlay_index = underlay_index + 1;
            end
        end
        break
    end
    if underlayconsiderindex > overlay_underlay_matrix_column
        underlayconsiderindex
        break
    end
    underlayindex = 1;
    underlay_link_score_matrix = zeros(1,overlay_underlay_matrix_column);
    while underlayindex <= overlay_underlay_matrix_column
        if ismember(underlayindex, underlayconsiderlist) == 0
            underlayindex
            [abnormaloverlaylist_row, abnormaloverlaylist_column] = size(abnormaloverlaylist)
            %abnormaloverlaylist_size = size(abnormaloverlaylist)
            abnormaloverlaylist_index = 1;
            underlay_link_score_upper = 0;
            %underlaylinkconditions = overlay_underlay_matrix(:,underlayindex);
            while abnormaloverlaylist_index <= abnormaloverlaylist_row
                if overlay_underlay_matrix(abnormaloverlaylist(abnormaloverlaylist_index),underlayindex) == 1
                    underlay_link_score_upper = underlay_link_score_upper + 1;
                end
                %if overlay_underlay_matrix(,) 
                abnormaloverlaylist_index = abnormaloverlaylist_index + 1;
            end
            underlay_link_score = (underlay_link_score_upper*1.0)/log((1/linkfailurematrix(underlayindex)) - 1);
            underlay_link_score_matrix(underlayindex) = underlay_link_score
        end
        underlayindex = underlayindex + 1;
    end
    underlay_link_score_matrix
    chosen_underlink_index = 1;
    underlayindex = 1;
    while underlayindex <= overlay_underlay_matrix_column
        if underlay_link_score_matrix(underlayindex) > underlay_link_score_matrix(chosen_underlink_index)
            chosen_underlink_index = underlayindex;
        end
        underlayindex = underlayindex + 1;
    end
    if ismember(chosen_underlink_index, underlayconsiderlist) == 0
        underlayconsiderlist(underlayconsiderindex) = chosen_underlink_index;
        normal_underlay_list(chosen_underlink_index) = 0
        underlayconsiderindex = underlayconsiderindex + 1;
    else
        break
    end
    
    chosen_underlink_overlay_l = overlay_underlay_matrix(:,chosen_underlink_index);
    chosen_underlink_overlay_l_index = 1;
    while chosen_underlink_overlay_l_index <= overlay_underlay_matrix_row
        if chosen_underlink_overlay_l(chosen_underlink_overlay_l_index) == 1
            if ismember(chosen_underlink_overlay_l_index, chosen_overlay_link_index_list) == 1
                if ismember(overlayconditions_index, abnormaloverlaynumlist) == 1
                    [ abnormal_index ] = findindexinmatrix( overlayconditions_index, abnormaloverlaynumlist);
                    abnormaloverlaylist(abnormal_index) = [];
                    abnormaloverlaynumlist(abnormal_index) = [];
                    %abnormaloverlaylist(overlayconditions_index) = [];
                    abnormaloverlayindex = abnormaloverlayindex - 1;
                end
            end
        end
        chosen_underlink_overlay_l_index = chosen_underlink_overlay_l_index + 1;
    
    end
    
    normal_underlay_list
    underlayconsiderlist
    abnormaloverlaylist
    abnormaloverlaynumlist

    
        
    
end





% chosen_overlay_link_insert_index = 1;
% %overlay_cost = 0;
% while 1
%     overlay_link_cost_matrix = zeros(1,overlay_underlay_matrix_row);
%     overlay_link_index = 1;
%     while overlay_link_index <= overlay_underlay_matrix_row
%         if ismember(overlay_link_index, chosen_overlay_link_index_list)
%             overlay_link_index = overlay_link_index + 1;
%             continue
%         end
%         %underlay_link_index = 1
%         overlay_cost = 0;
%         underlay_link_index = 1;
%         while underlay_link_index <= new_link_matrix_row
%             if overlay_underlay_matrix(overlay_link_index, underlay_link_index) == 1
%                 if underlay_cover_link_index_list(underlay_link_index) > 0
%                     overlay_cost = overlay_cost + 1;
%                 end
%             end
%             underlay_link_index = underlay_link_index + 1;
%         end
%         overlay_cost = (overlay_cost*1.0)/overlay_cost_matrix(overlay_link_index);
%         overlay_link_cost_matrix(overlay_link_index) = overlay_cost;
%         overlay_link_index = overlay_link_index + 1;
%     end
%     %score_l = ()*1.0/overlay_cost_matrix(overlay_link_index);
%     choose_overlay_link_index = 1;
%     overlay_link_scan_index = 1;
%     while overlay_link_scan_index <= overlay_underlay_matrix_row
%         if overlay_link_cost_matrix(overlay_link_scan_index) > overlay_link_cost_matrix(choose_overlay_link_index)
%             choose_overlay_link_index = overlay_link_scan_index;
%         end
%         overlay_link_scan_index = overlay_link_scan_index + 1; 
%     end
%     if abs(overlay_link_cost_matrix(choose_overlay_link_index)-0) < 0.001
%         break;
%     end
%     chosen_overlay_link_index_list(chosen_overlay_link_insert_index) = choose_overlay_link_index;
%     underlay_link_index = 1;
%     %choose_overlay_link_index
%     while underlay_link_index <= new_link_matrix_row
%         if overlay_underlay_matrix(choose_overlay_link_index, underlay_link_index) == 1
%             if underlay_cover_link_index_list(underlay_link_index) > 0
%                 underlay_cover_link_index_list(underlay_link_index) = underlay_cover_link_index_list(underlay_link_index) - 1;
%             end
%         end
%         underlay_link_index = underlay_link_index + 1;
%     end
%     if (size(chosen_overlay_link_index_list) == size(overlay_cost_matrix))
%         break
%     end
%     chosen_overlay_link_insert_index = chosen_overlay_link_insert_index + 1;
% 
% end



end



