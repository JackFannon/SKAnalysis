#ifndef ReweightIBDtoSRN_H
#define ReweightIBDtoSRN_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "TMVA/Reader.h"
#include "MTreeReader.h"
#include "skroot.h"
#include "TFile.h"
#include "TRint.h"
#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"
#include "TMath.h"
#include "TSystemDirectory.h"
#include "TSystem.h"


/**
 * \class ReweightIBDtoSRN
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: J.Fannon $
* $Date: 2022/06/30 10:44:00 $
*/

class ReweightIBDtoSRN: public Tool {


	public:
	
	ReweightIBDtoSRN(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Executre function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.
	
	
	private:
	
	std::string inputName;
	std::string outputName;
	
	TFile* inputFile;
	TTree* inputTree;
	
	TFile* outputFile;
	TTree* outputTree;
	
	MCInfo* MC;
	TBranch* mcbranch;
	
	int entryNum;
	int numofEntries;
	
	
	
	int verbosity = 1;
	int m_verbose;
	int v_error = 0;
	
	const char *modelsPath="./FluxModels/";
	const char *ext=".cc";
	std::vector<const char*> modelNames;
	std::vector <int> nBins_all;
	std::vector<float> binWidth_all;
	std::vector<float> minE_all;
	double* fluxValues[100];
	
	float protonsPerKton = pow(6.67, 31);
	float fiducial = 22.5;
	
	std::vector<float> weightedFlux;
	
	double dsigma_sv(float enu, double costheta);
	bool Read_Flux(const char* fileName, float &minE, float &binWidth, int &nBins, double *flux);
	float weight_enu(float truthE, float ctheta, float minE, float binWidth, int nbins, double *flux);
};


#endif
