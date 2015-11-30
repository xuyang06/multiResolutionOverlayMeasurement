clc
file_path = 'D:\Research\MultiOverlay\New folder\analysis_result'

[ node_num_matrix, overlay_link_num_matrix, overlay_link_same_matrix,overlay_link_false_positive_matrix,overlay_link_false_negative_matrix, greedy_same_matrix, greedy_false_positive_matrix, greedy_false_negative_matrix, greedy_unknown_matrix, bayesian_same_matrix,bayesian_false_positive_matrix, bayesian_false_negative_matrix, bayesian_unknown_matrix, greedy_same_matrix_withoutuk, greedy_false_positive_matrix_withoutuk, greedy_false_negative_matrix_withoutuk, bayesian_same_matrix_withoutuk,bayesian_false_positive_matrix_withoutuk, bayesian_false_negative_matrix_withoutuk ] = analysisresultreader( file_path)




figure(1)
plot(node_num_matrix, overlay_link_num_matrix,'LineStyle','-','Color','m','LineWidth',2);


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
%xlim([0 K_upper])
%ylim([0 1])
%h_legend = legend('Correct Prob', 'False Positive Prob','False Negative Prob')
%set(h_legend,'FontSize',14);
%hold off
xlabel('Node Number','FontSize',14)
ylabel('Overlay Link Number','FontSize',14)
title('Overlay Link Number')

figure(2)
plot(node_num_matrix, overlay_link_same_matrix,'LineStyle','-','Color','m','LineWidth',2);
hold on 
plot(node_num_matrix, overlay_link_false_positive_matrix,'LineStyle','-','Color','c','LineWidth',2);
plot(node_num_matrix, overlay_link_false_negative_matrix,'LineStyle','-','Color','k','LineWidth',2);
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
%xlim([0 K_upper])
%ylim([0 1])
h_legend = legend('Correct Prob', 'False Positive Prob','False Negative Prob')
set(h_legend,'FontSize',14);
%hold off
xlabel('Node Number','FontSize',14)
ylabel('Probability','FontSize',14)
title('Overlay Link Inference')

figure(3)
plot(node_num_matrix, greedy_same_matrix,'LineStyle','-','Color','m','LineWidth',2);
hold on 
plot(node_num_matrix, greedy_false_positive_matrix,'LineStyle','-','Color','c','LineWidth',2);
plot(node_num_matrix, greedy_false_negative_matrix,'LineStyle','-','Color','k','LineWidth',2);
plot(node_num_matrix, greedy_unknown_matrix,'LineStyle','-','Color','b','LineWidth',2);
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
%xlim([0 K_upper])
%ylim([0 1])
h_legend = legend('Correct Prob', 'False Positive Prob','False Negative Prob', 'Unknown Prob')
set(h_legend,'FontSize',14);
%hold off
xlabel('Node Number','FontSize',14)
ylabel('Probability','FontSize',14)
title('Maximum Likelihood Estimation')

figure(4)
plot(node_num_matrix, bayesian_same_matrix,'LineStyle','-','Color','m','LineWidth',2);
hold on 
plot(node_num_matrix, bayesian_false_positive_matrix,'LineStyle','-','Color','c','LineWidth',2);
plot(node_num_matrix, bayesian_false_negative_matrix,'LineStyle','-','Color','k','LineWidth',2);
plot(node_num_matrix, bayesian_unknown_matrix,'LineStyle','-','Color','b','LineWidth',2);
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
%xlim([0 K_upper])
%ylim([0 1])
h_legend = legend('Correct Prob', 'False Positive Prob','False Negative Prob', 'Unknown Prob')
set(h_legend,'FontSize',14);
%hold off
xlabel('Node Number','FontSize',14)
ylabel('Probability','FontSize',14)
title('Bayesian Inference Model')



figure(5)
plot(node_num_matrix, greedy_same_matrix_withoutuk,'LineStyle','-','Color','m','LineWidth',2);
hold on 
plot(node_num_matrix, greedy_false_positive_matrix_withoutuk,'LineStyle','-','Color','c','LineWidth',2);
plot(node_num_matrix, greedy_false_negative_matrix_withoutuk,'LineStyle','-','Color','k','LineWidth',2);
%plot(node_num_matrix, greedy_unknown_matrix,'LineStyle','-','Color','b','LineWidth',2);
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
%xlim([0 K_upper])
%ylim([0 1])
h_legend = legend('Correct Prob', 'False Positive Prob','False Negative Prob')
set(h_legend,'FontSize',14);
%hold off
xlabel('Node Number','FontSize',14)
ylabel('Probability','FontSize',14)
title('Maximum Likelihood Estimation without Unknown')
ylim([0 1])

figure(6)
plot(node_num_matrix, bayesian_same_matrix_withoutuk,'LineStyle','-','Color','m','LineWidth',2);
hold on 
plot(node_num_matrix, bayesian_false_positive_matrix_withoutuk,'LineStyle','-','Color','c','LineWidth',2);
plot(node_num_matrix, bayesian_false_negative_matrix_withoutuk,'LineStyle','-','Color','k','LineWidth',2);
%plot(node_num_matrix, bayesian_unknown_matrix,'LineStyle','-','Color','b','LineWidth',2);
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
%xlim([0 K_upper])
%ylim([0 1])
h_legend = legend('Correct Prob', 'False Positive Prob','False Negative Prob')
set(h_legend,'FontSize',14);
%hold off
xlabel('Node Number','FontSize',14)
ylabel('Probability','FontSize',14)
title('Bayesian Inference Model without Unknown')
ylim([0 1])










