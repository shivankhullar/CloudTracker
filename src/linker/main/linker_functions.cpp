#include "linker_functions.h"
#include "read_params.h"
#include "structs_and_classes.h"
#include "utilities.h"
#include "io.h"


void load_to_cloudlist(std::vector<CloudStruct> &cloud_list, CloudStruct &cloud)
{
        for (int i=0; i<cloud_list.size(); i++)
        {
                if (cloud.final_name.compare(cloud_list[i].final_name)==0)
                {       
                        std::cout << "This cloud already exists in cloud_list" << std::endl;
                         return;
                }
        }
	cloud_list.push_back(cloud);
        std::cout << "Added cloud " << cloud.final_name << " to cloud_list. Size: " << cloud_list.size() << "\n" << std::endl;
}

void load_to_tracked_cloud_list(std::vector<std::vector<CloudStruct>> &tracked_cloud_list, CloudStruct &cloud, std::string key, int index)
{
        if (key=="new_gen")
        {
                std::cout << "Adding the cloud to tracked cloud list" << std::endl;
                std::vector<CloudStruct> temp; temp.push_back(cloud);
                tracked_cloud_list.push_back(temp);
        }
	if (key=="nth_gen")
        {
                tracked_cloud_list[index].push_back(cloud);
        }
}

int find_proper_child(std::vector<std::string> &child_list_names, std::vector<double> &child_list_fracs, std::vector<std::string> &child_list_names_original,
                        std::vector<CloudStruct> &cloud_list, int index_to_omit, int snap_num, Params &params)
{
        // Exit if there are no child clouds.
        if (child_list_fracs.size() == 0) {
                return -1;
        }
        // These dummy vectors are important for recurrent calls. We need to reduce the array size by 1 each time.
        std::vector<double> u;                  // array to keep track of fractions
        std::vector<std::string> v;             // array to keep track of names
	std::vector<int> inds;
        for (int i=0; i<child_list_fracs.size(); i++)
        {
                if (i==index_to_omit) { continue;}
		if (child_list_fracs[i] < params.threshold_frac_for_child) {continue;}          // ignore if the fraction is less than the threshold.
                u.push_back(child_list_fracs[i]);                                               // store the fraction in an array to find the max.
                v.push_back(child_list_names[i]);                                               // store the names in an array.
		inds.push_back(i);
        }



        int max_index = -1;
        double max_fraction = -1.0;

        for (int i = 0; i < u.size(); i++) {
                //if (i == index_to_omit || child_list_fracs[i] < params.threshold_frac_for_child) {
                //continue;  // Skip the current iteration if it's the omitted index or the fraction is below the threshold
                //}
                
                if (u[i] > max_fraction) {
                max_fraction = u[i];
                max_index = i;
                }
        }

        if (max_index == -1) {
                std::cout << "No proper child found " << std::endl;
                return -1;  // No proper child found
        }

        // Check if the selected proper child is already in cloud_list
        std::string compare_name = v[max_index] + "Snap" + std::to_string(snap_num);
        //std::cout << "Checking if the proper child : " << compare_name << " is already in cloud_list" << std::endl;
        //std::cout << "Cloud list format " << cloud_list[0].final_name << std::endl;
        //std::cout << "Cloud list size " << cloud_list.size() << std::endl;

        // If the proper child is already in cloud_list (assigned to be the descendant of a more massive cloud), then we need to find the next proper child.
        CloudStruct child_cloud = initialize_cloud(snap_num, v[max_index], params);
        int status = check_if_cloud_exists_in_list(child_cloud, cloud_list);
        if (status==1) {
                std::cout << "This cloud " << child_cloud.final_name << "already exists in cloud_list, skipping!!" << std::endl;
                return find_proper_child(v, u, child_list_names_original, cloud_list, max_index, snap_num, params);
                //continue;
        }
        else {
                std::cout << "No match found in cloudlist for: " << v[max_index] << std::endl;
                // Print the last 10 elements of cloud_list
                std::cout << "Last 10 elements of cloud_list: " << std::endl;
                for (int i=9; i>=0; i--) {
                        std::cout << cloud_list[cloud_list.size()-i-1].final_name << std::endl;
                }
                // Compare the cloud names in v and child_list_names to identify the index in the child_list_names array.
                for (int i=0; i<child_list_names_original.size(); i++)
                {
                        if (child_list_names_original[i].compare(v[max_index])==0)
                        {
                                std::cout << "Found a match for " << v[max_index] << " at index " << i << std::endl;
                                return i;
                        }
                }
                
        }

        // This was the code that wasn't working. I still don't get why it won't compare the strings properly and go inside the loop.
        //for (int i = 0; i < cloud_list.size(); i++) {
        //        //if (cloud_list[i].final_name.compare(compare_name) == 0) {
        //        if (compare_name.compare(cloud_list[i].final_name) == 0) {
        //        std::cout << "Child cloud: " << child_list_names[max_index] << " has already been assigned to a chain." << std::endl;
        //        return find_proper_child(child_list_names, child_list_fracs, cloud_list, max_index, snap_num, params);
        //        }
        //        else {
        //        std::cout << "No match found in cloudlist for: " << child_list_names[max_index] << std::endl;
        //        return max_index;  // Return the index of the proper child}
        //        }
        //}

        return max_index;  // Return the index of the proper child
}


