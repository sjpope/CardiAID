#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <queue>

// g++ -o final.exe final.cpp

using namespace std;

struct Variable {
    string name;
    string value;
    bool instantiated;

    Variable(string n) : name(n), value(""), instantiated(false) {}
};

// Structure to represent a rule
struct Rule {
    int number;
    vector<pair<string, string>> conditions; // (variable, expected value)
    pair<string, string> conclusion; // (variable, value)

    Rule(int num, vector<pair<string, string>> conds, pair<string, string> concl)
        : number(num), conditions(conds), conclusion(concl) {}
};

// Global variables and data structures
vector<Variable> variableList;
map<string, Variable*> variableMap;
vector<Rule> diagnosisRuleList;
vector<Rule> treatmentRuleList;
map<string, vector<int>> conclusionMap; // Maps conclusion variable to rule numbers

// Global lists and variable definitions
unordered_map<string, string> derivedGlobalVariables; // Stores derived conclusions
unordered_map<string, vector<string>> clauseVariableList; // Clause variable list for backward chaining
unordered_map<string, string> treatmentRules; // Treatment rules for forward chaining

void initialize_knowledge_base();

// Backward Chaining
string diagnosis();
void processBC(string variable);
bool searchConBC(string variable);
int ruleToClauseBC(int ruleNumber);
bool validateRi(const Rule& rule);

// Used for both algorithms
void updateVL(const vector<pair<string, string>>& conditions);

// Forward Chaining 
void treatment(string diagnosis);
void processFC(string diagnosis);
int clauseToRule();
int searchCVL(double variable);

int main() {
    // Populate the rule base for diagnosis and treatment
    initialize_knowledge_base();

    cout << "Welcome to the Cardiovascular Expert System!" << endl;

    // Begin Diagnosis using Backward Chaining
    cout << "Starting Diagnosis..." << endl;
    string result = diagnosis();

    if (result == "Inconclusive") {
        cout << "Diagnosis: Inconclusive" << endl;
    } else {
        cout << "Diagnosis: " << result << endl;
    }
    // Begin Treatment using Forward Chaining


    return 0;
}

