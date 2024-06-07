#ifndef IO_H
#define IO_H


#include <hdf5.h>
#include <vector>
#include <iostream>
#include <string>

#include "structs_and_classes.h"
#include "linker_functions.h"
#include "utilities.h"
#include "read_params.h"


hid_t get_last_group(Group_struct *subgroup_struct, hid_t group);
std::vector<double> read_cloud_data_double(Params &params, int snap_num, 
                    std::string field_to_read, std::string cloud_name);
double read_mass(Params &params, int snap_num, std::string cloud_name);
herr_t child_info(hid_t loc_id, const char *name, const H5L_info_t *linfo, void *opdata);
void get_child_list(Params &params, int snap_num, std::string cloud_name, 
                    std::vector<std::string> &child_list_names, 
                    std::vector<double> &child_list_fracs);
void print_list(std::vector<std::vector<CloudStruct>> &tracked_cloud_list);
void write_to_file(std::vector<std::vector<CloudStruct>> &tracked_cloud_list, Params &params, std::string field_name);



#endif // IO_H