#include "CalcSpadl.h"

CalcSpadl::CalcSpadl():Tool(){}


bool CalcSpadl::Initialise(std::string configfile, DataModel &data){
	if(configfile!="")  m_variables.Initialise(configfile);
	m_data= &data;
	m_log= m_data->Log;
	
	//get input file name
	m_variables.Get("inputName", inputName);
	//get output file name
	m_variables.Get("outputName", outputName);
	//get output tree name
	if(!m_variables.Get("outputTreeName", outputTreeName)) outputTreeName = "spallEvents";
	
	//read input file
	inputFile = TFile::Open(inputName.c_str());
	inputTree = (TTree *)inputFile->Get("data");
	//get branches from input file
	//reconstructed information
	inputTree->SetBranchAddress("LOWE", &myLowe, &lowebranch);
	//monte-carlo information
	inputTree->SetBranchAddress("MC", &myMC, &mcbranch);
	//muon branch
	inputTree->SetBranchAddress("MU", &myMu, &mubranch);
	
	
	//create output file
	outputFile = TFile::Open(outputName.c_str(), "RECREATE");
	//create output tree
	outputTree = new TTree(outputTreeName.c_str(), outputTreeName.c_str());
	//create output branches
	outputTree->Branch("bsvertex", &bsvertex, "bsvertex[4]/F");
	outputTree->Branch("bsdir", &bsdir, "bsdir[3]/F");
	outputTree->Branch("bsgood", &bsgood, "bsgood/F");
	outputTree->Branch("bsenergy", &bsenergy, "bsenergy/F");
	outputTree->Branch("bsn50", &bsn50, "bsn50/F");
	outputTree->Branch("spadll", &spadll, "spadl/F");
	outputTree->Branch("spadll_kirk", &spadll_kirk, "spadll_kirk/F");
	outputTree->Branch("spadlt", &spadlt, "spadlt/F");
	
	//get verbosity if set
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	
	entrynum = 0;
	nEntries = inputTree->GetEntries();
	
	/* TreeReader is segfaulting when trying to read the first entry of the MC lomu files
	#######################################
	
	//Get treereader name
	m_variables.Get("treeReaderName", treeReaderName);
	
	//Check treereader instance exits, if so get it
	if(m_data->Trees.count(treeReaderName)==0){
		Log("Failed to find TreeReader "+treeReaderName+" in DataModel!",v_error,verbosity);
		return false;
	} else {
		myTreeReader = m_data->Trees.at(treeReaderName);
	}
	
	//Get branches from Tree Reader
	//MC branch
	myTreeReader->Get("MC", myMC);
	//LOWE branch
	myTreeReader->Get("LOWE", myLowe);
	//HEADER branch
	myTreeReader->Get("HEADER", myHeader);
	//MU branch
	myTreeReader->Get("MU", myMu);
	#######################################
	*/
	
	return true;
}


bool CalcSpadl::Execute(){
	inputTree->GetEntry(entrynum);
	
	float appr;
	std::cout << entrynum << std::endl;
	//Skip neutrons for now
	if(myMC->ipvc[0] == 22){
		entrynum++;
		if(entrynum >= nEntries|| entrynum == 3000){
			m_data->vars.Set("StopLoop", true);
		}
		return true;
	}
	
	//is event a muon
	if(myMC->ipvc[0] == 13){
		for(int i = 0; i < 3; i++){
			muent[i] = myMu->muboy_entpos[0][i];
			mudir[i] = myMu->muboy_dir[i];
		}
		for(int i = 0; i < 200; i++){
			dedx[i] = myMu->muboy_dedx[i];
			//kirk_dedx[i] = *reinterpret_cast<const float*>(&myMu->muinfo[i]);
			//kirk_dedx[i] = static_cast<float>(myMu->muinfo[i]);
		}
	}else if(myMC->ipvc[0] == -12){
		//calculate the transverse distance dlt
		float loweVert[3];
		for(int i = 0; i < 3; i++){
			loweVert[i] = myLowe->bsvertex[i];
		}
		getdl(mudir, loweVert, muent, spadlt, appr);
		
		//create variables for Scott dedx dll calculation
		int scottIndex = 0;
		float peakSpall = 0.;
		//Scott dedx dll
		GetPeakScott(dedx, scottIndex);
		peakSpall = 50. * (scottIndex + 4);
		
		//create variables for Kirk's dedx dll calculation
		int kirkIndex = 0;
		float peakKirk =0.;
		//Kirk's dedx dll
		//GetPeakKirk(kirk_dedx, kirkIndex);
		//peakKirk = 50. * ((kirkIndex - 49) + 4);
		
		std::copy(std::begin(myLowe->bsvertex), std::end(myLowe->bsvertex), bsvertex);
		std::copy(std::begin(myLowe->bsdir),    std::end(myLowe->bsdir),    bsdir);
		bsgood = myLowe->bsgood[1];
		bsenergy = myLowe->bsenergy;
		bsn50 = myLowe->bsn50;
		
		spadll = peakSpall - appr;
		spadll_kirk = peakKirk - appr;
		
		outputTree->Fill();
	}
	
	//increment entry number
	entrynum++;
	
	//check to see if we're at the final entry, if so stop the loop
	if(entrynum >= nEntries || entrynum == 3000){
		m_data->vars.Set("StopLoop", true);
	}
	
	return true;
}


bool CalcSpadl::Finalise(){
	inputFile->Close();
	outputFile->Write();
	outputFile->Close();
	return true;
}

bool CalcSpadl::getdl(float mudir[3], float v[3], float muent[3], float &lt, float &proj){
	float diff[3];
	for(int i = 0; i < 3; i++) diff[i] = muent[i] - v[i];
	float prod[3];
	prod[0] = diff[1] * mudir[2] - diff[2] * mudir[1];
	prod[1] = diff[2] * mudir[0] - diff[0] * mudir[2];
	prod[2] = diff[0] * mudir[1] - diff[1] * mudir[0];
	lt = sqrt(pow(prod[0], 2) + pow(prod[1], 2) + pow(prod[2], 2));
	proj = (v[0] - muent[0]) * mudir[0] + (v[1] - muent[1]) * mudir[1] + (v[2] - muent[2]) * mudir[2];
	return true;
}

bool CalcSpadl::GetPeakScott(float dedx[200], int &index){
	int total = 0;
	for(int i = 0; i < 111; i++){
		int sum = 0;
		for(int j = 0; j < 10; j++){
			sum = sum + dedx[i+j];
		}
		if(sum > total){
			index = i;
			total = sum;
		}
	}
	return true;
}

bool CalcSpadl::GetPeakKirk(float dedx_kirk[200], int &index){
	int total_kirk = 0;
	for(int i = 0; i < 191; i++){
		int sum = 0;
		for (int j = 0; j < 10; j++){
			sum = sum + dedx_kirk[i+j];
		}
		if(sum > total_kirk){
			index = i;
			total_kirk = sum;
		}
	}
	return true;
}