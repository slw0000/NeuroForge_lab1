#include "visualization.h"

#include <fstream>
#include <iostream>


void nnlab::plot(const std::string& path)
{
    std::string command =
        "python3.12 code/plot.py " + path;

    int result = system(command.c_str());

    if (result != 0)
    {
        std::cerr << "Python visualization failed\n";
    }
}
