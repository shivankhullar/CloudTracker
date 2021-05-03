#include "linker_functions.h"
//#include "linker_structs.h"

#include <hdf5.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>

std::string get_snapshot_name(int i)
{
        std::string name;
        if (i<10) { name = "00"+std::to_string(i);}
        else if (i>=10 & i<100) {name = "0"+std::to_string(i);}
        else { name = std::to_string(i);}
        return name;
}

std::string get_cloud_name(int i, Params &params)
{
        std::string cloud_name;
        // customize this part below based on how many clouds there are.
        if (i<10) { cloud_name = params.cloud_prefix + "00"+ std::to_string(i);}
        else if (i>=10 & i<100) { cloud_name = params.cloud_prefix + "0"+ std::to_string(i);}
        else if (i>=100 & i<1000) { cloud_name = params.cloud_prefix + std::to_string(i);}
        else { cloud_name = params.cloud_prefix + std::to_string(i);}
        // std::cout << "Make sure to modify the get_cloud_name() function correctly." << std::endl;
        return cloud_name;
}

int find_num_clouds(int snap_num, Params &params)
{
        std::string fname = params.path+params.dat_filename_base_prefix+get_snapshot_name(snap_num)+params.dat_filename_base_suffix;
        std::ifstream file(fname);
        if (!file.is_open()) {
        std::cout << "Couldn't open file " << fname << std::endl;
        std::exit(-2);
        }

	std::string line;
        int c = 0;
        std::string hash = "#";
        while (std::getline(file, line))
        {
                if (line[0]=='#')
                {
                        continue;
                }
                c++;
        }
	file.close();
        return c;
}

hid_t get_last_group(Group_struct *subgroup_struct, hid_t group)
{
        if (subgroup_struct->subgroup!=NULL)
        {
                //std::cout << "Trying for group: " << subgroup_struct->name << std::endl;
                hid_t grp = H5Gopen(group, &subgroup_struct->name[0], H5P_DEFAULT);
                //group.openGroup(subgroup_struct->name);
                hid_t grp1 = get_last_group(subgroup_struct->subgroup, grp);
                //H5::Group grp1 = get_last_group(subgroup_struct->subgroup, grp);
                //std::cout << "Hi, opened group: " << subgroup_struct->name << std::endl;
                return grp1;
                //gname = file_arch->subgroup->name;
        }
	else
	{
                //std::cout << "Reached bottom group, this is a dataset: " << subgroup_struct->name << std::endl;
                //H5::Group grp = group.openGroup(subgroup_struct->name);
                //std::cout << "Hi, opened last group: " << subgroup_struct->name << std::endl;
                return group;
        }
}

std::vector<double> read_cloud_data_double(Params &params, int snap_num, std::string field_to_read, std::string cloud_name)
{
        Group_struct *file_arch = create_group(params.file_arch_root);
        file_arch->subgroup = create_group(cloud_name);
        file_arch->subgroup->subgroup = create_group(params.file_arch_cloud_subgroup);
        file_arch->subgroup->subgroup->subgroup = create_group(field_to_read);
        std::string fname = params.path+params.filename_base_prefix+get_snapshot_name(snap_num)+params.filename_base_suffix;
	
	hid_t file = H5Fopen (&fname[0], H5F_ACC_RDONLY, H5P_DEFAULT);
        //H5File file(fname, H5F_ACC_RDONLY);
        std::string root = file_arch->name;
        hid_t grp_root = H5Gopen(file, &root[0], H5P_DEFAULT);
        hid_t last_group = get_last_group(file_arch->subgroup, grp_root);
        hid_t dataset = H5Dopen(last_group, &field_to_read[0], H5P_DEFAULT);
        //H5::DataSet dataset = last_group.openDataSet(field_to_read);

        hid_t dataspace = H5Dget_space (dataset);
        int rank = H5Sget_simple_extent_ndims (dataspace);
        hsize_t dims_out[1];
        int status_n  = H5Sget_simple_extent_dims (dataspace, dims_out, NULL);
        std::vector<double> data(dims_out[0]);
        hid_t memspace = H5Screate_simple (rank, dims_out, NULL);
        //DataSpace memspace(rank, dims_out);
        herr_t status = H5Dread (dataset, H5T_IEEE_F64LE, memspace, dataspace,
                      H5P_DEFAULT, &data[0]);

	//dataset.read(&data[0], H5::PredType::IEEE_F64LE, memspace, dataspace);

        delete file_arch;
        //std::cout << "Read dataset" << std::endl;
        //print_array_double(data);
        return data;
}

int check_if_exists(std::vector<double> a, double b)
{
        int c = 0;
        for (int i=0; i<a.size(); i++)
        {
                if (a[i]==b) { c = c+1;}
        }
	return c;
}

