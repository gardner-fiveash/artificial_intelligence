//
//  hw1.cpp
//  Jason Hunter
//  Gardner Fiveash


#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
using namespace std;

void print(vector<string> rootVar, vector<string> learnVar, vector<string> facts, vector<string> rules) {
    cout << "Root Variables: " << endl;
    for (int i = 0; i < rootVar.size(); i++) {
        cout << "\t" << rootVar[i] << endl;
    }
    cout << endl;
    cout << "Learned Variables: " << endl;
    for (int j = 0; j < learnVar.size(); j++) {
        cout << "\t" << learnVar[j] << endl;
    }
    cout << endl;
    cout << "Facts: " << endl;
    for (int k = 0; k < facts.size(); k++) {
        cout << "\t" << facts[k] << endl;
    }
    cout << endl;
    cout << "Rules: " << endl;
    for (int l = 0; l < rules.size(); l++) {
        cout << "\t" << rules[l] << endl;
    }
    cout << "------------------------" << endl;
    cout << endl;
    cout << endl;
    
}

bool evalExpression (string s, vector<string>facts) {
    //cout << "s = " << s << endl;

    bool operand = false;
    for (int a = 0; a < s.length(); a++) {
        if (s[a] == '&') operand = true;
        if (s[a] == '|') operand = true;
        if (s[a] == '!') operand = true;
        if (s[a] == '(') operand = true;
        if (s[a] == ')') operand = true;
    }
    //cout << "operand = " << operand << endl;
    if (!operand) {
        for (int b = 0; b <facts.size(); b++){
            if (s == facts[b]) {
                //cout << "true = " << s << endl;
                return true;
            }
        }
        //cout << "false = " << s << endl;
        return false;
    }
    vector<int> openParen;
    vector<int> closeParen;
    string token1, token2;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(') {
            openParen.push_back(i); // save index of openParen for substrings
        }
        if (s[i] == ')') {
            closeParen.push_back(i); // save index of closeParen for substrings
        }
        if (s[i] == '!' && (openParen.size() == closeParen.size())){ //if character is negation
            //cout << "hit !" << "\t " << s << endl;
            if (s[i+1] == '(') { //if next character is (, recurse on substring where ( and ) are cut out
                //cout << "hello, (" << endl;
                int netOpen = 1; //find index by determining where #( = #)
                for (int j = i+2; j < s.length(); j++) {
                    if (s[j] == ')') {
                        netOpen -= 1;
                        //cout << "net open = " << netOpen << endl;
                    }
                    if (s[j] == '(') {
                        netOpen++;
                    }
                    if (netOpen == 0) {
                        //cout << "hit zero" << endl;
                        return !(evalExpression(s.substr(i+2, j-i-1),facts)); //substring to recurse on without outer parentheses
                    }
                }
            }
            else {
                int firstOpen = s.length(); //need to get whole variable name, therefore need to cut right before 1st operand/parenthesis
                for (int j = i+1; j < s.length(); j++) {
                    if ((j == '(' || j == ')' || j == '&' || j == '|' || j == '!')&& j <= firstOpen) {
                        firstOpen = j;
                    }
                }
                return !(evalExpression(s.substr(i+1, firstOpen - i - 1),facts)); //substring of just one fact
            }
            
        }
        if (s[i] == '&' && (openParen.size() == closeParen.size())) { //split based on & character if number of open parentheses = number of closed parentheses
            //cout << "hit &&" << "\t " << s << endl;

            int lowIndex = 0; //need to cut token1 regardless of paren
            int lowParenChange = 0;
            for (int k = (i-1); k > 0; k-=1){
                if (s[k] == ')') {
                    lowParenChange -= 1;
                }
                if (s[k] == '(') {
                    lowParenChange += 1;
                }
                if ((s[k] == '&' || s[k] == '|')&& (lowParenChange >= 0)) {
                    lowIndex = k;
                    break;
                }
            }
            int highIndex = s.size() - 1 ;
            int highParenChange = 0;
            for (int k = (i+1); k < s.size(); k+=1){
                if (s[k] == ')') {
                    highParenChange -= 1;
                }
                if (s[k] == '(') {
                    highParenChange += 1;
                }
                if ((s[k] == '&' || s[k] == '|')&& (highParenChange <= 0)) {
                    highIndex = k;
                    break;
                }
            }
            token1 = s.substr(lowIndex, i - lowIndex); //cut from lowIndex to &
            token2 = s.substr(i+1, highIndex-i); //cut from char after & to end
            if (token1[0] == '(' && token1[token1.length()-1] == ')') {
	      token1 = token1.substr(1,token1.length()-2); //if enclosed by parentheses, delete parentheses
            }
            if (token2[0] == '(' && token2[token2.length()-1] == ')') {
	      token2 = token2.substr(1,token2.length()-2); //if enclosed by parentheses, delete parentheses
            }
            return (evalExpression(token1, facts) && evalExpression(token2, facts)); //recursive call to both halves of expression
        }
        if (s[i] == '|' && (openParen.size() == closeParen.size())) { //split based on & character if number of open parentheses = number of closed parentheses
            //cout << "hit ||" << "\t " << s << endl;

            int lowIndex = 0;
            int lowParenChange = 0;
            for (int k = (i-1); k > 0; k-=1){
                if (s[k] == ')') {
                    lowParenChange -= 1;
                }
                if (s[k] == '(') {
                    lowParenChange += 1;
                }
                if ((s[k] == '&' || s[k] == '|')&& (lowParenChange >= 0)) {
                    lowIndex = k;
                    break;
                }
            }
            int highIndex = s.size() - 1 ;
            int highParenChange = 0;
            for (int k = (i+1); k < s.size(); k+=1){
                if (s[k] == ')') {
                    highParenChange -= 1;
                }
                if (s[k] == '(') {
                    highParenChange += 1;
                }
                if ((s[k] == '&' || s[k] == '|')&& (highParenChange <= 0)) {
                    highIndex = k;
                    break;
                }
            }
            token1 = s.substr(lowIndex, i - lowIndex); //cut from lowIndex to |
            token2 = s.substr(i+1, highIndex-i); //cut from char after & to end
            if (token1[0] == '(' && token1[token1.length()-1] == ')') {
	      token1 = token1.substr(1,token1.length()-2); //if enclosed by parentheses, delete parentheses
            }
            if (token2[0] == '(' && token2[token2.length()-1] == ')') {
	      token2 = token2.substr(1,token2.length()-2); //if enclosed by parentheses, delete parentheses
            }
            return (evalExpression(token1, facts) || evalExpression(token2, facts)); //recursive call to both halves of expression
        }

        
    }
    return false;
}

