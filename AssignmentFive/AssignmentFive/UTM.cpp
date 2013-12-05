/*
	Name: Karl Anthony James
	E-Mail: kjames21@uco.edu
	Assignment: a05 (UTM)
	Due: 12/4/2013
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

#include "UTM.h"

void UTM::ReadUTM(ifstream& inputFile){
	getline(inputFile, this->utmMetadata.comment);

	string beginEndString;
	getline(inputFile, beginEndString);
	string stateString = "";
	int j = 0;
	while (beginEndString[j] != ' ' && j < beginEndString.size()){
		stateString += beginEndString[j];
		j++;
	}
	FixNewLine(stateString);
	if (!WindowsSafeEmpty(stateString)){
		this->utmMetadata.startState = atoi(stateString.c_str());
	}

	stateString = "";
	while (beginEndString[j] == ' ' && j < beginEndString.size()){ j++; }
	
	while (beginEndString[j] != ' ' && j < beginEndString.size()){
		stateString += beginEndString[j];
		j++;
	}
	FixNewLine(stateString);
	if (!WindowsSafeEmpty(stateString)){
		this->utmMetadata.stopState = atoi(stateString.c_str());
	}

	bool doneReadingConfig = false;
	string stateMapLine;
	while (getline(inputFile, stateMapLine)){
		int i = 0;
		while (i < stateMapLine.size() && stateMapLine[i] != '\n' && stateMapLine[i] != '\r'){
			while (i < stateMapLine.size() && stateMapLine[i] != '#' && stateMapLine[i] != '\n' && stateMapLine[i] != '\r' && !doneReadingConfig){
				string configEvalLine = "";
				int from = 0;
				while (i < stateMapLine.size() && stateMapLine[i] != 'b'){

					if (stateMapLine[i] == 'a')
						from++;

					configEvalLine += stateMapLine[i];
					i++;
				}
				configEvalLine += stateMapLine[i];
				i++;

				int to = 0;
				while (i < stateMapLine.size() && stateMapLine[i] != 'b'){

					if (stateMapLine[i] == 'a')
						to++;

					configEvalLine += stateMapLine[i];
					i++;
				}
				configEvalLine += stateMapLine[i];
				i++;

				string read = "";
				int readEnd = (i + 3) > stateMapLine.size() ? stateMapLine.size() : i + 3;
				for (; i < readEnd; i++){
					configEvalLine += stateMapLine[i];
					read += stateMapLine[i];
				}
				//configEvalLine += stateMapLine[i];
				
				if (read == "aaa")
					read = "a";
				else if (read == "aab")
					read = "b";
				else if (read == "aba")
					read = "D";
				else if (read == "abb")
					read = "#";
				else if (read == "baa")
					read = "A";
				else if (read == "bab")
					read = "B";

				string write = "";
				int writeEnd = (i + 3) > stateMapLine.size() ? stateMapLine.size() : i + 3;
				for (; i < writeEnd; i++){
					configEvalLine += stateMapLine[i];
					write += stateMapLine[i];
				}
				//configEvalLine += stateMapLine[i];
				
				if (write == "aaa")
					write = "a";
				else if (write == "aab")
					write = "b";
				else if (write == "aba")
					write = "D";
				else if (write == "abb")
					write = "#";
				else if (write == "baa")
					write = "A";
				else if (write == "bab")
					write = "B";

				string move = "";
				if (i < stateMapLine.size()){
					move += stateMapLine[i];
					configEvalLine += stateMapLine[i];
				}
				i++;

				string terminator = "";
				if (i < stateMapLine.size()){
					terminator += stateMapLine[i];

					if (terminator == "#"){
						//configEvalLine += stateMapLine[i];
						doneReadingConfig = true;
						i++;
					}
				}

				UTMStateAction action;
				action.replaceWith = write.size() > 0 ? write[0] : 'D';
				action.direction = (move.size() > 0 ? move[0] : 'b') == 'a' ? Left : Right;
				action.nextState = to;
				if (this->utmMetadata.utmMap.find(from) == this->utmMetadata.utmMap.end()){
					UTMState state;
					state.actions.insert(pair<char, UTMStateAction>((read.size() > 0 ? read[0] : 'D'), action));
					this->utmMetadata.utmMap.insert(pair<int, UTMState>(from, state));
				}
				else{
					if (this->utmMetadata.utmMap[from].actions.find((read.size() > 0 ? read[0] : 'D')) == this->utmMetadata.utmMap[from].actions.end()){
						this->utmMetadata.utmMap[from].actions.insert(pair<char, UTMStateAction>((read.size() > 0 ? read[0] : 'D'), action));
					}
				}

				FixNewLine(configEvalLine);
				if (!WindowsSafeEmpty(configEvalLine)){
					if (this->evaluations.size() > 0){
						this->evaluations[0].input += configEvalLine;
					}
					else{
						UTMEvaluation eval;
						eval.input = configEvalLine;
						eval.state = this->utmMetadata.startState;
						eval.success = false;

						this->evaluations.push_back(eval);
					}
				}
			}

			string stateEvaluation = "";
			while (i < stateMapLine.size() && stateMapLine[i] != '#' && stateMapLine[i] != '\n' && stateMapLine[i] != '\r'){
				stateEvaluation += stateMapLine[i];
				i++;
			}

			FixNewLine(stateEvaluation);
			if (!WindowsSafeEmpty(stateEvaluation)){
				UTMEvaluation eval;
				eval.input = stateEvaluation;
				eval.state = this->utmMetadata.startState;
				eval.success = false;

				this->evaluations.push_back(eval);
			}
			i++;
		}
	}
}

void UTM::EvaluateUTM(){

	for(int i = 0; i < this->evaluations.size(); i++){
		UTMEvaluation &eval = this->evaluations[i];
		eval.state = this->utmMetadata.startState;

		string test = eval.input;
		int index = 0;
		for(;;){
			char head = index < test.size() ? test[index] : 'D';
			if(this->utmMetadata.utmMap[eval.state].actions.find(head) != this->utmMetadata.utmMap[eval.state].actions.end()){
				UTMStateAction action = this->utmMetadata.utmMap[eval.state].actions[head];
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
			eval.success = eval.state == this->utmMetadata.stopState ? true : false;
		}
		else{
			eval.success = false;
		}
	}
}

void UTM::PrintResult(ostream &output){
	for(int i = 0; i < this->evaluations.size(); i++){
		output << this->evaluations[i].input << endl;
		output << (this->evaluations[i].success ? "ACCEPT" : "REJECT") << endl;
		output << endl;
	}
}

void UTM::FixNewLine(string &input){
	if(input != ""){
		//Check for the newline character
		if(input[0] == '\n') input = input.substr(1);
		if(input[input.size()-1] == '\n') input = input.substr(0, input.size()-1);

		//Check for the \r (sometimes used in windows)
		if(input[0] == '\r') input = input.substr(1);
		if(input[input.size()-1] == '\r') input = input.substr(0, input.size()-1);
	}
}

bool UTM::WindowsSafeEmpty(string input){
	if(input.empty() || input == "" || input.size() == 0 || input == "\n" || input == "\r") 
		return true;
	else
		return false;
}