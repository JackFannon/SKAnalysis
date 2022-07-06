#include "ApplySRNWeight.h"

ApplySRNWeight::ApplySRNWeight():Tool(){}


bool ApplySRNWeight::Initialise(std::string configfile, DataModel &data){
	
	if(configfile!="")  m_variables.Initialise(configfile);
	//m_variables.Print();
	
	m_data= &data;
	m_log= m_data->Log;
	
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	
	m_variables.Get("inputName", inputName);
	m_variables.Get("outputName", outputName);
	
	inputFile = TFile::Open(inputName.c_str());
	inputTree= (TTree *)inputFile->Get("data");
	for(int model; model < m_data->SRNModelNames.size(); model++){
		outputFile = TFile::Open((outputName+m_data->SRNModelNames[model]).c_str(), "RECREATE");
		outputFiles.push_back(outputFile);
		outputTree = inputTree->CloneTree(0);
		outputTrees.push_back(outputTree);
	}
	
	TBranch* exist = inputTree->FindBranch("MC");
	if(exist){
		inputTree->SetBranchAddress("MC", &MC);
	}else{Log("Error: MC branch not found in the input file", v_error, verbosity);
	}
	
	
	return true;
}


bool ApplySRNWeight::Execute(){
	
	return true;
}


bool ApplySRNWeight::Finalise(){
	
	return true;
}
