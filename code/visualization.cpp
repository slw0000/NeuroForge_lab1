#include "visualization.h"

#include <fstream>
#include <iostream>
#include <cstdlib>

namespace viz{
    /*
    void write_csv(
        const std::string& path,
        const std::vector<std::vector<double>>& data)
    {
        std::ofstream file(path,  std::ios::out | std::ios::trunc);

        if (!file)
        {
            std::cerr << "Cannot open file: " << path << std::endl;
            return;
        }

        file << "x,y,label\n";

        for (const auto& row : data)
        {
            if (row.size() < 3)
                continue;

            file << row[0] << ","
                << row[1] << ","
                << row[2] << "\n";
        }

        file.close();
    }*/


    void plot(const std::string& path)
    {
        std::string command =
            "python code/plot.py " + path;

        int result = system(command.c_str());

        if (result != 0)
        {
            std::cerr << "Python visualization failed\n";
        }
    }
}