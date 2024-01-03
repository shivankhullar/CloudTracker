//#ifndef READ_PARAMS_H
//#define READ_PARAMS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "structs_and_classes.h"
#include "matcher_functions.h"

void trim(std::string& s);
void printParams(const Params& params);
bool parseParams(const std::string& filename, Params& params, const std::string& name);
