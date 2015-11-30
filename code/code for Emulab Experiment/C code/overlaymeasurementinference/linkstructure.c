/*
 * linkstructure.c
 *
 *  Created on: Apr 9, 2014
 *      Author: xuyang06
 */

#include "linkstructure.h"
#include "filereader.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


/*void add_item(MBdynarray *testarray){
	MBdynarray_add_tail(testarray, 1);
	MBdynarray_add_tail(testarray, 2);

};*/

Overlaylink * default_overlaylink(){
	Overlaylink * overlaylinkitem = (Overlaylink *) malloc(sizeof(Overlaylink));
	overlaylinkitem->src_index = -1;
	overlaylinkitem->dst_index = -1;
	overlaylinkitem->underlaylinkarray = MBdynarray_create(0);
	overlaylinkitem->measured_success = -1;
	overlaylinkitem->cost = 0.0;
	overlaylinkitem->index = -1;
	return overlaylinkitem;
};


Underlaylink * default_underlaylink(){
	Underlaylink * underlaylinkitem = (Underlaylink *) malloc(sizeof(Underlaylink));
	underlaylinkitem->index = -1;
	underlaylinkitem->coverage_num = -1;
	underlaylinkitem->real_coverage_num = -1;
	underlaylinkitem->cost = 0.0;
	underlaylinkitem->failure_prob = -1.0;
	underlaylinkitem->bayesian_inferred_success = -1;
	underlaylinkitem->greedy_inferred_success = -1;
	underlaylinkitem->bayesian_prob = -1.0;
	underlaylinkitem->src_index = -1;
	underlaylinkitem->dst_index = -1;
	return underlaylinkitem;
};

Overlaylink * init_overlaylink(int src_index, int dst_index, int total_index){
	Overlaylink * overlaylinkitem = (Overlaylink *) malloc(sizeof(Overlaylink));
	overlaylinkitem->src_index = src_index;
	overlaylinkitem->dst_index = dst_index;
	overlaylinkitem->underlaylinkarray = MBdynarray_create(0);
	overlaylinkitem->cost = 0.0;
	overlaylinkitem->index = total_index;
	overlaylinkitem->measured_success = -1;
	return overlaylinkitem;
};

void print_underlay_link(Underlaylink *underlay_link_item){
	printf("Underlay: index->%d, src_index->%d, dst_index->%d, failure_prob->%e, coverage_num->%d, greedy_inferred_success->%d, bayesian_inferred_success->%d, bayesian_prob->%e, real_success->%d\n", underlay_link_item->index, underlay_link_item->src_index, underlay_link_item->dst_index, underlay_link_item->failure_prob, underlay_link_item->coverage_num, underlay_link_item->greedy_inferred_success, underlay_link_item->bayesian_inferred_success, underlay_link_item->bayesian_prob, underlay_link_item->real_success);
};

void print_underlay_link_array(MBdynarray *underlay_link_array){
	if (MBdynarray_get_count(underlay_link_array) >0){
		printf("Start to print underlay link array ...\n");
		int j;
		for (j = 0; j < MBdynarray_get_count(underlay_link_array); j++) {
			Underlaylink* underlaylinkitem = (Underlaylink*) MBdynarray_get(underlay_link_array, j);
			print_underlay_link(underlaylinkitem);
		}
		printf("End to print underlay link array ...\n");
	}
	//printf("Underlay: index->%d, src_index->%d, dst_index->%d, failure_prob->%e\r\n", underlay_link_item->index, underlay_link_item->src_index, underlay_link_item->dst_index, underlay_link_item->failure_prob);
};

int check_array_included(MBdynarray *sorted_underlay_index, int inserted_i){
	int j;
	int found = -1;
	for (j = 0; j < MBdynarray_get_count(sorted_underlay_index); j++) {
		int underlaylinkitem = (int*) MBdynarray_get(sorted_underlay_index, j);
		if (inserted_i == underlaylinkitem){
			found = j;
			break;
		}
	}
	return found;
};



void insert_sorted_array(MBdynarray *potential_underlay_index, int inserted_i){
	int j;
	int pre = 0;
	int pre_value = -1;
	int find_selected = 0;
	if (MBdynarray_get_count(potential_underlay_index) == 0){
		MBdynarray_add_tail(potential_underlay_index, inserted_i);
	}else{
		for (j = 0; j < MBdynarray_get_count(potential_underlay_index); j++) {
			int underlaylinkitem = (int*) MBdynarray_get(potential_underlay_index, j);
			if (underlaylinkitem >= inserted_i){
				//printf("underlaylinkitem->%d, inserted_i->%d, j->%d\n", underlaylinkitem, inserted_i, j);
				find_selected = 1;
				if (inserted_i != underlaylinkitem){
					if (j != 0){
						//printf("pre->%d\n", pre);
						MBdynarray_insert(potential_underlay_index, pre+1, inserted_i);
					}else{
						MBdynarray_add_head(potential_underlay_index, inserted_i);
					}
				}
				break;
			}else{
				pre = j;
			}
		}
		if(find_selected == 0){
			MBdynarray_insert(potential_underlay_index, MBdynarray_get_count(potential_underlay_index), inserted_i);
		}
	}
};


int check_failure_underlayarray_included(MBdynarray *failureunderlaylinkarray, int src_index, int dst_index){
	int j;
	int found = -1;
	for (j = 0; j < MBdynarray_get_count(failureunderlaylinkarray); j++) {
		FailureUnderlayLink* failureunderlaylinkitem = (FailureUnderlayLink*) MBdynarray_get(failureunderlaylinkarray, j);
		if ((src_index == failureunderlaylinkitem->src_index) && (dst_index == failureunderlaylinkitem->dst_index)){
			found = j;
			break;
		}
	}
	return found;
};



