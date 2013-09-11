
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

#include "DFA.h"

int main(int argc, char* argv[]){
	try{
		std::string path;
		std::string outputPath;
		if(argc <= 1){
			cout << "Enter the dfa file path: ";
			cin >> path;
		}
		else{
			path = argv[1];
		}

		if(argc > 2){
			outputPath = argv[2];
		}
		else{
			outputPath = "dfaOutput.dat";
		}

		DFA dfaMachine;

		ifstream inputFile(path.c_str());
		if(!inputFile)
			throw Exception("The dfa file " + path + " could not be found");

		ofstream outputFile(outputPath.c_str());
		if(!outputFile)
			throw Exception("Could not open output file " + outputPath + ".");

		dfaMachine.ReadDFA(inputFile);
		dfaMachine.EvaluateDFA();
		dfaMachine.PrintResult(cout);
		dfaMachine.PrintResult(outputFile);

		inputFile.close();
		outputFile.close();
	}
	catch(...){
		cout << endl << "An unexpected error occured. Ending program." << endl;
		exit(EXIT_FAILURE);
	}

	return 0;
}