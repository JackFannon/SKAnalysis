#ifndef PrintMCInfo_H
#define PrintMCInfo_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "MTreeReader.h"

#include "skroot.h"
#include "ConnectionTable.h"

#include "TH1F.h"

/**
 * \class PrintMCInfo
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
*/

class PrintMCInfo: public Tool {


	public:

	PrintMCInfo(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Executre function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.

	private:
	std::string treeReaderName;
	
	MTreeReader* myTreeReader=nullptr;
	const MCInfo* myMC=nullptr;
	const LoweInfo* myLowe=nullptr;
	const Header* myHeader=nullptr;
	
	std::vector<float> totalDifference;
	
	vector<float> differenceList = vector<float>(50,0);
	vector<int> differenceNums = vector<int>(50,0);
	
	TH1F* vertexResHx = new TH1F();
	TH1F* vertexResHy = new TH1F();
	TH1F* vertexResHz = new TH1F();
	
	int dataSrc=1;
	int v_error=0;
	int verbosity=1;
	float failedRecon=0;
	int numberofEvents=0;





};


#endif
