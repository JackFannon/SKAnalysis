#ifndef RelicMuonMatching_H
#define RelicMuonMatching_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "MTreeReader.h"
#include "skroot.h"
#include "ConnectionTable.h"
#include "ParticleCand.h"

/**
 * \class RelicMuonMatching
*/

class RelicMuonMatching: public Tool {
	
	
	public:
	
	RelicMuonMatching(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Executre function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.
	
	
	private:
	
	std::string treeReaderName;
	
	MTreeReader* myTreeReader = nullptr;
	
	const Header* myHeader=nullptr;
	const LoweInfo* myLowe=nullptr;
	
	
	int verbosity = 1;
	int m_verbose;
	int v_error = 0;
	
	unsigned long long int bitshiftTime(unsigned long long int t0Time, unsigned long long int hardwareTime);
	bool RelicMuonMatch(std::string particleType, std::deque<ParticleCand>& currentDeque, std::deque<ParticleCand>& targetDeque);
	
	bool muonFlag;
	bool relicFlag;
	
	unsigned long long int muonTime;
	unsigned long long int relicTime;
	float timeDiff;
	unsigned long long int bitOne = 1;
};


#endif
