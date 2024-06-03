
#include <H5Cpp.h>
using namespace H5;


/// @brief	Matches clouds between two snapshots.
/// @param	params		Parameters for the code.


/// @brief     Writes the tracked clouds to an hdf5 file.
/// @param snapsnap : The structure containing the matched clouds.
/// @param params : The parameters for the code.
/// @param parent_num_clouds : The number of parent clouds.
/// @param child_num_clouds : The number of child clouds.
void write_to_hdf5_file(CitySnaps snapsnap, Params &params, int parent_num_clouds, int child_num_clouds)
{
        // file name
	H5std_string fname = params.path+params.write_filename_base_prefix + get_snapshot_name(snapsnap.snap_num1) + "_" +
				get_snapshot_name(snapsnap.snap_num2) + params.write_filename_base_suffix;

	std::cout << "=======================" <<std::endl;
	std::cout << "=======================" <<std::endl;
	std::cout << "Writing to file: " << fname << std::endl;
	std::cout << "=======================" <<std::endl;
	std::cout << "=======================" <<std::endl;
	

        // Create the hdf5 file
	hsize_t dimsf[1]; int rank = 1; dimsf[0] = 1;
	H5::H5File file_out(fname, H5F_ACC_TRUNC);
	H5std_string parent_group_name = "parent_group";
	H5std_string child_group_name = "child_group";
	H5std_string parent_group_subgroup_name = "children";
	H5std_string child_group_subgroup_name = "parents";

        // Create the groups
	H5::Group parent_group = file_out.createGroup(parent_group_name);
	H5::Group child_group = file_out.createGroup(child_group_name);	

        // Write the datasets in the parent group
	for (int i=0; i<parent_num_clouds; i++)
	{
		H5::Group cloud_data_parent = parent_group.createGroup(snapsnap.parent_group.members[i].name);

		H5::DataSpace dataspace( rank, dimsf);
                H5::DataType datatype (H5::PredType::IEEE_F64LE);
		H5::DataSet dataset = cloud_data_parent.createDataSet("total_mass_parent", datatype, dataspace );
                double value = snapsnap.parent_group.members[i].total_mass;
                dataset.write(&value, H5::PredType::IEEE_F64LE);
		//std::cout << "Written total_mass dataset" << std::endl;
		H5::Group children = cloud_data_parent.createGroup(parent_group_subgroup_name);
		for (int j=0; j<snapsnap.parent_group.members[i].num_children; j++)
		{
			//H5::Group children = cloud_data_parent.createGroup(parent_group_subgroup_name);
			H5::Group child_data = children.createGroup(snapsnap.parent_group.members[i].children[j].name);
			
			//std::cout << "Created all the relevant groups" << std::endl;
			//H5::DataSpace dataspace( rank, dimsf);
			//H5::DataType datatype (H5T_IEEE_F64LE);

                        // Write the three datasets: parents_mass_frac_to_child, childs_mass_frac_from_parent, child_total_mass
			H5::DataSet dataset_parent2 = child_data.createDataSet("parents_mass_frac_to_child", datatype, dataspace );
			double value2 = snapsnap.parent_group.members[i].children[j].parents_mass_frac_to_child;
			std::cout << "While writing, parents mass frac to child: " << value2 << ", " << snapsnap.parent_group.members[i].children[j].parents_mass_frac_to_child << std::endl;
			dataset_parent2.write(&value2, H5::PredType::IEEE_F64LE);

			H5::DataSet dataset_parent1 = child_data.createDataSet("childs_mass_frac_from_parent", datatype, dataspace );
                        double value1 = snapsnap.parent_group.members[i].children[j].childs_mass_frac_from_parent;
                        dataset_parent1.write(&value1, H5::PredType::IEEE_F64LE);

			H5::DataSet dataset_parent3 = child_data.createDataSet("child_total_mass", datatype, dataspace );
                        double value3 = snapsnap.parent_group.members[i].children[j].total_mass;
                        dataset_parent3.write(&value3, H5::PredType::IEEE_F64LE);
		}
	}

        // Write the datasets in the child group
	for (int i=0; i<child_num_clouds; i++) 
	{
		H5::Group cloud_data_child = child_group.createGroup(snapsnap.child_group.members[i].name);
		H5::DataSpace dataspace( rank, dimsf);
                H5::DataType datatype (H5::PredType::IEEE_F64LE);
		H5::DataSet dataset_child = cloud_data_child.createDataSet("total_mass_child", datatype, dataspace );
                double value = snapsnap.child_group.members[i].total_mass;
                dataset_child.write(&value, H5::PredType::IEEE_F64LE);
		H5::Group parents = cloud_data_child.createGroup(child_group_subgroup_name);

                // Write the datasets in the child group
                for (int j=0; j<snapsnap.child_group.members[i].num_parents; j++)
                {
			//H5::Group parents = cloud_data_child.createGroup(child_group_subgroup_name);
                        H5::Group parents_data = parents.createGroup(snapsnap.child_group.members[i].parents[j].name);
                        //H5::DataSpace dataspace( rank, dimsf);
                        //H5::DataType datatype (H5T_IEEE_F64LE);
                        
                        // Write the three datasets: parents_mass_frac_to_child, childs_mass_frac_from_parent, parent_total_mass
			H5::DataSet dataset_child1 = parents_data.createDataSet("childs_mass_frac_from_parent", datatype, dataspace );
                        double value1 = snapsnap.child_group.members[i].parents[j].childs_mass_frac_from_parent;
			dataset_child1.write(&value1, H5::PredType::IEEE_F64LE);
			
			H5::DataSet dataset_child2 = parents_data.createDataSet("parents_mass_frac_to_child", datatype, dataspace );
                        double value2 = snapsnap.child_group.members[i].parents[j].parents_mass_frac_to_child;
			std::cout << "While writing child_group clouds, parents mass frac to child: " << value2 << ", " << std::endl;
                        dataset_child2.write(&value2, H5::PredType::IEEE_F64LE);
			
			H5::DataSet dataset_child3 = parents_data.createDataSet("parent_total_mass", datatype, dataspace );
                        double value3 = snapsnap.child_group.members[i].parents[j].total_mass;
                        dataset_child3.write(&value3, H5::PredType::IEEE_F64LE);
                }
	}
	
}