// Populate diagnosis and treatment rules
void initialize_knowledge_base() {
    // Diagnosis rules (Backward Chaining)
    // diagnosisRuleList.push_back(Rule(1, {{"chest_pain", "yes"}}, {"go_to_node", "2"}));
    // diagnosisRuleList.push_back(Rule(2, {{"chest_pain", "no"}}, {"go_to_node", "3"}));
    // diagnosisRuleList.push_back(Rule(3, {{"shortness_of_breath", "yes"}}, {"go_to_node", "4"}));
    // diagnosisRuleList.push_back(Rule(4, {{"shortness_of_breath", "no"}}, {"go_to_node", "5"}));
    // diagnosisRuleList.push_back(Rule(5, {{"dizziness_or_fainting", "yes"}}, {"go_to_node", "6"}));
    // diagnosisRuleList.push_back(Rule(6, {{"dizziness_or_fainting", "no"}}, {"go_to_node", "7"}));
    // diagnosisRuleList.push_back(Rule(7, {{"heart_palpitations", "yes"}}, {"diagnosis", "Possible Heart Arrhythmias"}));
    // diagnosisRuleList.push_back(Rule(8, {{"heart_palpitations", "no"}}, {"go_to_node", "10"}));
    // diagnosisRuleList.push_back(Rule(9, {{"neck_or_jaw_pain", "yes"}}, {"diagnosis", "Possible Coronary Artery Disease"}));
    // diagnosisRuleList.push_back(Rule(10, {{"neck_or_jaw_pain", "no"}}, {"diagnosis", "Inconclusive"}));
    // diagnosisRuleList.push_back(Rule(11, {{"fainting_spells", "yes"}}, {"diagnosis", "Possible Heart Valve Disease"}));
    // diagnosisRuleList.push_back(Rule(12, {{"fainting_spells", "no"}}, {"go_to_node", "8"}));
    // diagnosisRuleList.push_back(Rule(13, {{"swelling_in_legs_or_fatigue", "yes"}}, {"go_to_node", "9"}));
    // diagnosisRuleList.push_back(Rule(14, {{"swelling_in_legs_or_fatigue", "no"}}, {"diagnosis", "Inconclusive"}));
    // diagnosisRuleList.push_back(Rule(15, {{"chest_fluttering", "yes"}}, {"diagnosis", "Possible Heart Arrhythmias"}));
    // diagnosisRuleList.push_back(Rule(16, {{"chest_fluttering", "no"}}, {"diagnosis", "Inconclusive"}));
    // diagnosisRuleList.push_back(Rule(17, {{"fatigue_quickly_on_exertion", "yes"}}, {"diagnosis", "Consider Cardiomyopathy or Congenital Heart Defects"}));
    // diagnosisRuleList.push_back(Rule(18, {{"fatigue_quickly_on_exertion", "no"}}, {"diagnosis", "Inconclusive"}));
    // diagnosisRuleList.push_back(Rule(19, {{"swelling_or_cyanosis", "yes"}}, {"diagnosis", "Swelling: Consider Cardiomyopathy, Cyanosis: Consider Congenital Heart Defects"}));
    // diagnosisRuleList.push_back(Rule(20, {{"swelling_or_cyanosis", "no"}}, {"diagnosis", "Inconclusive"}));
    
    // Rule 1: Possible Heart Arrhythmias (Palpitations)
    diagnosisRuleList.push_back(
        Rule(1, 
            {{"Chest pain", "Yes"}, {"Shortness of breath", "Yes"}, {"Heart palpitations", "Yes"}},
            {"Diagnosis", "Possible Heart Arrhythmias"}
        )
    );

    // Rule 2: Possible Coronary Artery Disease
    diagnosisRuleList.push_back(
        Rule(2, 
            {{"Chest pain", "Yes"}, {"Shortness of breath", "No"}, {"Pain in neck or jaw", "Yes"}},
            {"Diagnosis", "Possible Coronary Artery Disease"}
        )
    );

    // Rule 3: Possible Heart Valve Disease
    diagnosisRuleList.push_back(
        Rule(3, 
            {{"Chest pain", "No"}, {"Dizziness or fainting", "Yes"}, {"Fainting spells", "Yes"}},
            {"Diagnosis", "Possible Heart Valve Disease"}
        )
    );

    // Rule 4: Possible Heart Arrhythmias (Fainting and Fluttering)
    diagnosisRuleList.push_back(
        Rule(4, 
            {{"Chest pain", "No"}, {"Dizziness or fainting", "Yes"}, {"Fainting spells", "No"}, {"Fluttering sensation", "Yes"}},
            {"Diagnosis", "Possible Heart Arrhythmias"}
        )
    );

    // Rule 5: Consider Cardiomyopathy or Congenital Heart Defects (Exertion Fatigue)
    diagnosisRuleList.push_back(
        Rule(5, 
            {{"Chest pain", "No"}, {"Dizziness or fainting", "No"}, {"Swelling in legs or fatigue", "Yes"}, {"Fatigue upon exertion", "Yes"}},
            {"Diagnosis", "Consider Cardiomyopathy or Congenital Heart Defects"}
        )
    );

    // Rule 6: Swelling - Cardiomyopathy / Cyanosis - Congenital Heart Defects
    diagnosisRuleList.push_back(
        Rule(6, 
            {{"Chest pain", "No"}, {"Dizziness or fainting", "No"}, {"Swelling in legs or fatigue", "No"}, {"Swelling in legs", "Yes"}},
            {"Diagnosis", "Consider Cardiomyopathy or Congenital Heart Defects"}
        )
    );

    // Rule 7: Inconclusive - Shortness of Breath No, Neck Pain No
    diagnosisRuleList.push_back(
        Rule(7, 
            {{"Chest pain", "Yes"}, {"Shortness of breath", "No"}, {"Pain in neck or jaw", "No"}},
            {"Diagnosis", "Inconclusive"}
        )
    );

    // Rule 8: Inconclusive - Fainting spells No, Fluttering No
    diagnosisRuleList.push_back(
        Rule(8, 
            {{"Chest pain", "No"}, {"Dizziness or fainting", "Yes"}, {"Fainting spells", "No"}, {"Fluttering sensation", "No"}},
            {"Diagnosis", "Inconclusive"}
        )
    );

    // Rule 9: Inconclusive - Swelling in legs or fatigue No
    diagnosisRuleList.push_back(
        Rule(9, 
            {{"Chest pain", "No"}, {"Dizziness or fainting", "No"}, {"Swelling in legs or fatigue", "No"}},
            {"Diagnosis", "Inconclusive"}
        )
    );
    


    // Treatment rules (Forward Chaining)
    treatmentRuleList.push_back(Rule(1, {{"diagnosis", "Possible Coronary Artery Disease"}}, {"treatment", "Lifestyle changes, Medications, Surgery"}));
    
    
    // diagnosisRules["Heart Disease Diagnosis"] = "Coronary Artery Disease";
    // diagnosisRules["Chest Pain"] = "Heart Arrhythmias";
    // diagnosisRules["Shortness of Breath"] = "Cardiomyopathy";
    // diagnosisRules["Dizziness"] = "Heart Valve Disease";
    
    // // Clause variable list for backward chaining
    clauseVariableList["1"] = {"Chest Pain"};
    //, "Shortness of Breath"};
    // clauseVariableList["2"] = {"Dizziness", "Fainting"};
    // clauseVariableList["3"] = {"Palpitations"};
    // clauseVariableList["4"] = {"Swelling", "Cyanosis"};

    for (int i = 0; i < diagnosisRuleList.size(); ++i) {
        // map<string, vector<int>> conclusionMap; // Maps conclusion variable to rule numbers

        conclusionMap[diagnosisRuleList[i].conclusion.first].push_back(i);
    }
    // 
    // treatmentRules["Coronary Artery Disease"] = "Lifestyle changes, Medications, Surgery";
    // treatmentRules["Heart Arrhythmias"] = "Anti-arrhythmic Drugs, Pacemaker";
    // treatmentRules["Cardiomyopathy"] = "Low-Sodium Diet, Medications, Heart Transplant";
    // treatmentRules["Heart Valve Disease"] = "Medications, Valve Replacement Surgery";
}

