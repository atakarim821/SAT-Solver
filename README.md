# SAT-Solver

Implementation of Davis Putnam Loveland Logemann (DPLL) algorithm to solve NP complete SAT problems by simple searching over domains and applying unit propagation and pure literals check. Also used MOM's heuristic to choose literal to start search. 
SAT solver takes the input boolean expression in CNF form in the standard [DIMACS](https://www.cs.utexas.edu/users/moore/acl2/manuals/current/manual/index-seo.php/SATLINK____DIMACS#:~:text=DIMACS%20format%20is%20a%20standard,gives%20the%20competitions'%20official%20description.) file format. The output is also a file, stating UNSAT (if formula is not satisfiable) or SAT (if formula is satisfiable) followed to assignment to each variable which can satisfy the given expression.

Finally I used the SAT solver to solve the N-Queen problem.

