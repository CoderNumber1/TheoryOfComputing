/*
	Name: Karl Anthony James
	E-Mail: kjames21@uco.edu
	Assignment: a03 (PDA)
	Due: 10/28/2013
	Class Time: Theory of Computing M/W 5:45 PM
*/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <stack>
#include <vector>

using namespace std;

struct PDAState {
	map<int,int> stateMap;
};

struct PDAStateConfig{
	bool accepting;
	bool failing;
	int operation;
};

struct PDAMetadata {
	string comment;
	string alphabet;
	int startState;
	map<int,PDAStateConfig> stateConfig;
	map<char, PDAState> pdaMap;
};

struct PDAEvaluation {
	string input;
	int state;
	bool success;
};

struct Exception{
	Exception(string message){
		cout << endl << message << endl;
	}
};

class PDA {
public:
	void ReadPDA(ifstream& inputFile);
	void EvaluatePDA();
	void PrintResult(ostream& output);
private:
	string path;
	PDAMetadata pdaMetadata;
	vector<PDAEvaluation> evaluations;
	void FixNewLine(string &input);
	bool WindowsSafeEmpty(string input);
	void Evaluate(int state, PDAEvaluation& eval, stack<char>& inputStack, char input, stack<char>& evalStack);
};