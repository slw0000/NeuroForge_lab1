#pragma once
#include <string>
#include <vector>

namespace viz{

    void write_csv(
        const std::string& path,
        const std::vector<std::vector<double>>& data
    );


    void plot(const std::string& path);
}