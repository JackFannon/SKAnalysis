#ifndef DatToVect_H
#define DatToVect_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Tool.h"


/*
\class DatToVect
*/

class DatToVect: public Tool {
	
	
	public:
	
	DatToVect(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Executre function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.
	
	
	private:
	std::string input_file;  // Input file location
	std::string output_file; // Output file location
	
	std::ifstream i_file; // Input file ifstream
	std::ofstream o_file; // Output file ofstream
	
	int mu_pdg = 13; // Muon PDG
	
	int muons_per_file; // Number of muon primaries to write to each file
	int file_number = 0; // File number to label each file with
	int muons_in_file = -1; // Number of muons currently written to the file
	
	std::ofstream new_o_file;
	
	std::string line;
	
	bool GeneratePrimary(std::ofstream&);
	
	
};


#endif
