

#include <hdf5.h>
#include "io.h"


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
