//Xuan Tan, Ming-Hao Lee
//Project Phase 2.1: Scanner for expressions
#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include "Token.h"
#include "Node.h"
#include "AST.h"
using namespace std;

const int array_max = 100; // array max length is 100

string checkMatch(string a, regex i, regex n, regex s, regex k){
    if(regex_match(a, k)) {
        return "KEYWORD";
    }
    else if(regex_match(a, i)){
        return "IDENTIFIER";
    }
    else if(regex_match(a, n)){
        return "NUMBER";
    }
     else if(regex_match(a, s)){
        return "SYMBOL";
    }
    else{
        return "ERROR";
    }
}

void extractTokens(string str, vector<string>& list){
    // word variable to store word
    string word;
    // making a string stream
    stringstream iss(str);
    // Read and print each word.
    while (iss >> word) {
      list.push_back(word);
    }
}

int main(int argc, char *argv[]) {
    ifstream inputFile(argv[1]);
    ofstream outputFile(argv[2]);

    string lines[array_max];
    //string tokens[array_max][array_max];
    string currLine;
    int tokenIndex = 0;
    int lineCount = 0;
    vector<int> tokensInLine;
    vector<string> errorList;
    vector<string> realTokens;
    string errorChar;
    string errorString = "ERROR READING: ";
    vector<Token> token;
	Node* root;
	

    regex IDENTIFIER("([a-z]|[A-Z])([a-z]|[A-Z]|[0-9])*"); // set the rules of regex 
    regex NUMBER("[0-9]+");                                // set the rules of regex 
    regex SYMBOL("[\\+ | \\- | \\* | \\/ | \\( | \\) | \\:= | \\;]+");
    regex KEYWORD("if|then|else|endif|while|do|endwhile|skip");


    while (!(inputFile.peek() == EOF)){
        getline(inputFile, lines[lineCount]);
        lineCount++;
    }
    vector<vector<string> > tokens(lineCount);

    //get unprocessed token
    for(int i=0; i < lineCount; i++){
        currLine = lines[i];
        extractTokens(currLine, tokens[i]);
    }

    //process token
    for(int i = 0; i < tokens.size(); i++){
        for (int j = 0; j < tokens[i].size(); j++) {
            if (checkMatch(tokens[i][j], IDENTIFIER, NUMBER, SYMBOL, KEYWORD) != "ERROR") {
                realTokens.push_back(tokens[i][j]);
            }
            else {
                string temp;
                string sub;
                string lasttemp;
                int z = 0;
                if (tokens[i][j].length() > 1) {
                    while (z < tokens[i][j].length()) {
                        lasttemp = temp;
                        string toCheck(1, tokens[i][j].at(z));
                        temp = temp + toCheck;
                        sub = tokens[i][j].substr(z + 1, tokens[i][j].length() - z);
                        if (temp.length() == 1) {
                            if (checkMatch(temp, IDENTIFIER, NUMBER, SYMBOL, KEYWORD) == "ERROR") {
                                errorString = errorString + "\"" + temp + "\" ";
                                temp = "";
                                if (checkMatch(sub, IDENTIFIER, NUMBER, SYMBOL, KEYWORD) != "ERROR") {
                                    realTokens.push_back(sub);
                                    sub = "";
                                    break;
                                }
                            }
                        }
                        else {
                            if (checkMatch(lasttemp, IDENTIFIER, NUMBER, SYMBOL, KEYWORD) != "ERROR") {
                                if (checkMatch(temp, IDENTIFIER, NUMBER, SYMBOL, KEYWORD) == "ERROR") {
                                    if (checkMatch(toCheck, IDENTIFIER, NUMBER, SYMBOL, KEYWORD) != "ERROR") {
                                        realTokens.push_back(lasttemp);
                                        lasttemp = "";
                                        temp = toCheck;
                                    }
                                    else {
                                        realTokens.push_back(lasttemp);
                                        temp = "";
                                        errorString = errorString + "\"" + tokens[i][j].at(z) + "\" ";
                                        if (checkMatch(sub, IDENTIFIER, NUMBER, SYMBOL, KEYWORD) != "ERROR") {
                                            realTokens.push_back(sub);
                                            sub = "";
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        z++;
                    }
                    if (temp != "") {
                        realTokens.push_back(temp);
                        temp = "";
                    }
                }
                else {
                    errorString = errorString + "\"" + tokens[i][j].at(z) + "\" ";
                }
                
            }

        }
        //outputFile << "Tokens: " << endl;
        for(int z = 0; z < realTokens.size(); z++) {
            outputFile << realTokens.at(z) << " : " << checkMatch(realTokens.at(z), IDENTIFIER, NUMBER, SYMBOL, KEYWORD) << endl;
            Token t(realTokens.at(z), checkMatch(realTokens.at(z), IDENTIFIER, NUMBER, SYMBOL, KEYWORD));
            token.push_back(t);
        }
        realTokens.clear();
        /*if (errorString != "ERROR READING: ") {
            outputFile << errorString << endl;
            errorString = "ERROR READING: ";
        }*/
        //outputFile << endl;
    }
    //for (int i = 0; i < token.size(); i++) {
		outputFile << "AST: " << endl << endl;
		root = parse(token);
		printAST(root, 0, outputFile); // print the AST 
	//}

    return 0;
}