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
	
	newinputFile = TFile::Open(inputName.c_str());
	inputTree= (TTree *)newinputFile->Get("data");
	for(int model = 0; model < m_data->SRNModelNames.size(); model++){
		std::cout << outputName+m_data->SRNModelNames[model]+".root" << std::endl;
		//		TFile* outputFile = TFile::Open((outputName+m_data->SRNModelNames[model]+ ".root").c_str(), "RECREATE");
		outputFiles.push_back(TFile::Open((outputName+m_data->SRNModelNames[model]+ ".root").c_str(), "RECREATE"));
		std::cout << "well hello there" << std::endl;
		outputTrees.push_back(inputTree->CloneTree(0));
	}
	
	std::cout << "SIZE OF TREE LIST " << outputTrees.size() << std::endl;
	
	TBranch* exist = inputTree->FindBranch("MC");
	if(exist){
		inputTree->SetBranchAddress("MC", &MC);
	}else{Log("Error: MC branch not found in the input file", v_error, verbosity);
	}
	
	numofEntries = inputTree->GetEntries();
	
	std::cout << "NUM OF ENTRIES" << numofEntries << std::endl;
	return true;
}


bool ApplySRNWeight::Execute(){
	
	if(!m_data->applyReweight){
		//		std::cout << "First apply loop" << std::endl;
		inputTree->GetEntry(entryNum);
		
		float neutP;
		neutP = sqrt(pow(MC->pvc[2][0], 2) + pow(MC->pvc[2][1], 2) + pow(MC->pvc[2][2], 2));
		neutEnergyHist->Fill(neutP);
		
		
		entryNum++;
	}else if(m_data->applyReweight){
		
		for(int bin = 1; bin < neutEnergyHist->GetNbinsX(); bin++){
			neutEBinContent.push_back(neutEnergyHist->GetBinContent(bin));
		}
		entryNum = 0;
		float writtenSum[neutEBinContent.size()] = {};
		while(entryNum < numofEntries){
			inputTree->GetEntry(entryNum);
			float neutP = sqrt(pow(MC->pvc[2][0], 2) + pow(MC->pvc[2][1], 2) + pow(MC->pvc[2][2], 2));
			for(int model = 0; model < m_data->SRNModelNames.size(); model++){
				for(int bContent = 0; bContent < m_data->SRNWeights[model].size(); bContent++){
					if(neutEBinContent[bContent] != 0 && neutP < (bContent+1)/2 && neutP > bContent/2){
						std::cout << writtenSum[bContent]/neutEBinContent[bContent] << " " << m_data->SRNWeights[model][bContent] << std::endl;
						if(writtenSum[bContent]/neutEBinContent[bContent] < m_data->SRNWeights[model][bContent]){
							outputFiles[model]->cd();
							std::cout << "before fill" << std::endl;
							outputTrees[model]->Fill();
							std::cout << "after fill" << std::endl;
							writtenSum[bContent] = writtenSum[bContent] + 1;
							std::cout << "writing to tree" << std::endl;
						}
					}
				}
			}
			entryNum++;
		}
		m_data->vars.Set("StopLoop", true);
	}
	return true;
}


bool ApplySRNWeight::Finalise(){
	std::cout << "beginning write out " << outputFiles.size() << std::endl;
	for(int model = 0; model < outputFiles.size(); model++){
		outputFiles[model]->Write();
		outputFiles[model]->Close();
	}
	newinputFile->Close();
	
	return true;
}
