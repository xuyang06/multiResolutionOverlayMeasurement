/*
 * filereader.c
 *
 *  Created on: Apr 28, 2014
 *      Author: xuyang06
 */

#include "linkstructure.h"
#include "filereader.h"
#include <math.h>

#define MAXLINE 300000
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX_SIZE 300000
//#define MAX_VAL 60000000000.00


char *trim(char *s) {
    char *ptr;
    if (!s)
        return NULL;   // handle NULL string
    if (!*s)
        return s;      // handle empty string
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    return s;
};

int get_node_index_from_char(char * to_parsing_char){
	
	return atoi(to_parsing_char+4);
};


void read_failure_table_config_file(const char * filename, MBdynarray *underlaylinkarray){
	
	char line[MAXLINE];
	const char * split = " ";
	FILE * fp = NULL;
	int line_index = 0;
	printf("start to read file!\n");
	if ((fp = fopen(filename, "r")) == NULL ){
		printf("could not read file!\n");
		fprintf(stderr, "Can't open %s.\n", filename);
		exit(1);
	}
	
	char * start_p;
	char * end_p;
	char * failure_prob_p;
	int start_index;
	int end_index;
	

	int same_failure_prob = 0;
	
	while (fgets(line, MAXLINE, fp) != NULL){
		if (line_index == 0){
			char * first_line_str = trim(line);
			printf("11111\n");
			printf("first line: %s. \n", first_line_str);
			if (strcmp(first_line_str, "diff") == 0){
				same_failure_prob = 0;
			}else if (strcmp(first_line_str, "same") == 0){
				same_failure_prob = 1;
			}
			
		}else{
			if (same_failure_prob == 1){
				double failure_prob = atof(trim(line));
				printf("failure_prob: %f \n", failure_prob);
				int j;
				for (j = 0; j < MBdynarray_get_count(underlaylinkarray); j++) {
					Underlaylink* underlaylinkitem = (Underlaylink*) MBdynarray_get(underlaylinkarray, j);
					underlaylinkitem->failure_prob = failure_prob;
				}
				break;
			}else{
				start_p = trim(strtok(line, split));
				end_p = trim(strtok(NULL, split));
				failure_prob_p = trim(strtok(NULL, split));
				printf("failure_prob_p: %s \n", failure_prob_p);
				start_index = get_node_index_from_char(start_p);
				end_index = get_node_index_from_char(end_p);
				double failure_prob = atof(failure_prob_p);
				int found_underlay = check_underlayarray_included(underlaylinkarray, start_index, end_index);
				Underlaylink* underlaylinkitem = (Underlaylink*) MBdynarray_get(underlaylinkarray, found_underlay);
				underlaylinkitem->failure_prob = failure_prob;
				found_underlay = check_underlayarray_included(underlaylinkarray, end_index, start_index);
				Underlaylink* tmp_underlaylinkitem = (Underlaylink*) MBdynarray_get(underlaylinkarray, found_underlay);
				tmp_underlaylinkitem->failure_prob = failure_prob;
			}
		}
		line_index = line_index + 1;
	}
	fclose(fp);
	//printf("end to read file!\n");
};





void read_routing_config_file(const char * filename, MBdynarray *overlaylinkarray, MBdynarray *underlaylinkarray, double failure_prob){
	int overlay_index = 0;
	int underlay_index = 0;
	char line[MAXLINE];
	const char * split = " ";
	FILE * fp = NULL;
	int line_index = 0;
	printf("start to read file!\n");
	if ((fp = fopen(filename, "r")) == NULL ){
		printf("could not read file!\n");
		fprintf(stderr, "Can't open %s.\n", filename);
		exit(1);
	}
	
	Overlaylink * temp_overlaylink_item = default_overlaylink();
	char * start_p;
	char * end_p;
	int start_index;
	int end_index;
	int same_omit = 0;
	
	while (fgets(line, MAXLINE, fp) != NULL){
		
		if (line_index%2 == 0){
			start_p = trim(strtok(line, split));
			end_p = trim(strtok(NULL, split));
			if (strcmp(start_p, end_p) == 0){
				same_omit = 1;
			}else{
				same_omit = 0;
				
				start_index = get_node_index_from_char(start_p);
				end_index = get_node_index_from_char(end_p);
				temp_overlaylink_item = init_overlaylink(start_index, end_index, overlay_index);
				//print_overlay_link(temp_overlaylink_item);
				MBdynarray_add_tail(overlaylinkarray, temp_overlaylink_item);
				overlay_index = overlay_index + 1;
			}
			line_index = line_index + 1;
		}else{
			//print_overlay_link(temp_overlaylink_item);
			if (same_omit == 0){
				int pre_index;
				char * temp_p;
				temp_p = trim(strtok(line, split));
				pre_index = get_node_index_from_char(temp_p);
				//printf("pre_index: %d\n",pre_index);
				while(temp_p != NULL){
					temp_p = trim(strtok(NULL, split));
					//printf("temp_p: %s, len:%d\n", temp_p,strlen(temp_p));
					if ((temp_p == NULL) || (strlen(temp_p) == 0)){
						break;
					}
					
					int temp_index = get_node_index_from_char(temp_p);
					//printf("pre_index: %d, temp_index: %d\n",pre_index, temp_index);
					int found_underlay = check_underlayarray_included(underlaylinkarray, pre_index, temp_index);
					//printf("found_underlay: %d\n",found_underlay);
					if (found_underlay == -1){
						
						//Underlaylink * new_underlaylink_item = init_underlaylink(pre_index, temp_index, underlay_index, failure_prob);
						Underlaylink * new_underlaylink_item = init_underlaylink(pre_index, temp_index, underlay_index);
						//print_underlay_link(new_underlaylink_item);
						underlay_index = underlay_index + 1;
						
						//print_overlay_link(temp_overlaylink_item);
						MBdynarray_add_tail(temp_overlaylink_item->underlaylinkarray, new_underlaylink_item);
						MBdynarray_add_tail(underlaylinkarray, new_underlaylink_item);
						
					}else{
						Underlaylink* underlaylinkitem = (Underlaylink*) MBdynarray_get(underlaylinkarray, found_underlay);
						MBdynarray_add_tail(temp_overlaylink_item->underlaylinkarray, underlaylinkitem);
					}
					pre_index = temp_index;
					
					//print_overlay_link_array(overlaylinkarray);
					
					//MBdynarray_add_tail(underlaylinkarray, new_underlaylink_item);
					//underlay_index = underlay_index + 1;
				}
				
				temp_overlaylink_item->cost = (double) MBdynarray_get_count(temp_overlaylink_item->underlaylinkarray);
				//print_overlay_link_array(overlaylinkarray);
			}
			line_index = line_index + 1;
		}
	}
	//printf("end to read file!\n");
	fclose(fp);
	//printf("end to read file!\n");
};


