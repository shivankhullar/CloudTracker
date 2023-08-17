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
#include "linker_structs.h"


std::string get_snapshot_name(int i);
std::string get_cloud_name(int i, Params &params);
int find_num_clouds(int snap_num, Params &params);
hid_t get_last_group(Group_struct *subgroup_struct, hid_t group);
std::vector<double> read_cloud_data_double(Params &params, int snap_num, std::string field_to_read, std::string cloud_name);
int check_if_exists(std::vector<double> a, double b);
int get_num_particles(Params &params, int snap_num, std::string cloud_name);
double read_mass(Params &params, int snap_num, std::string cloud_name);
herr_t child_info(hid_t loc_id, const char *name, const H5L_info_t *linfo, void *opdata);
void get_child_list(Params &params, int snap_num, std::string cloud_name, std::vector<std::string> &child_list_names, std::vector<double> &child_list_fracs);
void load_to_cloudlist(std::vector<CloudStruct> &cloud_list, CloudStruct &cloud);
void load_to_tracked_cloud_list(std::vector<std::vector<CloudStruct>> &tracked_cloud_list, CloudStruct &cloud, std::string key, int index);
int find_proper_child(std::vector<std::string> &child_list_names, std::vector<double> &child_list_fracs, std::vector<std::string> &child_list_names_original, std::vector<CloudStruct> &cloud_list, int index_to_omit, int snap_num, Params &params);
int check_if_cloud_exists_in_list(CloudStruct &cloud, std::vector<CloudStruct> &cloud_list);
void print_list(std::vector<std::vector<CloudStruct>> &tracked_cloud_list);
void write_to_file(std::vector<std::vector<CloudStruct>> &tracked_cloud_list, Params &params, std::string field_name);
void linker(Params &params);

#endif