void cal_underlay_link_array_result(FILE * result_file_fp, MBdynarray *failureunderlaylinkarray, MBdynarray *underlay_link_array, MBdynarray *overlaylinkarray, MBdynarray *uncovered_underlay_array){
	int total_sum = MBdynarray_get_count(underlay_link_array);
	int greedy_total_same_sum = 0;
	int greedy_total_false_positive_sum = 0;
	int greedy_total_false_negative_sum = 0;
	int greedy_total_unknown_sum = 0;
	int bayesian_total_same_sum = 0;
	int bayesian_total_false_positive_sum = 0;
	int bayesian_total_false_negative_sum = 0;
	int bayesian_total_unknown_sum = 0;
	int overlay_link_array_sum = MBdynarray_get_count(overlaylinkarray);
	int overlay_link_same_sum = 0;
	int overlay_link_false_positive_sum = 0;
	int overlay_link_false_negative_sum = 0;
	int overlay_link_unknown_sum = 0;

	int j;
	for (j = 0; j < MBdynarray_get_count(underlay_link_array); j++) {
		Underlaylink* underlaylinkitem = (Underlaylink*) MBdynarray_get(underlay_link_array, j);
		int failure_underlaylink_found = check_failure_underlayarray_included(failureunderlaylinkarray, underlaylinkitem->src_index, underlaylinkitem->dst_index);
		int uncovered_underlay_array_found = check_array_included(uncovered_underlay_array, underlaylinkitem->index);
		if (uncovered_underlay_array_found != -1){
			underlaylinkitem->real_success = -1;
			bayesian_total_unknown_sum = bayesian_total_unknown_sum + 1;
			greedy_total_unknown_sum = greedy_total_unknown_sum + 1;
		}else if (failure_underlaylink_found == -1){
			underlaylinkitem->real_success = 1;
			if (underlaylinkitem->greedy_inferred_success == 1){
				greedy_total_same_sum = greedy_total_same_sum + 1;
			}else{
				greedy_total_false_positive_sum = greedy_total_false_positive_sum + 1;
			}
			if (underlaylinkitem->bayesian_inferred_success == 1){
				bayesian_total_same_sum = bayesian_total_same_sum + 1;
			}else{
				bayesian_total_false_positive_sum = bayesian_total_false_positive_sum + 1;
			}
		}else{
			underlaylinkitem->real_success = 0;
			if (underlaylinkitem->greedy_inferred_success == 0){
				greedy_total_same_sum = greedy_total_same_sum + 1;
			}else{
				greedy_total_false_negative_sum = greedy_total_false_negative_sum + 1;
			}
			if (underlaylinkitem->bayesian_inferred_success == 0){
				bayesian_total_same_sum = bayesian_total_same_sum + 1;
			}else{
				bayesian_total_false_negative_sum = bayesian_total_false_negative_sum + 1;
			}
		}
	}

	//printf("selected overlay link array!\n");
	//print_overlay_link_array(overlaylinkarray);

	for (j = 0; j < MBdynarray_get_count(overlaylinkarray); j++) {
		Overlaylink* overlaylinkitem = (Overlaylink*) MBdynarray_get(overlaylinkarray, j);
		MBdynarray *underlaylinkarray_items = overlaylinkitem->underlaylinkarray;
		int found_failure_underlay_found = 0;
		int k;
		for (k = 0; k< MBdynarray_get_count(underlaylinkarray_items); k++){
			Underlaylink* underlaylinkitem = (Underlaylink*) MBdynarray_get(underlaylinkarray_items, k);
			int failurelinkarray_found = check_failure_underlayarray_included(failureunderlaylinkarray, underlaylinkitem->src_index, underlaylinkitem->dst_index);
			if ( failurelinkarray_found != -1 ){
				found_failure_underlay_found = 1;
				break;
			}
		}
		if (found_failure_underlay_found == 1){
			if (overlaylinkitem->measured_success == 0){
				/*if (first_cal == 1){
					fprintf(allnodes_print_file_fp,"error overlaylink: src->%d, dst->%d\n",overlaylinkitem->src_index, overlaylinkitem->dst_index);
					fprintf(allnodes_print_file_fp,"same!\n");
				}*/
				overlay_link_same_sum = overlay_link_same_sum + 1;
			}else{
				/*if (first_cal == 1){
					fprintf(allnodes_print_file_fp,"normal overlaylink: src->%d, dst->%d\n",overlaylinkitem->src_index, overlaylinkitem->dst_index);
					fprintf(allnodes_print_file_fp,"false negative!\n");
				}*/
				overlay_link_false_negative_sum = overlay_link_false_negative_sum + 1;
			}
		}else{
			if (overlaylinkitem->measured_success == 1){
				/*if (first_cal == 1){
					fprintf(allnodes_print_file_fp,"normal overlaylink: src->%d, dst->%d\n",overlaylinkitem->src_index, overlaylinkitem->dst_index);
					fprintf(allnodes_print_file_fp,"same!\n");
				}*/
				overlay_link_same_sum = overlay_link_same_sum + 1;
			}else{
				/*if (first_cal == 1){
					fprintf(allnodes_print_file_fp,"error overlaylink: src->%d, dst->%d\n",overlaylinkitem->src_index, overlaylinkitem->dst_index);
					fprintf(allnodes_print_file_fp,"false positive!\n");
				}*/
				overlay_link_false_positive_sum = overlay_link_false_positive_sum + 1;
			}
		}
	}


	double greedy_total_same_sum_prob = (double)greedy_total_same_sum/(double)total_sum;
	double greedy_total_false_positive_sum_prob = (double)greedy_total_false_positive_sum/(double)total_sum;
	double greedy_total_false_negative_sum_prob = (double)greedy_total_false_negative_sum/(double)total_sum;
	double greedy_total_unknown_sum_prob = (double)greedy_total_unknown_sum/(double)total_sum;
	double bayesian_total_same_sum_prob = (double)bayesian_total_same_sum/(double)total_sum;
	double bayesian_total_false_positive_sum_prob = (double)bayesian_total_false_positive_sum/(double)total_sum;
	double bayesian_total_false_negative_sum_prob = (double)bayesian_total_false_negative_sum/(double)total_sum;
	double bayesian_total_unknown_sum_prob = (double)bayesian_total_unknown_sum/(double)total_sum;
	double overlay_link_total_same_sum_prob = (double)overlay_link_same_sum*1.0/(double)overlay_link_array_sum*1.0;
	double overlay_link_total_false_positive_sum_prob = (double)overlay_link_false_positive_sum*1.0/(double)overlay_link_array_sum*1.0;
	double overlay_link_total_false_negative_sum_prob = (double)overlay_link_false_negative_sum*1.0/(double)overlay_link_array_sum*1.0;
	//printf("Failure Underlay Link Number -> %d,Underlay Link Number -> %d, Select Overlay Number -> %d\n", MBdynarray_get_count(failureunderlaylinkarray), MBdynarray_get_count(underlay_link_array), MBdynarray_get_count(overlaylinkarray));
	//printf("Overlay Link Cal: same prob->%e, false positive prob->%e, false negative prob->%e\n", overlay_link_total_same_sum_prob, overlay_link_total_false_positive_sum_prob, overlay_link_total_false_negative_sum_prob);
	//printf("Greedy Cal: same prob->%e, false positive prob->%e, false negative prob->%e, unknown prob->%e\n", greedy_total_same_sum_prob, greedy_total_false_positive_sum_prob, greedy_total_false_negative_sum_prob, greedy_total_unknown_sum_prob);
	//printf("Bayesian Cal: same prob->%e, false positive prob->%e, false negative prob->%e, unknown prob->%e\n", bayesian_total_same_sum_prob, bayesian_total_false_positive_sum_prob, bayesian_total_false_negative_sum_prob, bayesian_total_unknown_sum_prob);
	//printf("Underlay: index->%d, src_index->%d, dst_index->%d, failure_prob->%e\r\n", underlay_link_item->index, underlay_link_item->src_index, underlay_link_item->dst_index, underlay_link_item->failure_prob);
	printf("failureunderlaylinkarray number -> %d\n", MBdynarray_get_count(failureunderlaylinkarray));
	MBdynarray * overlay_node_index_array = get_overlay_node_array(overlaylinkarray);
	//printf("overlay_node_index_array -> %d\n", MBdynarray_get_count(overlay_node_index_array));
	print_integer_nodearray(overlay_node_index_array);
	printf("%f %f\n", greedy_total_unknown_sum_prob, bayesian_total_unknown_sum_prob);
	fprintf(result_file_fp,"%d %d\n",MBdynarray_get_count(overlay_node_index_array), overlay_link_array_sum);
	fprintf(result_file_fp,"%f %f %f\n",overlay_link_total_same_sum_prob, overlay_link_total_false_positive_sum_prob, overlay_link_total_false_negative_sum_prob);
	fprintf(result_file_fp,"%f %f %f %f\n",greedy_total_same_sum_prob, greedy_total_false_positive_sum_prob, greedy_total_false_negative_sum_prob, greedy_total_unknown_sum_prob);
	fprintf(result_file_fp,"%f %f %f %f\n",bayesian_total_same_sum_prob,bayesian_total_false_positive_sum_prob,bayesian_total_false_negative_sum_prob,bayesian_total_unknown_sum_prob);
	fprintf(result_file_fp,"%f %f %f\n",greedy_total_same_sum_prob/((1- greedy_total_unknown_sum_prob)*1.0), greedy_total_false_positive_sum_prob/((1- greedy_total_unknown_sum_prob)*1.0), greedy_total_false_negative_sum_prob/((1- greedy_total_unknown_sum_prob)*1.0));
	fprintf(result_file_fp,"%f %f %f\n",bayesian_total_same_sum_prob/((1- bayesian_total_unknown_sum_prob)*1.0), bayesian_total_false_positive_sum_prob/((1- bayesian_total_unknown_sum_prob)*1.0), bayesian_total_false_negative_sum_prob/((1- bayesian_total_unknown_sum_prob)*1.0));

};



void print_overlay_link(Overlaylink *overlay_link_item){
	printf("Start to print overlay link ...\n");
	printf("Overlay: index->%d, src_index->%d, dst_index->%d, cost->%e, measured_success->%d\n", overlay_link_item->index, overlay_link_item->src_index, overlay_link_item->dst_index, overlay_link_item->cost, overlay_link_item->measured_success);
	printf("Underlay link array: \n");
	print_underlay_link_array(overlay_link_item->underlaylinkarray);
	printf("End to print overlay link ...\n");
};

void print_overlay_link_array(MBdynarray *overlay_link_array){
	printf("Start to print overlay link array ...\n");
	int j;
	for (j = 0; j < MBdynarray_get_count(overlay_link_array); j++) {
		Overlaylink* overlaylinkitem = (Overlaylink*) MBdynarray_get(overlay_link_array, j);
		print_overlay_link(overlaylinkitem);
	}
	printf("End to print overlay link array ...\n");
	//printf("Underlay: index->%d, src_index->%d, dst_index->%d, failure_prob->%e\r\n", underlay_link_item->index, underlay_link_item->src_index, underlay_link_item->dst_index, underlay_link_item->failure_prob);
};


int check_overlayarray_included(MBdynarray *overlaylinkarray, int src_index, int dst_index){
	int j;
	int found = -1;
	for (j = 0; j < MBdynarray_get_count(overlaylinkarray); j++) {
		Overlaylink* overlaylinkitem = (Overlaylink*) MBdynarray_get(overlaylinkarray, j);
		if ((src_index == overlaylinkitem->src_index) && (dst_index == overlaylinkitem->dst_index)){
			found = j;
			break;
		}
	}
	return found;
};


void change_underlayLink_failure_prob(Underlaylink * underlaylinkitem,double failure_prob){
	underlaylinkitem->failure_prob = failure_prob;
};

