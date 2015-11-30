function [ same, false_positive, false_negative, link_underlay_ons, link_underlay_false_negatives, link_underlay_false_positives ] = calsame( currentlinkmatrix, normal_underlay_list)

[link_matrix_row, link_matrix_column] = size(currentlinkmatrix);

link_underlay_ons = zeros(link_matrix_row, 1);
link_underlay_false_negatives = zeros(link_matrix_row, 1);
link_underlay_false_positives = zeros(link_matrix_row, 1);


link_matrix_index = 1;
accuracy_amount = 0;
same = 1;
false_positive_total = 0.0;
normal_total = 0.0;
false_negative_total = 0.0;
abnormal_total = 0.0;
while link_matrix_index <= link_matrix_row
    if currentlinkmatrix(link_matrix_index) == 1
        link_underlay_ons(link_matrix_index) = 1;
        normal_total = normal_total + 1;
        if currentlinkmatrix(link_matrix_index) ~= normal_underlay_list(link_matrix_index)
            false_negative_total = false_negative_total + 1;
            link_underlay_false_negatives(link_matrix_index) = 1;
            same = 0;
        end
    else
        abnormal_total = abnormal_total + 1;
        if currentlinkmatrix(link_matrix_index) ~= normal_underlay_list(link_matrix_index)
            false_positive_total = false_positive_total + 1;
            link_underlay_false_positives(link_matrix_index) = 1;
            same = 0;
        end
    end
    
%     if currentlinkmatrix(link_matrix_index) == normal_underlay_list(link_matrix_index)
%         
%         accuracy_amount = accuracy_amount + 1;
%     else
%         same = 0;
%     end
    link_matrix_index = link_matrix_index + 1;
end
if same == 1
    false_positive = 0.0;
    false_negative = 0.0;
else
    false_positive = false_positive_total*1.0/abnormal_total;
    false_negative = false_negative_total*1.0/normal_total;
    %accuracy = accuracy_amount*1.0/link_matrix_row;
end



%UNTITLED16 Summary of this function goes here
%   Detailed explanation goes here


end

