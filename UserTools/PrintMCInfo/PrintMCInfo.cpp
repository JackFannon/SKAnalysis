#include "PrintMCInfo.h"

#include "TROOT.h"
#include "TCanvas.h"
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
#include "THStack.h"

PrintMCInfo::PrintMCInfo():Tool(){}


bool PrintMCInfo::Initialise(std::string configfile, DataModel &data){
	
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
		}else {
			myTreeReader = m_data->Trees.at(treeReaderName);
		}
	}
	
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	
	gStyle->SetOptStat( 0 );
	gStyle->SetPalette( 56 );
	gStyle->SetFillStyle( 4000 );
	gStyle->SetLabelSize(0.03, "xy");
	gStyle->SetTitleSize(0.02, "xy");
	gStyle->SetLabelOffset(0.01, "xy");
	gStyle->SetTitleOffset(1.5, "xy");
	
	
	vertexResHx->SetBins(50, -200, 200);
	vertexResHy->SetBins(50, -200, 200);
	vertexResHz->SetBins(50, -200, 200);
	
	return true;
}


bool PrintMCInfo::Execute(){
	// get MC, LOWE, and HEADER branches
	myTreeReader->Get("MC", myMC);
	myTreeReader->Get("LOWE", myLowe);
	myTreeReader->Get("HEADER", myHeader);
	
	numberofEvents++;
	
	// print out event number, MCVertex and reconstructed vertex
	std::cout << "Event number: " << myHeader->nevsk << std::endl;
	std::cout << "MCVertex: (" << myMC->pvtxvc[0][0] << ", " << myMC->pvtxvc[0][1] << ", " << myMC->pvtxvc[0][2] << ")" << std::endl;
	auto reconVertex = myLowe->bsvertex;
	std::cout << "Reconstructed vertex: (" << reconVertex[0] << ", " << reconVertex[1] << ", " << reconVertex[2] << ", " << reconVertex [3] << ")" << std::endl;
	std::cout << "MCEnergy: " << myMC->energy[2] << std::endl;
	
	// check if the reconstruction was successful, if not skip the rest of the execute
	if(reconVertex[0] == 9999){
		std::cout << "The reconstruction failed for this event." << std::endl;
		failedRecon++;
		std::cout << std::endl;
		return true;
	}
	
	// calculate and print the difference between the MC vector and the reconstructed vector
	std::cout << "The difference between the reconstructed coordinates and the MC coordinates is: ";
	float vertTotDiff =0.;
	for(int i = 0; i < 3; i++){
		float vertDiff = std::abs(reconVertex[i] - myMC->pvtxvc[0][i]);
		std::cout << vertDiff << ", ";
		vertTotDiff += pow((reconVertex[i]-myMC->pvtxvc[0][i]), 2);
	}
	std::cout << std::endl;
	
	// calculate and print out the absolute difference between the vertices
	float vectDifference = sqrt(vertTotDiff); 
	std::cout << "The total difference between the reconstructed and MC vertex is : " << vectDifference << std::endl;
	
	// fill the histograms with the difference between the individual coordinates
	vertexResHx->Fill(myMC->pvtxvc[0][0]-reconVertex[0]);
	vertexResHy->Fill(myMC->pvtxvc[0][1]-reconVertex[1]);
	vertexResHz->Fill(myMC->pvtxvc[0][2]-reconVertex[2]);
	
	std::cout << std::endl;
	
	return true;
}


bool PrintMCInfo::Finalise(){
	
	/*for(int i = 0; i < 50; i++){
		std::cout << differenceList[i] << "	" << differenceNums[i] << std::endl;
		if(differenceNums[i] > 0){
			vertexResHx->SetBinContent(i, differenceList[i]/differenceNums[i]);
		}
		else{
			vertexResHx->SetBinContent(i, 0);
		}
	}*/
	
	std::cout << "The fraction of events that failed to reconstruct is " << failedRecon/numberofEvents << std::endl;
	
	TCanvas* c1 = new TCanvas( "c1", "c1", 900, 900 );
	c1->UseCurrentStyle();
	c1->SetTopMargin( 0.06 );
	c1->SetBottomMargin( 0.10 );
	c1->SetLeftMargin( 0.16 );
	c1->SetRightMargin( 0.08 );
	c1->SetFillStyle( 4000 );
	
	vertexResHx->SetLineColor(kRed);
	vertexResHy->SetLineColor(kBlue);
	vertexResHz->SetLineColor(kGreen);
	
	THStack* hs = new THStack("hs","");
	hs->Add(vertexResHx);
	hs->Add(vertexResHy);
	hs->Add(vertexResHz);
	
	hs->Draw("nostack");
	
	hs->GetXaxis()->SetTitle("MC vertex - reconstructed vertex (cm)");
	
	TLegend* lg1 = new TLegend(0.75, 0.75, 0.95, 0.95);
	lg1->SetFillStyle(0);
	lg1->SetBorderSize(0);
	lg1->AddEntry(vertexResHx, "X coordinate", "l");
	lg1->AddEntry(vertexResHy, "Y coordinate", "l");
	lg1->AddEntry(vertexResHz, "Z coordinate", "l");
	
	lg1->Draw();
	
	c1->SaveAs("testPDF.pdf");
	
	return true;
}

