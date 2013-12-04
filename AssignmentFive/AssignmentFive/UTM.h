/*
	Name: Karl Anthony James
	E-Mail: kjames21@uco.edu
	Assignment: a05 (UTM)
	Due: 12/4/2013
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

struct UTMStateAction{
	char replaceWith;
	Direction direction;
	int nextState;
};

struct UTMState {
	map<char,UTMStateAction> actions;
};

struct UTMMetadata {
	string comment;
	int startState;
	int stopState;
	map<int, UTMState> utmMap;
};

struct UTMEvaluation {
	string input;
	int state;
	bool success;
};

struct Exception{
	Exception(string message){
		cout << endl << message << endl;
	}
};

class UTM {
public:
	void ReadUTM(ifstream& inputFile);
	void EvaluateUTM();
	void PrintResult(ostream& output);
private:
	string path;
	UTMMetadata utmMetadata;
	vector<UTMEvaluation> evaluations;
	int evaluatePortion(int start, int end, int state, string& evaluation);
	void FixNewLine(string &input);
	bool WindowsSafeEmpty(string input);
};