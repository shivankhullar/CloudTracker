/// @file       utilities.cpp 
/// @brief      Contains some generic utilities like finding the number 
///             of clouds in a snapshot, getting the name of the snapshot
///             or getting the name of the cloud if you have a number.
///             Also contains functions for checking two arrays and seeing
///             whether they have something in common, and getting the number
///             of particles in a cloud.
///              
/// @author     Shivan Khullar
/// @date       June 2024



#include "../../../include/linker/utilities.h"
#include "../../../include/linker/io.h"
#include "../../../include/linker/read_params.h"



/// @brief      Function to obtain the snapshot name by adding 
///             required number of zeros
/// @param i    The snapshot number
/// @return 
std::string get_snapshot_name(int i)
{
        std::string name;
        if (i<10) { name = "00"+std::to_string(i);}
        else if (i>=10 & i<100) {name = "0"+std::to_string(i);}
        else { name = std::to_string(i);}
        return name;
}



/// @brief              Function to get the cloud name by adding 
///                     required number of zeros
/// @param i            The cloud number
/// @param params       The parameters struct
/// @return             The cloud name
std::string get_cloud_name(int i, Params &params)
{
        std::string cloud_name;
        // customize this part below based on how many clouds there are.
        if (i<10) { cloud_name = params.cloud_prefix + "000"+ std::to_string(i);}
        else if (i>=10 & i<100) { cloud_name = params.cloud_prefix + "00"+ std::to_string(i);}
        else if (i>=100 & i<1000) { cloud_name = params.cloud_prefix + "0"+std::to_string(i);}
        else { cloud_name = params.cloud_prefix + std::to_string(i);}
        // std::cout << "Make sure to modify the get_cloud_name() function correctly." << std::endl;
        return cloud_name;
}



/// @brief              Function to find the number of clouds in a snapshot
/// @param snap_num     The snapshot number
/// @param params       The parameters struct
/// @return             The number of clouds in the snapshot
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



/// @brief              Function to check if a number exists in a vector
/// @param a            The vector
/// @param b            The number to check
/// @return             The number of times the number exists in the vector
int check_if_exists(std::vector<double> a, double b)
{
        int c = 0;
        for (int i=0; i<a.size(); i++)
        {
                if (a[i]==b) { c = c+1;}
        }
	return c;
}



/// @brief              Function to get the number of particles in a cloud
/// @param params       The parameters struct
/// @param snap_num     The snapshot number
/// @param cloud_name   The cloud name
/// @return             The number of particles in the cloud
int get_num_particles(Params &params, int snap_num, std::string cloud_name)
{
        std::vector<double> pIDs = read_cloud_data_double(params, snap_num, params.file_arch_masses_field, cloud_name);
        std::vector<double> particleIDs;
        for (int i=0; i<pIDs.size(); i++)
        {
                if (check_if_exists(particleIDs, pIDs[i])==0)
                {
                        particleIDs.push_back(pIDs[i]);
                }
        }
	return particleIDs.size();
}