Underlaylink * init_underlaylink(int src_index, int dst_index, int total_index){
	Underlaylink * underlaylinkitem = (Underlaylink *) malloc(sizeof(Underlaylink));
	underlaylinkitem->index = total_index;
	underlaylinkitem->cost = 1.0;
	underlaylinkitem->failure_prob = -1.0;
	underlaylinkitem->src_index = src_index;
	underlaylinkitem->dst_index = dst_index;
	underlaylinkitem->coverage_num = -1;
	underlaylinkitem->real_coverage_num = -1;
	underlaylinkitem->greedy_inferred_success = -1;
	underlaylinkitem->bayesian_inferred_success = -1;
	underlaylinkitem->real_success = -1;
	underlaylinkitem->bayesian_prob = -1.0;
	return underlaylinkitem;
};



int check_underlayarray_included(MBdynarray *underlaylinkarray, int src_index, int dst_index){
	int j;
	int found = -1;
	for (j = 0; j < MBdynarray_get_count(underlaylinkarray); j++) {
		Underlaylink* underlaylinkitem = (Underlaylink*) MBdynarray_get(underlaylinkarray, j);
		if ((src_index == underlaylinkitem->src_index) && (dst_index == underlaylinkitem->dst_index)){
			found = j;
			break;
		}
	}
	return found;
};






void generate_underlay_coverage_num(MBdynarray *underlaylinkarray){
	//int underlaylinkarray_len = ;
	int i;
	for (i = 0; i < MBdynarray_get_count(underlaylinkarray); i++) {
		Underlaylink *underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray, i);
		if (underlaylinkitem->failure_prob <= 0.1){
			underlaylinkitem->coverage_num = 1;
			underlaylinkitem->real_coverage_num = 1;
		}else if(underlaylinkitem->failure_prob <= 0.2){
			underlaylinkitem->coverage_num = 2;
			underlaylinkitem->real_coverage_num = 2;
		}else{
			underlaylinkitem->coverage_num = 3;
			underlaylinkitem->real_coverage_num = 3;
		}
		//printf("%d: %s\n", i, (const char*)MBdynarray_get(underlaylinkarray, i));
	}

};

void cal_overlay_cost(MBdynarray *overlaylinkarray){
	int i;
	for (i = 0; i < MBdynarray_get_count(overlaylinkarray); i++) {
		Overlaylink *overlaylinkitem = (Overlaylink*)MBdynarray_get(overlaylinkarray, i);
		double overlaylinkitem_cost = 0.0;
		MBdynarray *underlaylinkarray = overlaylinkitem->underlaylinkarray;
		int j;
		for (j = 0; j < MBdynarray_get_count(underlaylinkarray); j++) {
			Underlaylink *underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray, j);
			overlaylinkitem_cost = overlaylinkitem_cost + underlaylinkitem->cost;
		}
		overlaylinkitem->cost = overlaylinkitem_cost;
	}
};


int check_covered_underlay_array_included(MBdynarray *covered_underlay, int underlaylink_index){
	int j;
	int found = -1;
	for (j = 0; j < MBdynarray_get_count(covered_underlay); j++) {
		int covered_underlay_item = (int*) MBdynarray_get(covered_underlay, j);
		if ( covered_underlay_item == underlaylink_index){
			found = j;
			break;
		}
	}
	return found;
};

MBdynarray *get_overlay_node_array(MBdynarray *selected_overlayLink_array){
	//printf("start overlay node array!\n");
	MBdynarray *overlay_node_index_array = MBdynarray_create(0);
	int i;
	for (i = 0; i < MBdynarray_get_count(selected_overlayLink_array); i++) {
		Overlaylink *overlaylinkitem = (Overlaylink*)MBdynarray_get(selected_overlayLink_array, i);
		//int overlaylinkitem_src_index = overlaylinkitem->src_index;
		//int overlaylinkitem_dst_index = overlaylinkitem->dst_index;
		int overlaylinkitem_src_index_found = check_array_included(overlay_node_index_array, overlaylinkitem->src_index);
		int overlaylinkitem_dst_index_found = check_array_included(overlay_node_index_array, overlaylinkitem->dst_index);
		if ( overlaylinkitem_src_index_found == -1 ){
			insert_sorted_array(overlay_node_index_array, overlaylinkitem->src_index);
		}
		if ( overlaylinkitem_dst_index_found == -1 ){
			insert_sorted_array(overlay_node_index_array, overlaylinkitem->dst_index);
		}
	}
	//printf("end overlay node array!\n");
	return overlay_node_index_array;
};





void get_uncovered_underlay_array(MBdynarray *selected_overlayLink_array, MBdynarray *underlaylinkarray, MBdynarray *uncovered_underlay_array){
	MBdynarray *covered_underlay = MBdynarray_create(0);
	int i;
	for (i = 0; i < MBdynarray_get_count(selected_overlayLink_array); i++) {
		Overlaylink *overlaylinkitem = (Overlaylink*)MBdynarray_get(selected_overlayLink_array, i);
		MBdynarray *underlaylinkarray = overlaylinkitem->underlaylinkarray;
		int j;
		for (j = 0; j < MBdynarray_get_count(underlaylinkarray); j++) {
			Underlaylink *underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray, j);
			int array_found = check_array_included(covered_underlay, underlaylinkitem->index);
			if ( array_found == -1 ){
				insert_sorted_array(covered_underlay, underlaylinkitem->index);
			}
		}
	}
	//MBdynarray *uncovered_underlay_array = MBdynarray_create(0);
	get_complement_set(covered_underlay, MBdynarray_get_count(underlaylinkarray), uncovered_underlay_array);
	/*int j;
	printf("uncovered underlay array !!!!\n");
	printf("Start to print uncovered underlay array ...\n");
	for (j = 0; j < MBdynarray_get_count(uncovered_underlay_array); j++) {
		printf("%d ",  (int) MBdynarray_get(uncovered_underlay_array, j));
	}
	printf("\n");
	printf("End to print uncovered underlay array ...\n");*/

	//return uncovered_underlay_array;
};








MBdynarray *greedy_select_overlay(MBdynarray *overlaylinkarray, MBdynarray *underlaylinkarray, MBdynarray *noncoverednodearray){
	MBdynarray *covered_underlay = MBdynarray_create(0);
	MBdynarray *selected_overlay = MBdynarray_create(0);
	MBdynarray *removed_overlay_index_array = MBdynarray_create(0);
	int underlaylinkarray_i = 0;
	for (; underlaylinkarray_i < MBdynarray_get_count(underlaylinkarray); underlaylinkarray_i++){
		Underlaylink *underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray, underlaylinkarray_i);
		underlaylinkitem->real_coverage_num = underlaylinkitem->coverage_num;
	}

	//printf("here! \n");
	while (1){
		int selected_overlaylinkitem_index = -1;
		double selected_overlaylink_score = 0.0;
		int i;
		for (i = 0; i < MBdynarray_get_count(overlaylinkarray); i++) {
			int check_removedoverlayindex_found = check_array_included(removed_overlay_index_array, i);
			//printf("check_removedoverlayindex_found-> %d, end53!\n", check_removedoverlayindex_found);
			if (check_removedoverlayindex_found == -1){
				//printf("end33!\n");
				Overlaylink *overlaylinkitem = (Overlaylink*)MBdynarray_get(overlaylinkarray, i);
				//printf("here1! \n");
				int src_index_found = check_noncoverednodearray_included(noncoverednodearray, overlaylinkitem->src_index);
				//printf("here2! \n");
				int dst_index_found = check_noncoverednodearray_included(noncoverednodearray, overlaylinkitem->dst_index);
				//printf("src_index->%d, dst_index->%d, src_index_found->%d, dst_index_found->%d\n", overlaylinkitem->src_index, overlaylinkitem->dst_index, src_index_found, dst_index_found);
				if ((src_index_found == -1) && (dst_index_found == -1)){
					//double overlaylinkitem_cost = 0.0;
					//printf("end43!\n");
					MBdynarray *underlaylinkarray = overlaylinkitem->underlaylinkarray;
					int underlaylink_cover_total = 0;
					int j;
					for (j = 0; j < MBdynarray_get_count(underlaylinkarray); j++) {
						Underlaylink *underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray, j);
						if (underlaylinkitem->real_coverage_num > 0){
							underlaylink_cover_total = underlaylink_cover_total + 1;
						}
							//overlaylinkitem_cost = overlaylinkitem_cost + underlaylinkitem->cost;
					}
					double current_overlaylink_score = (underlaylink_cover_total*1.00)/overlaylinkitem->cost;
					if (current_overlaylink_score > selected_overlaylink_score) {
						selected_overlaylinkitem_index = i;
						selected_overlaylink_score = current_overlaylink_score;
					}
				}
			}
			//	overlaylinkitem->cost = overlaylinkitem_cost;
		}
		if ((selected_overlaylinkitem_index == -1) || (selected_overlaylink_score < 0.0001)){
			//printf("selected_overlaylinkitem_index: %d, selected_overlaylink_score: %lf\n", selected_overlaylinkitem_index, selected_overlaylink_score);
			break;
		}
		Overlaylink *overlaylinkremoveditem = (Overlaylink *) MBdynarray_get(overlaylinkarray, selected_overlaylinkitem_index);
		MBdynarray_add_tail(removed_overlay_index_array, selected_overlaylinkitem_index);
		MBdynarray *underlaylinkarray_toremove = overlaylinkremoveditem->underlaylinkarray;
		int j;
		for (j = 0; j < MBdynarray_get_count(underlaylinkarray_toremove); j++) {
			Underlaylink *underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray_toremove, j);
			if (underlaylinkitem->real_coverage_num > 0){
				underlaylinkitem->real_coverage_num = underlaylinkitem->real_coverage_num - 1;
				if (underlaylinkitem->real_coverage_num == 0){
					int array_found = check_array_included(covered_underlay, underlaylinkitem->index);
					if ( array_found == -1 ){
						insert_sorted_array(covered_underlay, underlaylinkitem->index);
					}
				}
			}
		//overlaylinkitem_cost = overlaylinkitem_cost + underlaylinkitem->cost;
		}
		MBdynarray_add_tail(selected_overlay, overlaylinkremoveditem);
		if (MBdynarray_get_count(covered_underlay) == MBdynarray_get_count(underlaylinkarray)){
			//printf("MBdynarray_get_count(covered_underlay): %d, MBdynarray_get_count(underlaylinkarray): %d\n", MBdynarray_get_count(covered_underlay), MBdynarray_get_count(underlaylinkarray));
			break;
		}
	}
	//printf("covered_underlay!!\n");
	//print_integer_nodearray(covered_underlay);

	//printf("covered_underlay->%d, underlaylinkarray->%d\n", MBdynarray_get_count(covered_underlay), MBdynarray_get_count(underlaylinkarray));
	//printf("selected overlay!\n");
	//print_overlay_link_array(selected_overlay);
	return selected_overlay;
};


