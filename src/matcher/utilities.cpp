/// @file       matcher.cpp 
/// @brief      This file contains the utility functions for the matcher application.

/// @author     Shivan Khullar
/// @date       June 2024

#include "../../include/matcher/structs_and_classes.h"
#include <string>
#include "../../include/matcher/matcher_functions.h"


/// @brief      Check if a double exists in a vector
/// @param      a       The vector to check
/// @param      b       The double to check for
int check_if_value_exists_in_array(std::vector<double> a, double b)
{
        int c = 0;
        for (int i=0; i<a.size(); i++)
        {
                if (a[i]==b) { c = c+1;}
        }
    return c;
}


/// @brief      Get the first index of a double in a vector
/// @param      a       The vector to check
/// @param      b       The double to check for
int get_first_index(std::vector<double> a, double b)
{
        for (int i=0; i<a.size(); i++)
        {
                if (a[i]==b) { return i;}
        }
    return -1;
}


/// @brief Function to print a double array
/// @param a : the array to be printed
void print_array_double(std::vector<double> a)
{
        for (int i = 0; i<a.size(); i++)
        {
                std::cout << a[i] << std::endl;
        }
}

/// @brief Function to print an int array
/// @param a : the array to be printed
void print_array_int(std::vector<int> a)
{
        for (int i = 0; i<a.size(); i++)
        {
                std::cout << a[i] << std::endl;
        }
}


/// @brief    Function to sum the elements of a double array
/// @param a : the array to be summed
/// @return  the sum of the elements of the array
double sum_array(std::vector<double> a)
{
        double sum = 0;
        for (int i=0; i<a.size(); i++)
        {
                sum = sum + a[i];
        }
	return sum;
}




/// @brief Function to get the name of the snapshot part of a Cloud
/// @param i : the snapshot number
/// @return the name of a snapshot as a string, with leading zeros to make it a 3-digit number
std::string get_snapshot_name(int i)
{
	std::string name;
	if (i<10) { name = "00"+std::to_string(i);}
	else if (i>=10 & i<100) {name = "0"+std::to_string(i);}
	else { name = std::to_string(i);}
	return name;
}


int find_num_clouds(int snap_num, Params &params)
{
	std::string fname = params.path+params.dat_filename_base_prefix+get_snapshot_name(snap_num)+params.dat_filename_base_suffix;
        std::ifstream file(fname);
        if (!file.is_open()) {
        std::cout << "Couldn't open file " << fname << std::endl;
        std::exit(-2);
        }

        std::string line;
        int c = 0;
        std::string hash = "#";
        while (std::getline(file, line))
        {
                if (line[0]=='#')
                {
                        continue;
                }
                c++;
        }
        file.close();
        return c;
}
