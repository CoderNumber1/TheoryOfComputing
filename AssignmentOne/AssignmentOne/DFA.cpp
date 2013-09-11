
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;

#include "DFA.h"

void DFA::ReadDFA(ifstream& inputFile){
	getline(inputFile, this->dfaMetadata.comment);
	getline(inputFile, this->dfaMetadata.alphabet);
	FixNewLine(this->dfaMetadata.alphabet);//I've got to take into account the \r used in windows files.

	for(int i = 0; i < this->dfaMetadata.alphabet.size(); i++){
		DFAState state;
		state.stateMap = map<int,int>();

		this->dfaMetadata.dfaMap.insert(pair<char,DFAState>(this->dfaMetadata.alphabet[i],state));
	}
	
	int stateCount = 0;
	string stateMapLine;
	inputFile >> stateCount;
	getline(inputFile, stateMapLine);
	for(int i = 0; i < stateCount; i++){
		getline(inputFile, stateMapLine);
		int alphaIndex = 0;
		for(int j = 0; j < stateMapLine.size(); j++){
			string stateString = "";
			while(stateMapLine[j] != ' ' && j < stateMapLine.size()){
				stateString += stateMapLine[j];
				j++;
			}
			FixNewLine(stateString);
			if(!WindowsSafeEmpty(stateString)){
				int nextState = atoi(stateString.c_str());
				this->dfaMetadata.dfaMap[this->dfaMetadata.alphabet[alphaIndex++]].stateMap.insert(std::pair<int,int>(i+1,nextState));
			}
		}
	}

	string startStateString;
	getline(inputFile, startStateString);
	this->dfaMetadata.startState = atoi(startStateString.c_str());

	this->dfaMetadata.stateSuccess = map<int,bool>();
	for(int i = 1; i <= stateCount; i++){
		this->dfaMetadata.stateSuccess.insert(std::pair<int,bool>(i,false));
	}

	string numGoodStatesString;
	int numGoodStates;
	string goodStateDesc;
	getline(inputFile, goodStateDesc);
	for(int i = 0; i < goodStateDesc.find(' '); i++){
		numGoodStatesString += goodStateDesc[i];
	}
	numGoodStates = atoi(numGoodStatesString.c_str());
	
	for(int i = goodStateDesc.find(' ') + 1; i < goodStateDesc.size(); i++){
		string stateString = "";
		while(goodStateDesc[i] != ' ' && i < goodStateDesc.size()){
			stateString += goodStateDesc[i];
			i++;
		}
		if(!WindowsSafeEmpty(stateString)){
			int state = atoi(stateString.c_str());
			this->dfaMetadata.stateSuccess[state] = true;
		}
	}

	string testLine;
	while(getline(inputFile, testLine)){
		FixNewLine(testLine);
		if(WindowsSafeEmpty(testLine)) continue;

		DFAEvaluation eval;
		eval.input = testLine;
		eval.state = this->dfaMetadata.startState;
		eval.success = false;

		this->evaluations.push_back(eval);
	}
}

void DFA::EvaluateDFA(){

	for(int i = 0; i < this->evaluations.size(); i++){
		DFAEvaluation &eval = this->evaluations[i];
		eval.state = this->dfaMetadata.startState;
		for(int j = 0; j < eval.input.size(); j++){
			int nextState = this->dfaMetadata.dfaMap[eval.input[j]].stateMap[eval.state];
			if(nextState > 0){
				eval.state = this->dfaMetadata.dfaMap[eval.input[j]].stateMap[eval.state];
			}
			else{
				throw Exception("Invalid state or alphabet letter.");
			}
		}

		eval.success = this->dfaMetadata.stateSuccess[eval.state] ? true : false;
	}
}

void DFA::PrintResult(ostream &output){
	for(int i = 0; i < this->evaluations.size(); i++){
		output << this->evaluations[i].input << endl;
		output << (this->evaluations[i].success ? "ACCEPT" : "REJECT") << endl;
		output << endl;
	}
}

void DFA::FixNewLine(string &input){
	if(input != ""){
		//Check for the newline character
		if(input[0] == '\n') input = input.substr(1);
		if(input[input.size()-1] == '\n') input = input.substr(0, input.size()-1);

		//Check for the \r (sometimes used in windows)
		if(input[0] == '\r') input = input.substr(1);
		if(input[input.size()-1] == '\r') input = input.substr(0, input.size()-1);
	}
}

bool DFA::WindowsSafeEmpty(string input){
	if(input.empty() || input == "" || input.size() == 0 || input == "\n" || input == "\r") 
		return true;
	else
		return false;
}