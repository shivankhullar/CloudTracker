#ifndef IO_H
#define IO_H

#include "structs_and_classes.h"
#include "matcher_functions.h"
#include "utilities.h"
#include <vector>
#include <fstream>
#include <hdf5.h>

void write_to_hdf5_file(CitySnaps& snapsnap, Params& params, int parent_num_clouds, int child_num_clouds);
//void write_to_hdf5_file(CitySnaps snapsnap, Params &params, int parent_num_clouds, int child_num_clouds);
void writeCloudData(hid_t group_id, const MemberCloud& member, int child_flag);
void writeGroupDataChildren(hid_t group_id, const std::string& subgroup_name, const MemberCloud& member);
void writeGroupDataParents(hid_t group_id, const std::string& subgroup_name, const MemberCloud& member);
hid_t get_last_group(Group_struct* subgroup_struct, hid_t group_id);
std::vector<int> read_cloud_data_int(Params& params, int snap_num, const std::string& field_to_read, const std::string& cloud_name);
std::vector<double> read_cloud_data_double(Params& params, int snap_num, std::string& field_to_read, std::string& cloud_name);

#endif