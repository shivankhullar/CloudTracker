#include "matcher_functions.h"
#include "structs_and_classes.h"

//#include <H5Cpp.h>
#include <hdf5.h>
#include <iostream>
#include <string>
#include <vector>
//using namespace H5;


void write_to_hdf5_file(CitySnaps& snapsnap, Params& params, int parent_num_clouds, int child_num_clouds) {
    std::string fname = params.path + params.write_filename_base_prefix + get_snapshot_name(snapsnap.snap_num1) + "_" +
                        get_snapshot_name(snapsnap.snap_num2) + params.write_filename_base_suffix;

    std::cout << "=======================\n"
              << "Writing to file: " << fname << "\n"
              << "=======================\n";

    hid_t file_id = H5Fcreate(fname.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hid_t parent_group_id = H5Gcreate(file_id, "parent_group", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    hid_t child_group_id = H5Gcreate(file_id, "child_group", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    for (int i = 0; i < parent_num_clouds; ++i) {
        hid_t cloud_data_parent_id = H5Gcreate(parent_group_id, snapsnap.parent_group.members[i].name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        writeCloudData(cloud_data_parent_id, snapsnap.parent_group.members[i], 0);
        writeGroupDataChildren(cloud_data_parent_id, "children", snapsnap.parent_group.members[i]);
        H5Gclose(cloud_data_parent_id);
    }

    for (int i = 0; i < child_num_clouds; ++i) {
        hid_t cloud_data_child_id = H5Gcreate(child_group_id, snapsnap.child_group.members[i].name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        writeCloudData(cloud_data_child_id, snapsnap.child_group.members[i], 1);
        writeGroupDataParents(cloud_data_child_id, "parents", snapsnap.child_group.members[i]);
        H5Gclose(cloud_data_child_id);
    }

    H5Gclose(parent_group_id);
    H5Gclose(child_group_id);
    H5Fclose(file_id);
}

void writeCloudData(hid_t group_id, const MemberCloud& member, int child_flag) {
    hsize_t dims[1] = {1};
    hid_t dataspace_id = H5Screate_simple(1, dims, nullptr);
    hid_t datatype_id = H5Tcopy(H5T_NATIVE_DOUBLE);
    H5Tset_order(datatype_id, H5T_ORDER_LE);

    if (child_flag==0){
        hid_t dataset_id = H5Dcreate(group_id, "total_mass_parent", datatype_id, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &member.total_mass);
        H5Sclose(dataspace_id);
        H5Tclose(datatype_id);
        H5Dclose(dataset_id);
    }
    else {
        hid_t dataset_id = H5Dcreate(group_id, "total_mass_child", datatype_id, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &member.total_mass);
        H5Sclose(dataspace_id);
        H5Tclose(datatype_id);
        H5Dclose(dataset_id);
    }
    

    
}

void writeGroupDataChildren(hid_t group_id, const std::string& subgroup_name, const MemberCloud& member) {
    hid_t subgroup_id = H5Gcreate(group_id, subgroup_name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    for (const auto& child : member.children) {
        hid_t child_group_id = H5Gcreate(subgroup_id, child.name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

        hsize_t dims[1] = {1};
        hid_t dataspace_id = H5Screate_simple(1, dims, nullptr);
        hid_t datatype_id = H5Tcopy(H5T_NATIVE_DOUBLE);
        H5Tset_order(datatype_id, H5T_ORDER_LE);

        // Write parents_mass_frac_to_child
        hid_t dataset_id = H5Dcreate(child_group_id, "parents_mass_frac_to_child", datatype_id, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &child.parents_mass_frac_to_child);
        H5Dclose(dataset_id);

        // Write childs_mass_frac_from_parent
        dataset_id = H5Dcreate(child_group_id, "childs_mass_frac_from_parent", datatype_id, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &child.childs_mass_frac_from_parent);
        H5Dclose(dataset_id);

        // Write child_total_mass
        dataset_id = H5Dcreate(child_group_id, "child_total_mass", datatype_id, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &child.total_mass);
        H5Dclose(dataset_id);

        H5Sclose(dataspace_id);
        H5Tclose(datatype_id);
        H5Gclose(child_group_id);
    }

    H5Gclose(subgroup_id);
}

void writeGroupDataParents(hid_t group_id, const std::string& subgroup_name, const MemberCloud& member) {
    hid_t subgroup_id = H5Gcreate(group_id, subgroup_name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    for (const auto& parent : member.parents) {
        hid_t child_group_id = H5Gcreate(subgroup_id, parent.name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

        hsize_t dims[1] = {1};
        hid_t dataspace_id = H5Screate_simple(1, dims, nullptr);
        hid_t datatype_id = H5Tcopy(H5T_NATIVE_DOUBLE);
        H5Tset_order(datatype_id, H5T_ORDER_LE);

        // Write parents_mass_frac_to_child
        hid_t dataset_id = H5Dcreate(child_group_id, "parents_mass_frac_to_child", datatype_id, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &parent.parents_mass_frac_to_child);
        H5Dclose(dataset_id);

        // Write childs_mass_frac_from_parent
        dataset_id = H5Dcreate(child_group_id, "childs_mass_frac_from_parent", datatype_id, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &parent.childs_mass_frac_from_parent);
        H5Dclose(dataset_id);

        // Write child_total_mass
        dataset_id = H5Dcreate(child_group_id, "parent_total_mass", datatype_id, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &parent.total_mass);
        H5Dclose(dataset_id);

        H5Sclose(dataspace_id);
        H5Tclose(datatype_id);
        H5Gclose(child_group_id);
    }

    H5Gclose(subgroup_id);
}

/// @brief Function to print a double array
/// @param a : the array to be printed
void print_array_double(std::vector<double> a)
{
        for (int i = 0; i<a.size(); i++)
        {
                std::cout << a[i] << std::endl;
        }
}

/// @brief Function to print an int array
/// @param a : the array to be printed
void print_array_int(std::vector<int> a)
{
        for (int i = 0; i<a.size(); i++)
        {
                std::cout << a[i] << std::endl;
        }
}


/// @brief Function to get the name of the snapshot part of a Cloud
/// @param i : the snapshot number
std::string get_snapshot_name(int i)
{
	std::string name;
	if (i<10) { name = "00"+std::to_string(i);}
	else if (i>=10 & i<100) {name = "0"+std::to_string(i);}
	else { name = std::to_string(i);}
	return name;
}


std::vector<int> read_cloud_data_int(Params& params, int snap_num, const std::string& field_to_read, const std::string& cloud_name) {
    Group_struct* file_arch = create_group(params.file_arch_root);
    file_arch->subgroup = create_group(cloud_name);
    file_arch->subgroup->subgroup = create_group(params.file_arch_cloud_subgroup);
    file_arch->subgroup->subgroup->subgroup = create_group(field_to_read);
    std::string fname = params.path + params.filename_base_prefix + get_snapshot_name(snap_num) + params.filename_base_suffix;

    // Print the file name
    std::cout << "Reading file: " << fname << std::endl;

    hid_t file_id = H5Fopen(fname.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0) {
        std::cerr << "Failed to open file: " << fname << std::endl;
        delete file_arch;
        return {};
    }

    hid_t root_group_id = H5Gopen(file_id, file_arch->name.c_str(), H5P_DEFAULT);
    hid_t last_group_id = get_last_group(file_arch->subgroup, root_group_id);
    H5Gclose(root_group_id);

    hid_t dataset_id = H5Dopen(last_group_id, field_to_read.c_str(), H5P_DEFAULT);
    hid_t dataspace_id = H5Dget_space(dataset_id);

    hsize_t dims[1];
    H5Sget_simple_extent_dims(dataspace_id, dims, nullptr);

    std::vector<int> data(dims[0]);
    hid_t memspace_id = H5Screate_simple(1, dims, nullptr);
    H5Dread(dataset_id, H5T_NATIVE_INT, memspace_id, dataspace_id, H5P_DEFAULT, data.data());

    H5Sclose(memspace_id);
    H5Sclose(dataspace_id);
    H5Dclose(dataset_id);
    H5Gclose(last_group_id);
    H5Fclose(file_id);

    delete file_arch;
    return data;
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

std::vector<double> read_cloud_data_double(Params &params, int snap_num, std::string& field_to_read, std::string& cloud_name)
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


double sum_array(std::vector<double> a)
{
        double sum = 0;
        for (int i=0; i<a.size(); i++)
        {
                sum = sum + a[i];
        }
	return sum;
}



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
		//print_array_double(pIDs);
                
		MemberCloud snapcloud(cloud_name, pIDs, masses);
                if (key.compare("parent")==0) { snapsnap.parent_group.add_member(snapcloud);   //} 
			std::cout << "Added Cloud" << i << " to parent group" << std::endl;}
                else if (key.compare("child")==0) { snapsnap.child_group.add_member(snapcloud);
			std::cout << "Added Cloud" << i << " to child group" << std::endl;}
                else { std::cout << "Don't change the format of the key being passed around." << std::endl;}
        }
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

