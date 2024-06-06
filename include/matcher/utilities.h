

#ifndef UTILITIES_H
#define UTILITIES_H

#include "structs_and_classes.h"
#include <vector>

int check_if_value_exists_in_array(std::vector<double> a, double b);
int get_first_index(std::vector<double> a, double b);
void print_array_double(std::vector<double> a);
void print_array_int(std::vector<int> a);
double sum_array(std::vector<double> a);
std::string get_snapshot_name(int i);
int find_num_clouds(int snap_num, Params &params);


#endif