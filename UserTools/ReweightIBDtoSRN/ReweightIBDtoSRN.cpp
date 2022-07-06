#include "ReweightIBDtoSRN.h"
#include <iostream>
#include <fstream>
#include <cmath>

ReweightIBDtoSRN::ReweightIBDtoSRN():Tool(){}

bool ReweightIBDtoSRN::Initialise(std::string configfile, DataModel &data){
	if(configfile!="")  m_variables.Initialise(configfile);
	//m_variables.Print();
	
	m_data= &data;
	m_log= m_data->Log;
	
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	
	m_variables.Get("inputName", inputName);
	m_variables.Get("outputName", outputName);
	
	inputFile = TFile::Open(inputName.c_str());
	inputTree = (TTree *)inputFile->Get("data");
	
	outputFile = TFile::Open(outputName.c_str(), "RECREATE");
	
	TBranch* exist = inputTree->FindBranch("MC");
	if(exist){
		inputTree->SetBranchAddress("MC", &MC);
	}
	
	TSystemDirectory* modelsDir = new TSystemDirectory(modelsPath, modelsPath);
	TList *modelList = modelsDir->GetListOfFiles();
	
	//read in SRN model fluxes
	if(!modelList){
		Log("Error: Model list is empty! Check the directory path/location of the SRN flux models", v_error, verbosity);
		return false;
	} else{
		TSystemFile* fluxFile;
		TString fileName;
		TIter next(modelList);
		while ((fluxFile=(TSystemFile*)next())){
			fileName = fluxFile->GetName();
			if (!fluxFile->IsDirectory() && !fileName.EndsWith(ext) && !fileName.EndsWith(".root")){
				TString modelName = gSystem->BaseName(fileName);
				modelName.Resize(modelName.Length() - 4);
				TObjArray* fileObj = modelName.Tokenize(".");
				modelName = ((TObjString*) fileObj->At(0))->String();
				int nBins;
				std::vector<float> flux;
				float binWidth;
				float minE;
				Read_Flux(Form("%s/%s", modelsPath, fileName.Data()), minE, binWidth, nBins, flux);
				flux_all.push_back(flux);
				minE_all.push_back(minE);
				nBins_all.push_back(nBins);
				binWidth_all.push_back(binWidth);
				modelNames.push_back(modelName.Data());
			}
		}
	}
	
	for(int i = 0; i < modelNames.size(); i++){
		std::cout << "Look at me " << modelNames[i] << std::endl;
	}
	
	//TFile* outputFile = new TFile(outputName.c_str(), "RECREATE");
	outputTree = inputTree->CloneTree(0);
	
	numofEntries = inputTree->GetEntries();
	for(int i = 0; i < modelNames.size(); i++){
		outputTree->Branch(Form("weight_%s", modelNames[i].c_str()), &weightedFlux[i], Form("weight_%s/F", modelNames[i].c_str()));
		}
	
	return true;
}


bool ReweightIBDtoSRN::Execute(){
	inputTree->GetEntry(entryNum);
	float neutP;
	float posiP;
	float cosTheta;
	
	//get the momenta of the neutrino (neutP) and the positron (posiP), and calculate the angle between the two vectors (cosTheta)
	neutP = sqrt(pow(MC->pvc[2][0], 2) + pow(MC->pvc[2][1], 2) + pow(MC->pvc[2][2], 2));
	posiP = sqrt(pow(MC->pvc[1][0], 2) + pow(MC->pvc[1][1], 2) + pow(MC->pvc[1][2], 2));
	cosTheta = (MC->pvc[1][0] * MC->pvc[2][0] + MC->pvc[1][1] * MC->pvc[2][1] + MC->pvc[1][2] * MC->pvc[2][2])/(neutP * posiP);
	for(int modelIndex; modelIndex < modelNames.size(); modelIndex++){
		float wFlux = weight_enu(neutP, cosTheta, minE_all[modelIndex], binWidth_all[modelIndex], nBins_all[modelIndex], flux_all[modelIndex]);
		weightedFlux[modelIndex] = wFlux;
	}
	
	outputTree->Fill();
	
	entryNum++;
	
	if(entryNum >= numofEntries){
		m_data->vars.Set("StopLoop", true);
		outputFile->Write();
		outputFile->Close();
		inputFile->Close();
	}
	
	return true;
}


bool ReweightIBDtoSRN::Finalise(){
	
	return true;
}


bool ReweightIBDtoSRN::Read_Flux(const char* fileName, float &minE, float &binWidth, int &nBins, std::vector<float> &flux){
	int count = 0;
	double integral = 0;
	double loc_ene;
	double loc_flux;
	double ene[2500];
	FILE *currentFile = fopen(fileName, "r");
	std::ifstream data(fileName);
	while(data >> loc_ene >> loc_flux){
		ene[count] = loc_ene;
		flux.push_back(loc_flux);
		if (count){
			integral += 0.5 * (flux[count-1] + flux[count]) * (ene[count]-ene[count-1]);
		}
		count++;
	}
	nBins = count;
	minE = ene[0];
	binWidth = ene[1]-ene[0];
	fclose(currentFile);
	return true;
}

