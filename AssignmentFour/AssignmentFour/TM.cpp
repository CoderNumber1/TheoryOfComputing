/*
	Name: Karl Anthony James
	E-Mail: kjames21@uco.edu
	Assignment: a04 (TM)
	Due: 9/25/2013
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

#include "TM.h"

void TM::ReadTM(ifstream& inputFile){
	getline(inputFile, this->tmMetadata.comment);
	getline(inputFile, this->tmMetadata.alphabet);
	FixNewLine(this->tmMetadata.alphabet);//I've got to take into account the \r used in windows files.

	int stateCount = 0;
	int edgeCount = 0;
	string stateMapLine;
	inputFile >> stateCount;
	getline(inputFile, stateMapLine);
	/*inputFile >> edgeCount;
	getline(inputFile, stateMapLine);*/
	for(;;){
		getline(inputFile, stateMapLine);
		int alphaIndex = 0;
		int s1=-1,s2,j=0;
		string triggerString = "";
		
		string stateString = "";
		while(stateMapLine[j] != ' ' && j < stateMapLine.size()){
			stateString += stateMapLine[j];
			j++;
		}
		FixNewLine(stateString);
		if(!WindowsSafeEmpty(stateString)){
			s1 = atoi(stateString.c_str());
		}

		if(s1 < 0)
			break;

		j++;
		stateString = "";
		while(stateMapLine[j] != ' ' && j < stateMapLine.size()){
			stateString += stateMapLine[j];
			j++;
		}
		FixNewLine(stateString);
		if(!WindowsSafeEmpty(stateString)){
			s2 = atoi(stateString.c_str());
		}

		triggerString = stateMapLine.substr(++j);
		FixNewLine(triggerString);

		TMStateAction action;
		action.replaceWith = triggerString[1];
		action.direction = triggerString[2] == 'r' ? Right : Left;
		action.nextState = s2;
		if(this->tmMetadata.tmMap.find(s1) == this->tmMetadata.tmMap.end()){
			TMState state;
			state.actions.insert(pair<char,TMStateAction>(triggerString[0],action));
			this->tmMetadata.tmMap.insert(pair<int,TMState>(s1,state));
		}
		else{
			if(this->tmMetadata.tmMap[s1].actions.find(triggerString[0]) == this->tmMetadata.tmMap[s1].actions.end()){
				this->tmMetadata.tmMap[s1].actions.insert(pair<char,TMStateAction>(triggerString[0],action));
			}
		}
	}

	string startStateString;
	getline(inputFile, startStateString);
	this->tmMetadata.startState = atoi(startStateString.c_str());

	this->tmMetadata.stateSuccess = map<int,bool>();
	for(int i = 0; i <= stateCount; i++){
		this->tmMetadata.stateSuccess.insert(std::pair<int,bool>(i,false));
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
			this->tmMetadata.stateSuccess[state] = true;
		}
	}

	string testLine;
	while(getline(inputFile, testLine)){
		FixNewLine(testLine);
		if(WindowsSafeEmpty(testLine)) continue;

		TMEvaluation eval;
		eval.input = testLine;
		eval.state = this->tmMetadata.startState;
		eval.success = false;

		this->evaluations.push_back(eval);
	}
}

void TM::EvaluateTM(){

	for(int i = 0; i < this->evaluations.size(); i++){
		TMEvaluation &eval = this->evaluations[i];
		eval.state = this->tmMetadata.startState;

		string test = eval.input;
		int index = 0;
		for(;;){
			char head = index < test.size() ? test[index] : 'D';
			if(this->tmMetadata.tmMap[eval.state].actions.find(head) != this->tmMetadata.tmMap[eval.state].actions.end()){
				TMStateAction action = this->tmMetadata.tmMap[eval.state].actions[head];
				if(action.direction == Left && index == 0)
					break;
				else{
					test[index] = action.replaceWith;
					index += action.direction == Right ? 1 : -1;
					eval.state = action.nextState;
				}
			}
			else
				break;
		}

		if(eval.state >= 0){
			eval.success = this->tmMetadata.stateSuccess[eval.state] ? true : false;
		}
		else{
			eval.success = false;
		}
	}
}

void TM::PrintResult(ostream &output){
	for(int i = 0; i < this->evaluations.size(); i++){
		output << this->evaluations[i].input << endl;
		output << (this->evaluations[i].success ? "ACCEPT" : "REJECT") << endl;
		output << endl;
	}
}

void TM::FixNewLine(string &input){
	if(input != ""){
		//Check for the newline character
		if(input[0] == '\n') input = input.substr(1);
		if(input[input.size()-1] == '\n') input = input.substr(0, input.size()-1);

		//Check for the \r (sometimes used in windows)
		if(input[0] == '\r') input = input.substr(1);
		if(input[input.size()-1] == '\r') input = input.substr(0, input.size()-1);
	}
}

bool TM::WindowsSafeEmpty(string input){
	if(input.empty() || input == "" || input.size() == 0 || input == "\n" || input == "\r") 
		return true;
	else
		return false;
}