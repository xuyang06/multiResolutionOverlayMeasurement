function [ chosen_overlay_link_index_list, underlay_cover_real_link_index_list, chosen_overlay_link_index_list_column, total_chosen_overlay_cost ] = greedyalgoselectoverlay( new_link_matrix, overlay_underlay_matrix, overlay_cost_matrix, linkfailurematrix)
%UNTITLED9 Summary of this function goes here
%   Detailed explanation goes here

underlay_cover_link_index_list = new_link_matrix(:,4);




chosen_overlay_link_index_list = [];

[overlay_underlay_matrix_row, overlay_underlay_matrix_column] = size(overlay_underlay_matrix);
[new_link_matrix_row, new_link_matrix_column] = size(new_link_matrix);

underlay_cover_real_link_index_list = zeros(overlay_underlay_matrix_column, 1);
underlay_cover_intend_link_index_list = new_link_matrix(:,4);


%underlay_consider_list = zeros(1,overlay_underlay_matrix_column);


chosen_overlay_link_insert_index = 1;
%overlay_cost = 0;
while 1
    overlay_link_cost_matrix = zeros(1,overlay_underlay_matrix_row);
    overlay_link_failure_matrix = zeros(1,overlay_underlay_matrix_row);
    
    overlay_link_index = 1;
    while overlay_link_index <= overlay_underlay_matrix_row
        if ismember(overlay_link_index, chosen_overlay_link_index_list) == 1
            overlay_link_index = overlay_link_index + 1;
            continue
        end
        %underlay_link_index = 1
        overlay_cost = 0;
        overlay_failure_sum = 0.0;
        underlay_link_index = 1;
        while underlay_link_index <= new_link_matrix_row
            if overlay_underlay_matrix(overlay_link_index, underlay_link_index) == 1
                if underlay_cover_link_index_list(underlay_link_index) > 0
                    overlay_cost = overlay_cost + 1;
                    overlay_failure_sum = overlay_failure_sum + linkfailurematrix(underlay_link_index);
                end
            end
            underlay_link_index = underlay_link_index + 1;
        end
        overlay_cost = (overlay_cost*1.0)/overlay_cost_matrix(overlay_link_index);
        overlay_link_cost_matrix(overlay_link_index) = overlay_cost;
        overlay_link_failure_matrix(overlay_link_index) = overlay_failure_sum;
        overlay_link_index = overlay_link_index + 1;
    end
    %score_l = ()*1.0/overlay_cost_matrix(overlay_link_index);
    choose_overlay_link_index = 1;
    overlay_link_scan_index = 1;
    choose_overlay_cost = 0.0;
    while overlay_link_scan_index <= overlay_underlay_matrix_row
        if overlay_link_cost_matrix(overlay_link_scan_index) > overlay_link_cost_matrix(choose_overlay_link_index)
            choose_overlay_link_index = overlay_link_scan_index;
        end
        overlay_link_scan_index = overlay_link_scan_index + 1; 
    end
    if abs(overlay_link_cost_matrix(choose_overlay_link_index)) < 0.001
        break;
    end
    choose_overlay_cost = overlay_link_cost_matrix(choose_overlay_link_index);
    
    overlay_link_scan_index = 1;
    while overlay_link_scan_index <= overlay_underlay_matrix_row
        if abs(overlay_link_cost_matrix(overlay_link_scan_index) - overlay_link_cost_matrix(choose_overlay_link_index)) <= 0.001
            if overlay_link_failure_matrix(overlay_link_scan_index) < overlay_link_failure_matrix(choose_overlay_link_index)
                choose_overlay_link_index = overlay_link_scan_index;
            end
        end
        overlay_link_scan_index = overlay_link_scan_index + 1;
    end
    
    
    chosen_overlay_link_index_list(chosen_overlay_link_insert_index) = choose_overlay_link_index;
    underlay_link_index = 1;
    %choose_overlay_link_index
    while underlay_link_index <= new_link_matrix_row
        if overlay_underlay_matrix(choose_overlay_link_index, underlay_link_index) == 1
            if underlay_cover_link_index_list(underlay_link_index) > 0
                underlay_cover_link_index_list(underlay_link_index) = underlay_cover_link_index_list(underlay_link_index) - 1;
            end
        end
        underlay_link_index = underlay_link_index + 1;
    end
    if (size(chosen_overlay_link_index_list) == size(overlay_cost_matrix))
        break
    end
    chosen_overlay_link_insert_index = chosen_overlay_link_insert_index + 1;

end


underlay_link_index = 1; 
while underlay_link_index <= overlay_underlay_matrix_column
    underlay_cover_real_link_index_list(underlay_link_index) = underlay_cover_intend_link_index_list(underlay_link_index) - underlay_cover_link_index_list(underlay_link_index);
    underlay_link_index = underlay_link_index + 1;
end

total_chosen_overlay_cost = 0.0;

[chosen_overlay_link_index_list_row, chosen_overlay_link_index_list_column] = size(chosen_overlay_link_index_list);

chosen_overlay_link_index = 1;
while chosen_overlay_link_index <= chosen_overlay_link_index_list_column
    total_chosen_overlay_cost = total_chosen_overlay_cost + overlay_cost_matrix(chosen_overlay_link_index_list(chosen_overlay_link_index));
    chosen_overlay_link_index = chosen_overlay_link_index + 1;
end

end