void print_overlay_node_item(OverlayNode *overlay_node_item){
	printf("Overlay node item: index->%d, related_overlay_number->%d \n", overlay_node_item->index, overlay_node_item->related_overlay_number);
};

void print_overlay_node_array(MBdynarray *overlay_node_array){
	if (MBdynarray_get_count(overlay_node_array) >0){
		printf("Start to print overlay node array ...\n");
		int j;
		for (j = 0; j < MBdynarray_get_count(overlay_node_array); j++) {
			OverlayNode* overlay_node_item = (OverlayNode*) MBdynarray_get(overlay_node_array, j);
			print_overlay_node_item(overlay_node_item);
		}
		printf("End to print overlay node array ...\n");
	}
	//printf("Underlay: index->%d, src_index->%d, dst_index->%d, failure_prob->%e\r\n", underlay_link_item->index, underlay_link_item->src_index, underlay_link_item->dst_index, underlay_link_item->failure_prob);
};


void print_integer_nodearray(MBdynarray *integernodearray){
	printf("Start to print integer node array ...\n");
	int j;
	for (j = 0; j < MBdynarray_get_count(integernodearray); j++) {
		int noncoverednode_item = (int*) MBdynarray_get(integernodearray, j);
		printf("%d ", noncoverednode_item);
	}
	printf("\n");
	printf("End to print integer node array ...\n");
	//printf("Underlay: index->%d, src_index->%d, dst_index->%d, failure_prob->%e\r\n", underlay_link_item->index, underlay_link_item->src_index, underlay_link_item->dst_index, underlay_link_item->failure_prob);
};


MBdynarray *greedy_select_overlay_exclude_one_item(MBdynarray *selected_overlayLink_array, MBdynarray *overlaylinkarray, MBdynarray *underlaylinkarray, MBdynarray *noncoverednodearray){
	//MBdynarray *selected_overlayLink_array = greedy_select_overlay(overlaylinkarray, underlaylinkarray, noncoverednodearray, );
	MBdynarray *overlay_node_array =  MBdynarray_create(0);
	cal_overlayNode_info(selected_overlayLink_array, overlay_node_array);
	//print_overlay_node_array(overlay_node_array);
	//printf("end53!\n");
	int lowest_overlay_node_index = find_lowest_overlay_node_array_index(overlay_node_array);
	//printf("lowest_overlay_node_index-> %d\n", lowest_overlay_node_index);
	MBdynarray_add_tail(noncoverednodearray, lowest_overlay_node_index);
	//printf("noncoverednodearray!\n");
	//print_integer_nodearray(noncoverednodearray);
	//printf("end73!\n");
	//printf("%d\n", highest_overlay_node_index);
	selected_overlayLink_array = greedy_select_overlay(overlaylinkarray, underlaylinkarray, noncoverednodearray);
	//printf("uncovered_underlay_array -> %d!!\n", MBdynarray_get_count(uncovered_underlay_array));
	//print_integer_nodearray(uncovered_underlay_array);
	//MBdynarray_add_tail(uncovered_underlay_array, 100);
	//printf("end83!\n");
	return selected_overlayLink_array;
};


int check_removedoverlayindexarray_included(MBdynarray *removed_overlay_index_array, int overlay_index){
	int found = -1;
	int j;
	for (j = 0; j < MBdynarray_get_count(removed_overlay_index_array); j++) {
		int* removed_overlay_index_item = (int*) MBdynarray_get(removed_overlay_index_array, j);
		if (overlay_index == *removed_overlay_index_item){
			found = j;
			break;
		}
	}
	return found;
};



int check_noncoverednodearray_included(MBdynarray *noncoverednodearray, int node_index){
	//printf("here3! \n");
	int found = -1;
	int j;
	for (j = 0; j < MBdynarray_get_count(noncoverednodearray); j++) {
		//printf("here4! \n");
		int noncoverednodeitem = (int*) MBdynarray_get(noncoverednodearray, j);
		//printf("here5! \n");
		if (node_index == noncoverednodeitem){
			found = j;
			break;
		}
	}
	return found;
};



int check_overlaynodearray_included(MBdynarray *overlay_node_array, int node_index){
	int j;
	int found = -1;
	for (j = 0; j < MBdynarray_get_count(overlay_node_array); j++) {
		OverlayNode* overlay_node_item = (OverlayNode*) MBdynarray_get(overlay_node_array, j);
		if (node_index == overlay_node_item->index){
			found = j;
			break;
		}
	}
	return found;
};




void cal_overlayNode_info(MBdynarray *selected_overlayLink_array, MBdynarray *overlay_node_array){
	//MBdynarray *overlay_node_array = MBdynarray_create(0);
	int i;
	for (i = 0; i < MBdynarray_get_count(selected_overlayLink_array); i++) {
		Overlaylink *selected_overlayLink_item = (Overlaylink*)MBdynarray_get(selected_overlayLink_array, i);
		int selected_overlayLink_item_src_index = selected_overlayLink_item->src_index;
		int selected_overlayLink_item_dst_index = selected_overlayLink_item->dst_index;
		int selected_overlayLink_item_src_index_found = check_overlaynodearray_included(overlay_node_array, selected_overlayLink_item_src_index);
		int selected_overlayLink_item_dst_index_found = check_overlaynodearray_included(overlay_node_array, selected_overlayLink_item_dst_index);
		if (selected_overlayLink_item_src_index_found == -1){
			OverlayNode * overlaynodeitem = (OverlayNode *) malloc(sizeof(OverlayNode));
			overlaynodeitem->index = selected_overlayLink_item_src_index;
			overlaynodeitem->related_overlay_number = 1;
			MBdynarray_add_tail(overlay_node_array, overlaynodeitem);
		}else{
			OverlayNode * overlaynodeitem = (OverlayNode *)MBdynarray_get(overlay_node_array, selected_overlayLink_item_src_index_found);
			overlaynodeitem->related_overlay_number = overlaynodeitem->related_overlay_number + 1;
		}
		if (selected_overlayLink_item_dst_index_found == -1){
			OverlayNode * overlaynodeitem = (OverlayNode *) malloc(sizeof(OverlayNode));
			overlaynodeitem->index = selected_overlayLink_item_dst_index;
			overlaynodeitem->related_overlay_number = 1;
			MBdynarray_add_tail(overlay_node_array, overlaynodeitem);
		}else{
			OverlayNode * overlaynodeitem = (OverlayNode *)MBdynarray_get(overlay_node_array, selected_overlayLink_item_dst_index_found);
			overlaynodeitem->related_overlay_number = overlaynodeitem->related_overlay_number + 1;
		}
	}
	//return overlay_node_array;
};


int find_lowest_overlay_node_array_index(MBdynarray *overlay_node_array){
	int i;
	int lowest_overlay_number = 10000000;
	int lowest_overlay_node_index = -1;
	for (i = 0; i < MBdynarray_get_count(overlay_node_array); i++){
		OverlayNode * overlaynodeitem = (OverlayNode *)MBdynarray_get(overlay_node_array, i);
		if (overlaynodeitem->related_overlay_number < lowest_overlay_number){
			lowest_overlay_node_index = overlaynodeitem->index;
			lowest_overlay_number = overlaynodeitem->related_overlay_number;
		}
	}
	return lowest_overlay_node_index;
};



