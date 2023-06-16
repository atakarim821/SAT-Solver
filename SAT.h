#pragma once
#include <vector>

enum Answer
{
    Satisfied,
    Unsatisfied, 
    Normal, 
    Completed
};

class CNF
{
    public:
        std::vector<int> Assign; 
        std::vector<std::vector<int>> Clause;
        std::vector<int> frequency; 
        std::vector<int> variable_sign;
        
        CNF();
        CNF(const CNF&);
};

class SAT_SOLVER
{
    private:
        CNF cnf;
        int num_of_vairable;
        int num_of_clause;
        int Unit_Propagation(CNF&);
        // Function to do unit propagation to unit clause.
        int DPLL(CNF);
        // This is the Algorithm.
        int update(CNF&, int); 
         // This function will make changes to CNF formula whenever there is branching or unit clause/ Pure literals is found.
        void write_result(CNF&, int); 
        // File in which we have to write the assignment to all variable if satisfied.
    public:
        SAT_SOLVER();
        void read_file(std::ifstream&);
        // Reading DIMACS format file and initializing everything.
        void solve();
        //This function calls DPLL function.
};