void learn(vector<string>& rules, vector<string>& facts){ // ??? SHOULD FACTS BE A REFERENCE? SHOULD RULES?
    int change = 1;
    string input, output;
    while (change == 1){ //forward chaining
        change = 0; //say facts havnt changed yet
        for(int i = 0; i < rules.size(); i++) { //iterate through rules
            for (int j = 0; j < rules[i].length(); j++){
                if (rules[i][j] == '-') { //for each rule, determine where to split input and output. should be able to tell based on '-'
                    input = rules[i].substr(0, j-1); //input substring
                    output = rules[i].substr(j+3, rules[i].length() - input.length() - 3); // output substring
                    //cout << "input: " << input << "\t output: " << output << endl;
                    if (evalExpression(input,facts)) { //if input is true, check if output value needs to be changed.
                        //cout << "fact true" << endl;
                        int factExists = 0;
                        for (int k = 0; k< facts.size(); k++) {
                            if (facts[k] == output) factExists = 1; //if it is already a fact, change the int to 1 to avoid adding it to facts
                        }
                        if (factExists == 0) {
                            facts.push_back(output); // could be either var or !var, but doesn't matter
                            change = 1; //indicate a fact changed, therefore the rules must be looped through again to find new rules
                            //cout << "added a fact: " << output << endl;
                        }
                    }
                }
                
            }
        }
    }
}

