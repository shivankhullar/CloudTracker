//#include <mpi.h>
#include <iostream>
#include <string>
//#include <rarray>

#include <H5Cpp.h>
using namespace H5;

#include "structs_and_classes.h"
#include "matcher_functions.h"


int main()
{
	Params params;
	params.first_snap = 590; params.last_snap = 600;
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
	matcher(params);
}



	//CitySnaps snapsnap(590,591);
	//std::vector<int> pIDs = {10,10,12,14}; 
	//MemberCloud snapcloud("Cloud019", pIDs);	
	//std::cout << snapsnap.snap_num1 <<  " " << snapsnap.snap_name1 << std::endl;
	//std::cout << "Hi" << std::endl;
	//std::cout << snapcloud.name << " " << snapcloud.particleIDs[2] << std::endl;
	//snapsnap.parent_group.add_member(snapcloud);
	//std::cout << snapsnap.parent_group.members[0].name << " " << snapsnap.parent_group.members[0].particleIDs[2] << std::endl;
	//std::vector<int> pIDs1 = {10,10,22,14};
        //MemberCloud snapcloud1("Cloud020", pIDs1);
	//snapsnap.child_group.add_member(snapcloud1);
        //std::cout << snapsnap.child_group.members.size() << " "<< snapsnap.child_group.members[0].name << " " << snapsnap.child_group.members[0].particleIDs[2] << std::endl;
	
