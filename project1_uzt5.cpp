#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;

// To-DO -> 
// Use namespace std

// Structure to represent a variable
struct Variable {
    std::string name;
    std::string value;
    bool instantiated;

    Variable(std::string n) : name(n), value(""), instantiated(false) {}
};

// Structure to represent a rule
struct Rule {
    int number;
    std::vector<std::pair<std::string, std::string>> conditions; // (variable, expected value)
    std::pair<std::string, std::string> conclusion; // (variable, value)

    Rule(int num, std::vector<std::pair<std::string, std::string>> conds, std::pair<std::string, std::string> concl)
        : number(num), conditions(conds), conclusion(concl) {}
};

// Global variables and data structures
std::vector<Variable> variableList;
std::map<std::string, Variable*> variableMap;
std::vector<Rule> diagnosisRuleList;
std::vector<Rule> treatmentRuleList;
std::map<std::string, std::vector<int>> conclusionMap; // Maps conclusion variable to rule numbers
std::vector<std::string> derivedConclusions; // For forward chaining
std::queue<std::string> globalConclusionQueue; // For forward chaining

// Function prototypes
void initializeDiagnosis();
void initializeTreatment();
void backwardChaining();
void forwardChaining(std::string diagnosis);

void BC_Process(std::string variable);
void BC_update_VL(int Ri);
bool BC_validate_Ri(int Ri, std::string& conclusion);

void FC_main(std::string givenVariable);
void FC_process(std::string variable);
void FC_search_cvl(std::string variable);
void FC_clause_to_rule(int Ci);
void FC_update_VL(int Ci);
void FC_validate_Ri(int Ri);

// Utility functions
Variable* getVariable(std::string varName);
void resetVariables();

int main() {
    initializeDiagnosis();
    backwardChaining();

    // After processing, check if diagnosis is instantiated
    Variable* diagnosisVar = variableMap["diagnosis"];
    if (diagnosisVar->instantiated && !diagnosisVar->value.empty()) {
        std::cout << "\nDiagnosis: " << diagnosisVar->value << "\n" << std::endl;
        initializeTreatment();
        forwardChaining(diagnosisVar->value);
    } else {
        std::cout << "Unable to determine the diagnosis. Further medical evaluation is recommended." << std::endl;
    }

    return 0;
}

// Initialize variables and rules for diagnosis
void initializeDiagnosis() {
    // Create variables (symptoms and diagnosis)
    std::vector<std::string> symptoms = {
        "chest_pain", "shortness_of_breath", "heart_palpitations", "neck_or_jaw_pain",
        "dizziness", "fainting_spells", "swelling_in_legs", "fatigue",
        "fluttering_in_chest", "cyanosis", "rapid_heartbeat", "nausea",
        "sweating", "high_blood_pressure", "age_over_60", "family_history",
        "smoker", "obesity", "high_cholesterol", "diabetes",
        "diagnosis"
    };

    for (const auto& sym : symptoms) {
        variableList.push_back(Variable(sym));
        variableMap[sym] = &variableList.back();
    }

    // Create diagnosis rules (expand to at least 30 rules)
    // Rule format: Rule(number, conditions, conclusion)

    // Example rules
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
    
    // You can add more rules to further expand the system

    // Build conclusionMap for backward chaining
    for (int i = 0; i < diagnosisRuleList.size(); ++i) {
        conclusionMap[diagnosisRuleList[i].conclusion.first].push_back(i);
    }
}

// Backward Chaining Functions
void backwardChaining() {
    std::string goal_variable = "diagnosis";
    BC_Process(goal_variable);
}

void BC_Process(std::string variable) {
    Variable* var = getVariable(variable);
    if (var->instantiated) {
        // Variable already instantiated
        return;
    }
    if (conclusionMap.find(variable) == conclusionMap.end()) {
        // No rules conclude this variable
        return;
    }
    for (int Ri : conclusionMap[variable]) {
        // For each rule Ri that concludes 'variable'
        BC_update_VL(Ri);
        std::string conclusion;
        if (BC_validate_Ri(Ri, conclusion)) {
            // Conclusion is assigned
            var->value = conclusion;
            var->instantiated = true;
            return;
        } else {
            // Continue to next rule
            continue;
        }
    }
}

void BC_update_VL(int Ri) {
    Rule& rule = diagnosisRuleList[Ri];
    for (auto& cond : rule.conditions) {
        std::string varName = cond.first;
        Variable* var = getVariable(varName);
        if (!var->instantiated) {
            // Check if variable is in the conclusion of some rule
            if (conclusionMap.find(varName) != conclusionMap.end()) {
                // Variable can be derived, call Process(variable)
                BC_Process(varName);
            } else {
                // Ask user to provide value
                std::string val;
                while (true) {
                    std::cout << "Do you have " << varName << "? (yes/no): ";
                    std::cin >> val;
                    if (val == "yes" || val == "no") {
                        break;
                    } else {
                        std::cout << "Please answer 'yes' or 'no'." << std::endl;
                    }
                }
                var->value = val;
                var->instantiated = true;
            }
        }
    }
}

