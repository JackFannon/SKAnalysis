#ifndef ReconResolution_H
#define ReconResolution_H

#include <string>
#include <iostream>
#include <map>

#include "Tool.h"
#include "MTreeReader.h"

#include "skroot.h"
#include "ConnectionTable.h"
#include "TH1F.h"


/**
 * \class ReconResolution
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
*/

class ReconResolution: public Tool {

	public:
	ReconResolution(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Execute function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.
	
	private:
	
	std::string treeReaderName;
	std::string treeReaderName2;
	
	MTreeReader* myTreeReader=nullptr;
	const MCInfo* myMC=nullptr;
	const LoweInfo* myLowe=nullptr;
	
	int dataSrc=1;
	int v_error=0;
	int verbosity=1;
	int overFourHund=0;
	
	std::vector<std::vector<float>> eneVertexDiff;
	
	TH1* vertexResH = new TH1D();
	TH1* vertexResHC;





};


#endif
