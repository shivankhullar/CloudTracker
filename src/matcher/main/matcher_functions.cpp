/// @file       matcher_functions.cpp 
/// @brief      Contains the definitions of the functions used in the matcher application.
///             This includes the compare_particles, mass_frac, get_cloud_name, and load_clouds_to_group functions.
///             The compare_particles function compares two particle ID lists and counts the number of matching IDs.
///             The mass_frac function calculates the mass fraction of particles between two clouds.
///             The get_cloud_name function returns the name of the cloud based on the cloud number.
///             The load_clouds_to_group function loads the cloud data into the parent or child group.
///             The matcher function is the main driver of the matcher application.
///             It matches the particles among different cloud entities.
/// @author     Shivan Khullar
/// @date       June 2024


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

/// @brief Compares two particle ID lists and counts the number of matching IDs.
/// @param a The first list of particle IDs.
/// @param b The second list of particle IDs.
/// @return The number of matching particle IDs.
int compare_particles(std::vector<double> a, std::vector<double> b)
{
        int count = 0; // number of common particles
	int dups = 0;  // number of duplicate particles, ignored for now
	std::vector<double> c;
        for (int i=0; i<a.size();i++)
        {
                for (int j=0; j<b.size(); j++)
                {
                        if (a[i] == b[j]) 
			{ 
				count = count+1;
			}
                }
        }
	return count;
}


/// @brief                              Calculates the mass fraction of particles between two clouds.
/// @param parent_pIDs                  The list of particle IDs of the parent cloud.
/// @param child_pIDs                   The list of particle IDs of the child cloud.
/// @param parent_masses                The list of masses of the parent cloud.
/// @param child_masses                 The list of masses of the child cloud.
/// @param num_common_particles         The number of particles that are common between the two clouds.
/// @param childs_mass_frac_from_parent The mass fraction of the child cloud, sourced from the parent cloud.
/// @param parents_mass_frac_to_child   The mass fraction of the parent cloud, donated to the child cloud.
void mass_frac(std::vector<double> parent_pIDs, std::vector<double> child_pIDs, std::vector<double> parent_masses, 
                std::vector<double> child_masses, int num_common_particles, double &childs_mass_frac_from_parent, 
                double &parents_mass_frac_to_child)
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
		std::cout << "Store parent masses:" << store_parent_masses.size() << " " << 
                                num_common_particles << " " << parent_masses.size() << std::endl;
		std::cout << "Store child masses:" << store_child_masses.size() << " " << 
                                num_common_particles << " " << child_masses.size() << std::endl;
		std::cout << "Sums: " << sum_array(store_parent_masses) << " " << 
                                sum_array(parent_masses) << std::endl;
	}
}




/// @brief              Returns the name of the cloud based on the cloud number.
/// @param i            The cloud number.
/// @param params       The parameters of the simulation.
/// @return             The name of the cloud.
std::string get_cloud_name(int i, Params &params)
{
        std::string cloud_name;
        // customize this part below based on how many clouds there are if needed.
        //if (i<10) { cloud_name = params.cloud_prefix + "000"+ std::to_string(i);}
        //else if (i>=10 & i<100) { cloud_name = params.cloud_prefix + "00"+ std::to_string(i);}
        //else if (i>=100 & i<1000) { cloud_name = params.cloud_prefix + "0"+ std::to_string(i);}
        if (i<10) { cloud_name = params.cloud_prefix + "0"+ std::to_string(i);}
        //else if (i>=10 & i<100) { cloud_name = params.cloud_prefix + "0"+ std::to_string(i);}
        //else if (i>=100 & i<1000) { cloud_name = params.cloud_prefix + std::to_string(i);}
        //else { cloud_name = params.cloud_prefix + std::to_string(i);}
        else {cloud_name = params.cloud_prefix + std::to_string(i);}
	// std::cout << "Make sure to modify the get_cloud_name() function correctly." << std::endl;
        return cloud_name;
}



/// @brief                      Loads the cloud data into the parent or child group.
/// @param snap_num             The snapshot number.
/// @param num_clouds_snap      The number of clouds in the snapshot.
/// @param params               The parameters of the simulation.
/// @param snapsnap             The CitySnaps object.
/// @param key                  The key to determine if the cloud is a parent or child.
void load_clouds_to_group(int snap_num, int num_clouds_snap, Params &params, CitySnaps &snapsnap, std::string key)
{
        for (int i=0; i<num_clouds_snap; i++)
        {
                std::string cloud_name = get_cloud_name(i, params);
		std::vector<double> masses = read_cloud_data_double(params, snap_num, 
                                                params.file_arch_masses_field, cloud_name);
		//print_array_double(masses);
		std::vector<double> pIDs = read_cloud_data_double(params, snap_num, 
                                                params.file_arch_pIDs_field, cloud_name);
                //std::vector<double> pIDgen = read_cloud_data_double(params, snap_num, 
                //                                params.file_arch_pIDgen_field, cloud_name);
		
		
		MemberCloud snapcloud(cloud_name, pIDs, masses);  //pIDgen, masses);
                if (key.compare("parent")==0) { snapsnap.parent_group.add_member(snapcloud);   //} 
			std::cout << "Added Cloud" << i << " to parent group" << std::endl;}
                else if (key.compare("child")==0) { snapsnap.child_group.add_member(snapcloud);
			std::cout << "Added Cloud" << i << " to child group" << std::endl;}
                else { std::cout << "Don't change the format of the key being passed around." << std::endl;}
        }
}



/// @brief              The main driver of the matcher application.
///                     Matches the particles among different cloud entities.
/// @param params       The parameters of the simulation.
/// @return             
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
                std::cout << "Number of clouds in parent and child snaps: " << num_clouds_parent_snap << " " << num_clouds_child_snap << std::endl;
                load_clouds_to_group(i, num_clouds_parent_snap, params, snapsnap, "parent");
                load_clouds_to_group(i+1, num_clouds_child_snap, params, snapsnap, "child");
                for (int j=0; j<num_clouds_parent_snap; j++)
                {
                        for (int k=0; k<num_clouds_child_snap;k++)
                        {
                                // Implement any custom criteria to exclude clouds from matching here. 
                                // Useful if say you know the clouds are within a certain distance of each other.

                                // Ignore clouds with less than a certain number of particles -- kernel size or approx resolution limit
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

