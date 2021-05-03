#include "structs_and_classes.h"
#include <string>
#include "matcher_functions.h"

int check_if_exists(std::vector<double> a, double b)
{
        int c = 0;
        for (int i=0; i<a.size(); i++)
        {
                if (a[i]==b) { c = c+1;}
        }
	return c;
}

int get_first_index(std::vector<double> a, double b)
{
        for (int i=0; i<a.size(); i++)
        {
                if (a[i]==b) { return i;}
        }
	return -1;
}


Group_struct* create_group(std::string name)
{
        Group_struct *group = new Group_struct();
        group->name = name;
        group->subgroup = NULL;
        return group;
}

ChildParentInstance::ChildParentInstance(std::string name1, double parents_mass_frac_to_child1, double childs_mass_frac_from_parent1, double total_mass1)
{
	name = name1;
	parents_mass_frac_to_child = parents_mass_frac_to_child1;
	childs_mass_frac_from_parent = childs_mass_frac_from_parent1;
	total_mass = total_mass1;
}


MemberCloud::MemberCloud(std::string name1, std::vector<double> pIDs, std::vector<double> masses1)
{
	name = name1;
	num_children = 0;
	num_parents = 0;
	for (int i=0; i<pIDs.size(); i++)
        {
                if (check_if_exists(particleIDs, pIDs[i])==0)
                {
                        particleIDs.push_back(pIDs[i]);
                        masses.push_back(masses1[i]);
                }

                else {
                      	int index = get_first_index(particleIDs, pIDs[i]);
                        masses[index] = masses[index] + masses1[i];
                }
        }
        total_mass = sum_array(masses);
}

void MemberCloud::add_child(std::string name, double parents_mass_frac_to_child, double childs_mass_frac_from_parent, double total_child_mass)
{
        ChildParentInstance child(name, parents_mass_frac_to_child, childs_mass_frac_from_parent, total_child_mass);
        children.push_back(child);
	num_children = num_children+1;
}

void MemberCloud::add_parent(std::string name, double parents_mass_frac_to_child, double childs_mass_frac_from_parent, double total_parent_mass)
{
       	ChildParentInstance parent(name, parents_mass_frac_to_child, childs_mass_frac_from_parent, total_parent_mass);
        parents.push_back(parent);
	num_parents = num_parents+1;
}


void CloudGroup::add_member(MemberCloud cloud)
{
        members.push_back(cloud);
}


CitySnaps::CitySnaps(int snapnums1, int snapnums2)
{
	snap_num1 = snapnums1;
        snap_num2 = snapnums2;
        snap_name1 = "Snap"+std::to_string(snapnums1);
        snap_name2 = "Snap"+std::to_string(snapnums2);
        parent_group.snap_name = snap_name1;
        child_group.snap_name = snap_name2;
}


