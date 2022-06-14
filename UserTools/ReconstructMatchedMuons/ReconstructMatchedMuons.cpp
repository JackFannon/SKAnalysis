#include "ReconstructMatchedMuons.h"
#include "fortran_routines.h"
#include "Constants.h"
#include "Algorithms.h"

#include <algorithm>

ReconstructMatchedMuons::ReconstructMatchedMuons():Tool(){}


bool ReconstructMatchedMuons::Initialise(std::string configfile, DataModel &data){
	
	if(configfile!="")  m_variables.Initialise(configfile);
	//m_variables.Print();
	
	m_data= &data;
	m_log= m_data->Log;
	
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	
	m_variables.Get("treeReaderName", treeReaderName);

	if(m_data->Trees.count(treeReaderName)==0){
	Log("Failed to find TreeReader "+treeReaderName+" in DataModel!",v_error,verbosity);
	return false;
	} else {
		myTreeReader = m_data->Trees.at(treeReaderName);
	}
	
	muonsToRec = m_data->muonsToRec;
	
	return true;
}


bool ReconstructMatchedMuons::Execute(){
	if(! muonsToRec.size()) return true;
	
	myTreeReader->Get("HEADER", myHeader);
	
	currentEntry = myTreeReader->GetEntryNumber();
	
	for(int i = 0; i < muonsToRec.size(); i++){
		if(muonsToRec[i].EntryNumber != currentEntry){
			myTreeReader->GetEntry(muonsToRec[i].EntryNumber);
			myTreeReader->Get("HEADER", myHeader);
		}
		//now fit muons using mufit
		lfclear_all_();
		//tentative routine to adjust saturation charge
		fix_maxqisk_();
		//apply mufit
		lfmufit_sk4_();
		
		float muEntry[4] = {};
		
		for(int j = 0; j < 4; j++){
			muEntry[j] = skroot_mu_.muboy_entpos[0][j];
		}
		
		//once per run water transparency update
		if(myHeader->nrunsk != lastRun){
			int days_to_run_start = skday_data_.relapse[skhead_.nrunsk];
			lfwater_(&days_to_run_start, &watert);
		}
		
		//recalculate dE/dx using Kirk's makededx.F TODO This needs importing into ToolFramework somehow!
		makededx_(&muEntry, &skroot_mu_.muboy_dir, &skchnl_.ihcab, &skq_.qisk, &skt_.tisk, &geopmt_.xyzpm, &skq_.nqisk,
				 &skhead_.nrunsk, &watert, &skroot_mu_.muinfo);
		
		//recalculate dE/dx using Scott's makededx.G
		makededx_intg_(&muEntry, &skroot_mu_.muboy_dir, &skroot_mu_.muboy_length, &skchnl_.ihcab, &skq_.qisk,
					  &skt_.tisk, &geopmt_.xyzpm, &skq_.nqisk, &skhead_.nrunsk, &skroot_mu_.muboy_dedx,
					  &sktqz_.ihtiflz, &skhead_.nevsk);
		
		float BFFPos[3];
		float HPos[3];
		float BFFGood;
		float modd;
		
		//apply BBF (single mu with goodness < 0,4 && relic energy > 12 MeV)
		
		if(skroot_mu_.muboy_status == 1 && skroot_mu_.muboy_goodness < 0.4 && *std::max_element(muonsToRec[i].matchedParticleEne.begin(), muonsToRec[i].matchedParticleEne.end()) > 12){
			std::cout << "Starting BFF..." << std::endl;
			newmufit_(&BFFPos, &HPos, &BFFGood);
			modd = sqrt(pow((HPos[0]-BFFPos[0]), 2) + pow((HPos[1]-BFFPos[1]), 2) + pow((HPos[2] - BFFPos[2]), 2));
			for(int j = 0; j < 3; j++){
				skroot_mu_.mubff_entpos[j] = BFFPos[j];
				skroot_mu_.mubff_dir[j] = (HPos[j] - BFFPos[j])/modd;
			}
			skroot_mu_.mubff_goodness = BFFGood;
			std::cout << "Ending BFF" << std::endl;
			// new dE/dx based on the brute force fitter track
			if(BFFGood > 0.3){
				for(int j =0; j < 3; j++){
					muEntry[j] = skroot_mu_.mubff_entpos[j];
				}
				makededx_(&muEntry, &skroot_mu_.muboy_dir, &skchnl_.ihcab, &skq_.qisk, &skt_.tisk, &geopmt_.xyzpm, &skq_.nqisk,
					 &skhead_.nrunsk, &watert, &skroot_mu_.muinfo);
				
				makededx_intg_(&muEntry, &skroot_mu_.muboy_dir, &skroot_mu_.muboy_length, &skchnl_.ihcab, &skq_.qisk,
								&skt_.tisk, &geopmt_.xyzpm, &skq_.nqisk, &skhead_.nrunsk, &skroot_mu_.muboy_dedx,
								&sktqz_.ihtiflz, &skhead_.nevsk);
			}
		}
		
		
		
		
	}
	
	//return the previous entry so that no issues are caused with other tools
	if(currentEntry != myTreeReader->GetEntryNumber()){
		myTreeReader->GetEntry(currentEntry);
	}
	return true;
}


bool ReconstructMatchedMuons::Finalise(){
	
	return true;
}
