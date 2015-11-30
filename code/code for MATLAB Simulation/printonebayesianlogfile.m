function [ output_args ] = printonebayesianlogfile( bayesianlogfile_name, total_underlay_link_num, K_upper)
%UNTITLED9 Summary of this function goes here
%   Detailed explanation goes here


%bayesianlogfile_name = 'result/bayesian_infer_2.txt';
%bayesianlogfile_name = 'bayesian_infer_2.txt';
%total_underlay_link_num = 176;
%total_underlay_link_num = 18;
simulation_num = 1000;
K_num = 0;

%K_upper = 16;
K_index_list = zeros(K_upper+1, 1);
link_same_prob_final_list = zeros(K_upper+1, 1);
link_false_positive_prob_final_list = zeros(K_upper+1, 1);
link_false_negative_prob_final_list = zeros(K_upper+1, 1);

while K_num <= K_upper
    K_index_list(K_num+1) = K_num;
    K_num = K_num + 1;
end

K_num = 0;

while K_num <= K_upper
   [link_same_prob_final, link_false_positive_prob_final, link_false_negative_prob_final] = readonebayesianlogfile( bayesianlogfile_name, total_underlay_link_num, simulation_num, K_num );
   link_same_prob_final_list(K_num+1) = link_same_prob_final;
   link_false_positive_prob_final_list(K_num+1) = link_false_positive_prob_final;
   link_false_negative_prob_final_list(K_num+1) = link_false_negative_prob_final;
   K_num = K_num + 1;
end

figure(1)
plot(K_index_list, link_same_prob_final_list,'LineStyle','-','Color','m','LineWidth',2);
hold on 
plot(K_index_list, link_false_positive_prob_final_list,'LineStyle','-','Color','c','LineWidth',2);
plot(K_index_list, link_false_negative_prob_final_list,'LineStyle','-','Color','k','LineWidth',2);
hold off

set(gca,'FontSize',14)
% plot(K_num,dense_multicast_rate,'LineStyle','-','Color','m','LineWidth',2)
% hold on
% plot(user_num,dense_server_rate,'LineStyle','-','Color','c','LineWidth',2)
% plot(user_num,sparse_multicast_rate,'LineStyle','-','Color','k','LineWidth',2)
% plot(user_num,sparse_server_rate,'LineStyle','-','Color','g','LineWidth',2)
%stairs(rate5,rateProb5,'LineStyle','-.','Color','r','LineWidth',2)
%stairs(rate6,rateProb6,'LineStyle',':','Color','b','LineWidth',2)
%stairs(rate7,rateProb7,'y:','LineWidth',2)
%xlim([100 1600].*1000);
xlim([0 K_upper])
ylim([0 1])
h_legend = legend('Correct Prob', 'False Positive Prob','False Negative Prob')
set(h_legend,'FontSize',14);
%hold off
xlabel('K Number','FontSize',14)
ylabel('Probability','FontSize',14)


end

