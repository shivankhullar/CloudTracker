#include "matcher_functions.h"
#include "structs_and_classes.h"

#include <H5Cpp.h>
using namespace H5;

void write_to_hdf5_file(CitySnaps snapsnap, Params &params, int parent_num_clouds, int child_num_clouds)
{
	H5std_string fname = params.path+params.write_filename_base_prefix + get_snapshot_name(snapsnap.snap_num1) + "_" +
				get_snapshot_name(snapsnap.snap_num2) + params.write_filename_base_suffix;

	std::cout << "=======================" <<std::endl;
	std::cout << "=======================" <<std::endl;
	std::cout << "Writing to file: " << fname << std::endl;
	std::cout << "=======================" <<std::endl;
	std::cout << "=======================" <<std::endl;
	
	hsize_t dimsf[1]; int rank = 1; dimsf[0] = 1;
	H5::H5File file_out(fname, H5F_ACC_TRUNC);
	H5std_string parent_group_name = "parent_group";
	H5std_string child_group_name = "child_group";
	H5std_string parent_group_subgroup_name = "children";
	H5std_string child_group_subgroup_name = "parents";

	H5::Group parent_group = file_out.createGroup(parent_group_name);
	H5::Group child_group = file_out.createGroup(child_group_name);	

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

			H5::DataSet dataset_parent2 = child_data.createDataSet("parents_mass_frac_to_child", datatype, dataspace );
			double value2 = snapsnap.parent_group.members[i].children[j].parents_mass_frac_to_child;
			dataset_parent2.write(&value2, H5::PredType::IEEE_F64LE);

			H5::DataSet dataset_parent1 = child_data.createDataSet("childs_mass_frac_from_parent", datatype, dataspace );
                        double value1 = snapsnap.parent_group.members[i].children[j].childs_mass_frac_from_parent;
                        dataset_parent1.write(&value1, H5::PredType::IEEE_F64LE);

			H5::DataSet dataset_parent3 = child_data.createDataSet("child_total_mass", datatype, dataspace );
                        double value3 = snapsnap.parent_group.members[i].children[j].total_mass;
                        dataset_parent3.write(&value3, H5::PredType::IEEE_F64LE);
		}
	}

	for (int i=0; i<child_num_clouds; i++) 
	{
		H5::Group cloud_data_child = child_group.createGroup(snapsnap.child_group.members[i].name);
		H5::DataSpace dataspace( rank, dimsf);
                H5::DataType datatype (H5::PredType::IEEE_F64LE);
		H5::DataSet dataset_child = cloud_data_child.createDataSet("total_mass_child", datatype, dataspace );
                double value = snapsnap.child_group.members[i].total_mass;
                dataset_child.write(&value, H5::PredType::IEEE_F64LE);
		H5::Group parents = cloud_data_child.createGroup(child_group_subgroup_name);

                for (int j=0; j<snapsnap.child_group.members[i].num_parents; j++)
                {
			//H5::Group parents = cloud_data_child.createGroup(child_group_subgroup_name);
                        H5::Group parents_data = parents.createGroup(snapsnap.child_group.members[i].parents[j].name);
                        //H5::DataSpace dataspace( rank, dimsf);
                        //H5::DataType datatype (H5T_IEEE_F64LE);
                        
			H5::DataSet dataset_child1 = parents_data.createDataSet("childs_mass_frac_from_parent", datatype, dataspace );
                        double value1 = snapsnap.child_group.members[i].parents[j].childs_mass_frac_from_parent;
			dataset_child1.write(&value1, H5::PredType::IEEE_F64LE);
			
			H5::DataSet dataset_child2 = parents_data.createDataSet("parents_mass_frac_to_child", datatype, dataspace );
                        double value2 = snapsnap.child_group.members[i].parents[j].parents_mass_frac_to_child;
                        dataset_child2.write(&value2, H5::PredType::IEEE_F64LE);
			
			H5::DataSet dataset_child3 = parents_data.createDataSet("parent_total_mass", datatype, dataspace );
                        double value3 = snapsnap.child_group.members[i].parents[j].total_mass;
                        dataset_child3.write(&value3, H5::PredType::IEEE_F64LE);
                }
	}
	
}