bool query(string s, vector<string> rules, vector<string> facts) {
    bool operand = false;
    for (int a = 0; a < s.length(); a++) {
        if (s[a] == '&') operand = true;
        if (s[a] == '|') operand = true;
        if (s[a] == '!') operand = true;
        if (s[a] == '(') operand = true;
        if (s[a] == ')') operand = true;
    }
    if (!operand) {
        for (int b = 0; b <facts.size(); b++){
            if (s == facts[b]) {
                return true;
            }
        }
        for(int i = 0; i < rules.size(); i++) { //iterate through rules
            string input, output;
            for (int j = 0; j < rules[i].length(); j++){
                if (rules[i][j] == '-') { //for each rule, determine where to split input and output. should be able to tell based on '-'
                    input = rules[i].substr(0, j-1); //input substring
                    output = rules[i].substr(j+3, rules[i].length() - j - 3); // output substring
                    //cout << "input: "<< input << "\t output: " << output << endl;
                }
                if (s == output) {
                    return query(input, rules, facts);
                }
            }
        }
    }
    vector<int> openParen;
    vector<int> closeParen;
    string token1, token2;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(') {
            openParen.push_back(i); // save index of openParen for substrings
        }
        if (s[i] == ')') {
            closeParen.push_back(i); // save index of closeParen for substrings
        }
        if (s[i] == '!'){ //if character is negation
            if (s[i+1] == '(') { //if next character is (, recurse on substring where ( and ) are cut out
                int netOpen = 1; //find index by determining where #( = #)
                for (int j = i+2; j < s.length(); j++) {
                    if (s[j] == ')') {
                        netOpen -= 1;
                    }
                    if (s[j] == '(') {
                        netOpen++;
                    }
                    if (netOpen == 0) {
                        return !(query(s.substr(i+2, j-i-1),rules, facts)); //substring to recurse on without outer parentheses
                    }
                }
            }
            else {
                int firstOpen = s.length(); //need to get whole variable name, therefore need to cut right before 1st operand/parenthesis
                for (int j = i+1; j < s.length(); j++) {
                    if ((j == '(' || j == ')' || j == '&' || j == '|' || j == '!')&& j <= firstOpen) {
                        firstOpen = j;
                    }
                }
                return !(query(s.substr(i+1, firstOpen - i - 1), rules, facts)); //substring of just one fact
            }
            
        }
        if (s[i] == '&' && (openParen.size() == closeParen.size())) { //split based on & character if number of open parentheses = number of closed parentheses
            int lowIndex = 0; //need to cut token1 regardless of paren
            int lowParenChange = 0;
            for (int k = (i-1); k > 0; k-=1){
                if (s[k] == ')') {
                    lowParenChange -= 1;
                }
                if (s[k] == '(') {
                    lowParenChange += 1;
                }
                if ((s[k] == '&' || s[k] == '|')&& (lowParenChange >= 0)) {
                    lowIndex = k;
                    break;
                }
            }
            int highIndex = s.size() - 1 ;
            int highParenChange = 0;
            for (int k = (i+1); k < s.size(); k+=1){
                if (s[k] == ')') {
                    highParenChange -= 1;
                }
                if (s[k] == '(') {
                    highParenChange += 1;
                }
                if ((s[k] == '&' || s[k] == '|')&& (highParenChange <= 0)) {
                    highIndex = k;
                    break;
                }
            }
            token1 = s.substr(lowIndex, i - lowIndex); //cut from lowIndex to &
            token2 = s.substr(i+1, highIndex-i); //cut from char after & to end
            if (token1[0] == '(' && token1[token1.length()-1] == ')') {
	      token1 = token1.substr(1,token1.length()-2); //if enclosed by parentheses, delete parentheses
            }
            if (token2[0] == '(' && token2[token2.length()-1] == ')') {
	      token2 = token2.substr(1,token2.length()-2); //if enclosed by parentheses, delete parentheses
            }
            return (query(token1, rules, facts) && query(token2, rules, facts)); //recursive call to both halves of expression
        }
        if (s[i] == '|' && (openParen.size() == closeParen.size())) { //split based on & character if number of open parentheses = number of closed parentheses
            int lowIndex = 0;
            int lowParenChange = 0;
            for (int k = (i-1); k > 0; k-=1){
                if (s[k] == ')') {
                    lowParenChange -= 1;
                }
                if (s[k] == '(') {
                    lowParenChange += 1;
                }
                if ((s[k] == '&' || s[k] == '|')&& (lowParenChange >= 0)) {
                    lowIndex = k;
                    break;
                }
            }
            int highIndex = s.size() - 1 ;
            int highParenChange = 0;
            for (int k = (i+1); k < s.size(); k+=1){
                if (s[k] == ')') {
                    highParenChange -= 1;
                }
                if (s[k] == '(') {
                    highParenChange += 1;
                }
                if ((s[k] == '&' || s[k] == '|')&& (highParenChange <= 0)) {
                    highIndex = k;
                    break;
                }
            }
            token1 = s.substr(lowIndex, i - lowIndex); //cut from lowIndex to |
            token2 = s.substr(i+1, highIndex-i); //cut from char after & to end
            if (token1[0] == '(' && token1[token1.length()-1] == ')') {
	      token1 = token1.substr(1,token1.length()-2); //if enclosed by parentheses, delete parentheses
            }
            if (token2[0] == '(' && token2[token2.length()-1] == ')') {
	      token2 = token2.substr(1,token2.length()-2); //if enclosed by parentheses, delete parentheses
            }
            return (query(token1, rules, facts) || query(token2, rules, facts)); //recursive call to both halves of expression
        }
        
        
    }
    return false;
}
/* bool explain(string s, vector<string> rules, vector<string> facts, vector<string> rootVar, vector<string> learnVar){

    if (query(s,rules,facts)) {
        
    }

} */

/* bool explain(string s, vector<string> rules, vector<string> facts, vector<string> rootVar, vector<string> learnVar){
    
    bool operand = false;
    for (int a = 0; a < s.length(); a++) {
        if (s[a] == '&') operand = true;
        if (s[a] == '|') operand = true;
        if (s[a] == '!') operand = true;
        if (s[a] == '(') operand = true;
        if (s[a] == ')') operand = true;
    }
    //cout << "operand = " << operand << endl;
    if (!operand) {
        bool inFacts = false;
        for (int b = 0; b <facts.size(); b++){
            if (s == facts[b]) {
                inFacts = true;
            }
        }
        if (inFacts) {
            cout << "I KNOW THAT " << s << endl;
            return true;
        }
        else {
        for(int i = 0; i < rules.size(); i++) { //iterate through rules
            string input, output;
            for (int j = 0; j < rules[i].length(); j++){
                if (rules[i][j] == '-') { //for each rule, determine where to split input and output. should be able to tell based on '-'
                    input = rules[i].substr(0, j-1); //input substring
                    output = rules[i].substr(j+3, rules[i].length() - j - 3); // output substring
                    //cout << "input: "<< input << "\t output: " << output << endl;
                }
                if (s == output) {
                    if (query(input, rules, facts)) {
                        cout << "BECAUSE " << input;
                        bool backward = explain(input, rules, facts, rootVar, learnVar);
                        cout << "I CAN PROVE " << s << " .";
                        return query(input, rules, facts);
                    }
                }
            }
            if (!query(input, rules, facts)) {
                cout << "BECAUSE IT IS NOT TRUE THAT" << input;
                return explain(input, rules, facts, rootVar, learnVar);
                cout << "I CANNOT PROVE " << s << " .";
            }
        }
            //if ()
        //cout << "false = " << s << endl;
        return false;
        }
    }
    return false;

} */

