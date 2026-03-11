#include "visualization.h"
#include <vector>



int main()
    {
        std::vector<std::vector<double>> data = {
        {1.0, 2.0, 0},
        {2.0, 3.5, 0},
        {5.0, 1.2, 1},
        {6.0, 2.0, 1},
        {6.0, 1.0, 3},
        {7.0, 2.0, 3},
        {9.0, 2.0, 3},
        {8.0, 3.0, 2}
        };

        viz::write_csv("data/answerAI.csv", data);

        viz::plot("data/answerAI.csv");

        return 0;
    }
