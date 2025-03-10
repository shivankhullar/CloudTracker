/// @file       read_params.cpp
/// @brief      Contains functions to read parameters from a file
///             The parameters are stored in a text file and read into a struct
///             Linker then reads these parameters to perform the linking process
///             
/// @author     Shivan Khullar
/// @date       June 2024


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "../../../include/linker/read_params.h"



/// @brief      Function to remove leading and trailing whitespaces from a string
/// @param s    The string to trim
void trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}



/// @brief          Function to print the parameters
/// @param params   The parameters struct
void printParams(const Params& params) {
    std::cout << "Path: " << params.path << std::endl;
    std::cout << "First Snap: " << params.first_snap << std::endl;
    std::cout << "Last Snap: " << params.last_snap << std::endl;
    std::cout << "Cloud Prefix: " << params.cloud_prefix << std::endl;
    std::cout << "Dat Filename Base Prefix: " << params.dat_filename_base_prefix << std::endl;
    std::cout << "Dat Filename Base Suffix: " << params.dat_filename_base_suffix << std::endl;
    std::cout << "Filename Base Prefix: " << params.filename_base_prefix << std::endl;
    std::cout << "Filename Base Suffix: " << params.filename_base_suffix << std::endl;
    std::cout << "File Arch Root: " << params.file_arch_root << std::endl;
    std::cout << "File Arch Cloud Subgroup: " << params.file_arch_cloud_subgroup << std::endl;
    std::cout << "File Arch Masses Field: " << params.file_arch_masses_field << std::endl;
    std::cout << "File Arch PIDs Field: " << params.file_arch_pIDs_field << std::endl;
    std::cout << "Write Filename Base Prefix: " << params.write_filename_base_prefix << std::endl;
    std::cout << "Write Filename Base Suffix: " << params.write_filename_base_suffix << std::endl;
    std::cout << "Particle Lower Limit: " << params.particle_lower_limit << std::endl;
    std::cout << "Threshold Frac For Child: " << params.threshold_frac_for_child << std::endl;
    std::cout << "Linker Output Filename Prefix: " << params.linker_output_filename_prefix << std::endl;
}



/// @brief          Function to parse the parameters from a file
/// @param filename The name of the file containing the parameters
/// @param params   The parameters struct
/// @param name     The name of the parameter
/// @param sim_name The name of the simulation
/// @return         True if the parameters are parsed successfully, false otherwise
bool parseParams(const std::string& filename, Params& params, const std::string& name, const std::string& sim_name) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream is_line(line);
        std::string key, value;
        if (std::getline(is_line, key, '=') && std::getline(is_line, value)) {
            trim(key);
            trim(value);

            if (key == "path") params.path = value + sim_name + "/CloudPhinderData/" + name + "/";
            else if (key == "first_snap") params.first_snap = std::stoi(value);
            else if (key == "last_snap") params.last_snap = std::stoi(value);
            else if (key == "cloud_prefix") params.cloud_prefix = value;
            else if (key == "dat_filename_base_prefix") params.dat_filename_base_prefix = value;
            else if (key == "dat_filename_base_suffix") params.dat_filename_base_suffix = "_"+name+value;
            else if (key == "filename_base_prefix") params.filename_base_prefix = value;
            else if (key == "filename_base_suffix") params.filename_base_suffix = "_"+name+value;
            else if (key == "file_arch_root") params.file_arch_root = value;
            else if (key == "file_arch_cloud_subgroup") params.file_arch_cloud_subgroup = value;
            else if (key == "file_arch_masses_field") params.file_arch_masses_field = value;
            else if (key == "file_arch_pIDs_field") params.file_arch_pIDs_field = value;
            else if (key == "write_filename_base_prefix") params.write_filename_base_prefix = value;
            else if (key == "write_filename_base_suffix") params.write_filename_base_suffix = "_"+name+value;
            else if (key == "particle_lower_limit") params.particle_lower_limit = std::stoi(value);
            else if (key == "threshold_frac_for_child") {params.threshold_frac_for_child = std::stof(value); params.linker_output_filename_prefix = value+"_";}
            else if (key == "linker_output_filename_prefix") params.linker_output_filename_prefix = value+name+"_thresh"+params.linker_output_filename_prefix;

            else std::cerr << "Unknown parameter: " << key << std::endl;
            // Continue for other parameters...
            }
        }
    return true;
}
