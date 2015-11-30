function [ node_num_matrix, overlay_link_num_matrix, overlay_link_same_matrix,overlay_link_false_positive_matrix,overlay_link_false_negative_matrix, greedy_same_matrix, greedy_false_positive_matrix, greedy_false_negative_matrix, greedy_unknown_matrix, bayesian_same_matrix,bayesian_false_positive_matrix, bayesian_false_negative_matrix, bayesian_unknown_matrix, greedy_same_matrix_withoutuk, greedy_false_positive_matrix_withoutuk, greedy_false_negative_matrix_withoutuk, bayesian_same_matrix_withoutuk,bayesian_false_positive_matrix_withoutuk, bayesian_false_negative_matrix_withoutuk] = analysisresultreader( file_path)
%graph_matrix = zeros(dimension_m);
%link_matrix = [];
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
file_fid = fopen(file_path, 'r');
total_index = 0;
while ~feof(file_fid)
    tline = fgetl(file_fid);
    total_index = total_index + 1;
end
%tline = fgetl(file_fid);
%[overlay_node_number, overlay_link_number] = strread(tline, '%d %d');
total_round_number = total_index/6;
fclose(file_fid);
node_num_matrix = zeros(total_round_number,1);
overlay_link_num_matrix = zeros(total_round_number,1);

overlay_link_same_matrix = zeros(total_round_number,1);
overlay_link_false_positive_matrix = zeros(total_round_number,1);
overlay_link_false_negative_matrix = zeros(total_round_number,1);

greedy_same_matrix = zeros(total_round_number,1);
greedy_false_positive_matrix = zeros(total_round_number,1);
greedy_false_negative_matrix = zeros(total_round_number,1);
greedy_unknown_matrix = zeros(total_round_number,1);

bayesian_same_matrix = zeros(total_round_number,1);
bayesian_false_positive_matrix = zeros(total_round_number,1);
bayesian_false_negative_matrix = zeros(total_round_number,1);
bayesian_unknown_matrix = zeros(total_round_number,1);


greedy_same_matrix_withoutuk = zeros(total_round_number,1);
greedy_false_positive_matrix_withoutuk = zeros(total_round_number,1);
greedy_false_negative_matrix_withoutuk = zeros(total_round_number,1);

bayesian_same_matrix_withoutuk = zeros(total_round_number,1);
bayesian_false_positive_matrix_withoutuk = zeros(total_round_number,1);
bayesian_false_negative_matrix_withoutuk = zeros(total_round_number,1);



file_fid = fopen(file_path, 'r');
count = 0;
round_index = 1;
while ~feof(file_fid)
    tline = fgetl(file_fid);
    if ~isempty(tline)
        if count == 0
            [node_num, overlay_link_num] = strread(tline, '%d %d')
            node_num_matrix(round_index) = node_num;
            overlay_link_num_matrix(round_index) = overlay_link_num;
            count = count + 1;
        elseif count == 1
            [overlay_link_same, overlay_link_false_positive, overlay_link_false_negative] = strread(tline, '%f %f %f');
            overlay_link_same_matrix(round_index) = overlay_link_same;
            overlay_link_false_positive_matrix(round_index) = overlay_link_false_positive;
            overlay_link_false_negative_matrix(round_index) = overlay_link_false_negative;
            count = count + 1;
        elseif count == 2
            [greedy_same, greedy_false_positive, greedy_false_negative, greedy_unknown] = strread(tline, '%f %f %f %f');
            greedy_same_matrix(round_index) = greedy_same;
            greedy_false_positive_matrix(round_index) = greedy_false_positive;
            greedy_false_negative_matrix(round_index) = greedy_false_negative;
            greedy_unknown_matrix(round_index) = greedy_unknown;
            count = count + 1;
        elseif count == 3
            [bayesian_same, bayesian_false_positive, bayesian_false_negative, bayesian_unknown] = strread(tline, '%f %f %f %f');
            bayesian_same_matrix(round_index) = bayesian_same;
            bayesian_false_positive_matrix(round_index) = bayesian_false_positive;
            bayesian_false_negative_matrix(round_index) = bayesian_false_negative;
            bayesian_unknown_matrix(round_index) = bayesian_unknown;
            count = count + 1;
        elseif count == 4
            [greedy_same, greedy_false_positive, greedy_false_negative] = strread(tline, '%f %f %f');
            greedy_same_matrix_withoutuk(round_index) = greedy_same;
            greedy_false_positive_matrix_withoutuk(round_index) = greedy_false_positive;
            greedy_false_negative_matrix_withoutuk(round_index) = greedy_false_negative;
            count = count + 1;
        elseif count == 5
            [bayesian_same, bayesian_false_positive, bayesian_false_negative] = strread(tline, '%f %f %f');
            bayesian_same_matrix_withoutuk(round_index) = bayesian_same;
            bayesian_false_positive_matrix_withoutuk(round_index) = bayesian_false_positive;
            bayesian_false_negative_matrix_withoutuk(round_index) = bayesian_false_negative;
            count = 0;
            round_index = round_index + 1;
        elseif round_index > total_round_number
            break;
        end
    end 
end


fclose(file_fid);



end



