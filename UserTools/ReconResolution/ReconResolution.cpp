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
#include "TMultiGraph.h"

#include <map>

ReconResolution::ReconResolution():Tool(){}


bool ReconResolution::Initialise(std::string configfile, DataModel &data){
	if(configfile!="")  m_variables.Initialise(configfile);
	
	m_data= &data;
	m_log= m_data->Log;
	
	m_variables.Get("verbosity",verbosity);
	m_variables.Get("firstReaderName",FirstReader);
	m_variables.Get("dataSrc",dataSrc);
	m_variables.Get("comparison",comparison);
	
	if(comparison){
		m_variables.Get("secondReaderName",SecondReader);
	}
	
	if(dataSrc == -1 || dataSrc == 1){
		// if getting data from TTree, check the TreeReader
		 if(m_data->Trees.count(FirstReader)==0){
			Log("Failed to find TreeReader "+FirstReader+" in DataModel!",v_error,verbosity);
			return false;
		} else {
			myFirstReader = m_data->Trees.at(FirstReader);
			if(comparison){
				mySecondReader = m_data->Trees.at(SecondReader);
			}
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
	
	//fill the 2D vector eneVertexDiff with 50 empty vectors.
	//these are filled later and represent each energy 0-1, 1-2, 2-3, 3-4 MeV etc.
	for(int i = 0; i < 60; i++){
		eneVertexDiff.push_back(std::vector<float>());
		if(comparison){
			eneVertexDiff2.push_back(std::vector<float>());
		}
	}
	return true;
}


bool ReconResolution::Execute(){
	
	eneVertexDiff = calcVertexDifference(myFirstReader, eneVertexDiff);
	if(comparison){
		eneVertexDiff2 = calcVertexDifference(mySecondReader, eneVertexDiff2);
	}
	
	return true;
}


bool ReconResolution::Finalise(){
	
	//ROOT things
	TCanvas* c1 = new TCanvas( "c1", "c1", 900, 900 );
	c1->UseCurrentStyle();
	c1->SetTopMargin( 0.06 );
	c1->SetBottomMargin( 0.10 );
	c1->SetLeftMargin( 0.16 );
	c1->SetRightMargin( 0.08 );
	c1->SetFillStyle( 4000 );
	
	TMultiGraph* res_mg = new TMultiGraph();
	
	//instance the vectors errorVec and resolution to be filled within the loop below and then read after the loop
	std::vector<float> errorVec;
	std::vector<float> newResolution;
	calcResolution(eneVertexDiff, errorVec, newResolution);
	TGraphErrors* firstGraph = MakeGraph(newResolution, errorVec, 1);
	res_mg->Add(firstGraph);
	
	if(comparison){
		std::vector<float> errorVec2;
		std::vector<float> newResolution2;
		calcResolution(eneVertexDiff2, errorVec2, newResolution2);
		TGraphErrors* secondGraph = MakeGraph(newResolution2, errorVec2, 2);
		res_mg->Add(secondGraph);
	}
	
	
	//ROOT graph options and title settings
	//I don't know why, but apparently to not have a title present on a graph you need the third line down below.
	//Otherwise the graph is saved with "Graph" in a big box plastered on it, just in case we didn't understand
	//what we were looking at
	res_mg->Draw("AP");
	res_mg->SetTitle("");
	res_mg->GetXaxis()->SetTitle("Positron energy (MeV)");
	res_mg->GetYaxis()->SetTitle("Reconstruction resolution (cm)");
	res_mg->GetHistogram()->GetXaxis()->SetRangeUser(0., 50.);

	//save the graph
	c1->SaveAs("./PDFs/ReconstructionPlots/r62859_NotToolRecon.pdf");
	
	if(c1) delete c1;
	
	return true;
}

TGraphErrors* ReconResolution::MakeGraph(std::vector<float> resolutionVect, std::vector<float> errorVect, int colour){
	//instance a TGraphErrors to plot the reconstruction resolution as a function of energy
	TGraphErrors* res_g = new TGraphErrors(0);
	
	//plot the points of reconstruction resolution against the energy (binned as ones so this is easy) and then
	//add the errors, which are calculated as the resolution divided by the number of entries in the bin that
	//contains that reconstruction difference.
	//this is a bit of a "fudge" (Liz Kneale circa. 2022) as the distribution isn't gaussian, the 68% value
	//is also a bit of a cheat
	for(int i = 0; i < resolutionVect.size(); i++){
		std::cout << "Plotting: " << i+1 << " against " << resolutionVect[i] << " with an error " << resolutionVect[i] / pow(errorVect[i], 0.5) << std::endl;
		res_g->SetPoint(i, i+1, resolutionVect[i]);
		res_g->SetPointError(i, 0., resolutionVect[i] / pow(errorVect[i], 0.5));
	}
	
	res_g->SetMarkerStyle(20);
	res_g->SetMarkerColor(colour);
	
	return res_g;
}

std::vector<std::vector<float>> ReconResolution::calcVertexDifference(MTreeReader* TreeReadersName, std::vector<std::vector<float>> vectdiffVertex){
	const MCInfo* myMC=nullptr;
	const LoweInfo* myLowe=nullptr;
	
	TreeReadersName->Get("MC", myMC);
	TreeReadersName->Get("LOWE", myLowe);
	
	auto reconVertex = myLowe->bsvertex;
	
	// calculate and print the difference between the MC vector and the reconstructed vector
	float vertTotDiff =0.;
	for(int i = 0; i < 3; i++){
		//std::cout << "RECON VERTEX: "<< reconVertex[i] << " MC VERTEX: " << myMC->pvtxvc[0][i] << std::endl;
		vertTotDiff += pow((reconVertex[i]-myMC->pvtxvc[0][i]), 2);
	}
	
	// calculate the absolute difference between the vertices
	float vectDifference = sqrt(vertTotDiff);
	
	//fill the appropriate vector in the 2D vector eneVertexDiff with the energy by determining the range that the
	//energy sits in.
	for(int i = 1; i < 51; i++){
		if(i < myLowe->energymc[0] && myLowe->energymc[0] < i + 1){
			vectdiffVertex[i-1].push_back(vectDifference);
		}
	}
	
	
	return vectdiffVertex;
}

bool ReconResolution::calcResolution(std::vector<std::vector<float>> vertexDiffvect, std::vector<float>& errorVec, std::vector<float>& resolution){
	//loop over the vectors in eneVertexDiff using 'i' as the vector
	for(std::vector<float> i: vertexDiffvect){
		
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
		
		delete energyHist;
		delete otherHist;
		delete vertexResHCN;
		
	}
	return true;
}


