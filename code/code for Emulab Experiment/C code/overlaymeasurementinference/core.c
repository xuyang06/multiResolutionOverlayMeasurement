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
	/*printf("start\n");
    MBdynarray *array = MBdynarray_create(0);  No preferred size
    unsigned int i;
    void * data;
    char *elements[] = {"A", "B", "C", "D", "E", "F"};
    const unsigned int n = sizeof(elements) / sizeof(const char*);

    for (i = 0; i < n; i++) {
        if (i % 2 == 0) {
            MBdynarray_add_head(array, elements[i]);
        }
        else {
            MBdynarray_add_tail(array, elements[i]);
        }
    }

    MBdynarray_insert(array, 0, "X");                               Same as MBdynarray_addhead
    MBdynarray_insert(array, MBdynarray_get_count(array) / 2, "Y");     Insert in the middle
    MBdynarray_insert(array, MBdynarray_get_count(array), "Z");         Same as MBdynarray_add_tail

    MBdynarray_set(array, MBdynarray_get_count(array) / 2, "P");
    MBdynarray_set(array, MBdynarray_get_count(array), "Q");            Same as MBdynarray_add_tail

    for (i = 0; i < MBdynarray_get_count(array); i++) {
        printf("%d: %s\n", i, (const char*)MBdynarray_get(array, i));
    }
    putchar('\n');

    for (i = 0; MBdynarray_get_count(array); i++) {
        const unsigned int action = i % 3;
        if (action == 0) {
            data = MBdynarray_remove_head(array);
        }
        else if (action == 1) {
            data = MBdynarray_remove_tail(array);
        }
        else {
            data = MBdynarray_remove(array, MBdynarray_get_count(array) / 2);
        }
        printf("Removed: %s\n", (const char*)data);
    }

    MBdynarray_delete(array);*/
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
	//printf("current absolute path:%s\n", current_absolute_path);

	//char * routing_config_file_name = "/data/routing";
	//char * ping_directory_name = "/data/3";
	char * result_file_name = "/data/analysis_result";

	//char * routing_config_file_name = "/data/4/routing";
	//char * ping_directory_name = "/data/4/data";

	//char * routing_config_file_name = "/data/5/routing";
	//char * ping_directory_name = "/data/5/data";

	char * routing_config_file_name = "/data/4/routing";
	char * ping_directory_name = "/data/4/data";

	//char * routing_config_file_name = "/data/7/routing";
	//char * ping_directory_name = "/data/7/3";

	char * failure_config_file_name = "/data/4/failuretable";
	//double failure_prob = 0.02;

	char current_absolute_path_copy[MAX_SIZE];
	strcpy(current_absolute_path_copy, current_absolute_path);
	char current_absolute_path_copy2[MAX_SIZE];
	strcpy(current_absolute_path_copy2, current_absolute_path);
	char current_absolute_path_copy3[MAX_SIZE];
	strcpy(current_absolute_path_copy3, current_absolute_path);




	const char * routing_config_file_absolute_path = strcat(current_absolute_path, routing_config_file_name);
	//printf("here\n");
	const char * failure_config_file_absolute_path = strcat(current_absolute_path_copy3, failure_config_file_name);
	const char * result_file_absolute_path = strcat(current_absolute_path_copy2, result_file_name);

	FILE * result_file_fp = NULL;
	if ((result_file_fp = fopen(result_file_absolute_path, "wb+")) == NULL ){
		printf("could not open result file!\n");
	    fprintf(stderr, "Can't open %s.\n", result_file_absolute_path);
	    exit(1);
	}



	/*char * allnodes_print_file_name = "/data/allnodes_print_result";
	const char * allnodes_print_file_absolute_path = strcat(current_absolute_path_copy4, allnodes_print_file_name);

	FILE * allnodes_print_file_fp = NULL;
	if ((allnodes_print_file_fp = fopen(allnodes_print_file_absolute_path, "wb+")) == NULL ){
		printf("could not open result file!\n");
		fprintf(stderr, "Can't open %s.\n", allnodes_print_file_absolute_path);
		exit(1);
	}*/



	MBdynarray *overlaylinkarray = MBdynarray_create(0);
	MBdynarray *underlaylinkarray = MBdynarray_create(0);
	//printf("here\n");

    read_routing_config_file(routing_config_file_absolute_path, overlaylinkarray, underlaylinkarray);
    //printf("here1\n");
    read_failure_table_config_file(failure_config_file_absolute_path, underlaylinkarray);
    //printf("begin to print\n");
    //printf("print original overlay link array !\n");
    //print_overlay_link_array(overlaylinkarray);
    //printf("Final overlay condition...\n");
    //print_overlay_link_array(overlaylinkarray);
    //printf("Final underlay condition...\n");
    //print_underlay_link_array(underlaylinkarray);


    int min_start;
    int max_start;
    int min_end;
    int max_end;
    MBdynarray *failureoverlaylinkarray = MBdynarray_create(0);
    MBdynarray *allnodes_pingarrary = MBdynarray_create(0);
    MBdynarray *noncoverednodearray = MBdynarray_create(0);
    MBdynarray *uncovered_underlay_array = MBdynarray_create(0);

    //read_test_ping_result_directory(strcat(current_absolute_path_copy, ping_directory_name), &min_start, &max_start, &min_end, &max_end, failurelinkarray, allnodes_pingarrary);

    //printf("start!\n");
    read_ping_result_directory(strcat(current_absolute_path_copy, ping_directory_name), &min_start, &max_start, &min_end, &max_end, failureoverlaylinkarray, allnodes_pingarrary);
    //printf("end!\n");

    //print_allnodes_pingarray(allnodes_pingarrary);
    //read_ping_result_directory(current_absolute_path, &min_start, &max_start, &min_end, &max_end, failurelinkarray, allnodes_pingarrary);
    //print_failure_overlay_link_array(failureoverlaylinkarray);
    MBdynarray *failureunderlaylinkarray = get_failure_underlay_link_array(failureoverlaylinkarray, overlaylinkarray);
	//print_failure_underlay_link_array(failureunderlaylinkarray);
    //printf("min_start: %d, max_start: %d, min_end: %d, max_end: %d\n", min_start, max_start, min_end, max_end);
    //printf("allnodes_pingarrary: len: %d\n", MBdynarray_get_count(allnodes_pingarrary));
    MBdynarray * allnodes_overlaymeasurement_array = MBdynarray_create(0);
    //printf("begin to parsing!\n");
    parsing_ping_result(&min_start, &max_start, &min_end, &max_end, failureoverlaylinkarray, allnodes_pingarrary, allnodes_overlaymeasurement_array);

    //printf("end to parsing!\n");
    //print_allnodes_overlayMeasurement(allnodes_overlaymeasurement_array);
    //printf("end2!\n");
    parsing_overlaymeasurement_result(allnodes_overlaymeasurement_array, overlaylinkarray);
    //print_allnodes_overlayMeasurement(allnodes_overlaymeasurement_array);
    //print_overlay_link_array(overlaylinkarray);
    //print_underlay_link_array(underlaylinkarray);






    generate_underlay_coverage_num(underlaylinkarray);
    //print_overlay_link_array(overlaylinkarray);
    //printf("overlaylink print!\n");
    //print_overlay_link_array(overlaylinkarray);
    //print_underlay_link_array(underlaylinkarray);
    //overlaylinkarray = greedy_select_overlay(overlaylinkarray, underlaylinkarray);
    MBdynarray *selected_overlayLink_array;
    //printf("end13!\n");
    selected_overlayLink_array = greedy_select_overlay(overlaylinkarray, underlaylinkarray, noncoverednodearray);
    MBdynarray * overlay_node_index_array = get_overlay_node_array(selected_overlayLink_array);
    //printf("overlay_node_index_array -> %d\n", MBdynarray_get_count(overlay_node_index_array));
    int total_overlay_node_count = MBdynarray_get_count(overlay_node_index_array);
    int iteration_number = total_overlay_node_count - 2;
    int iteration_i;
    uncovered_underlay_array = MBdynarray_create(0);
    get_uncovered_underlay_array(selected_overlayLink_array, underlaylinkarray, uncovered_underlay_array);
    //printf("uncovered_underlay_array12!\n");
    //print_integer_nodearray(uncovered_underlay_array);
    //printf("overlaylink select!\n");
    //print_overlay_link_array(selected_overlayLink_array);
            //print_underlay_link_array(underlaylinkarray);
    //printf("end4!\n");
    //printf("noncoverednodearray22!\n");
    //print_integer_nodearray(noncoverednodearray);
    //printf("uncovered_underlay_array1!\n");
    //print_integer_nodearray(uncovered_underlay_array);
    //print_underlay_link_array(underlaylinkarray);
    //print_overlay_link_array(selected_overlayLink_array);
    greedy_infer_underlay(selected_overlayLink_array, underlaylinkarray, uncovered_underlay_array);
    //printf("here!\n");
    bayesian_infer_underlay_cal(selected_overlayLink_array, underlaylinkarray, uncovered_underlay_array);
    //printf("bayesian infer underlay link infer!\n");
    //print_underlay_link_array(underlaylinkarray);
    double bayesian_prob_bound = 0.50;
    bayesian_underlay_link_array_infer_use_bound(underlaylinkarray, bayesian_prob_bound);
    //print_underlay_link_array(underlaylinkarray);
    cal_underlay_link_array_result(result_file_fp, failureunderlaylinkarray, underlaylinkarray, selected_overlayLink_array, uncovered_underlay_array);
    //print_underlay_link_array(underlaylinkarray);

    //printf("start next!\n");
    for (iteration_i = 0; iteration_i < iteration_number; iteration_i ++){
    	//printf("iteration: %d\n", iteration_i);
    	selected_overlayLink_array = greedy_select_overlay_exclude_one_item(selected_overlayLink_array, overlaylinkarray, underlaylinkarray, noncoverednodearray);

        uncovered_underlay_array = MBdynarray_create(0);
        get_uncovered_underlay_array(selected_overlayLink_array, underlaylinkarray, uncovered_underlay_array);
        //printf("uncovered_underlay_array12!\n");
        //print_integer_nodearray(uncovered_underlay_array);
            //uncovered_underlay_array =  get_complement_set(covered_underlay, MBdynarray_get_count(underlaylinkarray));
            //print_selected_overlayMeasurement(allnodes_overlaymeasurement_array, selected_overlayLink_array);
        //printf("overlaylink select!\n");
        //print_overlay_link_array(selected_overlayLink_array);
        //print_underlay_link_array(underlaylinkarray);
        //printf("end4!\n");
        //printf("noncoverednodearray22!\n");
        //print_integer_nodearray(noncoverednodearray);
        //printf("uncovered_underlay_array1!\n");
        //print_integer_nodearray(uncovered_underlay_array);
        //print_underlay_link_array(underlaylinkarray);

            //MBdynarray *testarray = MBdynarray_create(0);
            //add_item(testarray);
            //printf("testarray\n");
            //print_integer_nodearray(testarray);
        greedy_infer_underlay(selected_overlayLink_array, underlaylinkarray, uncovered_underlay_array);
        //printf("greedy infer underlay link infer!\n");
            //print_underlay_link_array(underlaylinkarray);
        //printf("start next........!!!!\n");
        bayesian_infer_underlay_cal(selected_overlayLink_array, underlaylinkarray, uncovered_underlay_array);
        //printf("11111111!!!\n");
        //printf("bayesian infer underlay link infer!\n");
        //print_underlay_link_array(underlaylinkarray);
        double bayesian_prob_bound = 0.50;
        bayesian_underlay_link_array_infer_use_bound(underlaylinkarray, bayesian_prob_bound);
        //print_underlay_link_array(underlaylinkarray);
        //printf("start next........!!!!!!!!\n");
        cal_underlay_link_array_result(result_file_fp, failureunderlaylinkarray, underlaylinkarray, selected_overlayLink_array, uncovered_underlay_array);
        //printf("start next........!!!!!!!!!!!\n");
        //print_underlay_link_array(underlaylinkarray);

    }



    //printf("end23!\n");
    //print_overlay_link_array(overlaylinkarray);





    printf("parsing ping result -- min_start: %d, max_start: %d, min_end: %d, max_end: %d\n", min_start, max_start, min_end, max_end);

    printf("failure underlay link number: %d\n", MBdynarray_get_count(failureunderlaylinkarray));
    print_failure_underlay_link_array(failureunderlaylinkarray);

    //allnodes_print_file_fp
    //printf("end43!\n");
    //printf("end3!\n");
    //selected_overlayLink_array = greedy_select_overlay_exclude_one_item(selected_overlayLink_array, overlaylinkarray, underlaylinkarray, noncoverednodearray);

    fclose(result_file_fp);

    return 0;
}

/*int main(void){
	int total_amount = 10;
	int amount_0 = 3;
	MBdynarray * all_comb_matrix = get_all_comb_binary_matrix(total_amount, amount_0 );
	printf("here\n");
	int i = 0;
	for (; i < MBdynarray_get_count(all_comb_matrix); i ++ ){
		MBdynarray *comb_item = (MBdynarray*)MBdynarray_get(all_comb_matrix, i);
		print_integer_nodearray(comb_item);
	}

	printf("index -> %d\n", MBdynarray_get_count(all_comb_matrix));

	return 0;
}*/
