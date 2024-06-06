#include "../../../include/matcher/matcher_functions.h"
#include "../../../include/matcher/structs_and_classes.h"
#include "../../../include/matcher/read_params.h"
#include "../../../include/matcher/io.h"
#include "../../../include/matcher/utilities.h"


//#include <H5Cpp.h>
#include <hdf5.h>
#include <iostream>
#include <string>
#include <vector>
//using namespace H5;


int compare_particles(std::vector<double> a, std::vector<double> b)
{
        int count = 0;
	int dups = 0;
	std::vector<double> c;
        for (int i=0; i<a.size();i++)
        {
                for (int j=0; j<b.size(); j++)
                {
                        if (a[i] == b[j]) 
			{ 
				count = count+1;
				//for (int k=0; k<c.size(); k++)
				//{
				//	if (b[j] == c[k]) {std::cout << "This is a duplicate particle, wtf..." << j << ", " << k << std::endl;
				//		dups = dups +1;}
				//}
				//c.push_back(b[j]);
			}
                }
        }
	//std::cout << "Sizes: " << a.size() << ", " << b.size() << ", dups: " << dups << std::endl; 
	return count;
}



void mass_frac(std::vector<double> parent_pIDs, std::vector<double> child_pIDs, std::vector<double> parent_masses, std::vector<double> child_masses, 
		int num_common_particles, double &childs_mass_frac_from_parent, double &parents_mass_frac_to_child)
{
        int count = 0;
        std::vector<double> store_parent_masses(num_common_particles);
        std::vector<double> store_child_masses(num_common_particles);
        for (int i=0; i<parent_pIDs.size();i++)
        {
                for (int j=0; j<child_pIDs.size(); j++)
                {
                        if (parent_pIDs[i] == child_pIDs[j]) {
                                store_parent_masses[count] = parent_masses[i];
                                store_child_masses[count] = child_masses[j];
                                count++;
                        }
                }
        }
        childs_mass_frac_from_parent = sum_array(store_child_masses) / sum_array(child_masses);
        parents_mass_frac_to_child = sum_array(store_parent_masses) / sum_array(parent_masses);
	std::cout << "Parents mass frac to child: " << parents_mass_frac_to_child << std::endl;
	if (childs_mass_frac_from_parent >=1 || parents_mass_frac_to_child>=1)
	{
		std::cout << "Store parent masses:" << store_parent_masses.size() << " " << num_common_particles << " " << parent_masses.size() << std::endl;
		//print_array_double(store_parent_masses);
		std::cout << "Store child masses:" << store_child_masses.size() << " " << num_common_particles << " " << child_masses.size() << std::endl;
		std::cout << "Sums: " << sum_array(store_parent_masses) << " " << sum_array(parent_masses) << std::endl;
		//print_array_double(store_child_masses);

		//std::cout << "Store parent masses:" << std::endl;
		//std::cout << " " << std::endl;
	}
	//std::cout << "Childs mass frac from parent value: " << childs_mass_frac_from_parent << std::endl;
	//std::cout << "Parents mass frac to child value: " << parents_mass_frac_to_child <<std::endl;
}


std::string get_cloud_name(int i, Params &params)
{
        std::string cloud_name;
        // customize this part below based on how many clouds there are.
        if (i<10) { cloud_name = params.cloud_prefix + "000"+ std::to_string(i);}
        else if (i>=10 & i<100) { cloud_name = params.cloud_prefix + "00"+ std::to_string(i);}
        else if (i>=100 & i<1000) { cloud_name = params.cloud_prefix + "0"+ std::to_string(i);}
        else { cloud_name = params.cloud_prefix + std::to_string(i);}
	// std::cout << "Make sure to modify the get_cloud_name() function correctly." << std::endl;
        return cloud_name;
}


