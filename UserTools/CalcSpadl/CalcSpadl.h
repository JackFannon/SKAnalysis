#ifndef CalcSpadl_H
#define CalcSpadl_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "skroot.h"
#include "MTreeReader.h"


/**
 * \class CalcSpadl
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
*/

class CalcSpadl: public Tool {


public:
	CalcSpadl();
	bool Initialise(std::string configfile,DataModel &data);
	bool Execute();
	bool Finalise();
	
private:
	
	bool getdl(float mudir[3], float v[3], float muent[3], float &lt, float &proj);
	bool GetPeakScott(float dedx[200], int &index);
	bool GetPeakKirk(float dedx_kirk[200], int &index);
	
	std::string treeReaderName;
	
	std::string inputName;
	
	std::string outputName;
	std::string outputTreeName;
	
	TFile* inputFile;
	TTree* inputTree;
	
	TFile* outputFile;
	TTree* outputTree;
	
	MTreeReader* myTreeReader=nullptr;
	
//	const Header* myHeader=nullptr;
	const LoweInfo* myLowe=nullptr;
	const MuInfo* myMu=nullptr;
	const MCInfo* myMC=nullptr;
	
	TBranch* lowebranch;
	TBranch* mcbranch;
	TBranch* mubranch;
	
	int entrynum;
	int nEntries;
	
	float muent[3];
	float mudir[3];
	float dedx[200];
	float kirk_dedx[200];
	
	float bsvertex[4];
	float bsdir[3];
	float bsgood;
	float bsenergy;
	int bsn50;
	float spadll;
	float spadll_kirk;
	float spadlt;
	
	int verbosity=1;
	int v_error=0;
	int v_warning=1;
	int v_message=2;
	int v_debug=3;
};


#endif
