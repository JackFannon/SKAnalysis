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
	
	m_variables.Get("fileName", inputName);
	m_variables.Get("outputName", outputName);
	
	inputFile = TFile::Open("./rootFiles/lowe_skdetsim.r628.root");
	inputTree = (TTree *)inputFile->Get("data");
	
	
	
	outputFile = TFile::Open("./testSpallTMVA.root", "RECREATE");
	TTree* outputTree = new TTree("data", "outputTree");
	
	
	
	TBranch* exist = inputTree->FindBranch("LOWE");
	if(exist){
		inputTree->SetBranchAddress("LOWE", &LOWE, &lowebranch);
	}
	
	outputTree->Branch("bsvertex", &bsvertex, "bsvertex/I");
	outputTree->Branch("bsresult", &bsresult, "bsresult/I");
	outputTree->Branch("bsdir", &bsdir, "bsdir/I");
	outputTree->Branch("bsgood", &bsgood, "bsgood/I");
	outputTree->Branch("bsdirks", &bsdirks, "bsdirks/I");
	outputTree->Branch("bseffhit", &bseffhit, "bseffhit/I");
	outputTree->Branch("bsenergy", &bsenergy, "bsenergy/I");
	outputTree->Branch("bsn50", &bsn50, "bsn50/I");
	outputTree->Branch("bscossun", &bscossun, "bscossun/I");
	outputTree->Branch("clvertex", &clvertex, "clvertex/I");
	outputTree->Branch("clresult", &clresult, "clresult/I");
	outputTree->Branch("cldir", &cldir, "cldir/I");
	outputTree->Branch("clgoodness", &clgoodness, "clgoodness/I");
	outputTree->Branch("cldirks", &cldirks, "cldirks/I");
	outputTree->Branch("cleffhit", &cleffhit, "cleffhit/I");
	outputTree->Branch("clenergy", &clenergy, "clenergy/I");
	outputTree->Branch("cln50", &cln50, "cln50/I");
	outputTree->Branch("clcossun", &clcossun, "clcossun/I");
	outputTree->Branch("latmnum", &latmnum, "latmnum/I");
	outputTree->Branch("latmh", &latmh, "latmh/I");
	outputTree->Branch("lmx24", &lmx24, "lmx24/I");
	outputTree->Branch("ltimediff", &ltimediff, "ltimediff/I");
	outputTree->Branch("lnsratio", &lnsratio, "lnsratio/I");
	outputTree->Branch("lsdir", &lsdir, "lsdir/I");
	outputTree->Branch("spaevnum", &spaevnum, "spaevnum/I");
	outputTree->Branch("spaloglike", &spaloglike, "spaloglike/I");
	outputTree->Branch("sparesq", &sparesq, "sparesq/I");
	outputTree->Branch("spadt", &spadt, "spadt/I");
	outputTree->Branch("spadll", &spadll, "spadll/I");
	outputTree->Branch("spadlt", &spadlt, "spadlt/I");
	outputTree->Branch("spamuyn", &spamuyn, "spamuyn/I");
	outputTree->Branch("spamugdn", &spamugdn, "spamugdn/I");
	outputTree->Branch("posmc", &posmc, "posmc/I");
	outputTree->Branch("dirmc", &dirmc, "dirmc/I");
	outputTree->Branch("pabsmc", &pabsmc, "pabsmc/I");
	outputTree->Branch("energymc", &energymc, "energymc/I");
	outputTree->Branch("darkmc", &darkmc, "darkmc/I");
	outputTree->Branch("islekeep", &islekeep, "islekeep/I");
	outputTree->Branch("bspatlik", &bspatlik, "bspatlik/I");
	outputTree->Branch("clpatlik", &clpatlik, "clpatlik/I");
	outputTree->Branch("lwatert", &lwatert, "lwatert/I");
	outputTree->Branch("lninfo", &lninfo, "lninfo/I");
	
	numofEntries = outputTree->GetEntries();
	
	return true;
}


bool SpallTMVA::Execute(){
	
	int entryNum;
	
	outputTree->GetEntry(entryNum);
	
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
	std::copy(std::begin(LOWE->cldir),    std::end(LOWE->cldir), cldir);
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
	std::copy(std::begin(LOWE->lsdir), std::end(LOWE->lsdir), lsdir);
	spaevnum = LOWE->spaevnum;
	spaloglike = LOWE->spaloglike;
	sparesq = LOWE->sparesq;
	spadt = LOWE->spadt;
	spadll = LOWE->spadll;
	spadlt = LOWE->spadlt;
	spamuyn = LOWE->spamuyn;
	spamugdn = LOWE->spamugdn;
	std::copy(std::begin(LOWE->posmc), std::end(LOWE->posmc), posmc);
	std::copy(std::begin(LOWE->dirmc), std::end(LOWE->dirmc), dirmc);
	std::copy(std::begin(LOWE->pabsmc), std::end(LOWE->pabsmc), pabsmc);
	std::copy(std::begin(LOWE->energymc), std::end(LOWE->energymc), energymc);
	darkmc = LOWE->darkmc;
	islekeep = LOWE->islekeep;
	bspatlik = LOWE->bspatlik;
	clpatlik = LOWE->clpatlik;
	lwatert = LOWE->lwatert;
	lninfo = LOWE->lninfo;
	std::copy(std::begin(LOWE->linfo), std::end(LOWE->linfo), linfo);
	
	outputTree->Fill();
	
	entryNum++;
	
	if(entryNum >= numofEntries){
		m_data->vars.Set("StopLoop", true);
	}
	
	return true;
}


bool SpallTMVA::Finalise(){
	
	outputTree->Write();
	
	return true;
}
