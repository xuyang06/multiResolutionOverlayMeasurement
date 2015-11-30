function [ normal_underlay_list, bayesian_unknown_ranking_list] = bayesianinference3( new_link_matrix, overlay_underlay_matrix, overlay_cost_matrix, linkfailurematrix, overlaylinkmatrix, chosen_overlay_link_index_list, threshold_p)
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
%normal_underlay_list
%underlayconsiderlist
%abnormaloverlaylist
%abnormaloverlaynumlist



[temp, underlayconsiderlist_size] = size(underlayconsiderlist);
%overlay_underlay_matrix_column
%underlayconsiderlist_size

unknown_underlayconsiderlist_size = overlay_underlay_matrix_column - underlayconsiderlist_size;
%unknown_underlayconsiderlist_size

unknown_underlayconsiderlist_combination_len = 2.^unknown_underlayconsiderlist_size;

unknown_underlayconsider_index_list = zeros(unknown_underlayconsiderlist_size, 1);

underlay_index = 1;
unknown_underlayconsiderlist_index = 1;

while underlay_index <= overlay_underlay_matrix_column
    if ismember(underlay_index, underlayconsiderlist) == 0
        unknown_underlayconsider_index_list(unknown_underlayconsiderlist_index) = underlay_index;
        unknown_underlayconsiderlist_index = unknown_underlayconsiderlist_index + 1;
    end 
    underlay_index = underlay_index + 1;
end

%unknown_underlayconsiderlist_combination_len


unknown_underlay_prob_sum_ue0_matrix = zeros(unknown_underlayconsiderlist_size,1);
unknown_underlay_comb_tmp_matrix = zeros(unknown_underlayconsiderlist_size,1);

prob_ue0_sum = 0.0;



%unknown_underlay_comb_matrix = zeros(unknown_underlayconsiderlist_combination_len, unknown_underlayconsiderlist_size);
%unknown_underlay_comb_prob_matrix = zeros(unknown_underlayconsiderlist_combination_len, 1);


unknown_comb_index = 1;
[abnormaloverlaylist_row, abnormaloverlaylist_column] = size(abnormaloverlaylist);


zero_number_upper = 4;




if unknown_underlayconsiderlist_size <=10 
    while unknown_comb_index <= unknown_underlayconsiderlist_combination_len
        unknown_underlay_index = 1;
        unknown_underlay_prob = 1;
        numerator = unknown_comb_index - 1;
        current_zero_number = 0;
        while unknown_underlay_index <= unknown_underlayconsiderlist_size
            %unknown_underlayconsiderlist_size
            %unknown_underlay_index
            denominator = 2.^(unknown_underlayconsiderlist_size - unknown_underlay_index);
            %numerator
            %denominator
            floor_num = floor(numerator/denominator);
            numerator = rem(numerator, denominator);
            unknown_underlay_comb_tmp_matrix(unknown_underlay_index, 1) = floor_num;
            %unknown_underlay_comb_matrix(unknown_comb_index, unknown_underlay_index) = floor_num;
            if floor_num == 1
                unknown_underlay_prob = unknown_underlay_prob*(1- linkfailurematrix(unknown_underlayconsider_index_list(unknown_underlay_index)));
            else
                current_zero_number = current_zero_number + 1;
                unknown_underlay_prob = unknown_underlay_prob*(linkfailurematrix(unknown_underlayconsider_index_list(unknown_underlay_index)));
            end

            unknown_underlay_index = unknown_underlay_index + 1;
        end
        abnormaloverlaylist_index = 1;
        found_wrong_overlay = 0;
        while abnormaloverlaylist_index <= abnormaloverlaylist_row
            abnormaloverlaylink_index = abnormaloverlaylist(abnormaloverlaylist_index);
            unknown_underlayconsiderlist_index = 1;
            abnormaloverlaymultiply = 1;
            abnormaloverlayencoutered = 0;
            while unknown_underlayconsiderlist_index <= unknown_underlayconsiderlist_size
                if overlay_underlay_matrix(abnormaloverlaylink_index,unknown_underlayconsider_index_list(unknown_underlayconsiderlist_index)) == 1
                    abnormaloverlayencoutered = 1;
                    if unknown_underlay_comb_tmp_matrix( unknown_underlayconsiderlist_index, 1) == 0
                        abnormaloverlaymultiply = 0;
                        break
                    end
                end
                unknown_underlayconsiderlist_index = unknown_underlayconsiderlist_index + 1;
            end
            if (abnormaloverlayencoutered == 1) && (abnormaloverlaymultiply == 1)
                found_wrong_overlay = 1;
                break;
            end
            abnormaloverlaylist_index = abnormaloverlaylist_index + 1;
        end
        if (found_wrong_overlay == 0)
            prob_ue0_sum = prob_ue0_sum + unknown_underlay_prob;
            unknown_underlay_index = 1;
            while unknown_underlay_index <= unknown_underlayconsiderlist_size
                if (unknown_underlay_comb_tmp_matrix(unknown_underlay_index, 1)== 0)
                    unknown_underlay_prob_sum_ue0_matrix(unknown_underlay_index, 1) = unknown_underlay_prob_sum_ue0_matrix(unknown_underlay_index, 1) + unknown_underlay_prob;
                end
                unknown_underlay_index = unknown_underlay_index + 1;
            end
        end
        unknown_comb_index = unknown_comb_index + 1;    
    end
