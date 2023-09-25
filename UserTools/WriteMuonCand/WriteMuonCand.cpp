#include "WriteMuonCand.h"
#include "fortran_routines.h"
#include "Constants.h"
#include "Algorithms.h"

WriteMuonCand::WriteMuonCand():Tool(){}


bool WriteMuonCand::Initialise(std::string configfile, DataModel &data){
	
	if(configfile!="")  m_variables.Initialise(configfile);
	//m_variables.Print();
	
	m_data= &data;
	m_log= m_data->Log;
	
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	
	m_variables.Get("treeReaderName", treeReaderName);
	m_variables.Get("treeWriterName", treeWriterName);
	
	myTreeWriter = m_data->Trees.at(treeWriterName);
	
	if(m_data->Trees.count(treeReaderName)==0){
	Log("Failed to find TreeReader "+treeReaderName+" in DataModel!",v_error,verbosity);
	return false;
	} else {
		myTreeReader = m_data->Trees.at(treeReaderName);
	}
	
	muonsToRec = m_data->muonsToRec;

	return true;
}


bool WriteMuonCand::Execute(){
	if(! muonsToRec.size()) return true;
	
	myTreeReader->Get("HEADER", myHeader);
	
	int currentEntry = myTreeReader->GetEntryNumber();
	
	for(int i = 0; i < muonsToRec.size(); i++){
		if(muonsToRec[i].EntryNumber != currentEntry){
			myTreeReader->GetEntry(muonsToRec[i].EntryNumber);
			myTreeReader->Get("HEADER", myHeader);
		}
		
		if(currentRun != myHeader->nrunsk){
			if(!currentRun){
				currentRun = myHeader->nrunsk;
				outputFile = "muons_"+to_string(currentRun)+".root";
			}else{
				currentRun = myHeader->nrunsk;
				outputFile = "muons_"+to_string(currentRun)+".root";
			}
		}
		
		lun = 20;
		
		//skroot_open_write_(&lun, outputFile.c_str(), outputFile.size());
		
		int io = 1;
		
		for(int i; i < branchestoSkip.size(); i++){
			skroot_zero_branch_(&lun, &io, branchestoSkip[i].c_str(), branchestoSkip[i].size());
		}
		
		skroot_set_mu_(&lun, 
						skroot_mu_.muentpoint, 
						skroot_mu_.mudir, 
						&skroot_mu_.mutimediff, 
						&skroot_mu_.mugoodness,
						&skroot_mu_.muqismsk, 
						&skroot_mu_.muyn, 
						&skroot_mu_.mufast_flag, 
						&skroot_mu_.muboy_status,
						&skroot_mu_.muboy_ntrack, 
						skroot_mu_.muboy_entpos, 
						skroot_mu_.muboy_dir,
						&skroot_mu_.muboy_goodness, 
						&skroot_mu_.muboy_length, 
						skroot_mu_.muboy_dedx, 
						skroot_mu_.mubff_entpos,
						skroot_mu_.mubff_dir, 
						&skroot_mu_.mubff_goodness, 
						&skroot_mu_.muninfo, 
						skroot_mu_.muinfo);
		
		delete_outside_hits_();
		skroot_set_tree_(&lun);
		skroot_fill_tree_(&lun);
	}
	
	//clear m_data->muonsToRec now that the contents of muonsToRec have been written out to a file.
	m_data->muonsToRec.clear();
	
	return true;
}


bool WriteMuonCand::Finalise(){
	
	return true;
}
