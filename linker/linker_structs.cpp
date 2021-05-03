#include "linker_structs.h"
#include "linker_functions.h"

Group_struct* create_group(std::string name)
{
        Group_struct *group = new Group_struct();
        group->name = name;
        group->subgroup = NULL;
        return group;
}


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
