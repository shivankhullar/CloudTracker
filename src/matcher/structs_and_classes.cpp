/// @file       matcher.cpp 
/// @brief 	    This file contains the utility functions for the matcher application.

/// @author     Shivan Khullar
/// @date       June 2024

#include <string>
//#include "../../include/matcher/structs_and_classes.h"
#include "../../include/matcher/structs_and_classes.h"
//#include "../../include/matcher/utilities.h"
#include "../../include/matcher/utilities.h"
//#include "../../include/matcher/matcher_functions.h"
#include "../../include/matcher/matcher_functions.h"



/// @brief      This is a class to create a group structure 
/// @param      name: The name of the group to be created
/// @return 
Group_struct* create_group(std::string name)
{
        Group_struct *group = new Group_struct();
        group->name = name;
        group->subgroup = NULL;
        return group;
}



/// @brief      This is a class to create a child parent instance
/// @param      name_arg: The name of the child parent instance
/// @param      parents_mass_frac_to_child_arg: The mass fraction from parent to child
/// @param      childs_mass_frac_from_parent_arg: The mass fraction from child to parent
/// @param      total_mass_arg: The total mass of the child parent instance
/// @return
ChildParentInstance::ChildParentInstance(std::string name_arg, double parents_mass_frac_to_child_arg, double childs_mass_frac_from_parent_arg, double total_mass_arg)
{
    name = name_arg;
    parents_mass_frac_to_child = parents_mass_frac_to_child_arg;
    childs_mass_frac_from_parent = childs_mass_frac_from_parent_arg;
    total_mass = total_mass_arg;
}



/// @brief      This is a class to create a member cloud
/// @param      name_arg: The name of the member cloud
/// @param      pIDs: The particle IDs of the member cloud
/// @param      pIDgen: The particle ID generator of the member cloud
/// @param      masses_arg: The masses of the member cloud
/// @return
MemberCloud::MemberCloud(std::string name_arg, std::vector<double> pIDs, std::vector<double> pIDgen, std::vector<double> masses_arg)
{
    // Initialize the member cloud
    name = name_arg;
    num_children = 0;
    num_parents = 0;
    for (int i=0; i<pIDs.size(); i++)
    {
        if (check_if_value_exists_in_array(particleIDs, pIDs[i])==0)
        {
            if (check_if_value_exists_in_array(particleIDgens, pIDgen[i])==0)
            {
                particleIDs.push_back(pIDs[i]);
                masses.push_back(masses_arg[i]);
            }
            else
            {
                int index = get_first_index(particleIDs, pIDs[i]);
                masses[index] = masses[index] + masses_arg[i];
            }
            //particleIDs.push_back(pIDs[i]);
            //masses.push_back(masses_arg[i]);
        }

        //else {
        //    int index = get_first_index(particleIDs, pIDs[i]);
        //    masses[index] = masses[index] + masses_arg[i];
        //}
    }
    total_mass = sum_array(masses);
}



/// @brief      This is a class to add a child to a member cloud
/// @param      name: The name of the child
/// @param      parents_mass_frac_to_child: The mass fraction from parent to child
/// @param      childs_mass_frac_from_parent: The mass fraction from child to parent
/// @param      total_child_mass: The total mass of the child
/// @return
void MemberCloud::add_child(std::string name, double parents_mass_frac_to_child, double childs_mass_frac_from_parent, double total_child_mass)
{
    ChildParentInstance child(name, parents_mass_frac_to_child, childs_mass_frac_from_parent, total_child_mass);
    children.push_back(child);
    num_children = num_children+1;
}



/// @brief      This is a class to add a parent to a member cloud
/// @param      name: The name of the parent
/// @param      parents_mass_frac_to_child: The mass fraction from parent to child
/// @param      childs_mass_frac_from_parent: The mass fraction from child to parent
/// @param      total_parent_mass: The total mass of the parent
/// @return
void MemberCloud::add_parent(std::string name, double parents_mass_frac_to_child, double childs_mass_frac_from_parent, double total_parent_mass)
{
    ChildParentInstance parent(name, parents_mass_frac_to_child, childs_mass_frac_from_parent, total_parent_mass);
    parents.push_back(parent);
    num_parents = num_parents+1;
}



/// @brief      This is a class to add a member cloud to a cloud group
/// @param      cloud: The member cloud to be added
/// @return
void CloudGroup::add_member(MemberCloud cloud)
{
    members.push_back(cloud);
}



/// @brief      This is a class to create a city snaps instance. A city contains member clouds from two snapshots being matched/compared.
/// @param      snapnums1: The first snapshot number
/// @param      snapnums2: The second snapshot number
/// @return
CitySnaps::CitySnaps(int snapnums1, int snapnums2)
{
    snap_num1 = snapnums1;
    snap_num2 = snapnums2;
    snap_name1 = "Snap"+std::to_string(snapnums1);
    snap_name2 = "Snap"+std::to_string(snapnums2);
    parent_group.snap_name = snap_name1;
    child_group.snap_name = snap_name2;
}



//int main(){};