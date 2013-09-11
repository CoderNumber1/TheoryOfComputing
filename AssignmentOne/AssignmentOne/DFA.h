/*
Name: Karl Anthony James
E-Mail: kjames21@uco.edu
Created: 9/8/2013
Description: DFA.h is meant to read DFA files and evaluate strings 
	for adherance to a language definition.
*/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

struct DFAState {
	map<int,int> stateMap;
};

struct DFAMetadata {
	string comment;
	string alphabet;
	int startState;
	map<int,bool> stateSuccess;
	map<char, DFAState> dfaMap;
};

struct DFAEvaluation {
	string input;
	int state;
	bool success;
};

struct Exception{
	Exception(string message){
		cout << endl << message << endl;
	}
};

class DFA {
public:
	void ReadDFA(ifstream& inputFile);
	void EvaluateDFA();
	void PrintResult(ostream& output);
private:
	string path;
	DFAMetadata dfaMetadata;
	vector<DFAEvaluation> evaluations;
	void FixNewLine(string &input);
	bool WindowsSafeEmpty(string input);
};