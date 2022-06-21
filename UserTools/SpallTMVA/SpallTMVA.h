#ifndef SpallTMVA_H
#define SpallTMVA_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "TMVA/Reader.h"
#include "MTreeReader.h"
#include "skroot.h"

/**
 * \class SpallTMVA
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
*/

class SpallTMVA: public Tool {
	
	
	public:
	
	SpallTMVA(); ///< Simple constructor
	bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
	bool Execute(); ///< Executre function used to perform Tool perpose. 
	bool Finalise(); ///< Finalise funciton used to clean up resorces.
	
	
	private:
	
	std::map<std::string, float> varContainer;
	std::string inputName;
	std::string outputName;
	
	TFile* inputFile;
	TTree* inputTree;
	
	TTree* outputTree;
	TFile* outputFile;
	
	LoweInfo* LOWE;
	TBranch* lowebranch;
	
	int numofEntries;
	
	float  bsvertex[4];    // bonsai fit
	float  bsresult[6];
	float  bsdir[3];
	float  bsgood[3];
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
