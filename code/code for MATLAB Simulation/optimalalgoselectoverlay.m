function [ chosen_overlay_link_index_list,  chosen_overlay_link_index_list_column, total_chosen_overlay_cost ] = optimalalgoselectoverlay1( new_link_matrix, overlay_underlay_matrix, overlay_cost_matrix, linkfailurematrix, underlay_cover_real_link_index_list, old_chosen_overlay_link_index_list)
%UNTITLED9 Summary of this function goes here
%   Detailed explanation goes here

underlay_cover_link_index_list = new_link_matrix(:,4);
chosen_overlay_link_index_list = [];

[overlay_underlay_matrix_row, overlay_underlay_matrix_column] = size(overlay_underlay_matrix);
[new_link_matrix_row, new_link_matrix_column] = size(new_link_matrix);


%underlay_consider_list = zeros(1,overlay_underlay_matrix_column);



%f_matrix = zeros(1, );

A_matrix = zeros(overlay_underlay_matrix_column, overlay_underlay_matrix_row);
b_matrix = zeros(overlay_underlay_matrix_column, 1);

f_matrix = overlay_cost_matrix;

underlay_index = 1;

%overlay_underlay_matrix_column


while underlay_index <= overlay_underlay_matrix_column
    overlay_index = 1;
    while overlay_index <= overlay_underlay_matrix_row
        if overlay_underlay_matrix(overlay_index, underlay_index) == 1
            A_matrix( underlay_index, overlay_index) = -1;
        end
        overlay_index = overlay_index + 1;
    end
    b_matrix(underlay_index) = -underlay_cover_real_link_index_list(underlay_index);
    underlay_index = underlay_index + 1;
end


%size(f_matrix)
%f_matrix
%A_matrix
%b_matrix



options = optimoptions('lsqlin','MaxIter', 150000)



x0 = zeros(overlay_underlay_matrix_row, 1);

overlay_index = 1;

while overlay_index <= overlay_underlay_matrix_row
    if ismember(overlay_index, old_chosen_overlay_link_index_list) == 1
        x0(overlay_index) = 1;
    end
    overlay_index = overlay_index + 1;
end


x_got_value = bintprog(f_matrix, A_matrix, b_matrix,[], [],x0, options);


chosen_overlay_link_insert_index = 1;
chosen_overlay_link_index = 1;

while chosen_overlay_link_insert_index <= overlay_underlay_matrix_row
    if x_got_value(chosen_overlay_link_insert_index) == 1
        %A_matrix()
        chosen_overlay_link_index_list(chosen_overlay_link_index) = chosen_overlay_link_insert_index;
        chosen_overlay_link_index = chosen_overlay_link_index + 1;
    end
    chosen_overlay_link_insert_index = chosen_overlay_link_insert_index + 1;
end


total_chosen_overlay_cost = 0.0;

[chosen_overlay_link_index_list_row, chosen_overlay_link_index_list_column] = size(chosen_overlay_link_index_list);

chosen_overlay_link_index = 1;
while chosen_overlay_link_index <= chosen_overlay_link_index_list_column
    total_chosen_overlay_cost = total_chosen_overlay_cost + overlay_cost_matrix(chosen_overlay_link_index_list(chosen_overlay_link_index));
    chosen_overlay_link_index = chosen_overlay_link_index + 1;
end

% 
%     
%     
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

end





