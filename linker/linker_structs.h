#ifndef LINKER_STRUCTS_H
#define LINKER_STRUCTS_H

#include <string>

struct Params{
	int last_snap, first_snap;
        std::string cloud_prefix, path, filename_base_prefix, filename_base_suffix, dat_filename_base_prefix, dat_filename_base_suffix;
        std::string write_filename_base_prefix, write_filename_base_suffix, linker_output_filename_prefix;
        std::string file_arch_root, file_arch_cloud_subgroup, file_arch_masses_field, file_arch_pIDs_field;
        int particle_lower_limit;
};

struct Group_struct{
        std::string name;
        Group_struct *subgroup;
};
Group_struct* create_group(std::string name);

struct CloudStruct{
        std::string name;
        int num, snap; double total_mass;
        std::string final_name;
        int end_of_gen;
        int flag;                       // 0 if not added yet. 1 if added.
};
CloudStruct initialize_cloud(int snap_num, std::string cloud_name, Params &params);

#endif
