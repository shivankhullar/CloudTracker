/// @file matcher.cpp 
/// @brief 	This is the main driver of the matcher application. Its purpose is to match the particles
///		among different cloud entities. 

/// @author Shivan Khullar
/// @date May 2021


//#include <mpi.h>
#include <iostream>
#include <string>
//#include <rarray>

#include <H5Cpp.h>
using namespace H5;

#include "structs_and_classes.h"
#include "matcher_functions.h"


using namespace std;
//#include "read_params.h"

//#include <boost/program_options.hpp>
//namespace po = boost::program_options;

/// @brief	Main driver. Calls the 'matcher' function after reading in the parameters. Reads in the params.ini config file and
///		assigns parameters to the code variables.
int main(int ac, char** av)
{
	// File to read for parameters.
//        std::string filename = "params.ini";
//        std::ifstream ifs(filename);

        // Using the boost library for parsing config files.

//        po::variables_map vm = read_params_file(ifs, filename);         // Read the variables from file
//        po::variables_map vm2 = read_cmd_line(ac, av, filename);        // Arguments can be parsed through command line too. Useful for ./exec --help.

        // Print out help message for the parameters if ./exec --help is used.
//        po::options_description desc = get_options_description(filename);	// Get the description of all the defined parameters.
//        if (vm2.count("help")) {
//                std::cout << desc << "\n";
//                return 1;
//        }


	Params params;
	string name;
	name = "n10_alpha10";
	params.path = "../../m12i_res57000_nofb/CloudPhinderData/"+name+"/";
	params.first_snap = 591; params.last_snap = 614;
	params.cloud_prefix = "Cloud";
	params.dat_filename_base_prefix = "bound_";
	params.dat_filename_base_suffix = "_"+name+".dat";
	params.filename_base_prefix = "Clouds_";
	params.filename_base_suffix = "_"+name+".hdf5";
	//params.path = "../sample_data/m12i_no_fb_3/n10_alpha2/";
	params.file_arch_root = "/";
	params.file_arch_cloud_subgroup = "PartType0";
	params.file_arch_masses_field = "Masses";
	params.file_arch_pIDs_field = "ParticleIDs";
	params.write_filename_base_prefix = "Tracked_Clouds_";
	params.write_filename_base_suffix = "_"+name+".hdf5";	
	params.particle_lower_limit = 32;
	matcher(params);
}
