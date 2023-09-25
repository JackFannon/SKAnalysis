#ifndef AddMuonInfo_H
#define AddMuonInfo_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "TROOT.h"
#include "ShowerParticle.h"
#include "ParentMuon.h"
#include "skroot.h"

class AddMuonInfo: public Tool {

public:
	AddMuonInfo();
	bool Initialise(std::string configfile,DataModel &data);
	bool Execute();
	bool Finalise();
	
private:
	
	//Input .txt files for the parent muons and spallation showers
	std::string muonsFile;
	std::string showersFile;
	
	//Input file and tree
	std::string inputName;
	TFile* inputFile;
	TTree* inputTree;
	
	//Output file and tree
	std::string outputName;
	std::string outputTreeName;
	TFile* outputFile;
	TTree* outputTree;
	
	//class objects for input branches
	const LoweInfo* LOWE=nullptr;
	const MCInfo* MC=nullptr;
	
	//input branches
	TBranch* lowebranch;
	TBranch* mcbranch;
	
	//output branch variables
	float bsvertex[4];
	float bsdir[3];
	float bsgood;
	float bsenergy;
	int bsn50;
	float spadll;
	float spadll_kirk;
	float spadlt;
	int atomicnumber;
	int massnumber;
	
	//Entry number for copyTree
	int entryNum;
	
	//Number of entries in tree
	int nEntries;
	
	std::map<int, float> partMasses;
	
	std::ifstream myShowers;
	std::ifstream myMuons;
	
	std::vector<std::vector<float>> energyDep;
	std::vector<float> maxDeposition;
	std::vector<std::vector<float>> spallationVects;
};


#endif