MBdynarray * get_complement_set_without_two(MBdynarray *sorted_underlay_index, MBdynarray *uncovered_underlay_array, int upper_len){
	MBdynarray *complement_set = MBdynarray_create(0);
	int j;
	for (j = 0; j < upper_len; j++) {
		int found_s_underlay_index = check_array_included(sorted_underlay_index, j);
		int found_u_underlay_index = check_array_included(uncovered_underlay_array, j);
		if ((found_s_underlay_index == -1) && (found_u_underlay_index == -1)){
			MBdynarray_add_tail(complement_set, j);
		}
	}
	return complement_set;
};




void get_complement_set(MBdynarray *sorted_underlay_index, int upper_len, MBdynarray *complement_set){
	//complement_set = MBdynarray_create(0);
	int sorted_underlay_j = 0;
	int j;
	for (j = 0; j < upper_len; j++) {
		int underlaylinkitem = (int) MBdynarray_get(sorted_underlay_index, sorted_underlay_j);
		if (underlaylinkitem == j){
			if (sorted_underlay_j < (MBdynarray_get_count(sorted_underlay_index)-1) ){
				sorted_underlay_j = sorted_underlay_j + 1;
			}
			continue;
		}else{
			MBdynarray_add_tail(complement_set, j);
		}
	}
	//return complement_set;
};





void greedy_infer_underlay(MBdynarray *selectedoverlaylinkarray, MBdynarray *underlaylinkarray, MBdynarray *uncovered_underlay_array){
	MBdynarray *considered_underlay_index = MBdynarray_create(0);
	MBdynarray *abnormal_overlay_index = MBdynarray_create(0);
	MBdynarray *potential_underlay_index = MBdynarray_create(0);

	int i;
	for (i = 0; i < MBdynarray_get_count(selectedoverlaylinkarray); i++) {
		Overlaylink *overlaylinkitem = (Overlaylink*)MBdynarray_get(selectedoverlaylinkarray, i);
		if	(overlaylinkitem->measured_success == 1){
			MBdynarray *underlaylinkarray = overlaylinkitem->underlaylinkarray;
			int j;
			for (j = 0; j < MBdynarray_get_count(underlaylinkarray); j++) {
				Underlaylink *underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray, j);
				//printf("measure success underlay: %d \n", underlaylinkitem->index);
				if (check_array_included(considered_underlay_index, underlaylinkitem->index) == -1){
					underlaylinkitem->greedy_inferred_success = 1;
					insert_sorted_array(considered_underlay_index, underlaylinkitem->index);
					/*int j;
					printf("here 1234567\n");
					printf("Start to print underlay considered link array ...\n");
					for (j = 0; j < MBdynarray_get_count(considered_underlay_index); j++) {
						printf("%d  \n",  (int) MBdynarray_get(considered_underlay_index, j));
					}
					printf("End to print underlay considered link array ...\n");*/
				}
			}
		}else{
			if (check_array_included(abnormal_overlay_index, i) == -1){
				//printf("measure abnormal overlay: %d \n", i);
				insert_sorted_array(abnormal_overlay_index, i);
			}
		}
	}

 	/*int j;
 	printf("Start to print uncovered underlay array ...\n");
 	for (j = 0; j < MBdynarray_get_count(uncovered_underlay_array); j++) {
 		printf("%d ",  (int) MBdynarray_get(uncovered_underlay_array, j));
 	}
 	printf("\n");
 	printf("End to print uncovered underlay array ...\n");


	printf("Start to print underlay considered link array ...\n");
	for (j = 0; j < MBdynarray_get_count(considered_underlay_index); j++) {
		printf("%d ",  (int) MBdynarray_get(considered_underlay_index, j));
	}
	printf("\n");
	printf("End to print underlay considered link array ...\n");

	printf("Start to print abnormal overlay link array ...\n");
	for (j = 0; j < MBdynarray_get_count(abnormal_overlay_index); j++) {
		printf("%d ",  (int) MBdynarray_get(abnormal_overlay_index, j));
	}
	printf("\n");
	printf("End to print abnormal overlay link array ...\n");


	printf("here111111!\n");*/
	get_complement_set(considered_underlay_index, MBdynarray_get_count(underlaylinkarray), potential_underlay_index);

	/*printf("Start to print potential_underlay_index array ...\n");
	for (j = 0; j < MBdynarray_get_count(potential_underlay_index); j++) {
		printf("%d  ",  (int) MBdynarray_get(potential_underlay_index, j));
	}
	printf("\n");
	printf("End to print potential_underlay_index array ...\n");*/

	//int i;
	for (i = 0; i < MBdynarray_get_count(underlaylinkarray); i++) {
		Underlaylink *underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray, i);
		int found_index = check_array_included(potential_underlay_index, underlaylinkitem->index);
		if (found_index != -1){
			if (underlaylinkitem->failure_prob > 0.5){
				underlaylinkitem->greedy_inferred_success = 0;
				insert_sorted_array(considered_underlay_index, underlaylinkitem->index);
				int j;
				MBdynarray *deleting_abnormal_overlay_index = MBdynarray_create(0);
				//int found_abnormal_overlay_index = -1;
				for (j = 0; j < MBdynarray_get_count(abnormal_overlay_index); j++){
					int found_tmp = 0;
					Overlaylink *overlaylinkitem = (Overlaylink*)MBdynarray_get(selectedoverlaylinkarray, (int)MBdynarray_get(abnormal_overlay_index, j));
					MBdynarray *underlaylinkarray_tmp = overlaylinkitem->underlaylinkarray;
					int k;
					for (k = 0; k < MBdynarray_get_count(underlaylinkarray_tmp); k++) {
						Underlaylink *underlaylinkitem_tmp = (Underlaylink*)MBdynarray_get(underlaylinkarray_tmp, k);
						if(underlaylinkitem_tmp->index == underlaylinkitem->index){
							found_tmp = 1;
							break;
						}
					}
					if (found_tmp == 1){
						MBdynarray_add_tail(deleting_abnormal_overlay_index, j);
						break;
					}
				}
				for(j = MBdynarray_get_count(deleting_abnormal_overlay_index)-1; j >=0; j--){
					int deleting_overlay_index = (int) MBdynarray_get(deleting_abnormal_overlay_index, j);
					MBdynarray_remove(abnormal_overlay_index, deleting_overlay_index);
				}
			}
		}
	}
	get_complement_set(considered_underlay_index, MBdynarray_get_count(underlaylinkarray), potential_underlay_index);
	/*printf("Current status ...\n");
	printf("Start to print potential_underlay_index array ...\n");
	for (j = 0; j < MBdynarray_get_count(potential_underlay_index); j++) {
		printf("%d ",  (int) MBdynarray_get(potential_underlay_index, j));
	}
	printf("\n");
	printf("End to print potential_underlay_index array ...\n");
	printf("Start to print abnormal overlay link array ...\n");
	for (j = 0; j < MBdynarray_get_count(abnormal_overlay_index); j++) {
		printf("%d ",  (int) MBdynarray_get(abnormal_overlay_index, j));
	}
	printf("\n");
	printf("End to print abnormal overlay link array ...\n");

	printf("here2222222222!\n");*/
	while(1){
		/*printf("111\n");
		printf("abnormal:%d\n", MBdynarray_get_count(abnormal_overlay_index));
		printf("considered:%d\n", MBdynarray_get_count(considered_underlay_index));
		printf("underlaylinkarray:%d\n", MBdynarray_get_count(underlaylinkarray));
		printf("uncovered:%d\n", MBdynarray_get_count(uncovered_underlay_array));
		printf("potential_underlay:%d\n", MBdynarray_get_count(potential_underlay_index));*/

		if ((MBdynarray_get_count(abnormal_overlay_index) == 0) && (MBdynarray_get_count(considered_underlay_index) != MBdynarray_get_count(underlaylinkarray) - MBdynarray_get_count(uncovered_underlay_array))){
			printf("MBdynarray_get_count(abnormal_overlay_index): %d, MBdynarray_get_count(considered_underlay_index): %d\n", MBdynarray_get_count(abnormal_overlay_index), MBdynarray_get_count(considered_underlay_index));
			int j;
			for (j = 0; j < MBdynarray_get_count(potential_underlay_index); j++){
				int considered_underlay_index = (int)MBdynarray_get(potential_underlay_index, j);
				int uncovered_underlay_array_found = check_array_included(uncovered_underlay_array, considered_underlay_index);
				if (uncovered_underlay_array_found == -1){
					Underlaylink *considered_underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray, considered_underlay_index);
					considered_underlaylinkitem->greedy_inferred_success = 1;
				}
			}
			break;
		}
		//printf("3333\n");
		if ((MBdynarray_get_count(abnormal_overlay_index) == 0) && (MBdynarray_get_count(considered_underlay_index) == MBdynarray_get_count(underlaylinkarray)- MBdynarray_get_count(uncovered_underlay_array))){
			printf("MBdynarray_get_count(abnormal_overlay_index): %d, MBdynarray_get_count(considered_underlay_index): %d\n", MBdynarray_get_count(abnormal_overlay_index), MBdynarray_get_count(considered_underlay_index));
			break;
		}
		//printf("222\n");
		int j;
		int selected_underlay_index = -1;
		double selected_underlay_score = 0;
		for (j = 0; j < MBdynarray_get_count(potential_underlay_index); j++){
			int considered_underlay_index = (int)MBdynarray_get(potential_underlay_index, j);
			int considered_underlay_common_val = 0;
			int k;
			for (k = 0; k < MBdynarray_get_count(abnormal_overlay_index); k++){
				int considerred_overlay_index = (int)MBdynarray_get(abnormal_overlay_index, k);
				Overlaylink *overlaylinkitem_tmp = (Overlaylink*)MBdynarray_get(selectedoverlaylinkarray, considerred_overlay_index);
				MBdynarray *underlaylinkarray_tmp = overlaylinkitem_tmp->underlaylinkarray;
				int m;
				for (m = 0; m < MBdynarray_get_count(underlaylinkarray_tmp); m++){
					Underlaylink *underlaylinkitem_tmp = (Underlaylink*)MBdynarray_get(underlaylinkarray_tmp, m);
					if (underlaylinkitem_tmp->index == considered_underlay_index){
						considered_underlay_common_val = considered_underlay_common_val + 1;
						break;
					}
				}
			}
			Underlaylink *considered_underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray, considered_underlay_index);
			double considered_underlay_score = (considered_underlay_common_val*1.0)/log((1/considered_underlaylinkitem->failure_prob) -1);
			printf("considered_underlay_index -> %d, considered_underlay_score -> %lf\n",  considered_underlay_index, considered_underlay_score);
			if (considered_underlay_score > selected_underlay_score){
				selected_underlay_score = considered_underlay_score;
				selected_underlay_index = considered_underlay_index;
			}
		}
		//printf("333\n");
		if (selected_underlay_score <= 0.0001){
			//printf("Begin to score 0\n");
			for (j = 0; j < MBdynarray_get_count(potential_underlay_index); j++){
				int considered_underlay_index = (int)MBdynarray_get(potential_underlay_index, j);
				Underlaylink *considered_underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray, considered_underlay_index);
				considered_underlaylinkitem->greedy_inferred_success = 1;
			}
			//printf("End to score 0\n");
			break;
		}else{
			Underlaylink *selected_underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray, selected_underlay_index);
			selected_underlaylinkitem->greedy_inferred_success = 0;
			insert_sorted_array(considered_underlay_index, selected_underlaylinkitem->index);
			//int j;
			MBdynarray *deleting_abnormal_overlay_index = MBdynarray_create(0);
			for (j = 0; j < MBdynarray_get_count(abnormal_overlay_index); j++){
				int found_tmp = 0;
				Overlaylink *overlaylinkitem = (Overlaylink*)MBdynarray_get(selectedoverlaylinkarray, (int)MBdynarray_get(abnormal_overlay_index, j));
				MBdynarray *underlaylinkarray_tmp = overlaylinkitem->underlaylinkarray;
				int k;
				for (k = 0; k < MBdynarray_get_count(underlaylinkarray_tmp); k++) {
					Underlaylink *underlaylinkitem_tmp = (Underlaylink*)MBdynarray_get(underlaylinkarray_tmp, k);
					if(underlaylinkitem_tmp->index == selected_underlaylinkitem->index){
						found_tmp = 1;
						break;
					}
				}
				if (found_tmp == 1){
					MBdynarray_add_tail(deleting_abnormal_overlay_index, j);
				}
			}
			for(j = MBdynarray_get_count(deleting_abnormal_overlay_index)-1; j >=0; j--){
				int deleting_overlay_index = (int)MBdynarray_get(deleting_abnormal_overlay_index, j);
				MBdynarray_remove(abnormal_overlay_index, deleting_overlay_index);
			}
			get_complement_set(considered_underlay_index, MBdynarray_get_count(underlaylinkarray), potential_underlay_index);
			/*printf("Start to print abnormal overlay link array ...\n");
			for (j = 0; j < MBdynarray_get_count(abnormal_overlay_index); j++) {
				printf("%d ",  (int) MBdynarray_get(abnormal_overlay_index, j));
			}
			printf("\n");
			printf("End to print abnormal overlay link array ...\n");

			printf("Start to print potential_underlay_index array ...\n");
			for (j = 0; j < MBdynarray_get_count(potential_underlay_index); j++) {
				printf("%d ",  (int) MBdynarray_get(potential_underlay_index, j));
			}
			printf("\n");
			printf("End to print potential_underlay_index array ...\n");*/
		}
	}
	//printf("here2222222222!!!!!\n");
};


