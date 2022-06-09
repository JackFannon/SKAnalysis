#include "WallCut.h"
#include "geotnkC.h"
#include "fortran_routines.h"
#include "Constants.h"

#include <cmath>

WallCut::WallCut():Tool(){}


bool WallCut::Initialise(std::string configfile, DataModel &data){
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
	
	return true;
}


bool WallCut::Execute(){
	
	myTreeReader->Get("LOWE", myLowe);
	
	reconVertex = myLowe->bsvertex;
	
	float xyDistance;
	float IVRadius = 1690.;
	float wallDistance;
	float posHeight = 1810.;
	float negHeight = -1810.;
	
	
	xyDistance = sqrt(pow(reconVertex[0], 2) + pow(reconVertex[1], 2));
	
	
	float zDistance;
	
	if(reconVertex[2] < 0){zDistance = reconVertex[2] + posHeight;}
	else{zDistance = posHeight - reconVertex[2];}
	
	wallDistance = abs(IVRadius - xyDistance);
	
	if(wallDistance > zDistance){wallDistance = zDistance;}
	
	if(wallDistance < 200){
		m_data->vars.Set("Skip",true);
		std::cout << "Skipped due to wall" << std::endl;
	}
	
	return true;
}


bool WallCut::Finalise(){
	
	return true;
}
