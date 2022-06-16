#include "ROOT2csv.h"

ROOT2csv::ROOT2csv():Tool(){}


bool ROOT2csv::Initialise(std::string configfile, DataModel &data){
	if(configfile!="")  m_variables.Initialise(configfile);
	//m_variables.Print();
	
	m_data= &data;
	m_log= m_data->Log;
	
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	
	m_variables.Get("treeReaderName", treeReaderName);
	m_variables.Get("outputFileName", outputFileName);
	
	if(m_data->Trees.count(treeReaderName)==0){
		Log("Failed to find TreeReader "+treeReaderName+" in DataModel!",v_error,verbosity);
		return false;
	}else {
		myTreeReader = m_data->Trees.at(treeReaderName);
	}
	
	outputFile.open(outputFileName);
	
	outputFile << "bscossun, bsdir[0], bsdir[1], bsdir[2], bsdirks, bseffhit, bsenergy, bsgood, bsn50, bspatlik, "
					"bsresult, bsvertex[0], bsvertex[1], bsvertex[2] \n";
	return true;
}


bool ROOT2csv::Execute(){
	myTreeReader->Get("HEADER", myHeader);
	myTreeReader->Get("LOWE", myLowe);
	
	outputFile << myLowe->bscossun << ",";
	for(int i = 0; i < 3; i++){
		outputFile << myLowe->bsdir[i] << ",";
	}
	outputFile << myLowe->bsdirks << ",";
	outputFile << myLowe->bseffhit[0] << ",";
	outputFile << myLowe->bsenergy << ",";
	outputFile << myLowe->bsgood << ",";
	outputFile << myLowe->bsn50 << ",";
	outputFile << myLowe->bspatlik << ",";
	outputFile << myLowe->bsresult << ",";
	for(int i = 0; i < 3; i++){
		outputFile << myLowe->bsvertex[i] << ",";
	}
	outputFile << "\n";
	return true;
}


bool ROOT2csv::Finalise(){
	
	
	return true;
}