void get_all_comb_binary_matrix(int total_amount, int amount_0, MBdynarray * all_comb_matrix){
	all_comb_matrix = MBdynarray_create(0);
	if (total_amount == 0){

	}else if (total_amount == amount_0){
		int i;
		MBdynarray *one_item_matrix = MBdynarray_create(0);
		for(i = 0; i < amount_0; i++){
			MBdynarray_add_tail(one_item_matrix, 0);
		}
		//printf("total_amount -> %d, amount_0 -> %d \n", total_amount, amount_0);
		//print_integer_nodearray(one_item_matrix);
		MBdynarray_add_tail(all_comb_matrix, one_item_matrix);
		//return all_comb_matrix;
	}else if(amount_0 == 0){
		int i;
		MBdynarray *one_item_matrix = MBdynarray_create(0);
		for(i = 0; i < total_amount; i++){
			MBdynarray_add_tail(one_item_matrix, 1);
		}
		//printf("total_amount -> %d, amount_0 -> %d \n", total_amount, amount_0);
		//print_integer_nodearray(one_item_matrix);
		MBdynarray_add_tail(all_comb_matrix, one_item_matrix);
		//return all_comb_matrix;
	}else{
		int i;
		MBdynarray *all_comb_0_matrix = MBdynarray_create(0);
		get_all_comb_binary_matrix(total_amount-1, amount_0-1, all_comb_0_matrix);
		MBdynarray *array_0_index_matrix = MBdynarray_create(0);
		//MBdynarray *all_comb_1_matrix = get_all_comb_binary_matrix(total_amount-1, amount_0);
		for(i = 0; i < total_amount; i++){
			int found_array_index = check_array_included(array_0_index_matrix, i);
			if (found_array_index == -1){
				MBdynarray_add_tail(array_0_index_matrix, i);
				int all_comb_0_i = 0;
				for (;all_comb_0_i < MBdynarray_get_count(all_comb_0_matrix);all_comb_0_i++){
					MBdynarray *one_item_matrix = MBdynarray_create(0);
					MBdynarray *comb_0_item = (MBdynarray*)MBdynarray_get(all_comb_0_matrix, all_comb_0_i);
					int comb_0_i = 0;
					for (;comb_0_i < MBdynarray_get_count(comb_0_item);comb_0_i++){
						int comb_0_val = (int*)MBdynarray_get(comb_0_item, comb_0_i);
						if (comb_0_val == 0){
							if (comb_0_i < i){
								int found_0_array_index = check_array_included(array_0_index_matrix, comb_0_i);
								if (found_0_array_index != -1){
									continue;
								}
							}else{
								int found_0_array_index = check_array_included(array_0_index_matrix, comb_0_i+1);
								if (found_0_array_index != -1){
									continue;
								}
							}
						}
						if (comb_0_i != i){
							MBdynarray_add_tail(one_item_matrix, comb_0_val);
						}else if(comb_0_i == i){
							MBdynarray_add_tail(one_item_matrix, 0);
							MBdynarray_add_tail(one_item_matrix, comb_0_val);
						}
					}
					if( i == total_amount - 1){
						int found_0_array_index = check_array_included(array_0_index_matrix, i);
						if (found_0_array_index != -1){
							MBdynarray_add_tail(one_item_matrix, 0);
						}
					}
					//printf("total_amount -> %d, amount_0 -> %d, MBdynarray_get_count(comb_0_item)->%d \n", total_amount, amount_0, MBdynarray_get_count(comb_0_item));
					//print_integer_nodearray(one_item_matrix);
					if (MBdynarray_get_count(one_item_matrix) == total_amount){
						MBdynarray_add_tail(all_comb_matrix, one_item_matrix);
					}
				}
			}
			//print_integer_nodearray(one_item_matrix);
			/*MBdynarray *tmp_one_item_matrix = MBdynarray_create(0);
			int all_comb_1_i = 0;
			for (;all_comb_1_i < MBdynarray_get_count(all_comb_1_matrix);all_comb_1_i++){
				MBdynarray *comb_1_item = (MBdynarray*)MBdynarray_get(all_comb_1_matrix, all_comb_1_i);
				int comb_1_i = 0;
				for (;comb_1_i < MBdynarray_get_count(comb_1_item);comb_1_i++){
					int comb_1_val = (int*)MBdynarray_get(comb_1_item, comb_1_i);
					if (comb_1_i != i){
						MBdynarray_add_tail(tmp_one_item_matrix, comb_1_val);
					}else if(comb_1_i == i){
						MBdynarray_add_tail(tmp_one_item_matrix, 1);
						MBdynarray_add_tail(tmp_one_item_matrix, comb_1_val);
					}
				}
				printf("total_amount -> %d, amount_0 -> %d, MBdynarray_get_count(comb_1_item)->%d \n", total_amount, amount_0, MBdynarray_get_count(comb_1_item));
				print_integer_nodearray(tmp_one_item_matrix);
				MBdynarray_add_tail(all_comb_matrix, tmp_one_item_matrix);
			}*/
		}
		//return all_comb_matrix;
	}
};





