#include "ExtractLowE.h"

#include "TMVA/Reader.h"
#include "PathGetter.h"
#include "Candidate.h"
#include "TROOT.h"

ExtractLowE::ExtractLowE():Tool(){}


bool ExtractLowE::Initialise(std::string configfile, DataModel &data){
	
	if(configfile!="")  m_variables.Initialise(configfile);
	//m_variables.Print();
	
	m_data= &data;
	m_log= m_data->Log;
	
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	
	m_variables.Get("inputName", inputName);
	m_variables.Get("outputName", outputName);
	m_variables.Get("outputTreeName", outputTreeName);
	
	inputFile = TFile::Open(inputName.c_str());
	inputTree = (TTree *)inputFile->Get("data");
	
	outputFile = TFile::Open(outputName.c_str(), "RECREATE");
	outputTree = new TTree(outputTreeName.c_str(), outputTreeName.c_str());
	
	TBranch* exist = inputTree->FindBranch("LOWE");
	if(exist){
		inputTree->SetBranchAddress("LOWE", &LOWE, &lowebranch);
	}
	
	exist = inputTree->FindBranch("TQREAL");
	if(exist){
		inputTree->SetBranchAddress("TQREAL", &TQREAL, &tqrealbranch);
	}else{
		std::cout << "COULD NOT GET TQREAL BRANCH" << std::endl;
	}
	
	exist = inputTree->FindBranch("MuonTag");
	if(exist){
		inputTree->SetBranchAddress("MuonTag", &MuonTag, &MuonTagbranch);
	}
	
	//Charge and timing info
	outputTree->Branch("totalCharge", &totalCharge, "sumQ/F");
	
	outputTree->Branch("bsvertex", &bsvertex, "bsvertex[4]/F");
	outputTree->Branch("bsresult", &bsresult, "bsresult[6]/F");
	outputTree->Branch("bsdir", &bsdir, "bsdir[3]/F");
	outputTree->Branch("bsgood", &bsgood, "bsgood/F");
	outputTree->Branch("bsdirks", &bsdirks, "bsdirks/F");
	outputTree->Branch("bseffhit", &bseffhit, "bseffhit[12]/F");
	outputTree->Branch("bsenergy", &bsenergy, "bsenergy/F");
	outputTree->Branch("bsn50", &bsn50, "bsn50/F");
	outputTree->Branch("bscossun", &bscossun, "bscossun/F");
	outputTree->Branch("clvertex", &clvertex, "clvertex[4]/F");
	outputTree->Branch("clresult", &clresult, "clresult[3]/F");
	outputTree->Branch("cldir", &cldir, "cldir[3]/F");
	outputTree->Branch("clgoodness", &clgoodness, "clgoodness/F");
	outputTree->Branch("cldirks", &cldirks, "cldirks/F");
	outputTree->Branch("cleffhit", &cleffhit, "cleffhit[12]/F");
	outputTree->Branch("clenergy", &clenergy, "clenergy/F");
	outputTree->Branch("cln50", &cln50, "cln50/F");
	outputTree->Branch("clcossun", &clcossun, "clcossun/F");
	outputTree->Branch("latmnum", &latmnum, "latmnum/F");
	outputTree->Branch("latmh", &latmh, "latmh/F");
	outputTree->Branch("lmx24", &lmx24, "lmx24/F");
	outputTree->Branch("ltimediff", &ltimediff, "ltimediff/F");
	outputTree->Branch("lnsratio", &lnsratio, "lnsratio/F");
	outputTree->Branch("lsdir", &lsdir, "lsdir[3]/F");
	outputTree->Branch("spaevnum", &spaevnum, "spaevnum/F");
	outputTree->Branch("spaloglike", &spaloglike, "spaloglike/F");
	outputTree->Branch("sparesq", &sparesq, "sparesq/F");
	outputTree->Branch("spadt", &spadt, "spadt/F");
	outputTree->Branch("spadll", &spadll, "spadll/F");
	outputTree->Branch("spadlt", &spadlt, "spadlt/F");
	outputTree->Branch("spamuyn", &spamuyn, "spamuyn/F");
	outputTree->Branch("spamugdn", &spamugdn, "spamugdn/F");
	outputTree->Branch("posmc", &posmc, "posmc[3]/F");
	outputTree->Branch("pabsmc", &pabsmc, "pabsmc[2]/F");
	outputTree->Branch("energymc", &energymc, "energymc[2]/F");
	outputTree->Branch("darkmc", &darkmc, "darkmc/F");
	outputTree->Branch("islekeep", &islekeep, "islekeep/F");
	outputTree->Branch("bspatlik", &bspatlik, "bspatlik/F");
	outputTree->Branch("clpatlik", &clpatlik, "clpatlik/F");
	outputTree->Branch("lwatert", &lwatert, "lwatert/F");
	//	outputTree->Branch("lninfo", &lninfo, "lninfo[255]/F");
	
	numofEntries = inputTree->GetEntries();
	std::cout << "Number of entries is " << numofEntries << std::endl;
	entryNum=0;
	return true;
}