int check_failureoverlylinkarray_included(MBdynarray *failureoverlaylinkarray, int src_index, int dst_index){
	int j;
	int found = -1;
	for (j = 0; j < MBdynarray_get_count(failureoverlaylinkarray); j++) {
		FailureOverlayLink* failureoverlylinkitem = (FailureOverlayLink*) MBdynarray_get(failureoverlaylinkarray, j);
		if ((src_index == failureoverlylinkitem->src_index) && (dst_index == failureoverlylinkitem->dst_index)){
			found = j;
			break;
		}
	}
	return found;
};


int check_failureunderlylinkarray_included(MBdynarray *failureunderlaylinkarray, int src_index, int dst_index){
	int j;
	int found = -1;
	for (j = 0; j < MBdynarray_get_count(failureunderlaylinkarray); j++) {
		FailureUnderlayLink* failureunderlylinkitem = (FailureUnderlayLink*) MBdynarray_get(failureunderlaylinkarray, j);
		if ((src_index == failureunderlylinkitem->src_index) && (dst_index == failureunderlylinkitem->dst_index)){
			found = j;
			break;
		}
	}
	return found;
};

int check_allnodes_overlaymeasurement_array_included(MBdynarray *allnodes_overlaymeasurement_array, int src_index, int dst_index){
	int j;
	int found = -1;
	for (j = 0; j < MBdynarray_get_count(allnodes_overlaymeasurement_array); j++) {
		SingleNodeOverlayMeasurement *singlenode_overlayMeasurement = (SingleNodeOverlayMeasurement *) MBdynarray_get(allnodes_overlaymeasurement_array, j);
		if ((src_index == singlenode_overlayMeasurement->src_index) && (dst_index == singlenode_overlayMeasurement->dst_index)){
			found = j;
			break;
		}
	}
	return found;
};


void read_ping_result_file(char* directory_name, char * filename, int *min_start, int *max_start, int *min_end, int *max_end, MBdynarray *singlenodepingarrary, MBdynarray *failureoverlaylinkarray){
	const char * filename_split = ".";
	char line[MAXLINE];
	char filename_copy[MAX_SIZE];
	char directory_name_copy[MAX_SIZE];
	strcpy(filename_copy, filename);
	strcpy(directory_name_copy, directory_name);
	strcat(directory_name_copy, "/");
	strcat(directory_name_copy, filename);
	char * file_node_p = trim(strtok(filename, filename_split));
	int src_node_index = get_node_index_from_char(file_node_p);
	//printf("src_node_index: %d\n", src_node_index);
	
	char * value_split = "\r\n\t ";
	char * node_val_split = "-";
	FILE * fp = NULL;
	//int line_index = 0;
	
	//printf("filename: %s\n", filename);
	//printf("directory_name_copy: %s\n", directory_name_copy);
	//printf("len: %d\n",strlen(directory_name_copy));
	//fp = fopen(directory_name_copy, "r");
	
	if ((fp = fopen(directory_name_copy, "r")) == NULL ){
		printf("can't open!\n");
		fprintf(stderr, "Can't open %s.\n", filename);
		exit(1);
	}
	
	int min_start_index = -1;
	int max_start_index = -1;
	int min_end_index = -1;
	int max_end_index = -1;

	int line_index = 1;
	
	while (fgets(line, MAXLINE, fp) != NULL){
		//printf("line: %s", line);
		//printf("line_index: %d, strlen(line):%d \n", line_index, strlen(line));
		
		char * dst_index_p;
		char * dst_index_val_p;
		char * rtt_val_p;
		char * iperf = "/usr/local/etc/emulab/emulab-iperf";
		char * iperf_position;
		if(strlen(line) > 5){
			iperf_position = strstr(line, iperf);
			//printf("iperf_position: %s\n", iperf_position);
			if (iperf_position == NULL){
				
				int dst_node_index;
				double ping_rtt;
				//char * start_p;
				//printf("line: %s\n", trim(line));
				/*dst_index_p = strtok(trim(line), value_split);
				printf("now dst_index_p: %s\n", dst_index_p);
				while (dst_index_p!=NULL){
					printf("now dst_index_p: %s\n", dst_index_p);
					dst_index_p = strtok(NULL, value_split);
				}
	*/
				dst_index_p = trim(strtok(trim(line), value_split));
				//printf("dst_index_p: %s\n", dst_index_p);
				dst_index_p = trim(strtok(NULL, value_split));
				//printf("dst_index_p: %s\n", dst_index_p);
				dst_index_p = trim(strtok(NULL, value_split));
				//printf("dst_index_p: %s\n", dst_index_p);
				rtt_val_p = trim(strtok(NULL, value_split));
				dst_index_val_p = trim(strtok(dst_index_p, node_val_split));
				dst_node_index = get_node_index_from_char(dst_index_val_p);
				//printf("dst_node_index: %d\n", dst_node_index);
				ping_rtt = atof(rtt_val_p);
				//printf("ping_rtt: %f\n", ping_rtt);
				HeartPing * heart_ping = (HeartPing *) malloc(sizeof(HeartPing));
				heart_ping->dst_index = dst_node_index;
				heart_ping->index = line_index;
				heart_ping->rtt_value = ping_rtt;
				heart_ping->src_index = src_node_index;
				MBdynarray_add_tail(singlenodepingarrary, heart_ping);
			}else{
				//printf("iperf_position string! \n");
				//printf("line: %s\n", line);
				int dst_node_index;
				dst_index_p = trim(strtok(line, value_split));
				char * start_iperf = "start";
				char * start_iperf_position = strstr(dst_index_p, start_iperf);
				if (start_iperf_position != NULL){
					if ( min_start_index == -1 ){
						min_start_index = line_index;
						max_start_index = line_index;
					}else{
						max_start_index = MAX(max_start_index, line_index);
					}
				}else{
					if ( min_end_index == -1 ){
						min_end_index = line_index;
						max_end_index = line_index;
					}else{
						max_end_index = MAX(max_end_index, line_index);
					}
				}
				dst_index_p = trim(strtok(NULL, value_split));
				dst_index_p = trim(strtok(NULL, value_split));
				dst_index_p = trim(strtok(NULL, value_split));
				dst_node_index = get_node_index_from_char(dst_index_p);
				FailureOverlayLink * failure_overlay_link = (FailureOverlayLink *) malloc(sizeof(FailureOverlayLink));
				failure_overlay_link->src_index = src_node_index;
				failure_overlay_link->dst_index = dst_node_index;
				if (check_failureoverlylinkarray_included(failureoverlaylinkarray, src_node_index, dst_node_index) == -1){
					//printf("add failure link array! \n");
					//printf("src_node_index: %d, dst_node_index:%d\n", src_node_index, dst_node_index);
					MBdynarray_add_tail(failureoverlaylinkarray, failure_overlay_link);
				}
				//FailureLink * failure_link;
				FailureOverlayLink * tmp_failure_overlay_link = (FailureOverlayLink *) malloc(sizeof(FailureOverlayLink));
				tmp_failure_overlay_link->src_index = dst_node_index;
				tmp_failure_overlay_link->dst_index = src_node_index;
				if (check_failureoverlylinkarray_included(failureoverlaylinkarray, dst_node_index, src_node_index) == -1){
					printf("add failure link array! \n");
					printf("src_node_index: %d, dst_node_index:%d\n", src_node_index, dst_node_index);
					MBdynarray_add_tail(failureoverlaylinkarray, tmp_failure_overlay_link);
				}
			}
			line_index = line_index + 1;
		}
	}

	*min_start = min_start_index;
	*max_start = max_start_index;
	*min_end = min_end_index;
	*max_end = max_end_index;
	//printf("here99\n");
	fclose(fp);
};