int get_num_particles(Params &params, int snap_num, std::string cloud_name)
{
        std::vector<double> pIDs = read_cloud_data_double(params, snap_num, params.file_arch_masses_field, cloud_name);
        std::vector<double> particleIDs;
        for (int i=0; i<pIDs.size(); i++)
        {
                if (check_if_exists(particleIDs, pIDs[i])==0)
                {
                        particleIDs.push_back(pIDs[i]);
                }
        }
	return particleIDs.size();
}

double read_mass(Params &params, int snap_num, std::string cloud_name)
{
        std::string fname = params.path+params.write_filename_base_prefix + get_snapshot_name(snap_num) + "_" + get_snapshot_name(snap_num+1)
                                + params.write_filename_base_suffix;
        hid_t file = H5Fopen (&fname[0], H5F_ACC_RDONLY, H5P_DEFAULT);
        std::string root = "/";
        hid_t grp_root = H5Gopen(file, &root[0], H5P_DEFAULT);
        hid_t parent_group = H5Gopen(grp_root, "parent_group", H5P_DEFAULT);
        //std::string cloud_name = get_cloud_name(cloud_num, params);
        hid_t cloud_data_group = H5Gopen(parent_group, &cloud_name[0], H5P_DEFAULT);

	std::string dset_name = "total_mass_parent";
        hid_t dataset = H5Dopen(cloud_data_group, &dset_name[0], H5P_DEFAULT);
        hid_t dataspace = H5Dget_space (dataset);
        int rank = H5Sget_simple_extent_ndims (dataspace);
        hsize_t dims_out[1];
        int status_n  = H5Sget_simple_extent_dims (dataspace, dims_out, NULL);
        hid_t memspace = H5Screate_simple (rank, dims_out, NULL);
        double total_mass;
        herr_t status = H5Dread (dataset, H5T_IEEE_F64LE, memspace, dataspace,
                      H5P_DEFAULT, &total_mass);

        return total_mass;
}


herr_t child_info(hid_t loc_id, const char *name, const H5L_info_t *linfo, void *opdata)
{
        hid_t group;
        auto group_names=reinterpret_cast< std::vector<std::string>* >(opdata);
        group = H5Gopen2(loc_id, name, H5P_DEFAULT);

        group_names->push_back(name);
        //std::cout << "Name : " << name << std::endl;
        H5Gclose(group);
        return 0;
}

void get_child_list(Params &params, int snap_num, std::string cloud_name, std::vector<std::string> &child_list_names, std::vector<double> &child_list_fracs)
{
        std::string fname = params.path+params.write_filename_base_prefix + get_snapshot_name(snap_num) + "_" + get_snapshot_name(snap_num+1)
                                + params.write_filename_base_suffix;
        hid_t file = H5Fopen (&fname[0], H5F_ACC_RDONLY, H5P_DEFAULT);
        std::string root = "/";
        hid_t grp_root = H5Gopen(file, &root[0], H5P_DEFAULT);
        hid_t parent_group = H5Gopen(grp_root, "parent_group", H5P_DEFAULT);
        //std::string cloud_name = get_cloud_name(cloud_num, params);
        hid_t cloud_data_group = H5Gopen(parent_group, &cloud_name[0], H5P_DEFAULT);

        hid_t children = H5Gopen(cloud_data_group, "children", H5P_DEFAULT);

        std::vector<std::string> child_names;
	herr_t idx = H5Literate(children, H5_INDEX_NAME, H5_ITER_INC, NULL, child_info, &child_names);
        std::cout << "There are " << child_names.size() << " children for this cloud" << std::endl;
        for (int i=0; i<child_names.size(); i++)
        {
		//std::cout << "Child " << i << " is " << child_names[i] << std::endl;
                child_list_names.push_back(child_names[i]); //+"Snap"+get_snapshot_name(snap_num+1));
                std::string child_name = child_names[i]; //+get_snapshot_name(snap_num+1);
                hid_t child = H5Gopen(children, &child_name[0], H5P_DEFAULT);

                hid_t dataset = H5Dopen(child, "parents_mass_frac_to_child", H5P_DEFAULT);
                hid_t dataspace = H5Dget_space (dataset);
                int rank = H5Sget_simple_extent_ndims (dataspace);
                hsize_t dims_out[1];
                int status_n  = H5Sget_simple_extent_dims (dataspace, dims_out, NULL);
                hid_t memspace = H5Screate_simple (rank, dims_out, NULL);
                double frac;
                herr_t status = H5Dread (dataset, H5T_IEEE_F64LE, memspace, dataspace,
                      H5P_DEFAULT, &frac);
                child_list_fracs.push_back(frac);
                std::cout << "Child " << i << " is " << child_names[i] <<" or "<< child_list_names[i] << ", frac: " << frac << std::endl;
                //std::cout << "Total mass for this cloud is: " << total_mass << std::endl;
        }
}

