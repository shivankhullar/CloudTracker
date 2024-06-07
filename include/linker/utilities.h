#ifndef UTILITIES_H
#define UTILITIES_H

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


std::string get_snapshot_name(int i);
std::string get_cloud_name(int i, Params &params);
int find_num_clouds(int snap_num, Params &params);
int check_if_exists(std::vector<double> a, double b);
int get_num_particles(Params &params, int snap_num, std::string cloud_name);
void print_list(std::vector<std::vector<CloudStruct>> &tracked_cloud_list);
void write_to_file(std::vector<std::vector<CloudStruct>> &tracked_cloud_list, Params &params, std::string field_name);
void linker(Params &params);

#endif // UTILITIES_H