/*MBdynarray * get_next_comb_binary_matrix(MBdynarray * pre_binary_matrix){
	MBdynarray *next_comb_matrix = MBdynarray_create(0);
	int find_1_0_comb = 0;
	int find_1_index = -1;
	int i;
	for(i = 0; i < MBdynarray_get_count(pre_binary_matrix); i++){
		int binary_matrix_item = (int*) MBdynarray_get(pre_binary_matrix, i);
		if ((binary_matrix_item == 1) && (find_1_0_comb == 0)){
			find_1_index = i;
		}else if ((binary_matrix_item == 0) && (find_1_0_comb == 0)){
			if ((find_1_index != -1) && (find_1_index == (i-1))){
				find_1_0_comb = 1;
				break;
			}
		}
	}
	if (find_1_0_comb == 1){
		int j;
		for(j = 0; j < MBdynarray_get_count(pre_binary_matrix); j++){
			if (j == find_1_index){
				//int item_1 = 0;
				MBdynarray_add_tail(next_comb_matrix, 0);
			}else if ((j - 1) == find_1_index){
				//int item_0 = 1;
				MBdynarray_add_tail(next_comb_matrix, 1);
			}else{
				int binary_matrix_item = (int*) MBdynarray_get(pre_binary_matrix, j);
				MBdynarray_add_tail(next_comb_matrix, binary_matrix_item);
			}
		}
	}
	return next_comb_matrix;
};


MBdynarray * get_initial_binary_matrix(int total_amount, int amount_0){
	MBdynarray *initial_binary_matrix = MBdynarray_create(0);
	int amount_1 = total_amount - amount_0;
	int i;
	for(i = 0; i < total_amount; i++){
		if ( i < amount_1){
			//int item_1 = 1;
			MBdynarray_add_tail(initial_binary_matrix, 1);
		}else{
			//int item_0 = 0;
			MBdynarray_add_tail(initial_binary_matrix, 0);
		}
	}
	return initial_binary_matrix;
};*/




void bayesian_infer_underlay_cal(MBdynarray *selectedoverlaylinkarray, MBdynarray *underlaylinkarray, MBdynarray *uncovered_underlay_array){
	//printf("111!\n");
	MBdynarray *considered_underlay_index = MBdynarray_create(0);
	MBdynarray *abnormal_overlay_index = MBdynarray_create(0);
	MBdynarray *potential_underlay_index = MBdynarray_create(0);

	int i;
	for (i = 0; i < MBdynarray_get_count(selectedoverlaylinkarray); i++) {
		Overlaylink *overlaylinkitem = (Overlaylink*)MBdynarray_get(selectedoverlaylinkarray, i);
		if	(overlaylinkitem->measured_success == 1){
			MBdynarray *underlaylinkarray = overlaylinkitem->underlaylinkarray;
			int j;
			for (j = 0; j < MBdynarray_get_count(underlaylinkarray); j++) {
				Underlaylink *underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray, j);
				if (check_array_included(considered_underlay_index, underlaylinkitem->index) == -1){
					underlaylinkitem->bayesian_inferred_success = 1;
					underlaylinkitem->bayesian_prob = 0.0;
					insert_sorted_array(considered_underlay_index, underlaylinkitem->index);
				}
			}
		}else{
			if (check_array_included(abnormal_overlay_index, i) == -1){
				insert_sorted_array(abnormal_overlay_index, i);
			}
		}
	}
	potential_underlay_index = get_complement_set_without_two(considered_underlay_index, uncovered_underlay_array, MBdynarray_get_count(underlaylinkarray));
	//potential_underlay_index =  get_complement_set(considered_underlay_index, MBdynarray_get_count(underlaylinkarray));

	/*int j;
	printf("Start to print potential_underlay_index array ...\n");
	for (j = 0; j < MBdynarray_get_count(potential_underlay_index); j++) {
		printf("%d ",  (int) MBdynarray_get(potential_underlay_index, j));
	}
	printf("\n");
	printf("End to print potential_underlay_index array ...\n");


	//int j;
	printf("Start to print underlay considered link array ...\n");
	for (j = 0; j < MBdynarray_get_count(considered_underlay_index); j++) {
		printf("%d ",  (int) MBdynarray_get(considered_underlay_index, j));
	}
	printf("\n");
	printf("End to print underlay considered link array ...\n");

	printf("Start to print abnormal overlay link array ...\n");
	for (j = 0; j < MBdynarray_get_count(abnormal_overlay_index); j++) {
		printf("%d ",  (int) MBdynarray_get(abnormal_overlay_index, j));
	}
	printf("\n");
	printf("End to print abnormal overlay link array ...\n");



	printf("333!\n");
*/

	//int comb_matrix[comb_len][MBdynarray_get_count(potential_underlay_index)];
	//double comb_prob_matrix[comb_len];
	//int comb_feasible_matrix[comb_len];


	double underlay_bayesian_prob_ue0_sum[MBdynarray_get_count(potential_underlay_index)];
	double prob_ue0_sum = 0.0;
	int under_i = 0;
	for(; under_i < MBdynarray_get_count(potential_underlay_index); under_i++){
		underlay_bayesian_prob_ue0_sum[under_i] = 0.0;
	}

	if (MBdynarray_get_count(potential_underlay_index) <= 6){
		int comb_index;
		int comb_len = (int) pow(2, MBdynarray_get_count(potential_underlay_index));
		for (comb_index = 0; comb_index < comb_len; comb_index++) {
			int tmp_comb_matrix[MBdynarray_get_count(potential_underlay_index)];
			int underlay_index = 0;
			double underlay_prob = 1.0;
			int numerator = comb_index;
			for(;underlay_index < MBdynarray_get_count(potential_underlay_index);underlay_index++){
				int denominator = (int) pow(2, MBdynarray_get_count(potential_underlay_index)- underlay_index-1);
				int floor_num = numerator/denominator;
				//printf("1: denominator->%d, floor_num->%d, numerator->%d\n", denominator, floor_num, numerator);
				numerator = numerator%denominator;
				//printf("2: denominator->%d, floor_num->%d, numerator->%d\n", denominator, floor_num, numerator);
				tmp_comb_matrix[underlay_index] = floor_num;
				//comb_matrix[comb_index][underlay_index] = floor_num;
				if (floor_num == 1){
					underlay_prob = underlay_prob*(1-(((Underlaylink*)MBdynarray_get(underlaylinkarray,(int)MBdynarray_get(potential_underlay_index, underlay_index)))->failure_prob));
				}else{
					underlay_prob = underlay_prob*((((Underlaylink*)MBdynarray_get(underlaylinkarray,(int)MBdynarray_get(potential_underlay_index, underlay_index)))->failure_prob));
				}
			}
			/*printf("Start to print tmp_comb_matrix array ...\n");
			int j;
			for (j = 0; j < MBdynarray_get_count(potential_underlay_index); j++) {
				printf("%d ",  tmp_comb_matrix[j]);
			}
			printf("\n");
			printf("underlay_prob->%e.", underlay_prob);
			printf("End to print tmp_comb_matrix array ...\n");*/

			int found_underlay_error = 0;
			int abnormal_overlay_i = 0;
			for(; abnormal_overlay_i < MBdynarray_get_count(abnormal_overlay_index); abnormal_overlay_i ++){
				int found_overlay = 0;
				int found_overlay_error = 1;
				Overlaylink *abnormal_overlaylinkitem = (Overlaylink*)MBdynarray_get(selectedoverlaylinkarray, (int)MBdynarray_get(abnormal_overlay_index, abnormal_overlay_i));
				MBdynarray *ab_underlaylinkarray = abnormal_overlaylinkitem->underlaylinkarray;
				int j;
				for (j = 0; j < MBdynarray_get_count(ab_underlaylinkarray); j++) {
					Underlaylink *underlaylinkitem = (Underlaylink*)MBdynarray_get(ab_underlaylinkarray, j);
					int found_underlay_i = check_array_included(potential_underlay_index, underlaylinkitem->index);
					if ( found_underlay_i != -1){
						found_overlay = 1;
						if (tmp_comb_matrix[found_underlay_i] == 0){
							found_overlay_error = 0;
							break;
						}
					}
				}
				if ((found_overlay == 1) && (found_overlay_error == 1)){
					found_underlay_error = 1;
					break;
				}
			}

			if (found_underlay_error == 0){
				prob_ue0_sum = prob_ue0_sum + underlay_prob;
				int underlay_index = 0;
				for(;underlay_index < MBdynarray_get_count(potential_underlay_index);underlay_index++){
					if (tmp_comb_matrix[underlay_index] == 0){
						underlay_bayesian_prob_ue0_sum[underlay_index] = underlay_bayesian_prob_ue0_sum[underlay_index] + underlay_prob;
					}
				}
			}
		}
	}else{
		//printf("this place!!\n");
		int largest_0_number = 5;
		int index_0_number = 1;
		for (;index_0_number <= largest_0_number; index_0_number ++ ){
			//printf("inder_0_number: %d\n", index_0_number);
			MBdynarray * all_comb_matrix = MBdynarray_create(0);
			get_all_comb_binary_matrix(MBdynarray_get_count(potential_underlay_index), index_0_number, all_comb_matrix);
			//get_all_comb_binary_matrix(MBdynarray_get_count(potential_underlay_index), index_0_number, MBdynarray * all_comb_matrix);
			//printf("11111\n");
			int all_comb_index = 0;
			double underlay_prob = 1.0;
			for (; all_comb_index < MBdynarray_get_count(all_comb_matrix); all_comb_index++ ){
				MBdynarray *comb_item = (MBdynarray*)MBdynarray_get(all_comb_matrix, all_comb_index);
				//printf("comb_item_matrix\n");
				//print_integer_nodearray(comb_item);
				int item_i = 0;
				for (;item_i < MBdynarray_get_count(comb_item);item_i++){
					int item_i_val = (int*)MBdynarray_get(comb_item, item_i);
					if (item_i_val == 1){
						underlay_prob = underlay_prob*(1-(((Underlaylink*)MBdynarray_get(underlaylinkarray,(int)MBdynarray_get(potential_underlay_index, item_i)))->failure_prob));
					}else{
						underlay_prob = underlay_prob*((((Underlaylink*)MBdynarray_get(underlaylinkarray,(int)MBdynarray_get(potential_underlay_index, item_i)))->failure_prob));
					}
				}
				int found_underlay_error = 0;
				int abnormal_overlay_i = 0;
				for(; abnormal_overlay_i < MBdynarray_get_count(abnormal_overlay_index); abnormal_overlay_i ++){
					int found_overlay = 0;
					int found_overlay_error = 1;
					Overlaylink *abnormal_overlaylinkitem = (Overlaylink*)MBdynarray_get(selectedoverlaylinkarray, (int)MBdynarray_get(abnormal_overlay_index, abnormal_overlay_i));
					MBdynarray *ab_underlaylinkarray = abnormal_overlaylinkitem->underlaylinkarray;
					int j;
					for (j = 0; j < MBdynarray_get_count(ab_underlaylinkarray); j++) {
						Underlaylink *underlaylinkitem = (Underlaylink*)MBdynarray_get(ab_underlaylinkarray, j);
						int found_underlay_i = check_array_included(potential_underlay_index, underlaylinkitem->index);
						if ( found_underlay_i != -1){
							found_overlay = 1;
							int item_i_val = (int*)MBdynarray_get(comb_item, found_overlay);
							if (item_i_val == 0){
								found_overlay_error = 0;
								break;
							}
						}
					}
					if ((found_overlay == 1) && (found_overlay_error == 1)){
						found_underlay_error = 1;
						break;
					}
				}

				if (found_underlay_error == 0){
					//printf("underlay error!\n");
					prob_ue0_sum = prob_ue0_sum + underlay_prob;
					int underlay_index = 0;
					for(;underlay_index < MBdynarray_get_count(potential_underlay_index);underlay_index++){
						int item_i_val = (int*)MBdynarray_get(comb_item, item_i);
						if (item_i_val == 0){
							underlay_bayesian_prob_ue0_sum[underlay_index] = underlay_bayesian_prob_ue0_sum[underlay_index] + underlay_prob;
						}
					}
				}
			}
		}
	}
	//printf("55555555\n");
