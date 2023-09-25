#ifndef ApplySRNWeight_H
#define ApplySRNWeight_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "skroot.h"
#include "TH1F.h"


/**
* \class ApplySRNWeight
*
* This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
*/

class ApplySRNWeight: public Tool {


	public:
	
	ApplySRNWeight(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Executre function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.
	
	
	private:
	
	int verbosity = 1;
	int m_verbose;
	int v_error = 0;
	int entryNum = 0;
	int numofEntries;
	
	std::string inputName;
	std::string outputName;
	
	TFile* newinputFile;
	TTree* inputTree;
	std::vector<TFile*> outputFiles;
	std::vector<TTree*> outputTrees;
	
	TH1F* neutEnergyHist = new TH1F("", "", 180, 0, 90);
	
	MCInfo* MC = new MCInfo;
	
	std::vector<int> neutEBinContent;
	
};


#endif