void load_to_cloudlist(std::vector<CloudStruct> &cloud_list, CloudStruct &cloud)
{
        for (int i=0; i<cloud_list.size(); i++)
        {
                if (cloud.final_name.compare(cloud_list[i].final_name)==0)
                {
                         return;
                }
        }
	cloud_list.push_back(cloud);
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

int find_proper_child(std::vector<std::string> &child_list_names, std::vector<double> &child_list_fracs,
                        std::vector<CloudStruct> &cloud_list, int index_to_omit)
{
        if (child_list_fracs.size()==0) {return -1;}

        std::vector<double> v;
        std::vector<std::string> u;
        for (int i=0; i<child_list_fracs.size(); i++)
        {
                if (i==index_to_omit) { continue;}
                v.push_back(child_list_fracs[i]);
                u.push_back(child_list_names[i]);
        }
	std::vector<double>::iterator result;
        result = std::max_element(v.begin(), v.end());
        int index = std::distance(v.begin(), result);
        for (int i=0; i<cloud_list.size(); i++)
        {
                if (cloud_list[i].final_name.compare(child_list_names[index])==0)
                {
                        std::cout << "Child cloud: " << child_list_names[index] << " has already been assigned to a chain." << std::endl;
                        find_proper_child(u, v, cloud_list, index);
                }
        }
	return index;
	//std::cout << "max element at: " << std::distance(v.begin(), result) << '\n';
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

void print_list(std::vector<std::vector<CloudStruct>> &tracked_cloud_list)
{
        std::cout << "=======================" <<std::endl;
        std::cout << "=======================" <<std::endl;
        std::cout << "Tracked Cloud List: " << std::endl;
        std::cout << "Size: " << tracked_cloud_list.size() << "and " << tracked_cloud_list[0].size() << std::endl;
        for (int i=0; i<tracked_cloud_list.size(); i++)
        {
                for (int j=0; j<tracked_cloud_list[i].size(); j++)
                {
                        std::cout << tracked_cloud_list[i][j].final_name << ", ";
                }
                std::cout << std::endl;
        }
        std::cout << std::endl;
}


void write_to_file(std::vector<std::vector<CloudStruct>> &tracked_cloud_list, Params &params, std::string field_name)
{
        std::string file_name = params.path + params.linker_output_filename_prefix + get_snapshot_name(params.first_snap) + "_"
                                + get_snapshot_name(params.last_snap)+ "_" + field_name + ".txt";
        std::ofstream file(file_name);
        if (field_name.compare("names")==0)
        {
                for (int i=0; i<tracked_cloud_list.size(); i++)
                {
                        for (int j=0; j<tracked_cloud_list[i].size(); j++)
                        {
                                file << tracked_cloud_list[i][j].final_name << ", ";
                        }
                        file << std::endl;
                }
                file << std::endl;
        }
	if (field_name.compare("masses")==0)
        {
                for (int i=0; i<tracked_cloud_list.size(); i++)
                {
                        for (int j=0; j<tracked_cloud_list[i].size(); j++)
                        {
                                file << tracked_cloud_list[i][j].total_mass << ", ";
                        }
                        file << std::endl;
                }
                file << std::endl;
        }
}


void linker(Params &params)
{
        std::vector<std::vector<CloudStruct>> tracked_cloud_list;
        std::vector<CloudStruct> cloud_list; int snap_num, cloud_num;
        for (int i=params.first_snap; i<params.last_snap; i++)
        {
                snap_num = i;
                std::cout << "Starting with snapshot: " << snap_num << std::endl;
                int num_parent_clouds = find_num_clouds(snap_num, params);
                std::cout << "Total clouds in this snapshot: " << num_parent_clouds << std::endl;
                for (int j=0; j<num_parent_clouds; j++)
                {
                        cloud_num = j;
                        std::string cloud_name = get_cloud_name(cloud_num, params);
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
                        int index = find_proper_child(child_list_names, child_list_fracs, cloud_list, -1);
                        if (index!=-1) {
                        std::cout << "Proper child is: " << child_list_names[index] << ", Frac: " << child_list_fracs[index] << std::endl;}
			// Step 3 and 4.
                        if (index == -1) {tracked_cloud_list[j][last_index].end_of_gen = 1;}    // if there's no proper child, end of gen.
                        else
                        {
                                CloudStruct child_cloud = initialize_cloud(snap_num+1, child_list_names[index], params);
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

			
