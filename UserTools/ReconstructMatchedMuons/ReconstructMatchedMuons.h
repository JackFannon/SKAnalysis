#ifndef ReconstructMatchedMuons_H
#define ReconstructMatchedMuons_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "MTreeReader.h"
#include "skroot.h"
#include "ConnectionTable.h"

class ReconstructMatchedMuons: public Tool {
	
	
	public:
	
	ReconstructMatchedMuons(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Executre function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.
	
	
	private:
	
	int currentEntry = -1;
	
	std::string treeReaderName;
	MTreeReader* myTreeReader = nullptr;
	const Header* myHeader = nullptr;
	
	std::deque<ParticleCand> muonsToRec;
	
	int verbosity = 1;
	int m_verbose;
	int v_error = 0;
	
	int lastRun;
	float watert;
	
};


#endif
