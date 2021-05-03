#include "linker_functions.h"
#include "linker_structs.h"
#include <hdf5.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>


int main(int argc, char* argv[])
{
	Params params;
        params.first_snap = 590; params.last_snap = 599;
        params.cloud_prefix = "Cloud";
        params.dat_filename_base_prefix = "bound_";
        params.dat_filename_base_suffix = "_n10_alpha2.dat";
        params.filename_base_prefix = "Clouds_";
        params.filename_base_suffix = "_n10_alpha2.hdf5";
        params.path = "../sample_data/";
        params.file_arch_root = "/";
        params.file_arch_cloud_subgroup = "PartType0";
        params.file_arch_masses_field = "Masses";
        params.file_arch_pIDs_field = "ParticleIDs";
        params.write_filename_base_prefix = "Tracked_Clouds_";
        params.write_filename_base_suffix = "_n10_alpha2.hdf5";
        params.particle_lower_limit = 32;
	params.linker_output_filename_prefix = "Linked_Clouds_";
	linker(params);
}
