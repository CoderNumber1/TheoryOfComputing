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
#include <string>
#include <fstream>

using namespace std;

#include "PDA.h"

int main(int argc, char* argv[]){
	try{
		std::string path;
		std::string outputPath;
		if(argc <= 1){
			cout << "Enter the pda file path: ";
			cin >> path;
		}
		else{
			path = argv[1];
		}

		if(argc > 2){
			outputPath = argv[2];
		}
		else{
			outputPath = "pdaOutput.dat";
		}

		PDA pdaMachine;

		ifstream inputFile(path.c_str());
		if(!inputFile)
			throw Exception("The pda file " + path + " could not be found");

		ofstream outputFile(outputPath.c_str());
		if(!outputFile)
			throw Exception("Could not open output file " + outputPath + ".");

		pdaMachine.ReadPDA(inputFile);
		pdaMachine.EvaluatePDA();
		pdaMachine.PrintResult(cout);
		pdaMachine.PrintResult(outputFile);

		inputFile.close();
		outputFile.close();
	}
	catch(...){
		cout << endl << "An unexpected error occured. Ending program." << endl;
		exit(EXIT_FAILURE);
	}

	return 0;
}