/*
void read_test_ping_result_directory(char * directory_name, int *min_start, int *max_start, int *min_end, int *max_end, MBdynarray *failurelinkarray, MBdynarray *allnodes_pingarrary){
	DIR * FD = NULL;
	struct dirent* in_file;
	if (NULL == (FD = opendir(directory_name))){
		fprintf(stderr, "Error : Failed to open input directory - %s\n", directory_name);
		exit(1);
	}


	while ((in_file = readdir(FD))){
	         On linux/Unix we don't want current and parent directories
	         * On windows machine too, thanks Greg Hewgill

		printf("reading file: %s\n", in_file->d_name);
		if (!strcmp (in_file->d_name, "."))
			continue;
	    if (!strcmp (in_file->d_name, ".."))
	    	continue;
	         Open directory entry file for common operation
	         TODO : change permissions to meet your need!

	}
};
*/


void read_ping_result_directory(char * directory_name, int *min_start, int *max_start, int *min_end, int *max_end, MBdynarray *failureoverlaylinkarray, MBdynarray *allnodes_pingarrary){
	DIR * FD = NULL;
	struct dirent* in_file;
	if (NULL == (FD = opendir(directory_name))){
		fprintf(stderr, "Error : Failed to open input directory - %s\n", directory_name);
		exit(1);
	}

	int final_min_start = -1;
	int final_min_end = -1;
	int final_max_start = -1;
	int final_max_end = -1;
	//failurelinkarray = MBdynarray_create(0);
	//allnodes_pingarrary = MBdynarray_create(0);

	while ((in_file = readdir(FD))){
	        /* On linux/Unix we don't want current and parent directories
	         * On windows machine too, thanks Greg Hewgill
	         */
		printf("reading file: %s\n", in_file->d_name);
		if (!strcmp (in_file->d_name, "."))
			continue;
	    if (!strcmp (in_file->d_name, ".."))
	    	continue;
	        /* Open directory entry file for common operation */
	        /* TODO : change permissions to meet your need! */
	    int tmp_min_start;
	    int tmp_max_start;
	    int tmp_min_end;
	    int tmp_max_end;
	    MBdynarray *singlenodepingarrary_tmp = MBdynarray_create(0);

	    //printf("here1\n");
	    read_ping_result_file(directory_name, in_file->d_name, &tmp_min_start, &tmp_max_start, &tmp_min_end, &tmp_max_end, singlenodepingarrary_tmp, failureoverlaylinkarray);
	    //printf("here\n");
	    if (tmp_min_start != -1){
	    	if (final_min_start == -1){
	    		final_min_start = tmp_min_start;
	    		final_max_start = tmp_max_start;
	    		final_min_end = tmp_min_end;
	    		final_max_end = tmp_max_end;
	    	}else{
	    		final_min_start = MIN(final_min_start, tmp_min_start);
	    		final_max_start = MAX(final_max_start, tmp_max_start);
	    		final_min_end = MIN(final_min_end, tmp_min_end);
	    		final_max_end = MAX(final_max_end, tmp_max_end);
	    	}
	    }
	    //printf("here2\n");
	    MBdynarray_add_tail(allnodes_pingarrary, singlenodepingarrary_tmp);
	    //printf("here3\n");
	}
	*min_start = final_min_start;
	*max_start = final_max_start;
	*min_end = final_min_end;
	*max_end = final_max_end;
	//printf("here4\n");

};



