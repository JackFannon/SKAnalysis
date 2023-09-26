#include "evPlot.h"

evPlot::evPlot():Tool(){}


bool evPlot::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;
  m_log= m_data->Log;

  if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;

  return true;
}


bool evPlot::Execute(){

  return true;
}


bool evPlot::Finalise(){

  return true;
}
