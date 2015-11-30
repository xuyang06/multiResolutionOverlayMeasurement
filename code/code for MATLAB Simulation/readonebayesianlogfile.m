function [ link_same_prob_final, link_false_positive_prob_final, link_false_negative_prob_final] = readonebayesianlogfile( bayesianlogfile_name, total_underlay_link_num, simulation_num, K_num )
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

fid=fopen(bayesianlogfile_name, 'r');

line_index = 1;
error_line = [];
ranking_line = [];

total_sim_underlay_link_num = total_underlay_link_num*simulation_num;
total_same_num = 0;
total_false_positive_total = 0;
total_false_negative_total = 0;


link_same_prob_final = 0.0;
link_false_positive_prob_final = 0.0;
link_false_negative_prob_final = 0.0;
    



while 1
    tline=fgetl(fid);
    if ~ischar(tline)
        break
    end
    if rem(line_index, 2) == 1
        error_line = str2num(tline);
    else
        ranking_line = str2num(tline);
        [ranking_line_row, ranking_line_column] = size(ranking_line);
        consider_list_len = min(ranking_line_column, K_num);
        ranking_consider_i = 1;
        while ranking_consider_i <= consider_list_len
            consider_ranking_item = ranking_line(ranking_consider_i);
            if ismember(consider_ranking_item, error_line) == 1
                error_line_i = find(error_line==consider_ranking_item);
                error_line(error_line_i) = [];
            else
                total_false_positive_total = total_false_positive_total + 1;
            end
            ranking_consider_i = ranking_consider_i + 1;
        end
        [error_line_row, error_line_column] = size(error_line);
        total_false_negative_total = total_false_negative_total + error_line_column;
        %total_same_num = total_same_num + total_underlay_link_num - total_false_positive_total - total_false_negative_total;
    end
    
    %tline=str2num(tline);
    %tline
    %best_data = [best_data;tline];
    line_index = line_index + 1;
end

link_false_positive_prob_final = total_false_positive_total*1.0/total_sim_underlay_link_num;
link_false_negative_prob_final = total_false_negative_total*1.0/total_sim_underlay_link_num;
link_same_prob_final = 1 - link_false_positive_prob_final - link_false_negative_prob_final;


end

