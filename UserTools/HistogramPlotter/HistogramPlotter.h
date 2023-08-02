#ifndef HistogramPlotter_H
#define HistogramPlotter_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "TH1.h"
#include "THStack.h"
#include "MTreeReader.h"


/**
 * \class HistogramPlotter
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
*/

class HistogramPlotter: public Tool {


public:
	
	HistogramPlotter(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Executre function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.
	
	
private:
	
	//config variables
	int m_verbose;
	int v_error=1;
	int verbosity=1;
	int StackPlots = 1;
	int XMin = 0;
	int XMax = 100;
	int Interval = 5;
	std::string HistTitle = "Default title";
	std::string StackTitle = "Default stack title";
	int NHists = 1;
	std::string treeReaderName;
	
	MTreeReader* myTreeReader=nullptr;
	
	TFile* myRootFile=nullptr;
	TTree* myRootTree=nullptr;
	
	THStack* myStack=nullptr;
	
	std::vector<TH1F*> myHistList;
	
	std::vector<float>* MatchedTimeDiff=nullptr;
	int MuonTag;
	
};


#endif