else
    underlay_index_matrix = zeros(unknown_underlayconsiderlist_size,1);
    underlay_index = 1;
    while underlay_index <= unknown_underlayconsiderlist_size
        underlay_index_matrix(underlay_index, 1) = underlay_index;
        underlay_index = underlay_index + 1;
    end
    zero_number_index = 1;
    while zero_number_index <= zero_number_upper
        choose_comb = nchoosek(underlay_index_matrix, zero_number_index);
        [choose_comb_row, choose_comb_column] = size(choose_comb);
        choose_comb_row_index = 1;
        while choose_comb_row_index <= choose_comb_row
            unknown_underlay_comb_tmp_matrix = ones(unknown_underlayconsiderlist_size,1);
            choose_comb_column_index = 1;
            while choose_comb_column_index <= choose_comb_column
                unknown_underlay_comb_tmp_matrix(choose_comb(choose_comb_row_index,choose_comb_column_index), 1) = 0;
                choose_comb_column_index = choose_comb_column_index + 1;
            end
            unknown_underlayconsiderlist_index = 1;
            unknown_underlay_prob = 1;
            while unknown_underlayconsiderlist_index <= unknown_underlayconsiderlist_size
                if unknown_underlay_comb_tmp_matrix(unknown_underlayconsiderlist_index, 1) == 1
                    unknown_underlay_prob = unknown_underlay_prob*(1- linkfailurematrix(unknown_underlayconsider_index_list(unknown_underlayconsiderlist_index)));
                else
                    unknown_underlay_prob = unknown_underlay_prob*(linkfailurematrix(unknown_underlayconsider_index_list(unknown_underlayconsiderlist_index)));
                end
                    
                unknown_underlayconsiderlist_index = unknown_underlayconsiderlist_index + 1;
            end
            abnormaloverlaylist_index = 1;
            found_wrong_overlay = 0;
            while abnormaloverlaylist_index <= abnormaloverlaylist_row
                abnormaloverlaylink_index = abnormaloverlaylist(abnormaloverlaylist_index);
                unknown_underlayconsiderlist_index = 1;
                abnormaloverlaymultiply = 1;
                abnormaloverlayencoutered = 0;
                while unknown_underlayconsiderlist_index <= unknown_underlayconsiderlist_size
                    if overlay_underlay_matrix(abnormaloverlaylink_index, unknown_underlayconsider_index_list(unknown_underlayconsiderlist_index)) == 1
                        abnormaloverlayencoutered = 1;
                        if unknown_underlay_comb_tmp_matrix( unknown_underlayconsiderlist_index, 1) == 0
                            abnormaloverlaymultiply = 0;
                            break
                        end
                    end
                    unknown_underlayconsiderlist_index = unknown_underlayconsiderlist_index + 1;
                end
                if (abnormaloverlayencoutered == 1) && (abnormaloverlaymultiply == 1)
                    found_wrong_overlay = 1;
                    break;
                end
                abnormaloverlaylist_index = abnormaloverlaylist_index + 1;
            end
            if (found_wrong_overlay == 0)
                prob_ue0_sum = prob_ue0_sum + unknown_underlay_prob;
                unknown_underlay_index = 1;
                while unknown_underlay_index <= unknown_underlayconsiderlist_size
                    if (unknown_underlay_comb_tmp_matrix(unknown_underlay_index, 1)== 0)
                        unknown_underlay_prob_sum_ue0_matrix(unknown_underlay_index, 1) = unknown_underlay_prob_sum_ue0_matrix(unknown_underlay_index, 1) + unknown_underlay_prob;
                    end
                    unknown_underlay_index = unknown_underlay_index + 1;
                end
            end
            choose_comb_row_index = choose_comb_row_index + 1;
        end
        zero_number_index = zero_number_index + 1;
    end
end





unknown_underlay_bayesian_prob_list = zeros(unknown_underlayconsiderlist_size, 1);
%unknown_underlay_bayesian_prob_list

unknown_underlayconsiderlist_index = 1;

while unknown_underlayconsiderlist_index <= unknown_underlayconsiderlist_size
%     unknown_comb_index = 1;
%     P_sum_ue0 = 0;
%     while unknown_comb_index <= unknown_underlayconsiderlist_combination_len
%         %unknown_underlay_index = unknown_underlayconsider_index_list(unknown_underlayconsiderlist_index);
%         if unknown_underlay_comb_matrix(unknown_comb_index, unknown_underlayconsiderlist_index) == 0
%             P_sum_ue0 = P_sum_ue0 + unknown_underlay_comb_prob_matrix(unknown_comb_index);
%         end
%         unknown_comb_index = unknown_comb_index + 1;
%     end
    P_sum_bayesian = unknown_underlay_prob_sum_ue0_matrix(unknown_underlayconsiderlist_index, 1)/prob_ue0_sum;
    unknown_underlay_bayesian_prob_list(unknown_underlayconsiderlist_index) = P_sum_bayesian;
    if P_sum_bayesian >= threshold_p
        normal_underlay_list(unknown_underlayconsider_index_list(unknown_underlayconsiderlist_index)) = 0;
    else
        normal_underlay_list(unknown_underlayconsider_index_list(unknown_underlayconsiderlist_index)) = 1;
    end
    unknown_underlayconsiderlist_index = unknown_underlayconsiderlist_index + 1;
end

[bayesian_prob_sorting, bayesian_prob_ranking] = sort(unknown_underlay_bayesian_prob_list);
bayesian_prob_decreasing_ranking = fliplr(bayesian_prob_ranking);

bayesian_unknown_ranking_list = zeros(1, unknown_underlayconsiderlist_size);
bayesian_unknown_ranking_i = 1;
while bayesian_unknown_ranking_i <= unknown_underlayconsiderlist_size
    bayesian_unknown_ranking_list(1,bayesian_unknown_ranking_i) = unknown_underlayconsider_index_list(bayesian_prob_decreasing_ranking(bayesian_unknown_ranking_i));
    bayesian_unknown_ranking_i = bayesian_unknown_ranking_i + 1;
end


%unknown_underlay_bayesian_prob_list

%clear







end



