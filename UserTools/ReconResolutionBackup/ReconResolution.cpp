#include "ReconResolution.h"

#include "TROOT.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
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
#include "TLine.h"
#include "TGraph.h"
#include "TSpline.h"
#include "TGraphErrors.h"

#include <map>

ReconResolution::ReconResolution():Tool(){}


bool ReconResolution::Initialise(std::string configfile, DataModel &data){
	if(configfile!="")  m_variables.Initialise(configfile);
	
	m_data= &data;
	m_log= m_data->Log;
	
	m_variables.Get("verbosity",verbosity);
	m_variables.Get("treeReaderName",treeReaderName);
	m_variables.Get("dataSrc",dataSrc);
	
	
	if(dataSrc == -1 || dataSrc == 1){
		// if getting data from TTree, check the TreeReader
		 if(m_data->Trees.count(treeReaderName)==0){
			Log("Failed to find TreeReader "+treeReaderName+" in DataModel!",v_error,verbosity);
			return false;
		} else {
			myTreeReader = m_data->Trees.at(treeReaderName);
		}
	}
	
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	
	//set drawing options for the graph
	gStyle->SetOptStat( 0 );
	gStyle->SetPalette( 56 );
	gStyle->SetFillStyle( 4000 );
	gStyle->SetLabelSize(0.03, "xy");
	gStyle->SetTitleSize(0.04, "xy");
	gStyle->SetTitleOffset(1.5, "y");
	gStyle->SetTitleOffset(1.2, "x");
	
	//set binning on the histogram for 1 bin == 1 cm. Done here as setting this in the header file requires
	//changes to also be made here to compile properly. Could do this through a config variable instead
	vertexResH->SetBins(400, 0, 400);
	
	//fill the 2D vector eneVertexDiff with 50 empty vectors.
	//these are filled later and represent each energy 0-1, 1-2, 2-3, 3-4 MeV etc.
	for(int i = 0; i < 60; i++){
		eneVertexDiff.push_back(std::vector<float>());
	}
	
	return true;
}


bool ReconResolution::Execute(){
	
	myTreeReader->Get("MC", myMC);
	myTreeReader->Get("LOWE", myLowe);
	
	auto reconVertex = myLowe->bsvertex;
	
	// calculate and print the difference between the MC vector and the reconstructed vector
	float vertTotDiff =0.;
	for(int i = 0; i < 3; i++){
		//std::cout << "RECON VERTEX: "<< reconVertex[i] << " MC VERTEX: " << myMC->pvtxvc[0][i+3] << std::endl;
		vertTotDiff += pow((reconVertex[i]-myMC->pvtxvc[0][i+3]), 2);
	}
	
	// calculate the absolute difference between the vertices
	float vectDifference = sqrt(vertTotDiff);
	
	//fill the appropriate vector in the 2D vector eneVertexDiff with the energy by determining the range that the
	//energy sits in.
	for(int i = 1; i < 51; i++){
		if(i < myLowe->energymc[0] && myLowe->energymc[0] < i + 1){
			eneVertexDiff[i-1].push_back(vectDifference);
		}
	}
	
	return true;
}