void parsing_ping_result(int *min_start, int *max_start, int *min_end, int *max_end, MBdynarray *failurelinkarray, MBdynarray *allnodes_pingarrary, MBdynarray *allnodes_overlaymeasurement_array){
	printf("parsing ping result -- min_start: %d, max_start: %d, min_end: %d, max_end: %d\n", *min_start, *max_start, *min_end, *max_end);
	int min_abnormal_value = 0;
	int max_abnormal_value = 0;
	if (*max_start < *min_end){
		min_abnormal_value = *max_start;
		max_abnormal_value = *min_end;
	}else{
		min_abnormal_value = *min_end;
		max_abnormal_value = *max_start;
	}
	int i;
	for (i = 0; i < MBdynarray_get_count(allnodes_pingarrary); i++) {
		MBdynarray * singlenodepingarrary = (MBdynarray *) MBdynarray_get(allnodes_pingarrary, i);
		int j;
		for (j = 0; j < MBdynarray_get_count(singlenodepingarrary); j++){
			HeartPing * singlenodeheartping = (HeartPing *) MBdynarray_get(singlenodepingarrary, j);
			if (singlenodeheartping->src_index != singlenodeheartping->dst_index){
				int all_nodes_overlay_measurement_found = check_allnodes_overlaymeasurement_array_included(allnodes_overlaymeasurement_array, singlenodeheartping->src_index, singlenodeheartping->dst_index);
				if ( all_nodes_overlay_measurement_found == -1){
					SingleNodeOverlayMeasurement *singlenode_overlayMeasurement = (SingleNodeOverlayMeasurement *) malloc(sizeof(SingleNodeOverlayMeasurement));
					singlenode_overlayMeasurement->src_index = singlenodeheartping->src_index;
					singlenode_overlayMeasurement->dst_index = singlenodeheartping->dst_index;
					singlenode_overlayMeasurement->regular_avg = 0.0;
					singlenode_overlayMeasurement->regular_dev = 0.0;
					singlenode_overlayMeasurement->regular_count = 0;
					singlenode_overlayMeasurement->regular_median = 0.0;
					singlenode_overlayMeasurement->regular_rtt_array = MBdynarray_create(0);
					singlenode_overlayMeasurement->abnoraml_avg = 0.0;
					singlenode_overlayMeasurement->abnormal_dev = 0.0;
					singlenode_overlayMeasurement->abnormal_count = 0;
					singlenode_overlayMeasurement->abnormal_median = 0.0;
					singlenode_overlayMeasurement->abnormal_rtt_array = MBdynarray_create(0);
					if (singlenodeheartping->index < *min_start){
						//singlenode_overlayMeasurement->regular_avg = singlenode_overlayMeasurement->regular_avg  + singlenodeheartping->rtt_value;
						singlenode_overlayMeasurement->regular_count = singlenode_overlayMeasurement->regular_count + 1;
						MBdynarray_add_tail(singlenode_overlayMeasurement->regular_rtt_array, &singlenodeheartping->rtt_value);

					//}else if ((singlenodeheartping->index > *max_start) && (singlenodeheartping->index < *min_end)){
					}else if ((singlenodeheartping->index > min_abnormal_value) && (singlenodeheartping->index < max_abnormal_value)){
						//singlenode_overlayMeasurement->abnoraml_avg = singlenode_overlayMeasurement->abnoraml_avg  + singlenodeheartping->rtt_value;
						singlenode_overlayMeasurement->abnormal_count = singlenode_overlayMeasurement->abnormal_count + 1;
						MBdynarray_add_tail(singlenode_overlayMeasurement->abnormal_rtt_array, &singlenodeheartping->rtt_value);
					}
					MBdynarray_add_tail(allnodes_overlaymeasurement_array, singlenode_overlayMeasurement);
				}else{
					SingleNodeOverlayMeasurement * singlenode_overlayMeasurement = (SingleNodeOverlayMeasurement *) MBdynarray_get(allnodes_overlaymeasurement_array, all_nodes_overlay_measurement_found);
					if (singlenodeheartping->index < *min_start){
						//singlenode_overlayMeasurement->regular_avg = singlenode_overlayMeasurement->regular_avg  + singlenodeheartping->rtt_value;
						singlenode_overlayMeasurement->regular_count = singlenode_overlayMeasurement->regular_count + 1;
						MBdynarray_add_tail(singlenode_overlayMeasurement->regular_rtt_array, &singlenodeheartping->rtt_value);
					//}else if ((singlenodeheartping->index > *max_start) && (singlenodeheartping->index < *min_end)){
					}else if ((singlenodeheartping->index > min_abnormal_value) && (singlenodeheartping->index < max_abnormal_value)){
						//singlenode_overlayMeasurement->abnoraml_avg = singlenode_overlayMeasurement->abnoraml_avg  + singlenodeheartping->rtt_value;
						singlenode_overlayMeasurement->abnormal_count = singlenode_overlayMeasurement->abnormal_count + 1;
						MBdynarray_add_tail(singlenode_overlayMeasurement->abnormal_rtt_array, &singlenodeheartping->rtt_value);
					}
				}
			}
		}
	}
	MBdynarray *removed_overlaymeasurement_array = MBdynarray_create(0);
	for (i = 0; i < MBdynarray_get_count(allnodes_overlaymeasurement_array); i++) {
		SingleNodeOverlayMeasurement *singlenode_overlayMeasurement = (SingleNodeOverlayMeasurement *) MBdynarray_get(allnodes_overlaymeasurement_array, i);
		//print_singlenode_overlayMeasurement(singlenode_overlayMeasurement);
		bubbleSort(singlenode_overlayMeasurement->regular_rtt_array);
		if (MBdynarray_get_count(singlenode_overlayMeasurement->abnormal_rtt_array) == 0){
			MBdynarray_add_tail(removed_overlaymeasurement_array, i);
		}else{
			bubbleSort(singlenode_overlayMeasurement->abnormal_rtt_array);
			int num_regular_5_percent = MBdynarray_get_count(singlenode_overlayMeasurement->regular_rtt_array)/20;
			int num_abnormal_5_percent = MBdynarray_get_count(singlenode_overlayMeasurement->abnormal_rtt_array)/20;
			int j;
			for (j = 0; j < MBdynarray_get_count(singlenode_overlayMeasurement->regular_rtt_array); j++){
				if ((j >= num_regular_5_percent) && (j <= MBdynarray_get_count(singlenode_overlayMeasurement->regular_rtt_array) - 7*num_regular_5_percent - 1)){
					double *regular_rtt_val = (double *) MBdynarray_get(singlenode_overlayMeasurement->regular_rtt_array, j);
					singlenode_overlayMeasurement->regular_avg = singlenode_overlayMeasurement->regular_avg + *regular_rtt_val;
				}
			}
			singlenode_overlayMeasurement->regular_median = *(double *) MBdynarray_get(singlenode_overlayMeasurement->regular_rtt_array, MBdynarray_get_count(singlenode_overlayMeasurement->regular_rtt_array)/2);
			for (j = 0; j < MBdynarray_get_count(singlenode_overlayMeasurement->abnormal_rtt_array); j++){
				if ((j >= num_abnormal_5_percent) && (j <= MBdynarray_get_count(singlenode_overlayMeasurement->abnormal_rtt_array) - 7*num_abnormal_5_percent - 1)){
					double *abnormal_rtt_val = (double *) MBdynarray_get(singlenode_overlayMeasurement->abnormal_rtt_array, j);
					singlenode_overlayMeasurement->abnoraml_avg = singlenode_overlayMeasurement->abnoraml_avg + *abnormal_rtt_val;
				}
			}
			singlenode_overlayMeasurement->abnormal_median = *(double *) MBdynarray_get(singlenode_overlayMeasurement->abnormal_rtt_array, MBdynarray_get_count(singlenode_overlayMeasurement->abnormal_rtt_array)/2);
			singlenode_overlayMeasurement->regular_avg = singlenode_overlayMeasurement->regular_avg/(double)(MBdynarray_get_count(singlenode_overlayMeasurement->regular_rtt_array) - 8*num_regular_5_percent);
			singlenode_overlayMeasurement->abnoraml_avg = singlenode_overlayMeasurement->abnoraml_avg/(double)(MBdynarray_get_count(singlenode_overlayMeasurement->abnormal_rtt_array) - 8*num_abnormal_5_percent);
			//singlenode_overlayMeasurement->regular_avg = singlenode_overlayMeasurement->regular_avg/(double)singlenode_overlayMeasurement->regular_count;
			//singlenode_overlayMeasurement->abnoraml_avg = singlenode_overlayMeasurement->abnoraml_avg/(double)singlenode_overlayMeasurement->abnormal_count;
			//printf("here3\n");
		}
	}
	for(i = MBdynarray_get_count(removed_overlaymeasurement_array)-1; i >=0; i--){
		MBdynarray_remove(allnodes_overlaymeasurement_array, i);
	}
	for (i = 0; i < MBdynarray_get_count(allnodes_overlaymeasurement_array); i++) {
		SingleNodeOverlayMeasurement *singlenode_overlayMeasurement = (SingleNodeOverlayMeasurement *) MBdynarray_get(allnodes_overlaymeasurement_array, i);
		//bubbleSort(singlenode_overlayMeasurement->regular_rtt_array);
		//bubbleSort(singlenode_overlayMeasurement->abnormal_rtt_array);
		int num_regular_5_percent = MBdynarray_get_count(singlenode_overlayMeasurement->regular_rtt_array)/20;
		int num_abnormal_5_percent = MBdynarray_get_count(singlenode_overlayMeasurement->abnormal_rtt_array)/20;
		int j;
		for (j = 0; j < MBdynarray_get_count(singlenode_overlayMeasurement->regular_rtt_array); j++){
			if ((j >= num_regular_5_percent) && (j <= MBdynarray_get_count(singlenode_overlayMeasurement->regular_rtt_array) - 7*num_regular_5_percent - 1)){
				double *regular_rtt_val = (double *) MBdynarray_get(singlenode_overlayMeasurement->regular_rtt_array, j);
				singlenode_overlayMeasurement->regular_dev = singlenode_overlayMeasurement->regular_dev + pow((*regular_rtt_val - singlenode_overlayMeasurement->regular_avg),2);
			}
		}
		for (j = 0; j < MBdynarray_get_count(singlenode_overlayMeasurement->abnormal_rtt_array); j++){
			if ((j >= num_abnormal_5_percent) && (j <= MBdynarray_get_count(singlenode_overlayMeasurement->abnormal_rtt_array) - 7*num_abnormal_5_percent - 1)){
				double *abnormal_rtt_val = (double *) MBdynarray_get(singlenode_overlayMeasurement->abnormal_rtt_array, j);
				singlenode_overlayMeasurement->abnormal_dev = singlenode_overlayMeasurement->abnormal_dev + pow((*abnormal_rtt_val - singlenode_overlayMeasurement->abnoraml_avg),2);
			}
		}
		singlenode_overlayMeasurement->regular_dev = singlenode_overlayMeasurement->regular_dev/(double)(MBdynarray_get_count(singlenode_overlayMeasurement->regular_rtt_array) - 8*num_regular_5_percent);
		singlenode_overlayMeasurement->abnormal_dev = singlenode_overlayMeasurement->abnormal_dev/(double)(MBdynarray_get_count(singlenode_overlayMeasurement->abnormal_rtt_array) - 8*num_abnormal_5_percent);
	}

/*

	for (i = 0; i < MBdynarray_get_count(allnodes_pingarrary); i++) {
		MBdynarray * singlenodepingarrary = (MBdynarray *) MBdynarray_get(allnodes_pingarrary, i);
		int j;
		for (j = 0; j < MBdynarray_get_count(singlenodepingarrary); j++){
			HeartPing * singlenodeheartping = (HeartPing *) MBdynarray_get(singlenodepingarrary, j);
			if (singlenodeheartping->src_index != singlenodeheartping->dst_index){
				int all_nodes_overlay_measurement_found = check_allnodes_overlaymeasurement_array_included(allnodes_overlaymeasurement_array, singlenodeheartping->src_index, singlenodeheartping->dst_index);
				SingleNodeOverlayMeasurement * singlenode_overlayMeasurement = (SingleNodeOverlayMeasurement *) MBdynarray_get(allnodes_overlaymeasurement_array, all_nodes_overlay_measurement_found);
				if (singlenodeheartping->index < *min_start){
					singlenode_overlayMeasurement->regular_dev = singlenode_overlayMeasurement->regular_dev + pow((singlenodeheartping->rtt_value - singlenode_overlayMeasurement->regular_avg),2);
				}else if ((singlenodeheartping->index > *max_start) && (singlenodeheartping->index < *min_end)){
					singlenode_overlayMeasurement->abnormal_dev = singlenode_overlayMeasurement->abnormal_dev + pow((singlenodeheartping->rtt_value - singlenode_overlayMeasurement->abnoraml_avg),2);
				}
			}
		}
	}
	for (i = 0; i < MBdynarray_get_count(allnodes_overlaymeasurement_array); i++) {
		SingleNodeOverlayMeasurement *singlenode_overlayMeasurement = (SingleNodeOverlayMeasurement *) MBdynarray_get(allnodes_overlaymeasurement_array, i);
		singlenode_overlayMeasurement->regular_dev = singlenode_overlayMeasurement->regular_dev/(double)singlenode_overlayMeasurement->regular_count;
		singlenode_overlayMeasurement->abnormal_dev = singlenode_overlayMeasurement->abnormal_dev/(double)singlenode_overlayMeasurement->abnormal_count;
	}*/
};

