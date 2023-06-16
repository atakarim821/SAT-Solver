#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "SAT.h"


CNF::CNF() {}

CNF::CNF(const CNF&f)
{
    Assign = f.Assign;
    Clause = f.Clause;
    frequency = f.frequency;
    variable_sign = f.variable_sign;
}

SAT_SOLVER::SAT_SOLVER() {}

void SAT_SOLVER::read_file(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line))
    {
        if(line[0]=='c') continue;
        else if(line[0]=='p')
        {
            std::stringstream ss(line);  
            std::string word;
            ss >> word;
            ss >> word;
            ss >> word;
            num_of_vairable = stoi(word);
            ss >> word;
            num_of_clause = stoi(word);
            break;
        }
    }
    cnf.Assign.clear();
    cnf.Assign.resize(num_of_vairable+1, 1);
    // Assign Variable stores the assigned value to each variable if formula is satisfiable. Initialize it with one.
    cnf.Clause.clear();
    cnf.Clause.resize(num_of_clause);
    // This is actually formula vector.
    cnf.frequency.clear();
    cnf.frequency.resize(num_of_vairable+1, 0);
    // frequecy vector will store occurence of each variable.
    cnf.variable_sign.clear();
    cnf.variable_sign.resize(num_of_vairable+1, 0);
    // It is to keep track, if a particular variable occur in normal form or negated form more often.
    // Get it clarified more later.

    // Now fill the clause vector.
    int i = 0;
    while(getline(file, line))
    {
        if(line[0]=='c' || line[0]=='p') continue;
        else
        {
            std::stringstream ss(line);  
            std::string word;
            while(ss >> word)
            {
                int literal = stoi(word);
                if(literal==0) break;
                cnf.Clause[i].push_back(literal);
                cnf.frequency[abs(literal)]++;
                if(literal>0) cnf.variable_sign[literal]++;
                else cnf.variable_sign[abs(literal)]--;
            }
            i++;
        }
    }
    // After the end of this function What I have achieved is following :-
    /*
    1) I have assigned the size to vector like assign, clause, frequency, variable_size of class CNF.
    2) I have completely assigned the clause vector which is nothing but the boolean expression in CNF form
    3) updated the frequency, variable_size vector.
    */
}

int SAT_SOLVER::Unit_Propagation(CNF& f)
{
   bool unit_clause_found = false;
   if(f.Clause.size() == 0)
   {
    return Answer::Satisfied;
   } 
    do
    {
        unit_clause_found = false;
        for(int i = 0; i<f.Clause.size(); i++)
        {
            if(f.Clause[i].size()==1)
            {
                unit_clause_found = true;
                f.Assign[abs(f.Clause[i][0])] = (f.Clause[i][0]>0)?1:0;
                f.frequency[abs(f.Clause[i][0])] = -1;
                int ans = update(f, f.Clause[i][0]);

                if(ans==Answer::Satisfied || ans==Answer::Unsatisfied) return ans;
                break;
            }
            else if(f.Clause[i].size()==0) return Answer::Unsatisfied;
        }
    }while (unit_clause_found);
    return Answer::Normal;
}

int SAT_SOLVER::update(CNF& f, int literal) // here if literal<0 then variable is in negated form. Or else it is in normal form
{ 
    for(int i=0; i<f.Clause.size(); i++)
    {
        for(int j = 0; j<f.Clause[i].size(); j++)
        {
            if(f.Clause[i][j]==literal)
            {
                f.Clause.erase(f.Clause.begin()+i);
                i--;
                if(f.Clause.size()==0) return Answer::Satisfied;
                break;
            }
            else if(f.Clause[i][j]+literal==0)
            {
                f.Clause[i].erase(f.Clause[i].begin()+j);
                j--;
                if(f.Clause[i].size()==0) return Answer::Unsatisfied;
                break;
            }
        }
    }
    return Answer::Normal;
}

int SAT_SOLVER::DPLL(CNF f)
{
    int ans = Unit_Propagation(f);
    if(ans == Answer::Satisfied) {write_result(f, ans); return Answer::Completed;}

    else if(ans == Answer::Unsatisfied) return Answer::Normal;
    // Here I am using MOM's Heuristic to branch over the most constrained variable.

    int i = distance(f.frequency.begin(), max_element(f.frequency.begin()+1, f.frequency.end()));
    for(int j = 1; j>=0 ;j--) // Assign j = 1 for +ve. Assign j= 0 for -ve.
    {
        CNF copied_formula = f;
        copied_formula.frequency[i] = -1;
        if(copied_formula.variable_sign[i]>=0) // means variable 'i' occur more often in normal form so branch with it's +ve first.
        {
            copied_formula.Assign[i] = j;
            int updated_result = update(copied_formula, i*j+(-1*i*(1-j)));
            if(updated_result==Answer::Satisfied)
            {
                write_result(copied_formula, updated_result);
                return Answer::Completed;
            }

            else if(updated_result==Answer::Unsatisfied) continue;
        }
        else
        {
            copied_formula.Assign[i] = 1-j;
            int updated_result = update(copied_formula,i*(1-j)+(-1*i*(j)) );
            if(updated_result==Answer::Satisfied)
            {
                write_result(copied_formula, updated_result);
                return Answer::Completed;
            }

            else if(updated_result==Answer::Unsatisfied) continue;
        }
        int DPLL_new = DPLL(copied_formula);
        if(DPLL_new==Answer::Completed) return DPLL_new;
    }
    return Answer::Normal;
}   

void SAT_SOLVER::write_result(CNF& f, int result)
{
    std::ofstream output_file("DIMACS.output", std::ofstream::out | std::ofstream::trunc);
    if(output_file.is_open())
    {
        if(result==Answer::Unsatisfied) 
        {
            output_file << "UNSAT\n";
            output_file.close();
        }
        else if(result==Answer::Satisfied)
        {
            output_file << "SAT\n";
            for(int i = 1 ; i<f.Assign.size(); i++) output_file << f.Assign[i]*i+(-1*i)*(1-f.Assign[i]) << " ";
            output_file << 0;
            output_file.close();
        }
    }
}

void SAT_SOLVER::solve()
{
    int answer = DPLL(cnf);
    if(answer==Answer::Normal) write_result(cnf, Answer::Unsatisfied);
}
