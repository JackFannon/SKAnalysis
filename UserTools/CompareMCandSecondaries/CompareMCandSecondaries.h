#ifndef CompareMCandSecondaries_H
#define CompareMCandSecondaries_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "MTreeReader.h"

#include "skroot.h"
#include "ConnectionTable.h"


/**
 * \class CompareMCandSecondaries
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
*/

class CompareMCandSecondaries: public Tool {
	
	
	public:
	
	CompareMCandSecondaries(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Executre function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.
	
	
	private:
	
	std::string datatreeReaderName;
	std::string sectreeReaderName;
	
	int dataSrc=1;
	int verbosity=1;
	int v_error=0;
	
	MTreeReader* dataTreeReader=nullptr;
	MTreeReader* secTreeReader=nullptr;
	
	
	//const MCInfo* myMC=nullptr;
	//const LoweInfo* myLowe=nullptr;
	
	
	
};


#endif
