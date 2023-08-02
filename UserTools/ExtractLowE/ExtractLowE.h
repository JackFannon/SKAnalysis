#ifndef ExtractLowE_H
#define ExtractLowE_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "TMVA/Reader.h"
#include "MTreeReader.h"
#include "skroot.h"

/**
 * \class ExtractLowE
 *
 *
 *
 * $Author: J.Fannon $
 * $Date: 2022/06/20 10:00:00 $
*/

class ExtractLowE: public Tool {
	
	public:
	
	ExtractLowE(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Executre function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.
	
	private:
	
	std::string inputName;
	std::string outputName;
	
	std::string outputTreeName;
	
	TFile* inputFile;
	TTree* inputTree;
	
	TTree* outputTree;
	
	TFile* outputFile;
	
	LoweInfo* LOWE;
	TBranch* lowebranch;
	
	TQReal* TQREAL;
	TBranch* tqrealbranch;
	
	int MuonTag;
	TBranch* MuonTagbranch;
	
	int entryNum;
	int numofEntries;
	
	int totalPMTsActivated;
	float totalCharge;
	
	float  bsvertex[4];    // bonsai fit
	float  bsresult[6];
	float  bsdir[3];
	float  bsgood;
	float  bsdirks;
	float  bseffhit[12];
	float  bsenergy;
	int    bsn50;
	float  bscossun;
	float  clvertex[4];    // clusfit
	float  clresult[4];
	float  cldir[3];
	float  clgoodness;
	float  cldirks;
	float  cleffhit[12];
	float  clenergy;
	int    cln50;
	float  clcossun;
	int    latmnum;
	int    latmh;
	int    lmx24;
	double ltimediff;
	float  lnsratio;
	float  lsdir[3];
	int    spaevnum;
	float  spaloglike;
	float  sparesq;
	float  spadt;
	float  spadll;
	float  spadlt;
	int    spamuyn;
	float  spamugdn;
	float  posmc[3];
	float  dirmc[2][3];
	float  pabsmc[2];
	float  energymc[2];
	float  darkmc;
	int    islekeep;
	float  bspatlik;
	float  clpatlik;
	float  lwatert;
	int    lninfo;
	int    linfo[255];
	
};


#endif
