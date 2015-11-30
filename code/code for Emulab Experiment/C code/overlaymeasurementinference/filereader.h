/*
 * filereader.h
 *
 *  Created on: Apr 28, 2014
 *      Author: xuyang06
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_

#include "dynarray.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <math.h>
#include <stdlib.h>

struct HeartPing {
	int 	src_index;
    int      dst_index;
    double    rtt_value;
    int       index;
};

typedef struct HeartPing HeartPing;

struct FailureOverlayLink {
	int     src_index;
	int     dst_index;
};

typedef struct FailureOverlayLink FailureOverlayLink;


struct FailureUnderlayLink {
	int     src_index;
	int     dst_index;
};

typedef struct FailureUnderlayLink FailureUnderlayLink;

struct SingleNodeOverlayMeasurement{
	double regular_avg;
	double regular_dev;
	double regular_median;
	MBdynarray * regular_rtt_array;
	int regular_count;
	double abnoraml_avg;
	double abnormal_dev;
	double abnormal_median;
	MBdynarray * abnormal_rtt_array;
	int abnormal_count;
	//int 	measured_success;
	int 	src_index;
	int 	dst_index;
};

typedef struct SingleNodeOverlayMeasurement SingleNodeOverlayMeasurement;

/*void read_routing_config_file(const char * filename, MBdynarray *overlaylinkarray, MBdynarray *underlaylinkarray, double failure_prob );
int check_failureoverlylinkarray_included(MBdynarray *failureoverlaylinkarray, int src_index, int dst_index);
int check_failureunderlylinkarray_included(MBdynarray *failureunderlaylinkarray, int src_index, int dst_index);
void read_ping_result_file(char* directory_name, char * filename, int *min_start, int *max_start, int *min_end, int *max_end, MBdynarray *singlenodepingarrary, MBdynarray *failurelinkarray);
void read_ping_result_directory(char * directory_name, int *min_start, int *max_start, int *min_end, int *max_end, MBdynarray *failurelinkarray, MBdynarray *allnodes_pingarrary);
void parsing_ping_result(int *min_start, int *max_start, int *min_end, int *max_end, MBdynarray *failurelinkarray, MBdynarray *allnodes_pingarrary, MBdynarray *allnodes_overlaymeasurement_array);
void parsing_overlaymeasurement_result(MBdynarray *allnodes_overlaymeasurement_array, MBdynarray *overlaylinkarray);


void print_failure_overlay_link(FailureOverlayLink *failure_overlay_link_item);
void print_failure_overlay_link_array(MBdynarray *failure_overlay_link_array);

void print_failure_underlay_link(FailureUnderlayLink *failure_underlay_link_item);
void print_failure_underlay_link_array(MBdynarray *failure_underlay_link_array);

void print_allnodes_pingarray(MBdynarray * allnodes_pingarrary);
void print_allnodes_overlayMeasurement(MBdynarray *allnodes_overlaymeasurement_array);

void read_test_ping_result_directory(char * directory_name, int *min_start, int *max_start, int *min_end, int *max_end, MBdynarray *failurelinkarray, MBdynarray *allnodes_pingarrary);
void bubbleSort(MBdynarray * arr);*/

#endif /* FILEREADER_H_ */
