#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int search_con(const string& variable);
int rule_to_clause(int Ri);
void update_VL(int Ci);
void validate_Ri(int Ri, string& conclusion);
void process(const string& variable);

void initialize_knowledge_base();


vector<string> conclusion_list;

vector<int> rule_list;
vector<int> clause_variable_list;

map<string, string> variable_list;
map<string, string> global_variable_list;



int rule_to_clause(int Ri) {

    /*
    This function will convert Rule # Ri to clause number, Ci, using the following formula. If the rule numbers are sequenced like 1,2,3,4,5, ……), formula is:
        
        CLAUSE NUMBER (Ci) = 4* (RULE NUMBER (Ri) - 1) + 1

    If the rule numbers are sequenced like 10,20,30,40,50, ……), formula is:

        CLAUSE NUMBER = 4* (RULE NUMBER  / 10 - 1) + 1

    It has been assumed that four slots have been assigned for each rule in the Clause Variable list. If other than four has been assigned replace 4 by that number.
    */

   // We expect to sequence rule numbers like 1,2,3...
    return 4 * (Ri - 1) + 1;
}