int check_if_cloud_exists_in_list(CloudStruct &cloud, std::vector<CloudStruct> &cloud_list)
{
        int flag = 0;
        for (int i=0; i<cloud_list.size(); i++)
        {
                if (cloud.final_name.compare(cloud_list[i].final_name)==0) {flag = 1;}
        }
	return flag;
}



// Slight re-write, because earlier version had a bug.

// @brief: This function links all the clouds together.
// @param: The parameter struct which contains meta info.
void linker(Params &params)
{
        // Initializing the tracked_cloud_list and cloud_list vectors of cloudstructs. 
        // These variables basically contain the names of clouds. 
        std::vector<std::vector<CloudStruct>> tracked_cloud_list;
        std::vector<CloudStruct> cloud_list; int snap_num, cloud_num;
        for (int i=params.first_snap; i<params.last_snap; i++)
        {
                snap_num = i;
                std::cout << "Starting with snapshot: " << snap_num << std::endl;
                // Find the total number of clouds in this snapshot.
                int num_parent_clouds = find_num_clouds(snap_num, params);
                std::cout << "Total clouds in this snapshot: " << num_parent_clouds << std::endl;
                for (int j=0; j<num_parent_clouds; j++)
                {
                        cloud_num = j;
                        std::string cloud_name = get_cloud_name(cloud_num, params);
                        // Ignore particles with less particles than the cutoff.
                        int num_parts = get_num_particles(params, snap_num, cloud_name);
                        if (num_parts<=params.particle_lower_limit) {continue;}
			

			//std::string cloud_name = get_cloud_name(cloud_num, params);
                        CloudStruct cloud = initialize_cloud(snap_num, cloud_name, params);
                        cloud.total_mass = read_mass(params, snap_num, cloud_name);
                        std::cout << "Total mass for " << cloud.final_name << " is " << cloud.total_mass <<
                                " with " << num_parts << " particles" << std::endl;
                        // Step 5: Check if the cloud exists already in cloud_list
                        int status = check_if_cloud_exists_in_list(cloud, cloud_list);

                        if (status==1) {continue;}
                        // Step 1: Add all the clouds in the first snap to cloud_list and tracked_cloud_list.
			else {
                              	std::cout << "This cloud does not already exist. Adding it." << std::endl;
                                load_to_cloudlist(cloud_list, cloud);
                                load_to_tracked_cloud_list(tracked_cloud_list, cloud, "new_gen", 0);
                                std::cout << "Tracked cloud list has " << tracked_cloud_list.size() << std::endl;
                        }
                        //print_list(tracked_cloud_list);
                }
                print_list(tracked_cloud_list);

                for (int j=0; j<tracked_cloud_list.size(); j++)
                {
                        int last_index = tracked_cloud_list[j].size()-1;

                        // Step 7: Only iterate over the ones that are not the end of their generation.
                        if (tracked_cloud_list[j][last_index].end_of_gen==1) {continue;}
                        std::cout << "======================================" << std::endl;
			std::cout << "For Cloud " << tracked_cloud_list[j][last_index].final_name << std::endl;

                        // Step 2: For each parent, find the proper child and add to cloud_list and tracked_cloud_list.
                        std::vector<std::string> child_list_names; std::vector<double> child_list_fracs;
                        get_child_list(params, snap_num, tracked_cloud_list[j][last_index].name, child_list_names, child_list_fracs);
                        std::cout << "There are " << child_list_names.size() << " children" << std::endl;
                        for (int k=0; k<child_list_names.size(); k++)
                        {
                                std::cout << "Child " << k << " is " << child_list_names[k] << " with " << child_list_fracs[k] << std::endl;
                        }
                        int index = find_proper_child(child_list_names, child_list_fracs, child_list_names, cloud_list, -1, snap_num+1, params);
                        if (index!=-1) {
                        std::cout << "Proper child is: " << child_list_names[index] << ", Frac: " << child_list_fracs[index] << std::endl;}
			// Step 3 and 4.
                        if (index == -1) 
                        {
                                tracked_cloud_list[j][last_index].end_of_gen = 1; 
                                std::cout << "Sorry, there's no proper child for this cloud, bloodline ends" << std::endl;
                        }       // if there's no proper child, end of gen.
                        else
                        {
                                CloudStruct child_cloud = initialize_cloud(snap_num+1, child_list_names[index], params);
				int status = check_if_cloud_exists_in_list(child_cloud, cloud_list);
				if (status==1) {
                                        std::cout << "This cloud already exists in cloud_list, skipping." << std::endl;
                                        continue;}
                                load_to_cloudlist(cloud_list, child_cloud);
                                load_to_tracked_cloud_list(tracked_cloud_list, child_cloud, "nth_gen", j);
                        }

                }
                //print_list(tracked_cloud_list);
        }
	print_list(tracked_cloud_list);

        write_to_file(tracked_cloud_list, params, "masses");
        write_to_file(tracked_cloud_list, params, "names");
        // Load all the parent clouds to this list.
        // Then find the proper child of each parent cloud and add that to the list. Set the 'added_or_not' flag for the cloud to be 1.
        // Then once this is done for all, look at the next set of snapshots, and for each cloud, determine if it exists as a child
        // in the list. If it doesn't add these ones to the list.

}
