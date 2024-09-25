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
queue<string> globalConclusionQueue; // Global conclusion queue for forward chaining

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
void searchCVL(string diagnosis);

int main() {
    // Populate the rule base for diagnosis and treatment
    initialize_knowledge_base();

    cout << "Welcome to the Cardiovascular Expert System!" << endl;

    // Begin Diagnosis using Backward Chaining
    cout << "Starting Diagnosis..." << endl;
    string result = diagnosis();

    if (result == "Inconclusive") 
    {
        cout << "Diagnosis: Inconclusive" << endl;
        return 0;
    } 
    
    
    cout << "Diagnosis: " << result << endl;
    cout << "Starting Treatment Recommendation..." << endl;

    // Begin Treatment using Forward Chaining
    processFC(result);


    return 0;
}

// Populate diagnosis and treatment rules
void initialize_knowledge_base() {

    // Diagnosis rules (Backward Chaining)
    diagnosisRuleList.push_back(Rule(1, {{"chest_pain", "yes"}, {"shortness_of_breath", "yes"}, {"sweating", "yes"}}, {"diagnosis", "Heart Attack"}));
    diagnosisRuleList.push_back(Rule(2, {{"chest_pain", "yes"}, {"neck_or_jaw_pain", "yes"}, {"nausea", "yes"}}, {"diagnosis", "Angina"}));
    diagnosisRuleList.push_back(Rule(3, {{"dizziness", "yes"}, {"fainting_spells", "yes"}, {"rapid_heartbeat", "yes"}}, {"diagnosis", "Arrhythmia"}));
    diagnosisRuleList.push_back(Rule(4, {{"fatigue", "yes"}, {"swelling_in_legs", "yes"}, {"shortness_of_breath", "yes"}}, {"diagnosis", "Heart Failure"}));
    diagnosisRuleList.push_back(Rule(5, {{"chest_pain", "yes"}, {"high_blood_pressure", "yes"}, {"age_over_60", "yes"}}, {"diagnosis", "Aortic Dissection"}));
    diagnosisRuleList.push_back(Rule(6, {{"shortness_of_breath", "yes"}, {"fatigue", "yes"}, {"cyanosis", "yes"}}, {"diagnosis", "Congenital Heart Disease"}));
    diagnosisRuleList.push_back(Rule(7, {{"chest_pain", "no"}, {"dizziness", "yes"}, {"rapid_heartbeat", "yes"}}, {"diagnosis", "Tachycardia"}));
    diagnosisRuleList.push_back(Rule(8, {{"chest_pain", "yes"}, {"smoker", "yes"}, {"age_over_60", "yes"}}, {"diagnosis", "Coronary Artery Disease"}));
    diagnosisRuleList.push_back(Rule(9, {{"fatigue", "yes"}, {"obesity", "yes"}, {"high_blood_pressure", "yes"}}, {"diagnosis", "Cardiomyopathy"}));
    diagnosisRuleList.push_back(Rule(10, {{"high_cholesterol", "yes"}, {"family_history", "yes"}, {"smoker", "yes"}}, {"diagnosis", "Atherosclerosis"}));

    diagnosisRuleList.push_back(Rule(11, {{"chest_pain", "yes"}, {"shortness_of_breath", "no"}, {"heart_palpitations", "yes"}}, {"diagnosis", "Pericarditis"}));
    diagnosisRuleList.push_back(Rule(12, {{"fatigue", "yes"}, {"dizziness", "yes"}, {"diabetes", "yes"}}, {"diagnosis", "Silent Ischemia"}));
    diagnosisRuleList.push_back(Rule(13, {{"shortness_of_breath", "yes"}, {"fatigue", "yes"}, {"swelling_in_legs", "yes"}}, {"diagnosis", "Pulmonary Hypertension"}));
    diagnosisRuleList.push_back(Rule(14, {{"chest_pain", "yes"}, {"shortness_of_breath", "yes"}, {"fever", "yes"}}, {"diagnosis", "Endocarditis"}));
    diagnosisRuleList.push_back(Rule(15, {{"chest_pain", "no"}, {"dizziness", "yes"}, {"heart_palpitations", "yes"}}, {"diagnosis", "Bradycardia"}));
    diagnosisRuleList.push_back(Rule(16, {{"fatigue", "yes"}, {"swelling_in_legs", "yes"}, {"obesity", "yes"}}, {"diagnosis", "Venous Insufficiency"}));
    diagnosisRuleList.push_back(Rule(17, {{"shortness_of_breath", "yes"}, {"chest_pain", "yes"}, {"smoker", "yes"}}, {"diagnosis", "Pulmonary Embolism"}));
    diagnosisRuleList.push_back(Rule(18, {{"high_blood_pressure", "yes"}, {"diabetes", "yes"}, {"age_over_60", "yes"}}, {"diagnosis", "Hypertensive Heart Disease"}));
    diagnosisRuleList.push_back(Rule(19, {{"chest_pain", "yes"}, {"nausea", "yes"}, {"sweating", "yes"}}, {"diagnosis", "Myocardial Infarction"}));
    diagnosisRuleList.push_back(Rule(20, {{"shortness_of_breath", "yes"}, {"rapid_heartbeat", "yes"}, {"anxiety", "yes"}}, {"diagnosis", "Panic Attack"}));

    diagnosisRuleList.push_back(Rule(21, {{"chest_pain", "yes"}, {"sweating", "yes"}, {"diabetes", "yes"}}, {"diagnosis", "Silent Heart Attack"}));
    diagnosisRuleList.push_back(Rule(22, {{"fatigue", "yes"}, {"dizziness", "yes"}, {"smoker", "yes"}}, {"diagnosis", "Peripheral Artery Disease"}));
    diagnosisRuleList.push_back(Rule(23, {{"chest_pain", "yes"}, {"shortness_of_breath", "yes"}, {"nausea", "yes"}}, {"diagnosis", "Unstable Angina"}));
    diagnosisRuleList.push_back(Rule(24, {{"heart_palpitations", "yes"}, {"dizziness", "yes"}, {"fainting_spells", "yes"}}, {"diagnosis", "Heart Block"}));
    diagnosisRuleList.push_back(Rule(25, {{"fatigue", "yes"}, {"swelling_in_legs", "yes"}, {"rapid_heartbeat", "yes"}}, {"diagnosis", "Dilated Cardiomyopathy"}));
    diagnosisRuleList.push_back(Rule(26, {{"chest_pain", "no"}, {"shortness_of_breath", "yes"}, {"coughing", "yes"}}, {"diagnosis", "Mitral Valve Prolapse"}));
    diagnosisRuleList.push_back(Rule(27, {{"fatigue", "yes"}, {"swelling_in_legs", "yes"}, {"cyanosis", "yes"}}, {"diagnosis", "Right Heart Failure"}));
    diagnosisRuleList.push_back(Rule(28, {{"dizziness", "yes"}, {"fainting_spells", "yes"}, {"age_over_60", "yes"}}, {"diagnosis", "Aortic Stenosis"}));
    diagnosisRuleList.push_back(Rule(29, {{"chest_pain", "yes"}, {"rapid_heartbeat", "yes"}, {"shortness_of_breath", "yes"}}, {"diagnosis", "Hypertrophic Cardiomyopathy"}));
    diagnosisRuleList.push_back(Rule(30, {{"chest_pain", "yes"}, {"shortness_of_breath", "yes"}, {"fatigue", "yes"}}, {"diagnosis", "Stable Angina"}));

    // Treatment rules (Forward Chaining)
    treatmentRuleList.push_back(Rule(1, {{"diagnosis", "Heart Attack"}}, {"treatment", "Call emergency services, administer aspirin, provide oxygen if available"}));
    treatmentRuleList.push_back(Rule(2, {{"diagnosis", "Angina"}}, {"treatment", "Rest, use nitroglycerin, and avoid exertion"}));
    treatmentRuleList.push_back(Rule(3, {{"diagnosis", "Arrhythmia"}}, {"treatment", "Medication (antiarrhythmics), monitor heart rate, possible cardioversion"}));
    treatmentRuleList.push_back(Rule(4, {{"diagnosis", "Heart Failure"}}, {"treatment", "Diuretics, ACE inhibitors, lifestyle changes, and monitor fluid intake"}));
    treatmentRuleList.push_back(Rule(5, {{"diagnosis", "Aortic Dissection"}}, {"treatment", "Immediate surgery, blood pressure control, and pain management"}));
    treatmentRuleList.push_back(Rule(6, {{"diagnosis", "Congenital Heart Disease"}}, {"treatment", "Surgical repair, medications, and regular monitoring"}));
    treatmentRuleList.push_back(Rule(7, {{"diagnosis", "Tachycardia"}}, {"treatment", "Beta-blockers, vagal maneuvers, and possible ablation"}));
    treatmentRuleList.push_back(Rule(8, {{"diagnosis", "Coronary Artery Disease"}}, {"treatment", "Statins, aspirin, lifestyle changes, and angioplasty if needed"}));
    treatmentRuleList.push_back(Rule(9, {{"diagnosis", "Cardiomyopathy"}}, {"treatment", "Medications, lifestyle changes, and possibly a heart transplant in severe cases"}));
    treatmentRuleList.push_back(Rule(10, {{"diagnosis", "Atherosclerosis"}}, {"treatment", "Statins, lifestyle changes, and possible angioplasty or surgery"}));

    treatmentRuleList.push_back(Rule(11, {{"diagnosis", "Pericarditis"}}, {"treatment", "Anti-inflammatory medications, pain relief, and rest"}));
    treatmentRuleList.push_back(Rule(12, {{"diagnosis", "Silent Ischemia"}}, {"treatment", "Medications (beta-blockers, nitrates), lifestyle changes, and regular monitoring"}));
    treatmentRuleList.push_back(Rule(13, {{"diagnosis", "Pulmonary Hypertension"}}, {"treatment", "Vasodilators, diuretics, and oxygen therapy"}));
    treatmentRuleList.push_back(Rule(14, {{"diagnosis", "Endocarditis"}}, {"treatment", "Antibiotics or antifungal treatment, surgery in severe cases"}));
    treatmentRuleList.push_back(Rule(15, {{"diagnosis", "Bradycardia"}}, {"treatment", "Pacemaker implantation in severe cases, monitor heart rate"}));
    treatmentRuleList.push_back(Rule(16, {{"diagnosis", "Venous Insufficiency"}}, {"treatment", "Compression stockings, leg elevation, and possible surgery"}));
    treatmentRuleList.push_back(Rule(17, {{"diagnosis", "Pulmonary Embolism"}}, {"treatment", "Anticoagulants, thrombolytic therapy, and hospitalization"}));
    treatmentRuleList.push_back(Rule(18, {{"diagnosis", "Hypertensive Heart Disease"}}, {"treatment", "Blood pressure control, medications, and lifestyle changes"}));
    treatmentRuleList.push_back(Rule(19, {{"diagnosis", "Myocardial Infarction"}}, {"treatment", "Emergency treatment, aspirin, possible angioplasty or surgery"}));
    treatmentRuleList.push_back(Rule(20, {{"diagnosis", "Panic Attack"}}, {"treatment", "Calm environment, breathing exercises, possible anti-anxiety medications"}));

    treatmentRuleList.push_back(Rule(21, {{"diagnosis", "Silent Heart Attack"}}, {"treatment", "Lifestyle changes, medications (aspirin, statins), and regular monitoring"}));
    treatmentRuleList.push_back(Rule(22, {{"diagnosis", "Peripheral Artery Disease"}}, {"treatment", "Lifestyle changes, medications, and possible angioplasty or surgery"}));
    treatmentRuleList.push_back(Rule(23, {{"diagnosis", "Unstable Angina"}}, {"treatment", "Hospitalization, nitroglycerin, and possibly surgery or angioplasty"}));
    treatmentRuleList.push_back(Rule(24, {{"diagnosis", "Heart Block"}}, {"treatment", "Pacemaker, medications, and regular heart monitoring"}));
    treatmentRuleList.push_back(Rule(25, {{"diagnosis", "Dilated Cardiomyopathy"}}, {"treatment", "Medications, lifestyle changes, and possible heart transplant in severe cases"}));
    treatmentRuleList.push_back(Rule(26, {{"diagnosis", "Mitral Valve Prolapse"}}, {"treatment", "Beta-blockers, possible valve repair or replacement"}));
    treatmentRuleList.push_back(Rule(27, {{"diagnosis", "Right Heart Failure"}}, {"treatment", "Diuretics, ACE inhibitors, and lifestyle changes"}));
    treatmentRuleList.push_back(Rule(28, {{"diagnosis", "Aortic Stenosis"}}, {"treatment", "Surgical valve replacement, medications to manage symptoms"}));
    treatmentRuleList.push_back(Rule(29, {{"diagnosis", "Hypertrophic Cardiomyopathy"}}, {"treatment", "Beta-blockers, calcium channel blockers, and possible surgery"}));
    treatmentRuleList.push_back(Rule(30, {{"diagnosis", "Stable Angina"}}, {"treatment", "Lifestyle changes, nitroglycerin, and regular monitoring"}));

    for (int i = 0; i < diagnosisRuleList.size(); ++i) {
        // map<string, vector<int>> conclusionMap; // Maps conclusion variable to rule numbers
        conclusionMap[diagnosisRuleList[i].conclusion.first].push_back(i);
    }
    
}

