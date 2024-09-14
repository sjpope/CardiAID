#ifndef BACKWARD_CHAIN_H
#define BACKWARD_CHAIN_H

#include <string>

using namespace std;

void process(const string& variable);
int search_con(const string& variable);
int rule_to_clause(int Ri);
void update_VL(int Ci);
void validate_Ri(int Ri, string& conclusion);

#endif