/*
	Name: Karl Anthony James
	E-Mail: kjames21@uco.edu
	Assignment: a04 (TM)
	Due: 9/25/2013
	Class Time: Theory of Computing M/W 5:45 PM
*/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

enum Direction { Left, Right };

struct TMStateAction{
	char replaceWith;
	Direction direction;
	int nextState;
};

struct TMState {
	map<char,TMStateAction> actions;
};

struct TMMetadata {
	string comment;
	string alphabet;
	int startState;
	map<int,bool> stateSuccess;
	map<int, TMState> tmMap;
};

struct TMEvaluation {
	string input;
	int state;
	bool success;
};

struct Exception{
	Exception(string message){
		cout << endl << message << endl;
	}
};

class TM {
public:
	void ReadTM(ifstream& inputFile);
	void EvaluateTM();
	void PrintResult(ostream& output);
private:
	string path;
	TMMetadata tmMetadata;
	vector<TMEvaluation> evaluations;
	int evaluatePortion(int start, int end, int state, string& evaluation);
	void FixNewLine(string &input);
	bool WindowsSafeEmpty(string input);
};