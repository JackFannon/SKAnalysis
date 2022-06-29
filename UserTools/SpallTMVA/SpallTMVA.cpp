#include "SpallTMVA.h"

#include "TMVA/Reader.h"
#include "PathGetter.h"
#include "Candidate.h"
#include "TROOT.h"

SpallTMVA::SpallTMVA():Tool(){}


bool SpallTMVA::Initialise(std::string configfile, DataModel &data){
	
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
	
	outputTree->Branch("bsvertex", &bsvertex, "bsvertex[4]/F");
	outputTree->Branch("bsresult", &bsresult, "bsresult/F");
	outputTree->Branch("bsdir", &bsdir, "bsdir[3]/F");
	outputTree->Branch("bsgood", &bsgood, "bsgood/F");
	outputTree->Branch("bsdirks", &bsdirks, "bsdirks/F");
	outputTree->Branch("bseffhit", &bseffhit, "bseffhit/F");
	outputTree->Branch("bsenergy", &bsenergy, "bsenergy/F");
	outputTree->Branch("bsn50", &bsn50, "bsn50/F");
	outputTree->Branch("bscossun", &bscossun, "bscossun/F");
	outputTree->Branch("clvertex", &clvertex, "clvertex[4]/F");
	outputTree->Branch("clresult", &clresult, "clresult/F");
	outputTree->Branch("cldir", &cldir, "cldir[3]/F");
	outputTree->Branch("clgoodness", &clgoodness, "clgoodness/F");
	outputTree->Branch("cldirks", &cldirks, "cldirks/F");
	outputTree->Branch("cleffhit", &cleffhit, "cleffhit/F");
	outputTree->Branch("clenergy", &clenergy, "clenergy/F");
	outputTree->Branch("cln50", &cln50, "cln50/F");
	outputTree->Branch("clcossun", &clcossun, "clcossun/F");
	outputTree->Branch("latmnum", &latmnum, "latmnum/F");
	outputTree->Branch("latmh", &latmh, "latmh/F");
	outputTree->Branch("lmx24", &lmx24, "lmx24/F");
	outputTree->Branch("ltimediff", &ltimediff, "ltimediff/F");
	outputTree->Branch("lnsratio", &lnsratio, "lnsratio/F");
	outputTree->Branch("lsdir", &lsdir, "lsdir/F");
	outputTree->Branch("spaevnum", &spaevnum, "spaevnum/F");
	outputTree->Branch("spaloglike", &spaloglike, "spaloglike/F");
	outputTree->Branch("sparesq", &sparesq, "sparesq/F");
	outputTree->Branch("spadt", &spadt, "spadt/F");
	outputTree->Branch("spadll", &spadll, "spadll/F");
	outputTree->Branch("spadlt", &spadlt, "spadlt/F");
	outputTree->Branch("spamuyn", &spamuyn, "spamuyn/F");
	outputTree->Branch("spamugdn", &spamugdn, "spamugdn/F");
	outputTree->Branch("posmc", &posmc, "posmc/F");
	outputTree->Branch("dirmc", &dirmc, "dirmc/F");
	outputTree->Branch("pabsmc", &pabsmc, "pabsmc/F");
	outputTree->Branch("energymc", &energymc, "energymc/F");
	outputTree->Branch("darkmc", &darkmc, "darkmc/F");
	outputTree->Branch("islekeep", &islekeep, "islekeep/F");
	outputTree->Branch("bspatlik", &bspatlik, "bspatlik/F");
	outputTree->Branch("clpatlik", &clpatlik, "clpatlik/F");
	outputTree->Branch("lwatert", &lwatert, "lwatert/F");
	outputTree->Branch("lninfo", &lninfo, "lninfo/F");
	
	numofEntries = inputTree->GetEntries();
	return true;
}


bool SpallTMVA::Execute(){
	
	inputTree->GetEntry(entryNum);
	
	std::copy(std::begin(LOWE->bsvertex), std::end(LOWE->bsvertex), bsvertex);
	std::copy(std::begin(LOWE->bsresult), std::end(LOWE->bsresult), bsresult);
	std::copy(std::begin(LOWE->bsdir),    std::end(LOWE->bsdir),    bsdir);
	std::copy(std::begin(LOWE->bsgood),   std::end(LOWE->bsgood),   bsgood);
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
	std::copy(std::begin(LOWE->dirmc),    std::end(LOWE->dirmc),    dirmc);
	std::copy(std::begin(LOWE->pabsmc),   std::end(LOWE->pabsmc),   pabsmc);
	std::copy(std::begin(LOWE->energymc), std::end(LOWE->energymc), energymc);
	darkmc = LOWE->darkmc;
	islekeep = LOWE->islekeep;
	bspatlik = LOWE->bspatlik;
	clpatlik = LOWE->clpatlik;
	lwatert = LOWE->lwatert;
	lninfo = LOWE->lninfo;
	std::copy(std::begin(LOWE->linfo),    std::end(LOWE->linfo),    linfo);
	
	outputTree->Fill();
	
	entryNum++;
	
	if(entryNum >= numofEntries){
		m_data->vars.Set("StopLoop", true);
	}
	
	return true;
}


bool SpallTMVA::Finalise(){
	
	inputFile->Close();
	
	outputTree->Write();
	outputFile->Close();
	
	return true;
}