bool BC_validate_Ri(int Ri, std::string& conclusion) {
    Rule& rule = diagnosisRuleList[Ri];
    for (auto& cond : rule.conditions) {
        std::string varName = cond.first;
        std::string expectedValue = cond.second;
        Variable* var = getVariable(varName);
        if (!var->instantiated) {
            // Variable not instantiated
            return false;
        }
        if (var->value != expectedValue) {
            // Condition not satisfied
            return false;
        }
    }
    // All conditions satisfied
    conclusion = rule.conclusion.second;
    return true;
}

// Initialize variables and rules for treatment
void initializeTreatment() {
    // Reset variables
    resetVariables();

    // Create variables (diagnosis and treatments)
    variableList.push_back(Variable("treatment"));

    // Build variableMap
    for (auto& var : variableList) {
        variableMap[var.name] = &var;
    }

    // Create treatment rules
    // Rule format: Rule(number, conditions, conclusion)
    treatmentRuleList.push_back(Rule(1, {{"diagnosis", "Heart Attack"}}, {"treatment", "Immediate hospitalization and administration of thrombolytics"}));
    treatmentRuleList.push_back(Rule(2, {{"diagnosis", "Angina"}}, {"treatment", "Prescription of nitrates and beta-blockers"}));
    treatmentRuleList.push_back(Rule(3, {{"diagnosis", "Arrhythmia"}}, {"treatment", "Medication and possible pacemaker installation"}));
    treatmentRuleList.push_back(Rule(4, {{"diagnosis", "Heart Failure"}}, {"treatment", "Diuretics and ACE inhibitors"}));
    treatmentRuleList.push_back(Rule(5, {{"diagnosis", "Aortic Dissection"}}, {"treatment", "Emergency surgery"}));
    treatmentRuleList.push_back(Rule(6, {{"diagnosis", "Congenital Heart Disease"}}, {"treatment", "Surgical intervention and lifelong monitoring"}));
    treatmentRuleList.push_back(Rule(7, {{"diagnosis", "Tachycardia"}}, {"treatment", "Antiarrhythmic drugs"}));
    treatmentRuleList.push_back(Rule(8, {{"diagnosis", "Coronary Artery Disease"}}, {"treatment", "Lifestyle changes and statins"}));
    treatmentRuleList.push_back(Rule(9, {{"diagnosis", "Cardiomyopathy"}}, {"treatment", "Medications and lifestyle modifications"}));
    treatmentRuleList.push_back(Rule(10, {{"diagnosis", "Atherosclerosis"}}, {"treatment", "Cholesterol-lowering medications"}));
    treatmentRuleList.push_back(Rule(11, {{"diagnosis", "Pericarditis"}}, {"treatment", "Anti-inflammatory medications"}));
    treatmentRuleList.push_back(Rule(12, {{"diagnosis", "Silent Ischemia"}}, {"treatment", "Blood thinners and lifestyle changes"}));
    treatmentRuleList.push_back(Rule(13, {{"diagnosis", "Pulmonary Hypertension"}}, {"treatment", "Vasodilators"}));
    treatmentRuleList.push_back(Rule(14, {{"diagnosis", "Endocarditis"}}, {"treatment", "Antibiotic therapy"}));
    treatmentRuleList.push_back(Rule(15, {{"diagnosis", "Bradycardia"}}, {"treatment", "Pacemaker installation"}));
    treatmentRuleList.push_back(Rule(16, {{"diagnosis", "Venous Insufficiency"}}, {"treatment", "Compression stockings and leg elevation"}));
    treatmentRuleList.push_back(Rule(17, {{"diagnosis", "Pulmonary Embolism"}}, {"treatment", "Anticoagulant medications"}));
    treatmentRuleList.push_back(Rule(18, {{"diagnosis", "Hypertensive Heart Disease"}}, {"treatment", "Blood pressure management"}));
    treatmentRuleList.push_back(Rule(19, {{"diagnosis", "Myocardial Infarction"}}, {"treatment", "Immediate revascularization"}));
    treatmentRuleList.push_back(Rule(20, {{"diagnosis", "Panic Attack"}}, {"treatment", "Anxiolytics and psychotherapy"}));
    treatmentRuleList.push_back(Rule(21, {{"diagnosis", "Silent Heart Attack"}}, {"treatment", "Lifestyle changes and medication"}));
    treatmentRuleList.push_back(Rule(22, {{"diagnosis", "Peripheral Artery Disease"}}, {"treatment", "Exercise therapy and antiplatelet drugs"}));
    treatmentRuleList.push_back(Rule(23, {{"diagnosis", "Unstable Angina"}}, {"treatment", "Hospitalization and nitroglycerin"}));
    treatmentRuleList.push_back(Rule(24, {{"diagnosis", "Heart Block"}}, {"treatment", "Pacemaker installation"}));
    treatmentRuleList.push_back(Rule(25, {{"diagnosis", "Dilated Cardiomyopathy"}}, {"treatment", "Medications and possible transplant"}));
    treatmentRuleList.push_back(Rule(26, {{"diagnosis", "Mitral Valve Prolapse"}}, {"treatment", "Beta-blockers and monitoring"}));
    treatmentRuleList.push_back(Rule(27, {{"diagnosis", "Right Heart Failure"}}, {"treatment", "Diuretics and oxygen therapy"}));
    treatmentRuleList.push_back(Rule(28, {{"diagnosis", "Aortic Stenosis"}}, {"treatment", "Valve replacement surgery"}));
    treatmentRuleList.push_back(Rule(29, {{"diagnosis", "Hypertrophic Cardiomyopathy"}}, {"treatment", "Beta-blockers and calcium channel blockers"}));
    treatmentRuleList.push_back(Rule(30, {{"diagnosis", "Stable Angina"}}, {"treatment", "Lifestyle changes and medications"}));

    // Build Clause Variable List for forward chaining (we'll simulate it with an array)
    // Since our Clause Variable List (CVL) is not explicitly defined, we'll use the conditions in rules as clauses
}

