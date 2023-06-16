#include <iostream>
#include "SAT/SAT.h"
#include <fstream>
#include <sstream>
using namespace std;

int var(int r,int c, int N)
{
    return (r-1)*N + c-1;
}

void read_files(std::ifstream& file, int N)
{
    string line;
    std::getline(file, line);
    while(std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string word;
        while(ss>>word)
        {
            int literal = stoi(word);
            if(literal==0) break;
            if(literal<0){
                cout << "_ ";
            }
            else if(literal>0)
            {
                cout << "Q ";
            }
            if(literal%N==0) cout << "\n";
        }
    }
}

int main()
{
    int N; 
    N = 12;
    vector<vector<int>> clause;
    for(int r = 1; r<=N; r++)
    {
        vector<int> temp;
        for(int c = 1; c<=N; c++) temp.push_back(var(r,c,N)+1);
        clause.push_back(temp);
    }
    for(int v = 0; v<N*N; v++)
    {
        int row = v/N + 1; 
        int col = v%N + 1;
        // Handling Horizontal Attack
        for(int c = col+1; c<=N; c++){
            vector<int> temp{-1*var(row, col, N)-1, -1*var(row, c, N)-1};
            clause.push_back(temp);
        }
        // Handling Vertical Attack
        for(int r = row+1; r<=N; r++){
            vector<int> temp{-1*var(row, col, N)-1, -1*var(r, col, N)-1};
            clause.push_back(temp);
        }
        int right, left , down;
        right = N-col;
        left = col-1;
        down = N-row;
        // Handling Diagonal Attack
        for(int i = 1; i<=min(right, down); i++){
            vector<int> temp{-1*var(row, col, N)-1, -1*var(row+i, col+i, N)-1};
            clause.push_back(temp);
        }
        for(int i = 1; i<=min(left, down); i++){
            vector<int> temp{-1*var(row, col, N)-1, -1*var(row+i, col-i, N)-1};
            clause.push_back(temp);
        }
    }
    std::ofstream input("CNF_FILE" , std::ofstream::out | std::ofstream::trunc);
    if(input.is_open())
    {
        input << "p cnf " << N*N << " " << clause.size() << "\n";
        for(int i = 0 ; i<clause.size(); i++){
            for(auto literal : clause[i]){
                input << literal <<" ";
            }
            input << 0 << "\n";
        }
        input.close();
    }

    // USING SAT SOLVER TO SOLVE THE N QUEEN PROBLEM
    SAT_SOLVER solver;
    std::string filename;
    filename = "CNF_FILE";
    std::ifstream input_file(filename);
    if(input_file.is_open())
    {
        solver.read_file(input_file);
        input_file.close();
    }

    // Read the DIMACS.output file.
    solver.solve();

    filename = "DIMACS.output";
    std::ifstream output(filename);
    if(output.is_open())
    {
        read_files(output, N);
    }
    
    return 0;
}