string varValue(string s, vector<string>rootVar, vector<string>learnVar) {
    string answer;
    bool inRoot = false;
    int equalIndex = 0;
    bool operand = false;
    //cout << "in s == initial" << endl;
    
    for (int a = 0; a < s.length(); a++) {
        if (s[a] == '&') operand = true;
        if (s[a] == '|') operand = true;
        if (s[a] == '!') operand = true;
        if (s[a] == '(') operand = true;
        if (s[a] == ')') operand = true;
    }
    if (!operand) {
    for (int i = 0; i < rootVar.size(); i++) {
        for (int j = 0; j < rootVar[i].length(); j++) {
            if (rootVar[i][j] == '=') {
                equalIndex = j;
                //cout << rootVar[i] << endl;
                //cout << rootVar[i].substr(0, equalIndex-1) << endl;
                //cout << s << endl;
            }
        }
        if (rootVar[i].substr(0, equalIndex-1) == s) {
            inRoot = true;
            answer = rootVar[i].substr(equalIndex+3, rootVar[i].length() - equalIndex - 4);
        }
    }
    if (!inRoot) {
        for (int i = 0; i < learnVar.size(); i++) {
            for (int j = 0; j < learnVar[i].length(); j++) {
                if (learnVar[i][j] == '=') {
                    equalIndex = j;
                }
            }
            if (learnVar[i].substr(0, equalIndex-1) == s) {
                inRoot = true;
                answer = learnVar[i].substr(equalIndex+3, learnVar[i].length() - equalIndex - 4);
            }
        }
    }
    }
    else {
        //cout << "in else" << endl;
        bool operand = false;
        
        int startIndex = 0;
        string recurse;
        for (int i = 0; i < s.length(); i++ ){
            operand = false;
            if (s[i] == '&') operand = true;
            if (s[i] == '|') operand = true;
            if (s[i] == '!') operand = true;
            if (s[i] == '(') operand = true;
            if (s[i] == ')') operand = true;
            if (operand) {
                 /* answer += s[i];
                nonLetterIndex = i;
                bool operand2 = false;
                
                for (int j = i + 1; j < s.length(); j++) {
                    if (s[j] == '&') operand2 = true;
                    if (s[j] == '|') operand2 = true;
                    if (s[j] == '!') operand2 = true;
                    if (s[j] == '(') operand2 = true;
                    if (s[j] == ')') operand2 = true;
                    if (operand2) {
                        recurse = s.substr(i+1, j - i -1);
                        answer += varValue(recurse, rootVar, learnVar);
                        answer += s[j];
                        i = j;
                    }
                } */
                recurse = s.substr(startIndex, i - startIndex);
                answer += varValue(recurse, rootVar, learnVar);
                answer +=" ";
                answer += s[i];
                answer +=" ";
                startIndex = i;
            }
        }
        recurse = s.substr(startIndex+1, s.length() - startIndex);
        answer += varValue(recurse, rootVar, learnVar);

    }
    
    return answer;
}

 bool why(string s, string initial, vector<string> rules, vector<string> facts, vector<string> rootVar, vector<string> learnVar) {
     
     
     if (s == initial ) {
         bool operand = false;
         //cout << "in s == initial" << endl;

         for (int a = 0; a < s.length(); a++) {
             if (s[a] == '&') operand = true;
             if (s[a] == '|') operand = true;
             if (s[a] == '!') operand = true;
             if (s[a] == '(') operand = true;
             if (s[a] == ')') operand = true;
         }
         if (!operand) {
             bool inFacts = false;
             for (int b = 0; b <facts.size(); b++){
                 if (s == facts[b]) {
                     inFacts = true;
                 }
             }
             if (inFacts) {
                 cout << "I KNOW THAT " << varValue(s, rootVar, learnVar) << ". ";
                 return true;
             }
             else {
                 bool back = false;
                 for(int i = 0; i < rules.size(); i++) { //iterate through rules
                     string input, output;
                     for (int j = 0; j < rules[i].length(); j++){
                         if (rules[i][j] == '-') { //for each rule, determine where to split input and output. should be able to tell based on '-'
                             input = rules[i].substr(0, j-1); //input substring
                             output = rules[i].substr(j+3, rules[i].length() - j - 3); // output substring
                             //cout << "input: "<< input << "\t output: " << output << endl;
                             if (s == output) {
                                 //cout << "s = output" << endl;
                                 break;
                                 //cout << back << endl;
                             }
                         }
                         
                     }
                     //cout << "in the else of s == initial" << endl;
                     bool conclusion = why(input, initial, rules, facts, rootVar, learnVar);
                     if (conclusion) {
                         //cout << output << endl;
                         cout << "I THUS KNOW THAT " << varValue(initial, rootVar, learnVar) << ". ";
                         rules.push_back(output);
                         //why(output, rules, facts, rootVar, learnVar);
                         return true;
                     }
                     else {
                         //cout << output << endl;
                         cout << "THUS I CANNOT PROVE " << varValue(initial, rootVar, learnVar) << ". ";
                         return false;
                     }
                 }
             }
         }
         else {
             vector<int> openParen;
             vector<int> closeParen;
             string token1, token2;
             for (int i = 0; i < s.length(); i++) {
                 if (s[i] == '(') {
                     openParen.push_back(i); // save index of openParen for substrings
                 }
                 if (s[i] == ')') {
                     closeParen.push_back(i); // save index of closeParen for substrings
                 }
                 if (s[i] == '!'){ //if character is negation
                     if (s[i+1] == '(') { //if next character is (, recurse on substring where ( and ) are cut out
                         int netOpen = 1; //find index by determining where #( = #)
                         for (int j = i+2; j < s.length(); j++) {
                             if (s[j] == ')') {
                                 netOpen -= 1;
                             }
                             if (s[j] == '(') {
                                 netOpen++;
                             }
                             if (netOpen == 0) {
                                 bool bang = !(why(s.substr(i+2, j-i-1), initial, rules, facts, rootVar, learnVar)); //substring to recurse on without outer parentheses
                                 if (bang) {
                                     cout << "BECAUSE IT IS NOT TRUE THAT " << varValue(s, rootVar, learnVar) << " ";
                                     return bang;
                                 }
                                 else {
                                     cout << "BECAUSE" << varValue(s, rootVar, learnVar) << " ";
                                     return bang;
                                 }
                             }
                         }
                     }
                     else {
                         int firstOpen = s.length(); //need to get whole variable name, therefore need to cut right before 1st operand/parenthesis
                         for (int j = i+1; j < s.length(); j++) {
                             if ((j == '(' || j == ')' || j == '&' || j == '|' || j == '!')&& j <= firstOpen) {
                                 firstOpen = j;
                             }
                             bool bang = !(why(s.substr(i+2, j-i-1), initial, rules, facts, rootVar, learnVar)); //substring to recurse on without outer parentheses
                             if (bang) {
                                 cout << "BECAUSE IT IS NOT TRUE THAT" << varValue(s, rootVar, learnVar) << " ";
                                 return bang;
                             }
                             else {
                                 cout << "BECAUSE " << varValue(s, rootVar, learnVar) << " ";
                                 return bang;
                             }
                         }
                         
                     }
                     
                 }
                 if (s[i] == '&' && (openParen.size() == closeParen.size())) { //split based on & character if number of open parentheses = number of closed parentheses
                     int lowIndex = 0; //need to cut token1 regardless of paren
                     int lowParenChange = 0;
                     for (int k = (i-1); k > 0; k-=1){
                         if (s[k] == ')') {
                             lowParenChange -= 1;
                         }
                         if (s[k] == '(') {
                             lowParenChange += 1;
                         }
                         if ((s[k] == '&' || s[k] == '|')&& (lowParenChange >= 0)) {
                             lowIndex = k;
                             break;
                         }
                     }
                     int highIndex = s.size() - 1 ;
                     int highParenChange = 0;
                     for (int k = (i+1); k < s.size(); k+=1){
                         if (s[k] == ')') {
                             highParenChange -= 1;
                         }
                         if (s[k] == '(') {
                             highParenChange += 1;
                         }
                         if ((s[k] == '&' || s[k] == '|')&& (highParenChange <= 0)) {
                             highIndex = k;
                             break;
                         }
                     }
                     token1 = s.substr(lowIndex, i - lowIndex); //cut from lowIndex to &
                     token2 = s.substr(i+1, highIndex-i); //cut from char after & to end
                     if (token1[0] == '(' && token1[token1.length()-1] == ')') {
                         token1 = token1.substr(1,token1.length()-2); //if enclosed by parentheses, delete parentheses
                     }
                     if (token2[0] == '(' && token2[token2.length()-1] == ')') {
                         token2 = token2.substr(1,token2.length()-2); //if enclosed by parentheses, delete parentheses
                     }
                     bool both = (why(token1, initial, rules, facts, rootVar, learnVar) && why(token2, initial, rules, facts, rootVar, learnVar));
                     if (both) {
                         cout << "BECAUSE (" << varValue(token1, rootVar, learnVar) << " AND " << varValue(token2, rootVar, learnVar) << ") ";
                         return both;
                     }
                     else {
                         cout << "BECAUSE IT IS NOT TRUE THAT (" << varValue(token1, rootVar, learnVar) << " AND " << varValue(token2, rootVar, learnVar) << ") ";
                         return both;
                     }
                     
                     //recursive call to both halves of expression
                 }
                 if (s[i] == '|' && (openParen.size() == closeParen.size())) { //split based on & character if number of open parentheses = number of closed parentheses
                     int lowIndex = 0;
                     int lowParenChange = 0;
                     for (int k = (i-1); k > 0; k-=1){
                         if (s[k] == ')') {
                             lowParenChange -= 1;
                         }
                         if (s[k] == '(') {
                             lowParenChange += 1;
                         }
                         if ((s[k] == '&' || s[k] == '|')&& (lowParenChange >= 0)) {
                             lowIndex = k;
                             break;
                         }
                     }
                     int highIndex = s.size() - 1 ;
                     int highParenChange = 0;
                     for (int k = (i+1); k < s.size(); k+=1){
                         if (s[k] == ')') {
                             highParenChange -= 1;
                         }
                         if (s[k] == '(') {
                             highParenChange += 1;
                         }
                         if ((s[k] == '&' || s[k] == '|')&& (highParenChange <= 0)) {
                             highIndex = k;
                             break;
                         }
                     }
                     token1 = s.substr(lowIndex, i - lowIndex); //cut from lowIndex to |
                     token2 = s.substr(i+1, highIndex-i); //cut from char after & to end
                     if (token1[0] == '(' && token1[token1.length()-1] == ')') {
                         token1 = token1.substr(1,token1.length()-2); //if enclosed by parentheses, delete parentheses
                     }
                     if (token2[0] == '(' && token2[token2.length()-1] == ')') {
                         token2 = token2.substr(1,token2.length()-2); //if enclosed by parentheses, delete parentheses
                     }
                     bool either = (why(token1, initial, rules, facts, rootVar, learnVar) || why(token2, initial, rules, facts, rootVar, learnVar));
                     if (either) {
                         cout << "BECAUSE (" << varValue(token1, rootVar, learnVar) << " OR " << varValue(token2, rootVar, learnVar) << ") ";
                         return either;
                     }
                     else {
                         cout << "BECAUSE IT IS NOT TRUE THAT (" << varValue(token1, rootVar, learnVar) << " OR " << varValue(token2, rootVar, learnVar) << ") ";
                         return either;
                     } //recursive call to both halves of expression
                 }
             }
         }
     }
     else {
    bool operand = false;
    for (int a = 0; a < s.length(); a++) {
        if (s[a] == '&') operand = true;
        if (s[a] == '|') operand = true;
        if (s[a] == '!') operand = true;
        if (s[a] == '(') operand = true;
        if (s[a] == ')') operand = true;
    }
    if (!operand) {
        bool inFacts = false;
        for (int b = 0; b <facts.size(); b++){
            if (s == facts[b]) {
                inFacts = true;
            }
        }
        if (inFacts) {
            cout << "I KNOW THAT " << varValue(s, rootVar, learnVar) << ". ";
            return true;
        }
        else {
            bool back = false;
            for(int i = 0; i < rules.size(); i++) { //iterate through rules
                string input, output;
                for (int j = 0; j < rules[i].length(); j++){
                    if (rules[i][j] == '-') { //for each rule, determine where to split input and output. should be able to tell based on '-'
                        input = rules[i].substr(0, j-1); //input substring
                        output = rules[i].substr(j+3, rules[i].length() - j - 3); // output substring
                        //cout << "input: "<< input << "\t output: " << output << endl;
                        if (s == output) {
                            //cout << "s = output" << endl;
                            break;
                            //cout << back << endl;
                        }
                    }
                    
                }
                bool back = query(s, rules, facts);
                if (back) {
                    //cout << output << endl;
                    cout << "BECAUSE IT IS TRUE THAT " << varValue(input, rootVar, learnVar) << ", I CAN PROVE " << varValue(output, rootVar, learnVar) << ". ";
                    rules.push_back(output);
                    //why(output, rules, facts, rootVar, learnVar);
                    return true;
                }
                else {
                    //cout << output << endl;
                    cout << "BECAUSE IT IS NOT TRUE THAT " << varValue(input, rootVar, learnVar) << ", I CANNOT PROVE " << varValue(output, rootVar, learnVar) << ". ";
                    return false;
                }
            }
        }
    }
    vector<int> openParen;
    vector<int> closeParen;
    string token1, token2;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(') {
            openParen.push_back(i); // save index of openParen for substrings
        }
        if (s[i] == ')') {
            closeParen.push_back(i); // save index of closeParen for substrings
        }
        if (s[i] == '!'){ //if character is negation
            if (s[i+1] == '(') { //if next character is (, recurse on substring where ( and ) are cut out
                int netOpen = 1; //find index by determining where #( = #)
                for (int j = i+2; j < s.length(); j++) {
                    if (s[j] == ')') {
                        netOpen -= 1;
                    }
                    if (s[j] == '(') {
                        netOpen++;
                    }
                    if (netOpen == 0) {
                        bool bang = !(why(s.substr(i+2, j-i-1), initial, rules, facts, rootVar, learnVar)); //substring to recurse on without outer parentheses
                        if (bang) {
                            cout << "BECAUSE IT IS NOT TRUE THAT " << varValue(s, rootVar, learnVar) << " ";
                            return bang;
                        }
                        else {
                            cout << "BECAUSE" << varValue(s, rootVar, learnVar) << " ";
                            return bang;
                        }
                    }
                }
            }
            else {
                int firstOpen = s.length(); //need to get whole variable name, therefore need to cut right before 1st operand/parenthesis
                for (int j = i+1; j < s.length(); j++) {
                    if ((j == '(' || j == ')' || j == '&' || j == '|' || j == '!')&& j <= firstOpen) {
                        firstOpen = j;
                    }
                    bool bang = !(why(s.substr(i+2, j-i-1), initial, rules, facts, rootVar, learnVar)); //substring to recurse on without outer parentheses
                    if (bang) {
                        cout << "BECAUSE IT IS NOT TRUE THAT" << varValue(s, rootVar, learnVar) << " ";
                        return bang;
                    }
                    else {
                        cout << "BECAUSE " << varValue(s, rootVar, learnVar) << " ";
                        return bang;
                    }
                }
                
            }
            
        }
        if (s[i] == '&' && (openParen.size() == closeParen.size())) { //split based on & character if number of open parentheses = number of closed parentheses
            int lowIndex = 0; //need to cut token1 regardless of paren
            int lowParenChange = 0;
            for (int k = (i-1); k > 0; k-=1){
                if (s[k] == ')') {
                    lowParenChange -= 1;
                }
                if (s[k] == '(') {
                    lowParenChange += 1;
                }
                if ((s[k] == '&' || s[k] == '|')&& (lowParenChange >= 0)) {
                    lowIndex = k;
                    break;
                }
            }
            int highIndex = s.size() - 1 ;
            int highParenChange = 0;
            for (int k = (i+1); k < s.size(); k+=1){
                if (s[k] == ')') {
                    highParenChange -= 1;
                }
                if (s[k] == '(') {
                    highParenChange += 1;
                }
                if ((s[k] == '&' || s[k] == '|')&& (highParenChange <= 0)) {
                    highIndex = k;
                    break;
                }
            }
            token1 = s.substr(lowIndex, i - lowIndex); //cut from lowIndex to &
            token2 = s.substr(i+1, highIndex-i); //cut from char after & to end
            if (token1[0] == '(' && token1[token1.length()-1] == ')') {
                token1 = token1.substr(1,token1.length()-2); //if enclosed by parentheses, delete parentheses
            }
            if (token2[0] == '(' && token2[token2.length()-1] == ')') {
                token2 = token2.substr(1,token2.length()-2); //if enclosed by parentheses, delete parentheses
            }
            bool both = (why(token1, initial, rules, facts, rootVar, learnVar) && why(token2, initial, rules, facts, rootVar, learnVar));
            if (both) {
                cout << "BECAUSE (" << varValue(token1, rootVar, learnVar) << " AND " << varValue(token2, rootVar, learnVar) << ") ";
                return both;
            }
            else {
                cout << "BECAUSE IT IS NOT TRUE THAT (" << varValue(token1, rootVar, learnVar) << " AND " << varValue(token2, rootVar, learnVar) << ") ";
                return both;
            }

            //recursive call to both halves of expression
        }
        if (s[i] == '|' && (openParen.size() == closeParen.size())) { //split based on & character if number of open parentheses = number of closed parentheses
            int lowIndex = 0;
            int lowParenChange = 0;
            for (int k = (i-1); k > 0; k-=1){
                if (s[k] == ')') {
                    lowParenChange -= 1;
                }
                if (s[k] == '(') {
                    lowParenChange += 1;
                }
                if ((s[k] == '&' || s[k] == '|')&& (lowParenChange >= 0)) {
                    lowIndex = k;
                    break;
                }
            }
            int highIndex = s.size() - 1 ;
            int highParenChange = 0;
            for (int k = (i+1); k < s.size(); k+=1){
                if (s[k] == ')') {
                    highParenChange -= 1;
                }
                if (s[k] == '(') {
                    highParenChange += 1;
                }
                if ((s[k] == '&' || s[k] == '|')&& (highParenChange <= 0)) {
                    highIndex = k;
                    break;
                }
            }
            token1 = s.substr(lowIndex, i - lowIndex); //cut from lowIndex to |
            token2 = s.substr(i+1, highIndex-i); //cut from char after & to end
            if (token1[0] == '(' && token1[token1.length()-1] == ')') {
                token1 = token1.substr(1,token1.length()-2); //if enclosed by parentheses, delete parentheses
            }
            if (token2[0] == '(' && token2[token2.length()-1] == ')') {
                token2 = token2.substr(1,token2.length()-2); //if enclosed by parentheses, delete parentheses
            }
            bool either = (why(token1, initial, rules, facts, rootVar, learnVar) || why(token2, initial, rules, facts, rootVar, learnVar));
            if (either) {
                cout << "BECAUSE (" << varValue(token1, rootVar, learnVar) << " OR " << varValue(token2, rootVar, learnVar) << ") ";
                return either;
            }
            else {
                cout << "BECAUSE IT IS NOT TRUE THAT (" << varValue(token1, rootVar, learnVar) << " OR " << varValue(token2, rootVar, learnVar) << ") ";
                return either;
            } //recursive call to both halves of expression
        }
    }
        
    }
    return false;
    
}

