/*
	Name: Karl Anthony James
	E-Mail: kjames21@uco.edu
	Assignment: a02 (TG)
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

struct TGState {
	map<int,int> stateMap;
};

struct TGMetadata {
	string comment;
	string alphabet;
	int startState;
	map<int,bool> stateSuccess;
	map<string, TGState> tgMap;
};

struct TGEvaluation {
	string input;
	int state;
	bool success;
};

struct Exception{
	Exception(string message){
		cout << endl << message << endl;
	}
};

class TG {
public:
	void ReadTG(ifstream& inputFile);
	void EvaluateTG();
	void PrintResult(ostream& output);
private:
	string path;
	TGMetadata tgMetadata;
	vector<TGEvaluation> evaluations;
	int evaluatePortion(int start, int end, int state, string& evaluation);
	void FixNewLine(string &input);
	bool WindowsSafeEmpty(string input);
};