void print_delay_array(MBdynarray *arr){
	int j;
	for (j = 0; j < MBdynarray_get_count(arr); j++) {
		double* underlaylinkitem = (double*) MBdynarray_get(arr, j);
		printf("%e ", *underlaylinkitem);
	    	//print_underlay_link(underlaylinkitem);
	}
	printf("\n");
	
};


void print_delay_array_file(FILE * allnodes_print_file_fp, MBdynarray *arr){
	int j;
	//printf("Start to print delay array ...\n");
	for (j = 0; j < MBdynarray_get_count(arr); j++) {
		double* underlaylinkitem = (double*) MBdynarray_get(arr, j);
		fprintf(allnodes_print_file_fp, "%e ", *underlaylinkitem);
	    	//print_underlay_link(underlaylinkitem);
	}
	fprintf(allnodes_print_file_fp, "\n");
	//printf("End to print delay array ...\n");
};


void print_singlenode_overlayMeasurement_file(FILE * allnodes_print_file_fp, SingleNodeOverlayMeasurement *singlenode_overlayMeasurement){
	fprintf(allnodes_print_file_fp,"Single node overlay measurement: src_index->%d, dst_index->%d, regular_avg->%lf, regular_dev->%lf, regular_medium->%lf, abnoraml_avg->%lf, abnormal_dev->%lf, abnormal_medium->%lf\n", singlenode_overlayMeasurement->src_index, singlenode_overlayMeasurement->dst_index, singlenode_overlayMeasurement->regular_avg, singlenode_overlayMeasurement->regular_dev, singlenode_overlayMeasurement->regular_median , singlenode_overlayMeasurement->abnoraml_avg, singlenode_overlayMeasurement->abnormal_dev, singlenode_overlayMeasurement->abnormal_median);
	if (singlenode_overlayMeasurement->abnormal_median > singlenode_overlayMeasurement->regular_median + 4){
		fprintf(allnodes_print_file_fp,"abnormal happens!\n");
	}
	fprintf(allnodes_print_file_fp,"Print regular delay array: ");
	print_delay_array_file(allnodes_print_file_fp, singlenode_overlayMeasurement->regular_rtt_array);
	fprintf(allnodes_print_file_fp,"Print abnormal delay array: ");
	print_delay_array_file(allnodes_print_file_fp, singlenode_overlayMeasurement->abnormal_rtt_array);
};

