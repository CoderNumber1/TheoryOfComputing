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
#include <string>
#include <fstream>

using namespace std;

#include "TM.h"

int main(int argc, char* argv[]){
	try{
		std::string path;
		std::string outputPath;
		if(argc <= 1){
			cout << "Enter the tm file path: ";
			cin >> path;
		}
		else{
			path = argv[1];
		}

		if(argc > 2){
			outputPath = argv[2];
		}
		else{
			outputPath = "tmOutput.dat";
		}

		TM tmMachine;

		ifstream inputFile(path.c_str());
		if(!inputFile)
			throw Exception("The tm file " + path + " could not be found");

		ofstream outputFile(outputPath.c_str());
		if(!outputFile)
			throw Exception("Could not open output file " + outputPath + ".");

		tmMachine.ReadTM(inputFile);
		tmMachine.EvaluateTM();
		tmMachine.PrintResult(cout);
		tmMachine.PrintResult(outputFile);

		inputFile.close();
		outputFile.close();
	}
	catch(...){
		cout << endl << "An unexpected error occured. Ending program." << endl;
		exit(EXIT_FAILURE);
	}

	return 0;
}