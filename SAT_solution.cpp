#include <bits/stdc++.h>
#include "SAT/SAT.h"

int main()
{
    SAT_SOLVER solver;
    std::string filename;
    filename = "DIMACS";
    // cin >> filename;
    std::ifstream input_file(filename);
    if(input_file.is_open())
    {
        solver.read_file(input_file);
        input_file.close();
    }
    solver.solve();
    return 0;
}