bool ExtractLowE::Execute(){
	inputTree->GetEntry(entryNum);
	std::cout << entryNum << std::endl;
	if(MuonTag != 1){
		//std::cout << MuonTag << std::endl;
		//get total number of activated PMTs for this event
		totalPMTsActivated = TQREAL->cables.size();
		//reset the total charge for each event to zero before pulling it from the root file
		totalCharge = 0;
		
		
		for(int pmtNumber = 0; pmtNumber < totalPMTsActivated; pmtNumber++){
			//Grab charge info from TQReal
			totalCharge += TQREAL->Q.at(pmtNumber);
		}
		
		
		std::copy(std::begin(LOWE->bsvertex), std::end(LOWE->bsvertex), bsvertex);
		std::copy(std::begin(LOWE->bsresult), std::end(LOWE->bsresult), bsresult);
		std::copy(std::begin(LOWE->bsdir),    std::end(LOWE->bsdir),    bsdir);
		//	std::copy(std::begin(LOWE->bsgood),   std::end(LOWE->bsgood),   bsgood);
		bsgood = LOWE->bsgood[1];
		bsdirks = LOWE->bsdirks;
		std::copy(std::begin(LOWE->bseffhit), std::end(LOWE->bseffhit), bseffhit);
		bsenergy = LOWE->bsenergy;
		bsn50 = LOWE->bsn50;
		bscossun = LOWE->bscossun;
		std::copy(std::begin(LOWE->clvertex), std::end(LOWE->clvertex), clvertex);
		std::copy(std::begin(LOWE->clresult), std::end(LOWE->clresult), clresult);
		std::copy(std::begin(LOWE->cldir),    std::end(LOWE->cldir),    cldir);
		clgoodness = LOWE->clgoodness;
		cldirks = LOWE->cldirks;
		std::copy(std::begin(LOWE->cleffhit), std::end(LOWE->cleffhit), cleffhit);
		clenergy = LOWE->clenergy;
		cln50 = LOWE->cln50;
		clcossun = LOWE->clcossun;
		latmnum = LOWE->latmnum;
		latmh = LOWE->latmh;
		lmx24 = LOWE->lmx24;
		ltimediff = LOWE->ltimediff;
		lnsratio = LOWE->lnsratio;
		std::copy(std::begin(LOWE->lsdir),    std::end(LOWE->lsdir),    lsdir);
		spaevnum = LOWE->spaevnum;
		spaloglike = LOWE->spaloglike;
		sparesq = LOWE->sparesq;
		spadt = LOWE->spadt;
		spadll = LOWE->spadll;
		spadlt = LOWE->spadlt;
		spamuyn = LOWE->spamuyn;
		spamugdn = LOWE->spamugdn;
		std::copy(std::begin(LOWE->posmc),    std::end(LOWE->posmc),    posmc);
		//std::copy(std::begin(LOWE->dirmc),    std::end(LOWE->dirmc),    dirmc);
		std::copy(&LOWE->dirmc[0][0],         &LOWE->dirmc[0][0]+2*3,   &dirmc[0][0]);
		std::copy(std::begin(LOWE->pabsmc),   std::end(LOWE->pabsmc),   pabsmc);
		std::copy(std::begin(LOWE->energymc), std::end(LOWE->energymc), energymc);
		darkmc = LOWE->darkmc;
		islekeep = LOWE->islekeep;
		bspatlik = LOWE->bspatlik;
		clpatlik = LOWE->clpatlik;
		lwatert = LOWE->lwatert;
		lninfo = LOWE->lninfo;
		//	std::copy(std::begin(LOWE->linfo),    std::end(LOWE->linfo),    linfo);
		
		outputTree->Fill();
		
		
		
		totalCharge=0;
	}
	entryNum++;
		
	if(entryNum >= numofEntries){
		m_data->vars.Set("StopLoop", true);
	}
	
	return true;
}


bool ExtractLowE::Finalise(){
	
	inputFile->Close();
	
	outputFile->Write();
	outputFile->Close();
	
	return true;
}
