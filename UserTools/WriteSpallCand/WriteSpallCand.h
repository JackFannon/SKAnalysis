#ifndef WriteSpallCand_H
#define WriteSpallCand_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "MTreeReader.h"
#include "skroot.h"

class WriteSpallCand: public Tool {
	
	
	public:
	
	WriteSpallCand(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Executre function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.
	
	
	private:
	
	std::string treeReaderName;
	std::string treeWriterName;
	MTreeReader* myTreeReader = nullptr;
	MTreeReader* myTreeWriter = nullptr;
	const Header* myHeader=nullptr;
	
	std::string outputFile;
	
	std::vector<std::string> branchestoSkip;
	
	int lun;
	int currentRun = 0;
	
	int verbosity = 1;
	int m_verbose;
	int v_error = 0;
	
};


#endif
