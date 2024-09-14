#include "backward_chain.h"
#include "forward_chain.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

vector<string> conclusion_list;

vector<int> rule_list;
vector<int> clause_variable_list;

map<string, string> variable_list;
map<string, string> global_variable_list;


void initialize_knowledge_base();

int main(){

    /*
    Identify the Goal variable (the variable whose value needs to be determined)
 
    Call Process ( Goal variable)

    After the program is complete, check if the derived global variable list contains, Goal variable and it is not null. 

        If it does, 
            that is the answer of processing the Back ward chaining. 
            Print the value of the variable and pass it to Forward chaining program. 
        
        If it is null, 
            print a comment that Goal cannot be determined.
        
    */ 

    return 0;
}
