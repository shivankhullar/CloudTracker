#ifndef READ_PARAMS_H
#define READ_PARAMS_H

#include "linker_structs.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


void trim(std::string& s);
void printParams(const Params& params);
bool parseParams(const std::string& filename, Params& params, const std::string& name, const std::string& sim_name);

#endif
