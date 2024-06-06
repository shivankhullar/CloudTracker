#ifndef MATCHER_FUNCTIONS_H
#define MATCHER_FUNCTIONS_H

#include "structs_and_classes.h"
#include "read_params.h"
#include "io.h"
#include "utilities.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
//#include <H5Cpp.h>
//using namespace H5;

//void write_to_hdf5_file(CitySnaps snapsnap, Params &params, int parent_num_clouds, int child_num_clouds);
//H5::Group get_last_group(Group_struct *subgroup_struct, H5::Group group);
//std::string get_snapshot_name(int i);
//std::vector<int> read_cloud_data_int(Params &params, int snap_num, std::string field_to_read, std::string cloud_name);
//std::vector<double> read_cloud_data_double(Params &params, int snap_num, std::string field_to_read, std::string cloud_name);
//double sum_array(std::vector<double> a);
int compare_particles(std::vector<int> a, std::vector<int> b);
double mass_frac(std::vector<int> parent_pIDs, std::vector<int> child_pIDs, std::vector<double> parent_masses, std::vector<double> child_masses,
         		int num_common_particles);
std::string get_cloud_name(int i, Params &params);
void load_clouds_to_group(int snap_num, int num_clouds_parent_snap, Params &params, CitySnaps &snapsnap, std::string key);
//int find_num_clouds(int snap_num, Params &params);
void matcher(Params &params);

#endif
