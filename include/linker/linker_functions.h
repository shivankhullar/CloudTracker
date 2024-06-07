#ifndef LINKER_FUNCTIONS_H
#define LINKER_FUNCTIONS_H

#include <hdf5.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include "structs_and_classes.h"
#include "read_params.h"


void load_to_cloudlist(std::vector<CloudStruct> &cloud_list, CloudStruct &cloud);
void load_to_tracked_cloud_list(std::vector<std::vector<CloudStruct>> &tracked_cloud_list, CloudStruct &cloud, std::string key, int index);
int find_proper_child(std::vector<std::string> &child_list_names, std::vector<double> &child_list_fracs, std::vector<std::string> &child_list_names_original, std::vector<CloudStruct> &cloud_list, int index_to_omit, int snap_num, Params &params);
int check_if_cloud_exists_in_list(CloudStruct &cloud, std::vector<CloudStruct> &cloud_list);
void linker(Params &params);

#endif
