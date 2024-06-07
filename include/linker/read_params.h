#ifndef READ_PARAMS_H
#define READ_PARAMS_H

#include "structs_and_classes.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


void trim(std::string& s);
void printParams(const Params& params);
bool parseParams(const std::string& filename, Params& params, const std::string& name, const std::string& sim_name);

#endif