// Forward Chaining Functions
void forwardChaining(std::string givenVariable) {
    FC_main(givenVariable);
    // Print all derived conclusions
    std::cout << "Treatment Recommendations:\n";
    for (const auto& conclusion : derivedConclusions) {
        std::cout << "- " << conclusion << std::endl;
    }
}

void FC_main(std::string givenVariable) {
    // Initialize Global Conclusion Variable Queue
    globalConclusionQueue.push(givenVariable);

    // While queue is not empty
    while (!globalConclusionQueue.empty()) {
        std::string variable = globalConclusionQueue.front();
        globalConclusionQueue.pop();
        FC_process(variable);
    }
}

void FC_process(std::string variable) {
    FC_search_cvl(variable);
}

void FC_search_cvl(std::string variable) {
    // Find clauses (conditions) that contain the variable
    for (int Ci = 0; Ci < treatmentRuleList.size(); ++Ci) {
        Rule& rule = treatmentRuleList[Ci];
        for (auto& cond : rule.conditions) {
            if (cond.first == variable) {
                // Found matching clause, proceed
                FC_update_VL(Ci);
                FC_clause_to_rule(Ci);
                break;
            }
        }
    }
}

void FC_update_VL(int Ci) {
    Rule& rule = treatmentRuleList[Ci];
    for (auto& cond : rule.conditions) {
        Variable* var = getVariable(cond.first);
        if (!var->instantiated) {
            // For forward chaining, since the variable is given (diagnosis), we can instantiate it
            var->value = cond.second;
            var->instantiated = true;
        }
    }
}

void FC_clause_to_rule(int Ci) {
    // In this implementation, Ci is the index in the rule list
    int Ri = Ci;
    FC_validate_Ri(Ri);
}

void FC_validate_Ri(int Ri) {
    Rule& rule = treatmentRuleList[Ri];
    bool conditionsMet = true;
    for (auto& cond : rule.conditions) {
        Variable* var = getVariable(cond.first);
        if (var->value != cond.second) {
            conditionsMet = false;
            break;
        }
    }
    if (conditionsMet) {
        // Add the conclusion to derived conclusions and global queue
        Variable* conclVar = getVariable(rule.conclusion.first);
        if (!conclVar->instantiated) {
            conclVar->value = rule.conclusion.second;
            conclVar->instantiated = true;
            derivedConclusions.push_back(rule.conclusion.second);
            globalConclusionQueue.push(rule.conclusion.first);
        }
    }
}

// Utility Functions
Variable* getVariable(std::string varName) {
    if (variableMap.find(varName) != variableMap.end()) {
        return variableMap[varName];
    } else {
        // Create new variable if it doesn't exist
        variableList.push_back(Variable(varName));
        variableMap[varName] = &variableList.back();
        return &variableList.back();
    }
}

void resetVariables() {
    variableList.clear();
    variableMap.clear();
    derivedConclusions.clear();
    while (!globalConclusionQueue.empty()) {
        globalConclusionQueue.pop();
    }
}
