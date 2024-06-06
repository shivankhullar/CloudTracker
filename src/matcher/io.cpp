
#include "../../include/matcher/io.h"
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