void print_allnodes_overlayMeasurement_file(FILE * allnodes_print_file_fp, MBdynarray *allnodes_overlaymeasurement_array){
	fprintf(allnodes_print_file_fp,"print all nodes overlay measurement ...\n");
	fprintf(allnodes_print_file_fp,"all nodes overlay measure len: %d\n", MBdynarray_get_count(allnodes_overlaymeasurement_array));
	if (MBdynarray_get_count(allnodes_overlaymeasurement_array) >0){
		fprintf(allnodes_print_file_fp,"Start to print all nodes overlay measurement ...\n");
		int j;
		for (j = 0; j < MBdynarray_get_count(allnodes_overlaymeasurement_array); j++) {
			SingleNodeOverlayMeasurement* singlenode_overlayMeasurement = (SingleNodeOverlayMeasurement*) MBdynarray_get(allnodes_overlaymeasurement_array, j);
			print_singlenode_overlayMeasurement_file(allnodes_print_file_fp, singlenode_overlayMeasurement);
		}
		fprintf(allnodes_print_file_fp,"End to print all nodes overlay measurement ...\n");
	}
	//printf("Underlay: index->%d, src_index->%d, dst_index->%d, failure_prob->%e\r\n", underlay_link_item->index, underlay_link_item->src_index, underlay_link_item->dst_index, underlay_link_item->failure_prob);
};




