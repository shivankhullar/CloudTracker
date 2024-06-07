
#include "io.h"
#include "read_params.h"
#include "structs_and_classes.h"
#include "utilities.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void print_list(std::vector<std::vector<CloudStruct>> &tracked_cloud_list)
{
        std::cout << "=======================" <<std::endl;
        std::cout << "=======================" <<std::endl;
        std::cout << "Tracked Cloud List: " << std::endl;
        std::cout << "Size: " << tracked_cloud_list.size() << "and " << tracked_cloud_list[0].size() << std::endl;
        for (int i=0; i<tracked_cloud_list.size(); i++)
        {
                for (int j=0; j<tracked_cloud_list[i].size(); j++)
                {
                        std::cout << tracked_cloud_list[i][j].final_name << ", ";
                }
                std::cout << std::endl;
        }
        std::cout << std::endl;
}


void write_to_file(std::vector<std::vector<CloudStruct>> &tracked_cloud_list, Params &params, std::string field_name)
{
        std::string file_name = params.path + params.linker_output_filename_prefix + get_snapshot_name(params.first_snap) + "_"
                                + get_snapshot_name(params.last_snap)+ "_" + field_name + ".txt";
        std::cout << "Writing to file:" << file_name <<std::endl;
	std::ofstream file(file_name);
        if (field_name.compare("names")==0)
        {
                for (int i=0; i<tracked_cloud_list.size(); i++)
                {
                        for (int j=0; j<tracked_cloud_list[i].size(); j++)
                        {
                                file << tracked_cloud_list[i][j].final_name << ", ";
                        }
                        file << std::endl;
                }
                file << std::endl;
        }
	if (field_name.compare("masses")==0)
        {
                for (int i=0; i<tracked_cloud_list.size(); i++)
                {
                        for (int j=0; j<tracked_cloud_list[i].size(); j++)
                        {
                                file << tracked_cloud_list[i][j].total_mass << ", ";
                        }
                        file << std::endl;
                }
                file << std::endl;
        }
}