void print_array_double(std::vector<double> a)
{
        for (int i = 0; i<a.size(); i++)
        {
                std::cout << a[i] << std::endl;
        }
}

void print_array_int(std::vector<int> a)
{
        for (int i = 0; i<a.size(); i++)
        {
                std::cout << a[i] << std::endl;
        }
}


H5::Group get_last_group(Group_struct *subgroup_struct, H5::Group group)
{
        if (subgroup_struct->subgroup!=NULL)
        {
                //std::cout << "Trying for group: " << subgroup_struct->name << std::endl;
                H5::Group grp = group.openGroup(subgroup_struct->name);
                H5::Group grp1 = get_last_group(subgroup_struct->subgroup, grp);
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
        //return grp1;
}

std::string get_snapshot_name(int i)
{
	std::string name;
	if (i<10) { name = "00"+std::to_string(i);}
	else if (i>=10 & i<100) {name = "0"+std::to_string(i);}
	else { name = std::to_string(i);}
	return name;
}


std::vector<int> read_cloud_data_int(Params &params, int snap_num, std::string field_to_read, std::string cloud_name)
{
        Group_struct *file_arch = create_group(params.file_arch_root);
        file_arch->subgroup = create_group(cloud_name);
        file_arch->subgroup->subgroup = create_group(params.file_arch_cloud_subgroup);
        file_arch->subgroup->subgroup->subgroup = create_group(field_to_read);
        H5std_string fname = params.path+params.filename_base_prefix+get_snapshot_name(snap_num)+params.filename_base_suffix;

        H5File file(fname, H5F_ACC_RDONLY);
        H5std_string root = file_arch->name;
        H5::Group grp_root = file.openGroup(root);
        H5::Group last_group = get_last_group(file_arch->subgroup, grp_root);
        H5::DataSet dataset = last_group.openDataSet(field_to_read);
        H5::DataSpace dataspace = dataset.getSpace();

        int rank = dataspace.getSimpleExtentNdims();
        //hsize_t dims_out[rank];
        hsize_t dims_out[1];
        int ndims = dataspace.getSimpleExtentDims(dims_out, NULL);

        std::vector<int> data(dims_out[0]);
        //rarray<double,1> d(dims_out[0]);

        DataSpace memspace(rank, dims_out);
	dataset.read(&data[0], H5::PredType::NATIVE_INT16, memspace, dataspace);
	delete file_arch;
	//std::cout << "Read dataset" << std::endl;
	//print_array_int(data);
        return data;
}


std::vector<double> read_cloud_data_double(Params &params, int snap_num, std::string field_to_read, std::string cloud_name)
{
	Group_struct *file_arch = create_group(params.file_arch_root);
        file_arch->subgroup = create_group(cloud_name);
        file_arch->subgroup->subgroup = create_group(params.file_arch_cloud_subgroup);
        file_arch->subgroup->subgroup->subgroup = create_group(field_to_read);
	H5std_string fname = params.path+params.filename_base_prefix+get_snapshot_name(snap_num)+params.filename_base_suffix;
        
	H5File file(fname, H5F_ACC_RDONLY);
        H5std_string root = file_arch->name;
        H5::Group grp_root = file.openGroup(root);
	H5::Group last_group = get_last_group(file_arch->subgroup, grp_root);
        H5::DataSet dataset = last_group.openDataSet(field_to_read);
        H5::DataSpace dataspace = dataset.getSpace();
	
	int rank = dataspace.getSimpleExtentNdims();
        //hsize_t dims_out[rank];
        hsize_t dims_out[1];
        int ndims = dataspace.getSimpleExtentDims(dims_out, NULL);

	//std::vector<int> data(dims_out[0]);
	std::vector<double> data(dims_out[0]);
	//rarray<double,1> d(dims_out[0]);
	

        DataSpace memspace(rank, dims_out);
        dataset.read(&data[0], H5::PredType::IEEE_F64LE, memspace, dataspace);
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
        if (i<10) { cloud_name = params.cloud_prefix + "00"+ std::to_string(i);}
        else if (i>=10 & i<100) { cloud_name = params.cloud_prefix + "0"+ std::to_string(i);}
        else if (i>=100 & i<1000) { cloud_name = params.cloud_prefix + std::to_string(i);}
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

