#ifndef SkipTriggers_H
#define SkipTriggers_H

#include <string>
#include <iostream>
#include <bitset>

#include "Tool.h"
#include "MTreeReader.h"
#include "skroot.h"
#include "ConnectionTable.h"


/*
 * \class SkipTriggers
*/

class SkipTriggers: public Tool {
	
	
	public:
	
	SkipTriggers(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Executre function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.
	
	
	private:
	
	MTreeReader* myTreeReader = nullptr;
	const Header* myHeader = nullptr;
	
	std::string treeReaderName;
	std::bitset<sizeof(int)*8> triggerID;
	std::vector<int> triggerVect;
	
	int verbosity = 1;
	int m_verbose;
	int v_error = 0;
	
	int LETrig;
	int HETrig = 0;
	int SLETrig = 0;
	int ODTrig = 0;
	int PeriodicTrig = 0;
	int AfterTrig = 0;
	int VetoStartTrig = 0;
	int VetoStopTrig = 0;
	int RandWideTrig = 0;
	int IDLaserTrig = 0;
	int LEDTrig = 0;
	int NiTrig = 0;
	int ODLaserTrig = 0;
	int LEHitsumTrig = 0;
	int HEHitsumTrig = 0;
	int SLEHitsumTrig = 0;
	int ODHitsumTrig = 0;
	int SNBurstTrig = 0;
	int MuToEleTrig = 0;
	int LINACTrig = 0;
	int LINACMicroWaveTrig = 0;
	int PeriodicSimpTrig = 0;
	int SHETrig = 0;
	int AFTTrig = 0;
	int PedestalTrig = 0;
	int T2KTrig = 0;
	
	int Nskipped = 0;
	
};


#endif
