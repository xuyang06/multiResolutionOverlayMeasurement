clc
clear
%file_path = 'D:\r100-1.alt'
file_path = 'D:\r10-0.txt'
%dimension_m = 100;
dimension_m = 10;
aim_link_num = 40;
%aim_link_num = 12;

%aim_link_num = 400;
%aim_link_num = 400;
[graph_matrix,link_matrix] = graphreader( file_path, dimension_m, aim_link_num );
%graph_matrix
%link_matrix
%overlay_m = 7;
%[ chosenoverlay ] = chooserandomoverlay( dimension_m, overlay_m )
%overlay_m = 40;
overlay_m = 7;
[ chosenoverlay ] = chooserandomoverlay( dimension_m, overlay_m );
%chosenoverlay = [5 8 4 3 10 7 9 2]
[ overlay_underlay_matrix, select_underlay_matrix] = exhaustpath( link_matrix, graph_matrix, chosenoverlay );
[ linkfailurematrix1, linkfailurematrix2, linkfailurematrix3 ] = linkfailureprob( select_underlay_matrix );
final_underlay_number = 0;
final_chosen_overlay_number = zeros(1, 3);
link_same_prob_final_failure_index1 = [0.0 0.0];
link_fail_false_negative_prob_final_failure_index1 = [0.0 0.0];
link_fail_false_positive_prob_final_failure_index1 = [0.0 0.0];
link_same_prob_final_failure_index2 = [0.0 0.0];
link_fail_false_negative_prob_final_failure_index2 = [0.0 0.0];
link_fail_false_positive_prob_final_failure_index2 = [0.0 0.0];
link_same_prob_final_failure_index3 = [0.0 0.0];
link_fail_false_negative_prob_final_failure_index3 = [0.0 0.0];
link_fail_false_positive_prob_final_failure_index3 = [0.0 0.0];


failure_index = 1;
while failure_index <= 3
    linkfailurematrix = [];
    if failure_index == 1
        linkfailurematrix = linkfailurematrix1;
    elseif failure_index == 2
        linkfailurematrix = linkfailurematrix2;
    else
        linkfailurematrix = linkfailurematrix3;
    end
    
    bayesian_infer_file_name = ['bayesian_infer_',int2str(failure_index),'.txt'];
    bayesian_infer_fid=fopen(bayesian_infer_file_name,'w+');
    
    [ new_link_matrix, overlay_cost_matrix ] = generatecoverno( link_matrix, overlay_underlay_matrix, select_underlay_matrix,  linkfailurematrix);
    %new_link_matrix
    %[q, r] =qr(overlay_underlay_matrix)
    [new_link_row, new_link_column] = size(new_link_matrix);
    if failure_index == 1
        final_underlay_number = new_link_row;
    end
    [ chosen_overlay_link_index_list, underlay_cover_real_link_index_list, chosen_overlay_link_index_list_column, total_chosen_overlay_cost] = greedyalgoselectoverlay( new_link_matrix, overlay_underlay_matrix, overlay_cost_matrix, linkfailurematrix);
    [chosen_overlay_link_index_list_row, chosen_overlay_link_index_list_column] = size(chosen_overlay_link_index_list);
    final_chosen_overlay_number(failure_index) = chosen_overlay_link_index_list_column;
    %total_chosen_overlay_cost
    %[ optimal_chosen_overlay_link_index_list, optimal_chosen_overlay_link_index_list_column, optimal_total_chosen_overlay_cost] = optimalalgoselectoverlay1( new_link_matrix, overlay_underlay_matrix, overlay_cost_matrix, linkfailurematrix, underlay_cover_real_link_index_list, chosen_overlay_link_index_list);
    %size(optimal_chosen_overlay_link_index_list)
    %optimal_total_chosen_overlay_cost
    
    [ linkfailurerangeprob_matrix] = generatelinkfailurerangeprob( linkfailurematrix );
    
