#ifndef ReconResolution_H
#define ReconResolution_H

#include <string>
#include <iostream>
#include <map>

#include "Tool.h"
#include "MTreeReader.h"

#include "skroot.h"
#include "ConnectionTable.h"
#include "TGraphErrors.h"


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
	
	std::vector<std::vector<float>> calcVertexDifference(MTreeReader* TreeReadersName, std::vector<std::vector<float>> vectdiffVertex);
	bool calcResolution(std::vector<std::vector<float>> vertexDiffvect, std::vector<float>& errorVec, std::vector<float>& resolution);
	TGraphErrors* MakeGraph(std::vector<float> resolutionVect, std::vector<float> errorVect, int colour);
	
	std::string FirstReader;
	std::string SecondReader;
	
	MTreeReader* myFirstReader=nullptr;
	MTreeReader* mySecondReader=nullptr;
	
	int dataSrc=1;
	int v_error=0;
	int verbosity=1;
	int comparison=0;
	
	std::vector<std::vector<float>> eneVertexDiff;
	std::vector<std::vector<float>> eneVertexDiff2;





};


#endif