//Reweighting function
//Input flux is in cm^-2/s/MeV
float ReweightIBDtoSRN::weight_enu(float truthE, float ctheta, float minE, float binWidth, int nbins, std::vector<float> flux){
	int binNum = (int) (floor((truthE - minE)/binWidth));
	if (binNum < 0 || binNum >= nbins || binNum > flux.size() - 2){
		//		std::cout << "FAILED " << nbins << " " << binNum << " " << truthE << " " << minE << " " << binWidth << std::endl;
		return 0;
	}
	float residual = (truthE - minE - binWidth * binNum)/binWidth;
	float yminE = flux[binNum];
	float yup = flux[binNum + 1];
	float spec = yup * residual + yminE * (1 - residual);
	float weightFlux = spec * dsigma_sv(truthE, ctheta) * protonsPerKton * fiducial * 3600 * 24 * 2790.1;
	if(weightFlux > 100){
		std::cout << "resi: " << residual << " yminE: " << yminE << " yup: " << yup << " spec: " << spec << " weightFlux: " << weightFlux << " truthE: " << truthE << std::endl;
		}
	return weightFlux;
}

double ReweightIBDtoSRN::dsigma_sv(float enu, double costheta){
	double hbarc2 = 0.389379365e-21;
	double alpha = 1./137.035999139; // PDG 2018
	double g10 = -1.270;
	double me = 0.5109989461; // PDG 2018
	double MV2 = 710*710;
	double MA2 = 1030*1030;
	double mpi = 139.57061; // PDG 2018
	double xi = (2.7928473446-1) + 1.9130427; // (nu_p - 1) - nu_n from PDG 2018
	double delta = 939.565413 - 938.272081;
	double M = 0.5 * (939.565413 + 938.272081);
	double mn = M + delta/2;
	double mp = M - delta/2;
	double Gf = 1.1663787e-11;
	double cthc = 0.97420; // PDG 2018
	double epsilon = enu/mp;
	double delta2 = (pow(mn, 2) - pow(mp, 2) - pow(me, 2))/(2 * mp);
	double kappa = pow(1 + epsilon, 2) - pow(epsilon * costheta, 2);
	double ee = ((enu - delta2) * (1 + epsilon) + epsilon * costheta * sqrt(pow(enu - delta2, 2) - pow(me, 2) * kappa))/kappa;
	double t = pow(mn, 2) - pow(mp, 2) - 2 * mp * (enu - ee);
	double f1 = (1 - (1 + xi) * t/pow(2 * M, 2))/((1 - t/pow(2 * M, 2)) * pow(1 - t/MV2, 2));
	double f2 = xi/((1 - t/pow(2 * M, 2)) * pow(1 - t/MV2, 2));
	double g1 = g10/pow(1 - t/MA2, 2);
	double g2 = 2 * pow(M, 2) * g1/(pow(mpi, 2) - t);
	double A = (t - pow(me, 2)) * (4 * pow(f1, 2) * (4 * pow(M, 2) + t + pow(me, 2)) + 4 * pow(g1, 2) * (-4 * pow(M, 2) + t + pow(me, 2)) + pow(f2, 2) * (pow(t/M, 2) + 4 * t + 4 * pow(me, 2)) + 4 * pow(me, 2) * t * pow(g2/M, 2) + 8 * f1 * f2 * (2 * t + pow(me, 2)) + 16 * pow(me, 2) * g1 * g2) - pow(delta, 2) * ((pow(2 * f1, 2) + t * pow(f2/M, 2)) * (pow(2 * M, 2) + t - pow(me, 2)) + 4 * pow(g1, 2) * (pow(2 * M, 2) - t + pow(me, 2)) + pow(2 * me * g2, 2) * (t - pow(me, 2))/pow(M, 2) + 8 * f1 * f2 * (2 * t - pow(me, 2)) + pow(4 * me, 2) * g1 * g2) - 32 * pow(me, 2) * M * delta * g1 * (f1 + f2);
	A /= 16;
	double B = t * g1 * (f1 + f2) + 0.25 * pow(me, 2) * delta * (pow(f2, 2) + f1 * f2 + 2 * g1 * g2)/M;
	double C = 0.25 * (pow(f1, 2) + pow(g1, 2)) - 1./16 * t * pow(f2/M, 2);

	double smu = 2 * mp * (enu + ee) - pow(me, 2);
	double smp2 = 2 * mp * enu;
	double m2 = A - smu * B + pow(smu, 2) * C;
	double pe = sqrt(pow(ee, 2) - pow(me, 2));
	double fact = pe * epsilon/(1 + epsilon * (1 - ee/pe * costheta));
	double rad = 1 + alpha/M_PI * (6.0 + 3./2 * log(mp/(2 * ee)) + 1.2 * pow(me/ee, 1.5));
	double dsigmadee = 2 * mp * hbarc2 * pow(Gf * cthc/smp2, 2)/(2 * M_PI) * m2 * fact ;//* rad;
	return dsigmadee;
}
