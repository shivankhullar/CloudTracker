/// @file matcher.cpp 
/// @brief 	This is the main driver of the matcher application. Its purpose is to match the particles
///		among different cloud entities. 

/// @author Shivan Khullar
/// @date May 2021


//#include <mpi.h>
#include <iostream>
#include <string>
#include <hdf5.h>
#include <vector>

#include "../../../include/matcher/structs_and_classes.h"
#include "../../../include/matcher/matcher_functions.h"
#include "../../../include/matcher/read_params.h"
#include "../../../include/matcher/io.h"
#include "../../../include/matcher/utilities.h"

using namespace std;

//#include "read_params.h"

//#include <boost/program_options.hpp>
//namespace po = boost::program_options;

/// @brief	Main driver. Calls the 'matcher' function after reading in the parameters. Reads in the params.ini config file and
///		assigns parameters to the code variables.
int main(int argc, char** argv)
{

	if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <config_filename> <name> <sim_name>" << std::endl;
        return 1;
    }

    Params params;
    std::string name = argv[2];
	std::string sim_name = argv[3];
    if (!parseParams(argv[1], params, name, sim_name)) {
        return 1;
    }

    // Use the parsed params here
    // matcher(params);
    printParams(params);
    

	//Params params;
	//string name;
	//name = "n10_alpha10";
	//params.path = "../../m12i_res57000_nofb/CloudPhinderData/"+name+"/";
	//params.first_snap = 591; params.last_snap = 614;
	//params.cloud_prefix = "Cloud";
	//params.dat_filename_base_prefix = "bound_";
	//params.dat_filename_base_suffix = "_"+name+".dat";
	//params.filename_base_prefix = "Clouds_";
	//params.filename_base_suffix = "_"+name+".hdf5";
	////params.path = "../sample_data/m12i_no_fb_3/n10_alpha2/";
	//params.file_arch_root = "/";
	//params.file_arch_cloud_subgroup = "PartType0";
	//params.file_arch_masses_field = "Masses";
	//params.file_arch_pIDs_field = "ParticleIDs";
	//params.write_filename_base_prefix = "Tracked_Clouds_";
	//params.write_filename_base_suffix = "_"+name+".hdf5";	
	//params.particle_lower_limit = 32;
	
	matcher(params);
}