%         if chosen_index == 2
%             chosen_overlay_link_index_list = [];
%             chosen_overlay_link_index_list = optimal_chosen_overlay_link_index_list;
%         end
%     
    total_simulation_number = 1000;
    total_same_number = [0 0];
        %accuracy_value = 0.0;
    simulation_index = 1;
    false_positive_total = [0.0 0.0];
    false_negative_total = [0.0 0.0];

    link_underlay_on_total = zeros(new_link_row, 2);
    link_underlay_false_negative_total = zeros(new_link_row, 2);
    link_underlay_false_positive_total = zeros(new_link_row, 2);


    link_same_prob = zeros(new_link_row, 2);
    link_fail_false_negative_prob = zeros(new_link_row, 2);
    link_fail_false_positive_prob = zeros(new_link_row, 2);
    link_false_negative_prob = zeros(new_link_row, 2);
    link_false_positive_prob = zeros(new_link_row, 2);



    while simulation_index <= total_simulation_number
        %simulation_index
        [ currentlinkmatrix, failurelinkindexmatrix] = generatelinkfailure( linkfailurematrix , linkfailurerangeprob_matrix);
        [ overlaylinkmatrix ] = generateoverlaycondition( currentlinkmatrix, chosen_overlay_link_index_list, overlay_underlay_matrix );
        chosen_index = 1;
        while chosen_index <= 2
            %[ normal_underlay_list ] = greedyinferunderlaywithprob( new_link_matrix, overlay_underlay_matrix, overlay_cost_matrix, linkfailurematrix, overlaylinkmatrix, chosen_overlay_link_index_list)
            normal_underlay_list = []; 
            
            if chosen_index == 1
                [ normal_underlay_list ] = optimalinferunderlaywithprob( new_link_matrix, overlay_underlay_matrix, overlay_cost_matrix, linkfailurematrix, overlaylinkmatrix, chosen_overlay_link_index_list);

            else
                threshold_p = 0.01;
                [ normal_underlay_list, bayesian_unknown_ranking_list] = bayesianinference( new_link_matrix, overlay_underlay_matrix, overlay_cost_matrix, linkfailurematrix, overlaylinkmatrix, chosen_overlay_link_index_list, threshold_p);
                fprintf(bayesian_infer_fid,'%d ',failurelinkindexmatrix);
                fprintf(bayesian_infer_fid,'\r\n');
                fprintf(bayesian_infer_fid,'%d ',bayesian_unknown_ranking_list);
                fprintf(bayesian_infer_fid,'\r\n');
            end
            
            [ same, false_positive, false_negative, link_underlay_ons, link_underlay_false_negatives, link_underlay_false_positives ] = calsame( currentlinkmatrix, normal_underlay_list);
            if same == 1
                total_same_number(chosen_index) = total_same_number(chosen_index) + 1;
            else
                false_positive_total(chosen_index) = false_positive_total(chosen_index) + false_positive;
                false_negative_total(chosen_index) = false_negative_total(chosen_index) + false_negative;
            end
            new_link_index = 1;
            while new_link_index <= new_link_row
                link_underlay_on_total(new_link_index, chosen_index) = link_underlay_on_total(new_link_index, chosen_index) + link_underlay_ons(new_link_index);
                link_underlay_false_negative_total(new_link_index, chosen_index) = link_underlay_false_negative_total(new_link_index, chosen_index) + link_underlay_false_negatives(new_link_index);
                link_underlay_false_positive_total(new_link_index, chosen_index) = link_underlay_false_positive_total(new_link_index, chosen_index) + link_underlay_false_positives(new_link_index);
                new_link_index = new_link_index + 1;
            end
            chosen_index = chosen_index + 1;
        end
        simulation_index = simulation_index + 1;
    end
    fclose(bayesian_infer_fid);
    same_prob = [0.0 0.0];
    false_positive_prob = [0.0 0.0];
    false_negative_prob = [0.0 0.0];
    link_same_prob_final = [0.0 0.0];
    link_fail_false_negative_prob_final = [0.0 0.0];
    link_fail_false_positive_prob_final = [0.0 0.0];
    link_false_negative_prob_final = [0.0 0.0];
    link_false_positive_prob_final = [0.0 0.0];
    
    
    chosen_index = 1;
    while chosen_index <= 2

        same_prob(chosen_index) = total_same_number(chosen_index)*1.0/total_simulation_number;
        false_positive_prob(chosen_index) = false_positive_total(chosen_index)*1.0/(total_simulation_number - total_same_number(chosen_index));
        false_negative_prob(chosen_index) = false_negative_total(chosen_index)*1.0/(total_simulation_number - total_same_number(chosen_index));

        new_link_index = 1;
        while new_link_index <= new_link_row
            link_fail_num =  link_underlay_false_negative_total(new_link_index, chosen_index) + link_underlay_false_positive_total(new_link_index, chosen_index);
            link_total_num = total_simulation_number;
            link_same_prob(new_link_index, chosen_index) = (link_total_num - link_fail_num)* 1.0/link_total_num;
            link_fail_false_negative_prob(new_link_index, chosen_index) = (link_underlay_false_negative_total(new_link_index, chosen_index))* 1.0/link_fail_num;
            link_fail_false_positive_prob(new_link_index, chosen_index) = (link_underlay_false_positive_total(new_link_index, chosen_index))* 1.0/link_fail_num;
            link_false_negative_prob(new_link_index, chosen_index) = (link_underlay_false_negative_total(new_link_index, chosen_index))* 1.0/link_total_num;
            link_false_positive_prob(new_link_index, chosen_index) = (link_underlay_false_positive_total(new_link_index, chosen_index))* 1.0/link_total_num;

            new_link_index = new_link_index + 1;
        end
        %link_same_prob
        link_same_prob_final(chosen_index) = mean(link_same_prob(:,chosen_index));
        %link_fail_false_negative_prob
        link_fail_false_negative_prob_final(chosen_index) = mean(link_fail_false_negative_prob(:,chosen_index));
        %link_fail_false_positive_prob
        link_fail_false_positive_prob_final(chosen_index) = mean(link_fail_false_positive_prob(:,chosen_index));
        %link_false_negative_prob
        link_false_negative_prob_final(chosen_index) = mean(link_false_negative_prob(:,chosen_index));
        %link_false_positive_prob
        link_false_positive_prob_final(chosen_index) = mean(link_false_positive_prob(:,chosen_index));
        
        
        chosen_index = chosen_index + 1;
    end
    
    %same_prob
    %false_positive_prob
    %false_negative_prob
    %link_same_prob_final
    %link_fail_false_negative_prob_final
    %link_fail_false_positive_prob_final
    %link_false_negative_prob_final
    %link_false_positive_prob_final
    
    
    if failure_index == 1
        link_same_prob_final_failure_index1 = link_same_prob_final;
        link_fail_false_negative_prob_final_failure_index1 = link_false_negative_prob_final;
        link_fail_false_positive_prob_final_failure_index1 = link_false_positive_prob_final;
    elseif failure_index == 2
        link_same_prob_final_failure_index2 = link_same_prob_final;
        link_fail_false_negative_prob_final_failure_index2 = link_false_negative_prob_final;
        link_fail_false_positive_prob_final_failure_index2 = link_false_positive_prob_final;
    else
        link_same_prob_final_failure_index3 = link_same_prob_final;
        link_fail_false_negative_prob_final_failure_index3 = link_false_negative_prob_final;
        link_fail_false_positive_prob_final_failure_index3 = link_false_positive_prob_final;
    end
    
    
    
        
        %chosen_index = chosen_index + 1;
    failure_index = failure_index + 1;   