// Diagnosis using backward chaining
string diagnosis() {
    string goalVariable = "diagnosis";  // Diagnosis is the ultimate goal

    processBC(goalVariable);

    if (derivedGlobalVariables.find(goalVariable) != derivedGlobalVariables.end()) 
        return derivedGlobalVariables[goalVariable];
    
    
    return "Inconclusive";
    
}

// Process function for backward chaining
void processBC(string variable) 
{
    // Perform backward chaining on the given goal variable
    if (derivedGlobalVariables.find(variable) == derivedGlobalVariables.end()) 
    {
        bool foundConclusion = searchConBC(variable);

        if (!foundConclusion) 
            cout << "Goal cannot be determined for: " << variable << endl;
        
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
            return false;
        
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

                string output = "\nDo you have " + var + "? (yes/no): ";

                if (var == "obesity")
                    output = "Are you obese? (yes/no): ";
                else if (var == "smoker")
                    output = "Are you a smoker? (yes/no): ";
                else if (var == "age_over_60")
                    output = "Are you over 60 years old? (yes/no): ";

                cout << output;
                cin >> userInput;

                // Validate user input
                if (userInput == "yes" || userInput == "no") 
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
    try {
        // Push the initial diagnosis onto the conclusion queue
        globalConclusionQueue.push(diagnosis);

        // While the queue is not empty, process each diagnosis and find treatments
        while (!globalConclusionQueue.empty()) {
            string currentDiagnosis = globalConclusionQueue.front();
            globalConclusionQueue.pop();  
            
            // Search the Clause Variable List for possible conclusions (i.e., treatment rules)
            searchCVL(currentDiagnosis);
        }

        
    } catch (const exception& e) {
        cerr << "An error occurred during forward chaining: " << e.what() << endl;
    }
}

void searchCVL(string diagnosis) {
    // Iterate over all treatment rules
    for (const auto& rule : treatmentRuleList) {

        cout << "LOG: Checking (Treatment) rule: " << rule.number << endl;

        // If the rule's condition matches the diagnosis
        for (const auto& condition : rule.conditions) {
            if (condition.first == "diagnosis" && condition.second == diagnosis) {

                // Treatment is found
                cout << "\nRecommended Treatment for " << diagnosis << ": " << rule.conclusion.second << endl;

                // Add the treatment conclusion to derived variables
                derivedGlobalVariables["treatment"] = rule.conclusion.second;

                return;
            }
        }
    }
    
    cout << "No treatment found for the given diagnosis: " << diagnosis << endl << endl;
}