bool ReconResolution::Finalise(){
	
	std::cout << "THERE WERE " << overFourHund << " EVENTS OVER 400!!!!!!!!" << std::endl;
	
	//ROOT things
	TCanvas* c1 = new TCanvas( "c1", "c1", 900, 900 );
	c1->UseCurrentStyle();
	c1->SetTopMargin( 0.06 );
	c1->SetBottomMargin( 0.10 );
	c1->SetLeftMargin( 0.16 );
	c1->SetRightMargin( 0.08 );
	c1->SetFillStyle( 4000 );
	
	//instance the vectors errorVec and resolution to be filled within the loop below and then read after the loop
	std::vector<float> errorVec;
	std::vector<float> resolution;
	
	//loop over the vectors in eneVertexDiff using 'i' as the vector
	for(std::vector<float> i: eneVertexDiff){
		
		std::cout << i.size() << std::endl;
		
		if(not i.size()){
			continue;
		}
		
		//create two histograms
		//energyHist is used to retrieve the reconstruction resolution and uses more bins to give a more accurate
		//fit
		//otherHist is used to calculate the error on the reconstruction resolution. Less bins are used here as
		//otherwise there are not enough entries in the bin to calculate a suitable error
		TH1F* energyHist = new TH1F("", "", 400, 0, 400);
		TH1F* otherHist = new TH1F("", "", 80, 0, 400);
		
		//fill the histograms with the entries, j, from the vectors, i.
		for(float j: i){
			energyHist->Fill(j);
			otherHist->Fill(j);
		}
		
		//create a histogram to be used as the cummulative values for the resolution
		TH1F* vertexResHCN = new TH1F("","",400,0,400);
		
		//fill the histogram vertexResHCN with the vertex resolutions plotted cummulatively
		int runningsumN = 0;
		for(int bin=1; bin <= energyHist->GetNbinsX(); bin++){
			runningsumN += energyHist->GetBinContent(bin);
			vertexResHCN->SetBinContent(bin, runningsumN);
		}
		
		//normalise the vertexResHCN histogram by scaling the bin content by 1/(# of entries in the last bin)
		vertexResHCN->Scale(1/vertexResHCN->GetBinContent(vertexResHCN->GetNbinsX()));
		
		//loop over the bins of the vertexResHCN histogram until the point where the bin content is > 0.75. This
		//is to find an upper limit for the fit.
		float res = 0;
		int bin = 0;
		while(res < 0.75){
			bin++;
			if(bin > vertexResHCN->GetNbinsX()){
				break;
			}
			else{res = vertexResHCN->GetBinContent(bin);
			}
		}
		
		//fit the histogram with a quadratic
		vertexResHCN->Fit("pol2", "", "", 25, bin);
		
		//retrieve the function
		TF1* myfunc = vertexResHCN->GetFunction("pol2");
		
		//instance coefficients as the parameters.
		//the intercept has 0.68 taken away from it to calculate the point at y = 0.68 i.e. where 68% of events
		//have been reconstructed
		double intercept = myfunc->GetParameter(0) - 0.68;
		double linear = myfunc->GetParameter(1);
		double quad = myfunc->GetParameter(2);
		
		//calculate the resolution using the quadratic formula
		float fitRes = (- linear + pow(pow(linear,2)- 4. * quad * intercept, 0.5))/(2. * quad);
		
		//find the number of entries in the bin of otherHist where the resolution sits.
		//the /400 * 80 component is a compensation for the difference in binning
		if(otherHist->GetBinContent(round((fitRes/400.) * 80.))){
			errorVec.push_back(otherHist->GetBinContent(round((fitRes/400.) * 80.)));
		}else{errorVec.push_back(1);}
		
		//check for nan, this occurs for the case where energy = 0-1 as there are no entries in these bins
		if(isnan(fitRes)){
			resolution.push_back(0.);
		}else{resolution.push_back(fitRes);}
		
		std::cout << std::endl <<  "The resolution of the reconstruction is: " << fitRes << std::endl;
		
		//otherHist->Draw();
		//c1->WaitPrimitive();
	}
	
	//instance a TGraphErrors to plot the reconstruction resolution as a function of energy
	TGraphErrors* res_g = new TGraphErrors(0);
	
	//plot the points of reconstruction resolution against the energy (binned as ones so this is easy) and then
	//add the errors, which are calculated as the resolution divided by the number of entries in the bin that
	//contains that reconstruction difference.
	//this is a bit of a "fudge" (Liz Kneale circa. 2022) as the distribution isn't gaussian so the 68% value
	//is also a bit of a cheat
	for(int i = 0; i < resolution.size(); i++){
		std::cout << "Plotting: " << i+1 << " against " << resolution[i] << " with an error " << resolution[i] / pow(errorVec[i], 0.5) << std::endl;
		res_g->SetPoint(i, i+1, resolution[i]);
		res_g->SetPointError(i, 0., resolution[i] / pow(errorVec[i], 0.5));
	}
	
	
	
	//ROOT graph options and title settings
	//I don't know why, but apparently to not have a title present on a graph you need the third line down below.
	//Otherwise the graph is saved with "Graph" in a big box plastered on it, just in case we didn't understand
	//what we were looking at
	res_g->SetMarkerStyle(20);
	res_g->Draw("AP");
	res_g->SetTitle("");
	res_g->GetXaxis()->SetTitle("Positron energy (MeV)");
	res_g->GetYaxis()->SetTitle("Reconstruction resolution (cm)");
	res_g->GetXaxis()->SetLimits(0., 50.);
	
	//save the graph
	c1->SaveAs("./PDFs/ReconstructionPlots/r62859_NotToolRecon.pdf");
	
	//keep ToolFramework happy
	return true;
}