void print_singlenode_overlayMeasurement(SingleNodeOverlayMeasurement *singlenode_overlayMeasurement){
	printf("Single node overlay measurement: src_index->%d, dst_index->%d, regular_avg->%lf, regular_dev->%lf, regular_medium->%lf, abnoraml_avg->%lf, abnormal_dev->%lf, abnormal_medium->%lf\n", singlenode_overlayMeasurement->src_index, singlenode_overlayMeasurement->dst_index, singlenode_overlayMeasurement->regular_avg, singlenode_overlayMeasurement->regular_dev, singlenode_overlayMeasurement->regular_median , singlenode_overlayMeasurement->abnoraml_avg, singlenode_overlayMeasurement->abnormal_dev, singlenode_overlayMeasurement->abnormal_median);
	printf("Print regular delay array: ");
	print_delay_array(singlenode_overlayMeasurement->regular_rtt_array);
	printf("Print abnormal delay array: ");
	print_delay_array(singlenode_overlayMeasurement->abnormal_rtt_array);
};

void print_allnodes_overlayMeasurement(MBdynarray *allnodes_overlaymeasurement_array){
	printf("print all nodes overlay measurement ...\n");
	printf("all nodes overlay measure len: %d\n", MBdynarray_get_count(allnodes_overlaymeasurement_array));
	if (MBdynarray_get_count(allnodes_overlaymeasurement_array) >0){
		printf("Start to print all nodes overlay measurement ...\n");
		int j;
		for (j = 0; j < MBdynarray_get_count(allnodes_overlaymeasurement_array); j++) {
			SingleNodeOverlayMeasurement* singlenode_overlayMeasurement = (SingleNodeOverlayMeasurement*) MBdynarray_get(allnodes_overlaymeasurement_array, j);
			print_singlenode_overlayMeasurement(singlenode_overlayMeasurement);
		}
		printf("End to print all nodes overlay measurement ...\n");
	}
	//printf("Underlay: index->%d, src_index->%d, dst_index->%d, failure_prob->%e\r\n", underlay_link_item->index, underlay_link_item->src_index, underlay_link_item->dst_index, underlay_link_item->failure_prob);
};


void print_selected_overlayMeasurement(MBdynarray *allnodes_overlaymeasurement_array, MBdynarray *overlaylinkarray){
	printf("print selected overlay measurement ...\n");
	//printf("all nodes overlay measure len: %d\n", MBdynarray_get_count(allnodes_overlaymeasurement_array));
	//printf("Start to print all nodes overlay measurement ...\n");
	int j;
	for (j = 0; j < MBdynarray_get_count(allnodes_overlaymeasurement_array); j++) {
		SingleNodeOverlayMeasurement* singlenode_overlayMeasurement = (SingleNodeOverlayMeasurement*) MBdynarray_get(allnodes_overlaymeasurement_array, j);
		int found = check_overlayarray_included(overlaylinkarray, singlenode_overlayMeasurement->src_index, singlenode_overlayMeasurement->dst_index);
		if (found != -1){
			print_singlenode_overlayMeasurement(singlenode_overlayMeasurement);
		}
	}
	printf("End to print selected overlay measurement ...\n");

	//printf("Underlay: index->%d, src_index->%d, dst_index->%d, failure_prob->%e\r\n", underlay_link_item->index, underlay_link_item->src_index, underlay_link_item->dst_index, underlay_link_item->failure_prob);
};

MBdynarray * get_failure_underlay_link_array(MBdynarray *failure_overlay_link_array, MBdynarray *overlaylinkarray){
	MBdynarray *failure_underlay_link_array = MBdynarray_create(0);
	int i;
	for (i = 0; i < MBdynarray_get_count(failure_overlay_link_array); i++) {
		FailureOverlayLink* failure_overlay_link_item = (FailureOverlayLink*) MBdynarray_get(failure_overlay_link_array, i);
		int j;
		for (j = 0; j < MBdynarray_get_count(overlaylinkarray); j++) {
			Overlaylink * temp_overlaylink_item = (Overlaylink*) MBdynarray_get(overlaylinkarray, j);
			if ((failure_overlay_link_item->src_index == temp_overlaylink_item->src_index) && (failure_overlay_link_item->dst_index == temp_overlaylink_item->dst_index)){
				MBdynarray *underlaylinkarray = temp_overlaylink_item->underlaylinkarray;
				int k;
				for (k = 0; k < MBdynarray_get_count(underlaylinkarray); k++){
					Underlaylink *underlay_link_item = (Underlaylink *) MBdynarray_get(underlaylinkarray, k);
					int underlay_found = check_failureunderlylinkarray_included(failure_underlay_link_array, underlay_link_item->src_index, underlay_link_item->dst_index);
					if (underlay_found == -1){
						FailureUnderlayLink * failure_underlay_link_item = (FailureUnderlayLink *) malloc(sizeof(FailureUnderlayLink));
						failure_underlay_link_item->src_index = underlay_link_item->src_index;
						failure_underlay_link_item->dst_index = underlay_link_item->dst_index;
						MBdynarray_add_tail(failure_underlay_link_array, failure_underlay_link_item);
					}
				}

			}
		}
	}
	return failure_underlay_link_array;
};





void print_failure_overlay_link(FailureOverlayLink *failure_overlay_link_item){
	printf("failure overlay link: src_index->%d, dst_index->%d\n", failure_overlay_link_item->src_index, failure_overlay_link_item->dst_index);
};

void print_failure_overlay_link_array(MBdynarray *failure_overlay_link_array){
	printf("print failure overlay link array ...\n");
	printf("len: %d\n", MBdynarray_get_count(failure_overlay_link_array));
	if (MBdynarray_get_count(failure_overlay_link_array) >0){
		printf("Start to print failure overlay link array ...\n");
		int j;
		for (j = 0; j < MBdynarray_get_count(failure_overlay_link_array); j++) {
			FailureOverlayLink* failure_overlay_link_item = (FailureOverlayLink*) MBdynarray_get(failure_overlay_link_array, j);
			print_failure_overlay_link(failure_overlay_link_item);
		}
		printf("End to print failure overlay link array ...\n");
	}
	//printf("Underlay: index->%d, src_index->%d, dst_index->%d, failure_prob->%e\r\n", underlay_link_item->index, underlay_link_item->src_index, underlay_link_item->dst_index, underlay_link_item->failure_prob);
};

