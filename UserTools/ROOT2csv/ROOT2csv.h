#ifndef ROOT2csv_H
#define ROOT2csv_H

#include <string>
#include <iostream>
#include <fstream>

#include "Tool.h"
#include "MTreeReader.h"
#include "skroot.h"
#include "ConnectionTable.h"
#include "ParticleCand.h"


/**
 * \class ROOT2csv
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
*/

class ROOT2csv: public Tool {


public:
	
	ROOT2csv(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Executre function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.
	
	
private:
	
	int verbosity = 1;
	int m_verbose;
	int v_error = 0;
	
	std::string treeReaderName;
	std::string outputFileName;
	
	std::ofstream outputFile;
	
	MTreeReader* myTreeReader = nullptr;
	
	const Header* myHeader=nullptr;
	const LoweInfo* myLowe=nullptr;
	
};


#endif