/*
	printf("Start to print underlay_bayesian_prob_ue0_sum array ...\n");
	int k;
	for (k = 0; k < MBdynarray_get_count(potential_underlay_index); k++) {
		printf("%e ",  underlay_bayesian_prob_ue0_sum[k]);
	}
	printf("\n");
	printf("prob_ue0_sum->%e.", prob_ue0_sum);
	printf("End to print underlay_bayesian_prob_ue0_sum array ...\n");
*/
	//printf("222!\n");

/*
		{
					comb_feasible_matrix[comb_index] = 0;
				}else{
					comb_feasible_matrix[comb_index] = 1;
				}





		comb_prob_matrix[comb_index] = underlay_prob;
	}
	//int comb_index = 0;
	for (comb_index = 0; comb_index < comb_len; comb_index++) {
		int found_underlay_error = 0;
		int abnormal_overlay_i = 0;
		for(; abnormal_overlay_i < MBdynarray_get_count(abnormal_overlay_index); abnormal_overlay_i ++){
			int found_overlay = 0;
			int found_overlay_error = 1;
			Overlaylink *abnormal_overlaylinkitem = (Overlaylink*)MBdynarray_get(selectedoverlaylinkarray, (int)MBdynarray_get(abnormal_overlay_index, abnormal_overlay_i));
			MBdynarray *ab_underlaylinkarray = abnormal_overlaylinkitem->underlaylinkarray;
			int j;
			for (j = 0; j < MBdynarray_get_count(ab_underlaylinkarray); j++) {
				Underlaylink *underlaylinkitem = (Underlaylink*)MBdynarray_get(ab_underlaylinkarray, j);
				int found_underlay_i = check_array_included(potential_underlay_index, underlaylinkitem->index);
				if ( found_underlay_i != -1){
					found_overlay = 1;
					if(comb_matrix[abnormal_overlay_i][found_underlay_i] == 0){
						found_overlay_error = 0;
						break;
					}
				}
			}
			if ((found_overlay == 1) && (found_overlay_error == 1)){
				found_underlay_error = 1;
				break;
			}
		}
		if (found_underlay_error == 1){
			comb_feasible_matrix[comb_index] = 0;
		}else{
			comb_feasible_matrix[comb_index] = 1;
		}
	}
	double P_sum = 0.0;
	//int comb_index = 0;
	for (comb_index = 0; comb_index < comb_len; comb_index++) {
		if (comb_feasible_matrix[comb_index] == 1){
			P_sum = P_sum + comb_prob_matrix[comb_index];
		}
	}*/
	//printf("come here!");
	double underlay_bayesian_prob[MBdynarray_get_count(potential_underlay_index)];
	int underlay_consider_i;
	for (underlay_consider_i = 0; underlay_consider_i < MBdynarray_get_count(potential_underlay_index); underlay_consider_i++) {
		underlay_bayesian_prob[underlay_consider_i] = underlay_bayesian_prob_ue0_sum[underlay_consider_i]/prob_ue0_sum;
	}

	int underlaylink_i;
	for(underlaylink_i = 0; underlaylink_i < MBdynarray_get_count(underlaylinkarray); underlaylink_i++){
		Underlaylink *selected_underlaylinkitem = (Underlaylink*)MBdynarray_get(underlaylinkarray, underlaylink_i);
		int selected_underlaylink_i = selected_underlaylinkitem->index;
		int found_p_underlay_i = check_array_included(potential_underlay_index, selected_underlaylink_i);
		//int found_u_underlay_i = check_array_included(uncovered_underlay_array, selected_underlaylink_i);

		if (found_p_underlay_i != -1) {
		/*	selected_underlaylinkitem->bayesian_prob = 0.0;
		}else{*/
			selected_underlaylinkitem->bayesian_prob = underlay_bayesian_prob[found_p_underlay_i];
		}
	}
	//printf("4444444!!\n");
};




void bayesian_underlay_link_array_infer_use_bound(MBdynarray *underlay_link_array, double bayesian_prob_bound){
	int j;
	for (j = 0; j < MBdynarray_get_count(underlay_link_array); j++) {
		Underlaylink* underlaylinkitem = (Underlaylink*) MBdynarray_get(underlay_link_array, j);
		if (underlaylinkitem->bayesian_prob < 0.0){
			underlaylinkitem->bayesian_inferred_success = -1;
		}else if(underlaylinkitem->bayesian_prob >= bayesian_prob_bound){
			underlaylinkitem->bayesian_inferred_success = 0;
		}else{
			underlaylinkitem->bayesian_inferred_success = 1;
		}
/*			print_underlay_link(underlaylinkitem);
		}
		printf("End to print underlay link array ...\n");*/
	}
	//printf("Underlay: index->%d, src_index->%d, dst_index->%d, failure_prob->%e\r\n", underlay_link_item->index, underlay_link_item->src_index, underlay_link_item->dst_index, underlay_link_item->failure_prob);
};
