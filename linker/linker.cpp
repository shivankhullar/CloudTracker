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

using namespace std;

int main(int argc, char* argv[])
{
	Params params;
        params.first_snap = 601; params.last_snap = 614;
        string name;
	name = "n10_alpha2";
	params.path = "../../m12i_no_fb_3/CloudPhinderData/"+name+"/";
        params.cloud_prefix = "Cloud";
        params.dat_filename_base_prefix = "bound_";
        params.dat_filename_base_suffix = "_"+name+".dat";
        params.filename_base_prefix = "Clouds_";
        params.filename_base_suffix = "_"+name+".hdf5";
        params.file_arch_root = "/";
        params.file_arch_cloud_subgroup = "PartType0";
        params.file_arch_masses_field = "Masses";
        params.file_arch_pIDs_field = "ParticleIDs";
        params.write_filename_base_prefix = "Tracked_Clouds_";
        params.write_filename_base_suffix = "_"+name+".hdf5";
        params.particle_lower_limit = 32;
	params.threshold_frac_for_child=0.0;
	params.linker_output_filename_prefix = "Linked_Clouds_"+name+"_thresh0.0_";
	linker(params);
}
