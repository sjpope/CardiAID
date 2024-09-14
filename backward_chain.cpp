#include <backward_chain.h>

// Search for the variable in the conclusion list
int search_con(const string& variable);

// Convert Rule # Ri to clause number, Ci
int rule_to_clause(int Ri) {

    /*
    This function will convert Rule # Ri to clause number, Ci, using the following formula. 
    If the rule numbers are sequenced like 1,2,3,4,5, ……), formula is:
        
    CLAUSE NUMBER (Ci) = 4* (RULE NUMBER (Ri) - 1) + 1

    It has been assumed that four slots have been assigned for each rule in the Clause Variable list. 
    If other than four has been assigned replace 4 by that number.
    */
   
    return 4 * (Ri - 1) + 1;
}

// Update the Variable List (VL) based on the clause number
void update_VL(int Ci){
        

}

// Validate Rule # Ri
void validate_Ri(int Ri, string& conclusion){
        // Check conditions based on rule Ri
        // If the conditions are met, update the conclusion
}

void process(const string& variable);

void initialize_knowledge_base();