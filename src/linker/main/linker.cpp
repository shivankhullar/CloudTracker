
#include <hdf5.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>

#include "../../../include/linker/linker_functions.h"
#include "../../../include/linker/structs_and_classes.h"
#include "../../../include/linker/read_params.h"
#include "../../../include/linker/utilities.h"
#include "../../../include/linker/io.h"

using namespace std;

int main(int argc, char* argv[])
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
	
        linker(params);
}
