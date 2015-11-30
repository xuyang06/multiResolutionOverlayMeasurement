/*
 * linkstructure.h
 *
 *  Created on: Apr 8, 2014
 *      Author: xuyang06
 */

#ifndef LINKSTRUCTURE_H_
#define LINKSTRUCTURE_H_

#include "dynarray.h"
#include "filereader.h"


struct Underlaylink {
    int      index;
    int 	coverage_num;
    int 	real_coverage_num;
    double	  cost;
    double    failure_prob;
    int 	greedy_inferred_success;
    int		bayesian_inferred_success;
    int 	real_success;
    double 		bayesian_prob;
    int 	src_index;
    int 	dst_index;
};

typedef struct Underlaylink Underlaylink;

//typedef MBdynarray *Underlaylinkarray;

void generate_underlay_coverage_num(MBdynarray *underlaylinkarray);


struct Overlaylink{
	int 	index;
	double	cost;
	MBdynarray *underlaylinkarray;
	int 	measured_success;
	int 	src_index;
	int 	dst_index;
};
typedef struct Overlaylink Overlaylink;


struct OverlayNode{
	int 	index;
	int     related_overlay_number;
};
typedef struct OverlayNode OverlayNode;



//typedef MBdynarray *Overlaylinkarray;

Overlaylink * init_overlaylink(int src_index, int dst_index, int total_index);
int check_overlayarray_included(MBdynarray *overlaylinkarray, int src_index, int dst_index);
Underlaylink * init_underlaylink(int src_index, int dst_index, int total_index);
int check_underlayarray_included(MBdynarray *underlaylinkarray, int src_index, int dst_index);
void generate_underlay_coverage_num(MBdynarray *underlaylinkarray);
void cal_overlayNode_info(MBdynarray *selected_overlayLink_array, MBdynarray *overlay_node_array);
void get_complement_set(MBdynarray *sorted_underlay_index, int upper_len, MBdynarray *complement_set);
void print_underlay_link(Underlaylink *underlay_link_item);
void print_underlay_link_array(MBdynarray *underlay_link_array);
void print_overlay_link(Overlaylink *overlay_link_item);
void print_overlay_link_array(MBdynarray *overlay_link_array);

void cal_overlay_cost(MBdynarray *overlaylinkarray);
MBdynarray *greedy_select_overlay_exclude_one_item(MBdynarray *selected_overlayLink_array, MBdynarray *overlaylinkarray, MBdynarray *underlaylinkarray, MBdynarray *noncoverednodearray);
MBdynarray *greedy_select_overlay(MBdynarray *overlaylinkarray, MBdynarray *underlaylinkarray, MBdynarray *noncoverednodearray);
MBdynarray *get_overlay_node_array(MBdynarray *selected_overlayLink_array);


//void greedy_infer_underlay(MBdynarray *selectedoverlaylinkarray, MBdynarray *underlaylinkarray, );
Overlaylink * default_overlaylink();
Underlaylink * default_underlaylink();
void greedy_infer_underlay(MBdynarray *selectedoverlaylinkarray, MBdynarray *underlaylinkarray, MBdynarray *uncovered_underlay_array);
void bayesian_infer_underlay_cal(MBdynarray *selectedoverlaylinkarray, MBdynarray *underlaylinkarray, MBdynarray *uncovered_underlay_array);
//void cal_underlay_link_array_result(FILE * result_file_fp, MBdynarray *failureunderlaylinkarray, MBdynarray *underlay_link_array, MBdynarray *overlaylinkarray, MBdynarray *uncovered_underlay_array);
int check_removedoverlayindexarray_included(MBdynarray *removed_overlay_index_array, int overlay_index);


#endif /* LINKSTRUCTURE_H_ */