void load_clouds_to_group(int snap_num, int num_clouds_snap, Params &params, CitySnaps &snapsnap, std::string key)
{
        for (int i=0; i<num_clouds_snap; i++)
        {
                std::string cloud_name = get_cloud_name(i, params);
		std::vector<double> masses = read_cloud_data_double(params, snap_num, params.file_arch_masses_field, cloud_name);
		//print_array_double(masses);
		std::vector<double> pIDs = read_cloud_data_double(params, snap_num, params.file_arch_pIDs_field, cloud_name);
        std::vector<double> pIDgen = read_cloud_data_double(params, snap_num, params.file_arch_pIDgen_field, cloud_name);
		
		//print_array_double(pIDs);
                
		MemberCloud snapcloud(cloud_name, pIDs, pIDgen, masses);
                if (key.compare("parent")==0) { snapsnap.parent_group.add_member(snapcloud);   //} 
			std::cout << "Added Cloud" << i << " to parent group" << std::endl;}
                else if (key.compare("child")==0) { snapsnap.child_group.add_member(snapcloud);
			std::cout << "Added Cloud" << i << " to child group" << std::endl;}
                else { std::cout << "Don't change the format of the key being passed around." << std::endl;}
        }
}



void matcher(Params &params)
{
        int num_snaps = params.last_snap - params.first_snap + 1;
	std::cout << "Number of snapshots: " << num_snaps << " " << params.last_snap << " " << params.first_snap << std::endl;
        for (int i=params.first_snap; i<params.last_snap; i++)
        {
		std::cout << "Starting with snaps: " << i << ", " <<i+1 << std::endl;
                CitySnaps snapsnap(i, i+1);
                int num_clouds_parent_snap = find_num_clouds(i, params);
                int num_clouds_child_snap = find_num_clouds(i+1, params);
                load_clouds_to_group(i, num_clouds_parent_snap, params, snapsnap, "parent");
                load_clouds_to_group(i+1, num_clouds_child_snap, params, snapsnap, "child");
                for (int j=0; j<num_clouds_parent_snap; j++)
                {
                        for (int k=0; k<num_clouds_child_snap;k++)
                        {
				if (snapsnap.parent_group.members[j].particleIDs.size()<=params.particle_lower_limit || 
					snapsnap.child_group.members[k].particleIDs.size()<=params.particle_lower_limit) {continue;}

                                //std::cout << "Comparing clouds " << snapsnap.parent_group.members[j].name << " and " << snapsnap.child_group.members[k].name << std::endl;
				int num_common_particles = compare_particles(snapsnap.parent_group.members[j].particleIDs, 
					snapsnap.child_group.members[k].particleIDs);
                                
				if (num_common_particles == 0) {continue;}           // no particles match
                                else {
                                        //Find child_mass_from_parent and parent_mass_to_child.
					double childs_mass_frac_from_parent, parents_mass_frac_to_child;
                                        mass_frac(snapsnap.parent_group.members[j].particleIDs, snapsnap.child_group.members[k].particleIDs,
						snapsnap.parent_group.members[j].masses, snapsnap.child_group.members[k].masses,
						num_common_particles, childs_mass_frac_from_parent, parents_mass_frac_to_child);
					
					snapsnap.parent_group.members[j].add_child(snapsnap.child_group.members[k].name, 
						parents_mass_frac_to_child, childs_mass_frac_from_parent, snapsnap.child_group.members[k].total_mass);
					snapsnap.child_group.members[k].add_parent(snapsnap.parent_group.members[j].name, 
						parents_mass_frac_to_child, childs_mass_frac_from_parent, snapsnap.parent_group.members[j].total_mass);
					std::cout << "Particles matching: " << num_common_particles << " between cloud "<< j << " and " << k <<std::endl;
					std::cout << "Childs mass frac from this parent: " << childs_mass_frac_from_parent<< std::endl;
					std::cout << "Parents mass frac to this child: " << parents_mass_frac_to_child << std::endl;
					//snapsnap.parent_group.members[j].add_child();
                                        //std::cout << "Thanks, see you later! HAHAHA" << std::endl;
                                }
                        }
                }
        	write_to_hdf5_file(snapsnap, params, num_clouds_parent_snap, num_clouds_child_snap);
	}
}