// Diagnosis using backward chaining
string diagnosis() {
    string goalVariable = "Diagnosis";  // Diagnosis is the ultimate goal

    processBC(goalVariable);

    if (derivedGlobalVariables.find(goalVariable) != derivedGlobalVariables.end()) 
    {
        return derivedGlobalVariables[goalVariable];
    } 
    else 
    {
        return "No conclusion for diagnosis.";
    }
}

// Process function for backward chaining
void processBC(string variable) 
{
    // Search for conclusion

    // Loop
        // searchCon(variable) // goal variable, find matching variable in conclusion list and the corresponding Rule Number (Ri)
        // ruleToClause(ruleNumber) // Convert Rule # Ri to clause number, Ci 
        // updateVL(clauseNumber) // Update the variable list by asking user for values
          // If variable not in variable list, it could be in the clause of some rule.
          // call processBC(variable) for each variable in the clause

        // validateRi(ruleNumber, conclusion) // Validate the rule based on variables
          // Here we would validate the variables in the clause based on user input
          // For simplicity, we're just using dummy validation
          // conclusion = diagnosisRules[to_string(ruleNumber)]

    // Perform backward chaining on the given goal variable
    if (derivedGlobalVariables.find(variable) == derivedGlobalVariables.end()) 
    {

        bool foundConclusion = searchConBC(variable);

        if (!foundConclusion) 
        {
            cout << "Goal cannot be determined for: " << variable << endl;
        }
    }
}

// Search the conclusion list for a matching variable
bool searchConBC(string variable) 
{

    // FIX: We're searching for a connclusion w/ "diagnosis" 
    for (const auto& rule : diagnosisRuleList) 
    {

        if (rule.conclusion.first == variable) 
        {
            // We found a rule whose conclusion is the goal variable
            bool valid = validateRi(rule);

            if (valid) 
            {
                derivedGlobalVariables[rule.conclusion.first] = rule.conclusion.second;
                return true;
            }
        }
    }

    cout << "searchConBC is returning false " << variable << endl;
    return false;  // If no rule leads to the variable
}

// Validate the rule based on variables
bool validateRi(const Rule& rule) 
{

    cout << "Validating Rule " << rule.number << "..." << endl;
    // Check if the conditions for this rule are met
    for (const auto& condition : rule.conditions) 
    {

        cout << "Checking condition: " << condition.first << " = " << condition.second << endl;

        const string& variable = condition.first;
        const string& expectedValue = condition.second;

        // If the variable is not yet instantiated, we need to process it
        if (derivedGlobalVariables.find(variable) == derivedGlobalVariables.end()) 
        {

            updateVL({{variable, expectedValue}});

            cout << "Updated variable list with " << variable << " = " << derivedGlobalVariables[variable] << endl << endl << endl;
        }

        // If the condition is not satisfied, the rule is invalid
        if (derivedGlobalVariables[variable] != expectedValue) 
        {
            return false;
        }
    }
    return true;
}

// Ask user for values of variables in the clause and update the variable list
void updateVL(const vector<pair<string, string>>& conditions) 
{
    for (const auto& condition : conditions) 
    {
        const string& var = condition.first;

        if (derivedGlobalVariables.find(var) == derivedGlobalVariables.end()) 
        {
            string userInput;
            while (true) {
                cout << "Do you have " << var << "? (yes/no): ";
                cin >> userInput;

                // Validate user input
                if (userInput == "Yes" || userInput == "No") 
                {
                    derivedGlobalVariables[var] = userInput;
                    break;
                } 
                else 
                {
                    cout << "Invalid input. Please enter 'yes' or 'no'." << endl;
                }
            }
        }
    }
}

// FORWARD CHAINING
// Treatment using forward chaining
void processFC(string diagnosis) {
    /*
    a.	Instantiate the value of the variable in the variable list. Call search_cvl(variable)
    b.	return

    */ 
    cout << "Starting Treatment Recommendation..." << endl;

    if (treatmentRules.find(diagnosis) != treatmentRules.end()) {
        cout << "Recommended Treatment: " << treatmentRules[diagnosis] << endl;
    } else {
        cout << "No treatment found for the given diagnosis." << endl;
    }
}


