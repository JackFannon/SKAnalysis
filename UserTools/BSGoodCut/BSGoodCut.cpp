#include "BSGoodCut.h"
#include "fortran_routines.h"
#include "Constants.h"
BSGoodCut::BSGoodCut():Tool(){}


bool BSGoodCut::Initialise(std::string configfile, DataModel &data){
	if(configfile!="")  m_variables.Initialise(configfile);
	
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
	
	return true;
}


bool BSGoodCut::Execute(){
	
	myTreeReader->Get("LOWE", myLowe);
	
	float reconGoodness = myLowe->bsgood[1];
	float reconDirKS = myLowe->bsdirks;
	
	if(reconGoodness < 0.5 || (pow(reconGoodness,2) - pow(reconDirKS,2)) < 0.25 ){
		m_data->vars.Set("Skip", true);
	}
	
	return true;
}


bool BSGoodCut::Finalise(){
	
	return true;
}
