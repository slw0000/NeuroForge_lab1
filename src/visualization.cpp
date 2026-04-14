#include "../include/visualization.h"

#include <fstream>
#include <iostream>
#include <cstdlib>

void nnlab::plot(const std::string& path)
{
    std::string command =
        "python scripts/plot.py " + path;

    int result = system(command.c_str());

    if (result != 0)
    {
        throw std::runtime_error("Python visualization failed");
    }

}
