#include "HistogramPlotter.h"
#include "TCanvas.h"
#include "TTree.h"

HistogramPlotter::HistogramPlotter():Tool(){}


bool HistogramPlotter::Initialise(std::string configfile, DataModel &data){
	
	if(configfile!="")  m_variables.Initialise(configfile);
	//m_variables.Print();
	m_data= &data;
	m_log= m_data->Log;
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	m_variables.Get("StackPlots", StackPlots);
	m_variables.Get("XMin", XMin);
	m_variables.Get("XMax", XMax);
	m_variables.Get("Interval", Interval);
	m_variables.Get("HistTitle", HistTitle);
	m_variables.Get("StackTitle", StackTitle);
	m_variables.Get("NHists", NHists);
	m_variables.Get("treeReaderName", treeReaderName);
	
/*	if(m_data->Trees.count(treeReaderName)==0){
		Log("Failed to find TreeReader "+treeReaderName+" in DataModel!",v_error,verbosity);
		return false;
	} else {
		myTreeReader = m_data->Trees.at(treeReaderName);
	}*/
	
	int nBins = (XMax-XMin)/Interval;
	
	for(int i = 0; i < NHists; i++){
		myHistList.push_back(new TH1F("", HistTitle.c_str(), nBins, XMin, XMax));
	}
	
	myRootFile = TFile::Open("/disk03/usr8/jfannon/data/spallRedWallTestTest2.root", "READ");
	
	myRootFile->Print();
	//myRootFile->GetObject("data", myRootTree);
	myRootTree = (TTree*)myRootFile->Get("data");
	
	
	myRootTree->SetBranchAddress("MatchedTimeDiff", &MatchedTimeDiff);
	myRootTree->SetBranchAddress("MuonTag", &MuonTag);
	
	return true;
}


bool HistogramPlotter::Execute(){
	//use TreeReader to load in events -- might change this in future
	
//	myTreeReader->Get("MatchedTimeDiff", MatchedTimeDiff);
//	myTreeReader->Get("MuonTag", MuonTag);
	
	std::cout << "NUM OF ENTRIES " << myRootTree->GetEntries() << std::endl;
	
	for(int entry=0; entry < myRootTree->GetEntries(); entry++){
		myRootTree->GetEntry(entry);
		if(MuonTag==1){
			for(float TDiff:*MatchedTimeDiff){
				myHistList[0]->Fill(TDiff*pow(10,-9));
			}
		}
	}
	
	m_data->vars.Set("StopLoop", 1);
	
	
	return true;
}


bool HistogramPlotter::Finalise(){
	
	if(StackPlots){
		myStack = new THStack("myStack",StackTitle.c_str());
		for(TH1F* Hist:myHistList){
			myStack->Add(Hist);
		}
	}
	
	TCanvas* c = new TCanvas("c");
	c->cd();
	myStack->Draw();
	
	c->SaveAs("/home/jfannon/testout.pdf");
	
	return true;
}
