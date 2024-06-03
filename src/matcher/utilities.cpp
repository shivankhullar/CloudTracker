/// @file       matcher.cpp 
/// @brief      This file contains the utility functions for the matcher application.

/// @author     Shivan Khullar
/// @date       June 2024

//#include "structs_and_classes.h"
#include <string>
#include "matcher_functions.h"


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