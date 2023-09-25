#include "SkipTriggers.h"

SkipTriggers::SkipTriggers():Tool(){}


bool SkipTriggers::Initialise(std::string configfile, DataModel &data){
	
	if(configfile!="")  m_variables.Initialise(configfile);
	//m_variables.Print();
	
	m_data = &data;
	m_log = m_data->Log;
	
	m_variables.Get("LETrig",LETrig);
	m_variables.Get("HETrig",HETrig);
	m_variables.Get("SLETrig",SLETrig);
	m_variables.Get("ODTrig",ODTrig);
	m_variables.Get("PeriodicTrig",PeriodicTrig);
	m_variables.Get("AfterTrig",AfterTrig);
	m_variables.Get("VetoStartTrig",VetoStartTrig);
	m_variables.Get("VetoStopTrig",VetoStopTrig);
	m_variables.Get("RandWideTrig",RandWideTrig);
	m_variables.Get("IDLaserTrig",IDLaserTrig);
	m_variables.Get("LEDTrig",LEDTrig);
	m_variables.Get("NiTrig",NiTrig);
	m_variables.Get("ODLaserTrig",ODLaserTrig);
	m_variables.Get("LEHitsumTrig",LEHitsumTrig);
	m_variables.Get("HEHitsumTrig",HEHitsumTrig);
	m_variables.Get("SLEHitsumTrig",SLEHitsumTrig);
	m_variables.Get("ODHitsumTrig",ODHitsumTrig);
	m_variables.Get("SNBurstTrig",SNBurstTrig);
	m_variables.Get("MuToEleTrig",MuToEleTrig);
	m_variables.Get("LINACTrig",LINACTrig);
	m_variables.Get("LINACMicroWaveTrig",LINACMicroWaveTrig);
	m_variables.Get("PeriodicSimpTrig",PeriodicSimpTrig);
	m_variables.Get("SHETrig",SHETrig);
	m_variables.Get("AFTTrig",AFTTrig);
	m_variables.Get("PedestalTrig",PedestalTrig);
	m_variables.Get("T2KTrig",T2KTrig);
	
	triggerVect.push_back(LETrig);
	triggerVect.push_back(HETrig);
	triggerVect.push_back(SLETrig);
	triggerVect.push_back(ODTrig);
	triggerVect.push_back(PeriodicTrig);
	triggerVect.push_back(AfterTrig);
	triggerVect.push_back(VetoStartTrig);
	triggerVect.push_back(VetoStopTrig);
	triggerVect.push_back(0);
	triggerVect.push_back(0);
	triggerVect.push_back(0);
	triggerVect.push_back(RandWideTrig);
	triggerVect.push_back(IDLaserTrig);
	triggerVect.push_back(LEDTrig);
	triggerVect.push_back(NiTrig);
	triggerVect.push_back(ODLaserTrig);
	triggerVect.push_back(LEHitsumTrig);
	triggerVect.push_back(HEHitsumTrig);
	triggerVect.push_back(SLEHitsumTrig);
	triggerVect.push_back(ODHitsumTrig);
	triggerVect.push_back(0);
	triggerVect.push_back(0);
	triggerVect.push_back(SNBurstTrig);
	triggerVect.push_back(MuToEleTrig);
	triggerVect.push_back(LINACTrig);
	triggerVect.push_back(LINACMicroWaveTrig);
	triggerVect.push_back(0);
	triggerVect.push_back(PeriodicSimpTrig);
	triggerVect.push_back(SHETrig);
	triggerVect.push_back(AFTTrig);
	triggerVect.push_back(PedestalTrig);
	triggerVect.push_back(T2KTrig);
	
	// initlialise string Trigs to store trigger strings in
	std::string Trigs;
	// loop over the 31 trigger IDs and check if the trigger bit for each ID, i, is 1
	// pull trigger strings from Trigger_ID_To_Trigger using Constants::TriggerIDToTrigger 
	// and append them to trigs
	for(int i=0; i<=31; i++){
		if(triggerVect[i]) Trigs += " " + TriggerIDToTrigger(i) + ",";
	}
	// check that Trigs is not empty, or pop_back makes evDisp fail the execute
	// if Trigs is not empty then print the trigger flags that were set for the event
	// if not print a warning for the event that no trigger falgs were found
	if(!Trigs.empty()){
		Trigs.pop_back();
		std::cout << "Triggers selected:" << Trigs << "." << std::endl;
	} else{
		Log("All triggers have been set to skip!",v_error,verbosity);
		return false;
	}
	
	m_variables.Get("treeReaderName", treeReaderName);
	
	if(m_data->Trees.count(treeReaderName)==0){
		Log("Failed to find TreeReader "+treeReaderName+" in DataModel!",v_error,verbosity);
		return false;
	} else {
		myTreeReader = m_data->Trees.at(treeReaderName);
	}
	
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	
	return true;
}


bool SkipTriggers::Execute(){
	
	myTreeReader->Get("HEADER", myHeader);
	triggerID = myHeader->idtgsk;
	
	for(int i = 0; i < triggerVect.size(); i++){
		if((triggerID.test(i) && ! triggerVect[i])){
			Nskipped++;
			m_data->vars.Set("Skip",true);
			//std::cout << "Skipped due to triggers" << std::endl;
		}
	}
	
	return true;
}


bool SkipTriggers::Finalise(){
	
	triggerVect.clear();
	
	std::cout << "Number of events skipped due to triggers: " << Nskipped << std::endl;
	
	return true;
}
