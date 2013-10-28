/*
	Name: Karl Anthony James
	E-Mail: kjames21@uco.edu
	Assignment: a03 (PDA)
	Due: 10/28/2013
	Class Time: Theory of Computing M/W 5:45 PM
*/

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;

#include "PDA.h"

void PDA::ReadPDA(ifstream& inputFile){
	getline(inputFile, this->pdaMetadata.comment);
	getline(inputFile, this->pdaMetadata.alphabet);
	FixNewLine(this->pdaMetadata.alphabet);//I've got to take into account the \r used in windows files.

	for(int i = 0; i < this->pdaMetadata.alphabet.size(); i++){
		PDAState state;
		state.stateMap = map<int,int>();

		this->pdaMetadata.pdaMap.insert(pair<char,PDAState>(this->pdaMetadata.alphabet[i],state));
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
				this->pdaMetadata.pdaMap[this->pdaMetadata.alphabet[alphaIndex++]].stateMap.insert(std::pair<int,int>(i,nextState));
			}
		}
	}

	string startStateString;
	getline(inputFile, startStateString);
	this->pdaMetadata.startState = atoi(startStateString.c_str());

	for(int i = 0; i < stateCount; i++){
		getline(inputFile, stateMapLine);
		int configValues[3];
		int configIndex = 0;
		for(int j = 0; j < stateMapLine.size(); j++){
			string stateString = "";
			while(stateMapLine[j] != ' ' && j < stateMapLine.size()){
				stateString += stateMapLine[j];
				j++;
			}
			FixNewLine(stateString);
			if(!WindowsSafeEmpty(stateString) && configIndex <= 2){
				int nextConfig= atoi(stateString.c_str());
				configValues[configIndex++] = nextConfig;
			}
		}

		PDAStateConfig config;
		config.accepting = configValues[0] == 1;
		config.failing = configValues[1] == 1;
		config.operation = configValues[2];
		this->pdaMetadata.stateConfig[i] = config;
	}

	string testLine;
	while(getline(inputFile, testLine)){
		FixNewLine(testLine);
		if(WindowsSafeEmpty(testLine)) continue;

		PDAEvaluation eval;
		eval.input = testLine;
		eval.state = this->pdaMetadata.startState;
		eval.success = false;

		this->evaluations.push_back(eval);
	}
}

void PDA::EvaluatePDA(){
	for(int i = 0; i < this->evaluations.size(); i++){
		PDAEvaluation &eval = this->evaluations[i];
		stack<char> inputStack;
		stack<char> evalStack;

		eval.state = this->pdaMetadata.startState;
		for(int j = eval.input.size() - 1; j >= 0; j--){
			inputStack.push(eval.input[j]);
		}

		this->Evaluate(this->pdaMetadata.startState, eval, inputStack, 'D', evalStack);
	}
}

void PDA::Evaluate(int state, PDAEvaluation& eval, stack<char>& inputStack, char input, stack<char>& evalStack){
	PDAStateConfig config = this->pdaMetadata.stateConfig[state];
	eval.state = state;
	char nextInput;
	if(config.accepting){
		eval.success = config.accepting;
		return;
	}
	else if(config.failing && config.operation == 0){
		eval.success = false;
		return;
	}
	else{
		switch(config.operation){
		case 0:
			break;
		case 1:
			if(!inputStack.empty()){
				input = inputStack.top();
				inputStack.pop();
			}
			else{
				input = 'D';
			}
			break;
		case 2:
			evalStack.push(input);
			break;
		case 3:
			if(!evalStack.empty()){
				input = evalStack.top();
				evalStack.pop();
			}
			else{
				input = 'D';
			}
			break;
		}

		this->Evaluate(this->pdaMetadata.pdaMap[input].stateMap[state], eval, inputStack, input, evalStack);
	}
}

void PDA::PrintResult(ostream &output){
	for(int i = 0; i < this->evaluations.size(); i++){
		output << this->evaluations[i].input << endl;
		output << (this->evaluations[i].success ? "ACCEPT" : "REJECT") << endl;
		output << endl;
	}
}

void PDA::FixNewLine(string &input){
	if(input != ""){
		//Check for the newline character
		if(input[0] == '\n') input = input.substr(1);
		if(input[input.size()-1] == '\n') input = input.substr(0, input.size()-1);

		//Check for the \r (sometimes used in windows)
		if(input[0] == '\r') input = input.substr(1);
		if(input[input.size()-1] == '\r') input = input.substr(0, input.size()-1);
	}
}

bool PDA::WindowsSafeEmpty(string input){
	if(input.empty() || input == "" || input.size() == 0 || input == "\n" || input == "\r") 
		return true;
	else
		return false;
}