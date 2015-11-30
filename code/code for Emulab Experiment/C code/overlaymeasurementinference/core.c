/*
 * example.c
 *
 *  Created on: Apr 9, 2014
 *      Author: xuyang06
 */


#include <stdio.h>

#include "dynarray.h"
#include "filereader.h"
#include <math.h>
#include "linkstructure.h"
#define MAX_SIZE 100

int main(void)
{
	char current_absolute_path[MAX_SIZE];
	int cnt = readlink("/proc/self/exe", current_absolute_path, MAX_SIZE);
	if (cnt < 0 || cnt >= MAX_SIZE)
	{
	    printf("***Error***\n");
	    exit(-1);
	}
	int i;
	int slash_index = 1;
	for (i = cnt; i >=0; --i)
	{
	    if (current_absolute_path[i] == '/')
	    {
	    	if (slash_index == 1){
	    		slash_index = 0;
	    		continue;
	    	}
	        current_absolute_path[i] = '\0';
	        break;
	    }
	}
	
	char * result_file_name = "/data/analysis_result";

	
	char * routing_config_file_name = "/data/4/routing";
	char * ping_directory_name = "/data/4/data";

	
	char * failure_config_file_name = "/data/4/failuretable";
	
	char current_absolute_path_copy[MAX_SIZE];
	strcpy(current_absolute_path_copy, current_absolute_path);
	char current_absolute_path_copy2[MAX_SIZE];
	strcpy(current_absolute_path_copy2, current_absolute_path);
	char current_absolute_path_copy3[MAX_SIZE];
	strcpy(current_absolute_path_copy3, current_absolute_path);




	const char * routing_config_file_absolute_path = strcat(current_absolute_path, routing_config_file_name);
	const char * failure_config_file_absolute_path = strcat(current_absolute_path_copy3, failure_config_file_name);
	const char * result_file_absolute_path = strcat(current_absolute_path_copy2, result_file_name);

	FILE * result_file_fp = NULL;
	if ((result_file_fp = fopen(result_file_absolute_path, "wb+")) == NULL ){
		printf("could not open result file!\n");
	    fprintf(stderr, "Can't open %s.\n", result_file_absolute_path);
	    exit(1);
	}



	



	MBdynarray *overlaylinkarray = MBdynarray_create(0);
	MBdynarray *underlaylinkarray = MBdynarray_create(0);
	
    read_routing_config_file(routing_config_file_absolute_path, overlaylinkarray, underlaylinkarray);
    read_failure_table_config_file(failure_config_file_absolute_path, underlaylinkarray);
    

    int min_start;
    int max_start;
    int min_end;
    int max_end;
    MBdynarray *failureoverlaylinkarray = MBdynarray_create(0);
    MBdynarray *allnodes_pingarrary = MBdynarray_create(0);
    MBdynarray *noncoverednodearray = MBdynarray_create(0);
    MBdynarray *uncovered_underlay_array = MBdynarray_create(0);

    read_ping_result_directory(strcat(current_absolute_path_copy, ping_directory_name), &min_start, &max_start, &min_end, &max_end, failureoverlaylinkarray, allnodes_pingarrary);

    MBdynarray *failureunderlaylinkarray = get_failure_underlay_link_array(failureoverlaylinkarray, overlaylinkarray);

    MBdynarray * allnodes_overlaymeasurement_array = MBdynarray_create(0);
    parsing_ping_result(&min_start, &max_start, &min_end, &max_end, failureoverlaylinkarray, allnodes_pingarrary, allnodes_overlaymeasurement_array);

    parsing_overlaymeasurement_result(allnodes_overlaymeasurement_array, overlaylinkarray);






    generate_underlay_coverage_num(underlaylinkarray);
    
    MBdynarray *selected_overlayLink_array;
   
    selected_overlayLink_array = greedy_select_overlay(overlaylinkarray, underlaylinkarray, noncoverednodearray);
    MBdynarray * overlay_node_index_array = get_overlay_node_array(selected_overlayLink_array);
    int total_overlay_node_count = MBdynarray_get_count(overlay_node_index_array);
    int iteration_number = total_overlay_node_count - 2;
    int iteration_i;
    uncovered_underlay_array = MBdynarray_create(0);
    get_uncovered_underlay_array(selected_overlayLink_array, underlaylinkarray, uncovered_underlay_array);
    greedy_infer_underlay(selected_overlayLink_array, underlaylinkarray, uncovered_underlay_array);
    
    bayesian_infer_underlay_cal(selected_overlayLink_array, underlaylinkarray, uncovered_underlay_array);
    double bayesian_prob_bound = 0.50;
    bayesian_underlay_link_array_infer_use_bound(underlaylinkarray, bayesian_prob_bound);
    cal_underlay_link_array_result(result_file_fp, failureunderlaylinkarray, underlaylinkarray, selected_overlayLink_array, uncovered_underlay_array);
    

    
    for (iteration_i = 0; iteration_i < iteration_number; iteration_i ++){
    	
    	selected_overlayLink_array = greedy_select_overlay_exclude_one_item(selected_overlayLink_array, overlaylinkarray, underlaylinkarray, noncoverednodearray);

        uncovered_underlay_array = MBdynarray_create(0);
        get_uncovered_underlay_array(selected_overlayLink_array, underlaylinkarray, uncovered_underlay_array);
        greedy_infer_underlay(selected_overlayLink_array, underlaylinkarray, uncovered_underlay_array);
        
        bayesian_infer_underlay_cal(selected_overlayLink_array, underlaylinkarray, uncovered_underlay_array);
        
        double bayesian_prob_bound = 0.50;
        bayesian_underlay_link_array_infer_use_bound(underlaylinkarray, bayesian_prob_bound);
        
        cal_underlay_link_array_result(result_file_fp, failureunderlaylinkarray, underlaylinkarray, selected_overlayLink_array, uncovered_underlay_array);
        

    }



    





    printf("parsing ping result -- min_start: %d, max_start: %d, min_end: %d, max_end: %d\n", min_start, max_start, min_end, max_end);

    printf("failure underlay link number: %d\n", MBdynarray_get_count(failureunderlaylinkarray));
    print_failure_underlay_link_array(failureunderlaylinkarray);

   
    fclose(result_file_fp);

    return 0;
}


