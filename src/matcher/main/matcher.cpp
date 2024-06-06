/// @file 	matcher.cpp 
/// @brief 	This is the main driver of the matcher application. Its purpose is to match the particles
///			among different cloud entities. It reads in the parameters from the xyz_params.txt file and assigns
/// @author Shivan Khullar
/// @date 	June 2024


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




/// @brief	Main driver. Calls the 'matcher' function after reading in the parameters. 
/// 		Reads in the parameter file and
///			assigns parameters to the code variables.
/// @param	argc: Number of arguments
/// @param	argv: Array of arguments
/// @return	0 if successful, 1 if not
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
    printParams(params);
	matcher(params);
}
