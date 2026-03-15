#include "visualization.h"

#include <fstream>
#include <iostream>
#include <cstdlib>

void nnlab::plot(const std::string& path)
{
    std::string command =
        "python code/plot.py " + path;

    int result = system(command.c_str());

    if (result != 0)
    {
        throw std::runtime_error("Python visualization failed");
    }

}