bool conclude(string s, vector<string> rules, vector<string> facts, vector<string> rootVar, vector<string> learnVar) {
    if (why (s, s, rules, facts, rootVar, learnVar)) {
        cout << "I THUS KNOW THAT " << varValue(s, rootVar, learnVar) << endl;
    }
    else {
        cout << "THUS I CANNOT PROVE " << varValue(s, rootVar, learnVar) << endl;
    }
    return true;
}

int main() {
    string first;
    string second;
    string third;
    string fourth;
    string command;
    string status;
    string name;
    string result;
    string rule;
    //seems unnecessary
    char equalssign;
    string description;
    string expression;
    vector<string> rootVar;
    vector<string> learnVar;
    vector<string> facts;
    vector<string> rules;
    string keep_going;
    
    while (1) {
        //cout << "What would you like to do?" << endl;
        cin >> first;
        if (first == "Teach") {
            command = first;
            cin >> second;
            cin >> third;
            //Teach#1 (root/learned variable)
            if (second == "-R" || second == "-L") {
                status = second;
                name = third;
                cin >> equalssign;
                description = "";
                int index;
                char check = '0';
                int i = 0;
                //handling values of multiple words. e.g. "Sam Likes Ice Cream"
                while(check != '"') {
                    cin >> fourth;
                    index = fourth.size()-1;
                    check = fourth[index];
                    description = description + fourth + " ";
                    i++;
                }
                string output = name + " " + equalssign + " " + description;
                
                bool b = true;
                bool c = true;
                
                //check if variable to be added is already in either rootVar or learnVar
                
                for (int i = 0; i < rootVar.size(); i++) {
                    int index = rootVar[i].size()-1;
                    for (int k = 0; k < rootVar[i].size(); k++) {
                        if (rootVar[i][k] == '=') {
                            index = k;
                            break;
                        }
                    }
                    if (rootVar[i].substr(0, index-1) == name) {
                        b = false;
                    }
                }
                
                for (int i = 0; i < learnVar.size(); i++) {
                    int index = learnVar[i].size()-1;
                    for (int k = 0; k < learnVar[i].size(); k++) {
                        if (learnVar[i][k] == '=') {
                            index = k;
                            break;
                        }
                    }
                    if (learnVar[i].substr(0, index-1) == name) {
                        c = false;
                    }
                }
                
                if (second == "-R") {
                    if (b == true && c == true) {
                        rootVar.push_back(output);
                    }
                }
                else {
                    if ( b == true && c == true) {
                        learnVar.push_back(output);
                    }
                }
            }
            //Teach#3 (rule)
            else if (third == "->") {
                expression = second;
                cin >> fourth;
                result = fourth;
                rule = expression + " " + third + " " + result;
                rules.push_back(rule);
            }
            //Teach#2
            else {
                name = second;
                cin >> fourth;
                status = fourth;
                for (int i = 0; i < rootVar.size(); i++) {
                    int index = rootVar[i].size()-1;
                    for (int k = 0; k < rootVar[i].size(); k++) {
                        if (rootVar[i][k] == '=') {
                            index = k;
                            break;
                        }
                    }
                    bool d = true;
                    if (rootVar[i].substr(0, index-1) == name) {
                        if (status == "true"){
                            for (int i = 0; i < facts.size(); i++) {
                                if (facts[i] == name) {
                                    d = false;
                                }
                            }
                            if (d == true) {
                                facts.push_back(name);
                            }
                        }
                        else if (status == "false") {
                            
                            for (int i = 0; i < facts.size(); i++) {
                                for (int k = 0; k < learnVar.size(); k++) {
                                    int index = learnVar[k].size()-1;
                                    for (int l = 0; l <learnVar[k].size(); l++) {
                                        if (learnVar[k][l] == '=') {
                                            index = l;
                                            break;
                                        }
                                    }
                                    string var = learnVar[k].substr(0, index-1);
                                    if (facts[i] == var) {
                                        facts.erase(remove(facts.begin(), facts.end(), facts[i]), facts.end());
                                    }
                                }
                            }
                            for (int j = 0; j < facts.size(); j ++){
                                if (name == facts[j]) {
                                    facts.erase(remove(facts.begin(), facts.end(), facts[j]), facts.end());
                                }
                            }
                            
                        }
                        else {
                            cout << "Invalid input. Must say 'true' or 'false'." << endl;
                        }
                    }
                }
                
                for (int i = 0; i < learnVar.size(); i++) {
                    int index = learnVar[i].size()-1;
                    for (int k = 0; k < learnVar[i].size(); k++) {
                        if (learnVar[i][k] == '=') {
                            index = k;
                            break;
                        }
                    }
                    if (learnVar[i].substr(0, index-1) == name) {
                        cout << "ERROR MESSAGE: Can't set a learned variable to be true or false" << endl;
                        break;
                    }
                }
                
            }
        }
        else if (first == "List") {
            print(rootVar, learnVar, facts, rules);
        }
        else if (first == "Learn") {
            learn(rules, facts);
        }
        else if (first == "Query") {
            cin  >> second;
            if (query(second, rules, facts)) {
                cout << "true" << endl;
            }
            else {
                cout << "false" << endl;
            }
        }
        else if (first == "Why") {
            //cout << first << endl;
            cin  >> second;
            if (query(second, rules, facts)) {
                cout << "true" << endl;
            }
            else {
                cout << "false" << endl;
            }
            //bool EXPLAIN = why(second, second, rules, facts, rootVar, learnVar);
            bool explain = conclude(second,rules,facts,rootVar,learnVar);
            cout << endl;
            //cout << varValue("S", rootVar, learnVar) << endl;
        }
        else {
            break;
            //cout << "Invalid command. What would you like to do?" << endl;
            //TO DO: handle this with a loop
        }
    }
    return 0;
}



