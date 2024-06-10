/// @file       structs_and_classes.cpp 
/// @brief      Contains the structs and classes used in the linker program.
///              
/// @author     Shivan Khullar
/// @date       June 2024



#include "../../../include/linker/structs_and_classes.h"
#include "../../../include/linker/linker_functions.h"
#include "../../../include/linker/utilities.h"



/// @brief          Creates a group struct with the given name.   
/// @param name     The name of the group.
/// @return         A pointer to the created group struct.
Group_struct* create_group(std::string name)
{
        Group_struct *group = new Group_struct();
        group->name = name;
        group->subgroup = NULL;
        return group;
}



/// @brief              Initializes a cloud struct with the given snapshot number and cloud name.
/// @param snap_num     The snapshot number of the cloud.
/// @param cloud_name   The name of the cloud.
/// @param params       The parameters struct.
/// @return             The initialized cloud struct.
CloudStruct initialize_cloud(int snap_num, std::string cloud_name, Params &params)
{
        //std::string cloud_name = get_cloud_name(cloud_num);
        std::string snap_name = get_snapshot_name(snap_num);
        CloudStruct cloud; cloud.name = cloud_name;
        //cloud.num = cloud_num;
        cloud.snap = snap_num;
        cloud.final_name = cloud_name+"Snap"+snap_name;
        //if (cloud.flag == 1) {return cloud;}
        //else {cloud.flag = 0;}
        cloud.flag = 1;
        cloud.end_of_gen = 0;
        return cloud;
}