void print_failure_underlay_link(FailureUnderlayLink *failure_underlay_link_item){
	printf("failure underlay link: src_index->%d, dst_index->%d\n", failure_underlay_link_item->src_index, failure_underlay_link_item->dst_index);
};

void print_failure_underlay_link_array(MBdynarray *failure_underlay_link_array){
	printf("print failure underlay link array ...\n");
	printf("len: %d\n", MBdynarray_get_count(failure_underlay_link_array));
	if (MBdynarray_get_count(failure_underlay_link_array) >0){
		printf("Start to print failure underlay link array ...\n");
		int j;
		for (j = 0; j < MBdynarray_get_count(failure_underlay_link_array); j++) {
			FailureUnderlayLink* failure_underlay_link_item = (FailureUnderlayLink*) MBdynarray_get(failure_underlay_link_array, j);
			print_failure_underlay_link(failure_underlay_link_item);
		}
		printf("End to print failure underlay link array ...\n");
	}
	//printf("Underlay: index->%d, src_index->%d, dst_index->%d, failure_prob->%e\r\n", underlay_link_item->index, underlay_link_item->src_index, underlay_link_item->dst_index, underlay_link_item->failure_prob);
};



void print_allnodes_pingarray(MBdynarray * allnodes_pingarrary){
	printf("Start to print all nodes ping array ...\n");
	//printf("len: %d\n", MBdynarray_get_count(failure_link_array));
	if (MBdynarray_get_count(allnodes_pingarrary) >0){
		//printf("Start to print failure link array ...\r\n");
		int j;
		for (j = 0; j < MBdynarray_get_count(allnodes_pingarrary); j++) {
			MBdynarray* onenode_pingarrary = (MBdynarray*) MBdynarray_get(allnodes_pingarrary, j);
			HeartPing * one_node_first_heart_ping = (HeartPing *) MBdynarray_get(onenode_pingarrary, 0);
			printf("Src index: %d, len: %d \n", one_node_first_heart_ping->src_index, MBdynarray_get_count(onenode_pingarrary));

			//HeartPing * heart_ping = (HeartPing *) malloc(sizeof(HeartPing));
				//print_failure_link(failure_link_item);
			}
			//printf("End to print failure link array ...\r\n");
	}
	printf("End to print all nodes ping array ...\n");
};



void parsing_overlaymeasurement_result(MBdynarray *allnodes_overlaymeasurement_array, MBdynarray *overlaylinkarray){
	int i;
	for (i = 0; i < MBdynarray_get_count(overlaylinkarray); i++) {
		Overlaylink * overlaylink_item = (Overlaylink *) MBdynarray_get(overlaylinkarray, i);
		int overlaylink_item_src_index = overlaylink_item->src_index;
		int overlaylink_item_dst_index = overlaylink_item->dst_index;
		int j;
		for (j = 0; j < MBdynarray_get_count(allnodes_overlaymeasurement_array); j++){
			SingleNodeOverlayMeasurement *singlenode_overlayMeasurement = (SingleNodeOverlayMeasurement *) MBdynarray_get(allnodes_overlaymeasurement_array, j);
			if ((singlenode_overlayMeasurement->src_index == overlaylink_item_src_index) && (singlenode_overlayMeasurement->dst_index == overlaylink_item_dst_index)){
				double normal_upper_bound = singlenode_overlayMeasurement->regular_avg + 3*sqrt(singlenode_overlayMeasurement->regular_dev);
				/*if (singlenode_overlayMeasurement->abnoraml_avg > (normal_upper_bound+1.0)){
					//printf("overlaylink_item_src_index:->%d, overlaylink_item_dst_index:->%d, normal_upper_bound:->%e, abnormal_avg->%e\n", overlaylink_item_src_index, overlaylink_item_dst_index, normal_upper_bound, singlenode_overlayMeasurement->abnoraml_avg );
					overlaylink_item->measured_success = 0;
				}else{
					overlaylink_item->measured_success = 1;
				}*/
				if (singlenode_overlayMeasurement->abnormal_median > (singlenode_overlayMeasurement->regular_median) + 4){
									//printf("overlaylink_item_src_index:->%d, overlaylink_item_dst_index:->%d, normal_upper_bound:->%e, abnormal_avg->%e\n", overlaylink_item_src_index, overlaylink_item_dst_index, normal_upper_bound, singlenode_overlayMeasurement->abnoraml_avg );
					overlaylink_item->measured_success = 0;
				}else{
					overlaylink_item->measured_success = 1;
				}


			}
		}
	}
	/*				HeartPing * singlenodeheartping = (HeartPing *) MBdynarray_get(singlenodepingarrary, i);
					if (j == 0){
						singlenode_overlayMeasurement->src_index = singlenodeheartping->src_index;
						singlenode_overlayMeasurement->dst_index = singlenodeheartping->dst_index;
					}
					if (singlenodeheartping->index < *min_start){
						regular_sum = regular_sum + singlenodeheartping->rtt_value;
						regular_count = regular_count + 1;
					}
					if ((singlenodeheartping->index > *max_start) && (singlenodeheartping->index < *min_end)){
						abnoraml_sum = abnoraml_sum + singlenodeheartping->rtt_value;
						abnormal_count = abnormal_count + 1;
					}
				}

	}*/
};


void bubbleSort(MBdynarray * arr)
{
    int i,j;
    double * temp = (double *) malloc(sizeof(double));
    for( i = 0; i < MBdynarray_get_count(arr); i++ )
    {
        for( j = 0; j < MBdynarray_get_count(arr)-i-1; j++)
        {
        	double * j_temp = (double*) MBdynarray_get(arr, j);
        	double * j_1_temp = (double*) MBdynarray_get(arr, j+1);
        	if (* j_temp > * j_1_temp){
        		temp = (double*) MBdynarray_get(arr, j);
        		MBdynarray_set(arr, j, MBdynarray_get(arr, j+1));
        		MBdynarray_set(arr, j+1, temp);
        	}
        }
    }
};
