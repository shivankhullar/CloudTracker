#ifndef STRUCTS_AND_CLASSES_H
#define STRUCTS_AND_CLASSES_H

#include <iostream>
#include <string>
//#include <rarray>
#include <vector>

#include <H5Cpp.h>
using namespace H5;

int check_if_exists(std::vector<double> a, double b);
int get_first_index(std::vector<double> a, double b);


struct Group_struct{
        std::string name;
        Group_struct *subgroup;
};
Group_struct* create_group(std::string name);


struct Params{
        int last_snap, first_snap;
        std::string cloud_prefix, path, filename_base_prefix, filename_base_suffix, dat_filename_base_prefix, dat_filename_base_suffix;
	std::string write_filename_base_prefix, write_filename_base_suffix;
	std::string file_arch_root, file_arch_cloud_subgroup, file_arch_masses_field, file_arch_pIDs_field; 
	int particle_lower_limit;
};

class ChildParentInstance{
public:
       	std::string name;
	double childs_mass_frac_from_parent;
	double parents_mass_frac_to_child;
	double total_mass;
        ChildParentInstance(std::string name1, double parents_mass_frac_to_child1, double childs_mass_frac_from_parent1, double total_mass1);
};

class MemberCloud{
public:
        std::string name;
        std::vector<double> particleIDs;
       	std::vector<double> masses;
	std::vector<double> dummy_pIDs;
	std::vector<double> dummy_masses;
        double total_mass;
        std::vector<ChildParentInstance> children;
        std::vector<ChildParentInstance> parents;
	int num_children, num_parents;
	MemberCloud(std::string name1, std::vector<double> pIDs, std::vector<double> masses1);
	void add_child(std::string name, double parents_mass_frac_to_child, double childs_mass_frac_from_parent, double total_child_mass);
        void add_parent(std::string name, double parents_mass_frac_to_child, double childs_mass_frac_from_parent, double total_parent_mass);
};

struct CloudGroup{
        std::string snap_name;
        int snap_num;
        std::vector<MemberCloud> members;
        void add_member(MemberCloud cloud);
};

class CitySnaps{
public:
        std::string snap_name1, snap_name2;
        int snap_num1, snap_num2;
        CloudGroup parent_group, child_group;
        CitySnaps(int snapnums1, int snapnums2);
};


#endif