end    

disp(sprintf('Finally, the underlay link number is %d.',final_underlay_number));
disp('For Loss Probability following Random Distribution:');
disp(sprintf('The chosen overlay link number is %d;',final_chosen_overlay_number(1)));
disp('When using Maximum Likelihood Estimation to infer underlay link:');
disp(sprintf('Link same probability is %f, false negative probability is %f, false positive probability is %f;',link_same_prob_final_failure_index1(1), link_fail_false_negative_prob_final_failure_index1(1), link_fail_false_positive_prob_final_failure_index1(1)));
disp('When using Bayesian Inference Model to infer underlay link:');
disp(sprintf('Link same probability is %f, false negative probability is %f, false positive probability is %f;',link_same_prob_final_failure_index1(2), link_fail_false_negative_prob_final_failure_index1(2), link_fail_false_positive_prob_final_failure_index1(2)));



disp('For Loss Probability following Normal Distribution:');
disp(sprintf('The chosen overlay link number is %d;',final_chosen_overlay_number(2)));
disp('When using Maximum Likelihood Estimation to infer underlay link:');
disp(sprintf('Link same probability is %f, false negative probability is %f, false positive probability is %f;',link_same_prob_final_failure_index2(1), link_fail_false_negative_prob_final_failure_index2(1), link_fail_false_positive_prob_final_failure_index2(1)));
disp('When using Bayesian Inference Model to infer underlay link:');
disp(sprintf('Link same probability is %f, false negative probability is %f, false positive probability is %f;',link_same_prob_final_failure_index2(2), link_fail_false_negative_prob_final_failure_index2(2), link_fail_false_positive_prob_final_failure_index2(2)));


disp('For Loss Probability following Step Distribution:');
disp(sprintf('The chosen overlay link number is %d;',final_chosen_overlay_number(3)));
disp('When using Maximum Likelihood Estimation to infer underlay link:');
disp(sprintf('Link same probability is %f, false negative probability is %f, false positive probability is %f;',link_same_prob_final_failure_index3(1), link_fail_false_negative_prob_final_failure_index3(1), link_fail_false_positive_prob_final_failure_index3(1)));
disp('When using Bayesian Inference Model to infer underlay link:');
disp(sprintf('Link same probability is %f, false negative probability is %f, false positive probability is %f;',link_same_prob_final_failure_index3(2), link_fail_false_negative_prob_final_failure_index3(2), link_fail_false_positive_prob_final_failure_index3(2)));



%end