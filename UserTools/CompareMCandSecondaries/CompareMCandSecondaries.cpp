#include "CompareMCandSecondaries.h"

#include "TROOT.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "TPad.h"
#include "TStyle.h"
#include "TColor.h"
#include "TView.h"
#include "TLegend.h"
#include "TSystem.h"
#include "THStack.h"

CompareMCandSecondaries::CompareMCandSecondaries():Tool(){}


bool CompareMCandSecondaries::Initialise(std::string configfile, DataModel &data){
	
	if(configfile!="")  m_variables.Initialise(configfile);
	//m_variables.Print();
	
	m_data= &data;
	m_log= m_data->Log;
	
	m_variables.Get("datatreeReaderName",datatreeReaderName);
	
	m_variables.Get("sectreeReaderName",sectreeReaderName);
	
	if(dataSrc == -1 || dataSrc == 1){
		// if getting data from TTree, check the TreeReader
			if(m_data->Trees.count(datatreeReaderName)==0){
				Log("Failed to find TreeReader "+datatreeReaderName+" in DataModel!",v_error,verbosity);
				return false;
		}else {
			dataTreeReader = m_data->Trees.at(datatreeReaderName);
		}
	}
	
	secTreeReader = m_data->Trees.at(sectreeReaderName);
	
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	
	return true;
}


bool CompareMCandSecondaries::Execute(){
	
	const Header* myHeader=nullptr;
	
	int nscndprt;
	
	secTreeReader->Get("nscndprt", nscndprt);
	dataTreeReader->Get("HEADER", myHeader);
	
	std::cout << myHeader->nevsk << std::endl;
	
	std::cout << nscndprt << std::endl; 
	
	return true;
}


bool CompareMCandSecondaries::Finalise(){
	
	return true;
}
