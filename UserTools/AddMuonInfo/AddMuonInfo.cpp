#include "AddMuonInfo.h"

AddMuonInfo::AddMuonInfo():Tool(){}


bool AddMuonInfo::Initialise(std::string configfile, DataModel &data){
	
	if(configfile!="")  m_variables.Initialise(configfile);
	m_data= &data;
	m_log= m_data->Log;
	
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	m_variables.Get("inputName", inputName);
	m_variables.Get("muonsFile", muonsFile);
	m_variables.Get("showersFile", showersFile);
	m_variables.Get("outputName", outputName);
	m_variables.Get("outputTreeName", outputTreeName);
	
	inputFile = TFile::Open(inputName.c_str());
	inputTree = (TTree *)inputFile->Get("data");
	
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
	outputTree->Branch("spadlt", &spadlt, "spadlt/F");
	outputTree->Branch("atomicnumber", &atomicnumber, "atomicnumber/I");
	outputTree->Branch("massnumber", &massnumber, "massnumber/I");
	
	//Get MC branch
	TBranch* exist = inputTree->FindBranch("MC");
	if(exist){
		inputTree->SetBranchAddress("MC", &MC, &mcbranch);
	}else{
		std::cout << "Could not get MC branch!!!" << std::endl;
		return false;
	}
	
	exist = inputTree->FindBranch("LOWE");
	if(exist){
		inputTree->SetBranchAddress("LOWE", &LOWE, &lowebranch);
	}else{
		std::cout << "Could not get LOWE branch!!!" << std::endl;
		return false;
	}
	
	myShowers.open(showersFile);
	myMuons.open(muonsFile);
	
	std::string line;
	getline(myShowers,line);
	
	std::string line2;
	getline(myMuons, line2);
	
	entryNum = 0;
	nEntries = inputTree->GetEntries();
	
	partMasses[7] = 9999.;
	partMasses[13] = 0.13957039;
	partMasses[14] = 0.13957039;
	partMasses[23] = 0.1349768;
	
	return true;
}


bool AddMuonInfo::Execute(){
	
	int muCount, muNum, flukaID, currentMuon;	
	float x, y, z, px, py, pz;
	std::vector<ShowerParticle> showParts;
	currentMuon = 0;
	
	int muonFlag = 0;
	
	std::cout << "Executing" << std::endl;
	while (myShowers >> muCount >> muNum >> flukaID >> x >> y >> z >> px >> py >> pz) {
		if(muonFlag == 0){
			muonFlag = muNum;
		}
		
		//If we're now reading shower particles from a different muon then we need to start comparing the shower particles and clear the showParts vector
		//Should farm this out to a function really
		if(muonFlag != muNum){
			//Now need to loop over all of the shower partcles in showParts
			for(int i = 0; i < showParts.size(); i++){
				if(energyDep.size() && energyDep[energyDep.size()-1][0] == showParts[i].Pos[0] && energyDep[energyDep.size()-1][1] == showParts[i].Pos[1] && energyDep[energyDep.size()-1][2] == showParts[i].Pos[2]){
					energyDep[energyDep.size()-1][4] += (pow((showParts[i].Mom[0] + showParts[i].Mom[1] + showParts[i].Mom[2]),2))/(2*partMasses[showParts[i].PID]);
				}
				else{//if this muon's showers don't yet have an entry then add them to the vector
					std::vector<float> posEne;
					posEne.push_back(showParts[i].Pos[0]);
					posEne.push_back(showParts[i].Pos[1]);
					posEne.push_back(showParts[i].Pos[2]);
					if(partMasses[showParts[i].PID] != 9999){
						posEne.push_back((pow((showParts[i].Mom[0] + showParts[i].Mom[1] + showParts[i].Mom[2]),2))/(2*partMasses[showParts[i].PID]));
					}else{
						posEne.push_back((showParts[i].Mom[0] + showParts[i].Mom[1] + showParts[i].Mom[2]));
					}
					energyDep.push_back(posEne);
				}
			}
			for(int i = 0; i < energyDep.size(); i++){
				if(i == 0){
					maxDeposition = energyDep[i];
				}else if(maxDeposition[3] < energyDep[i][3]){
					maxDeposition = energyDep[i];
				}
			}
			//std::cout << maxDeposition[0] << "	" << maxDeposition[1] << "	" << maxDeposition[2] << std::endl;
			//std::cout << "The max energy deposition for muon " << muonFlag << " is: " << maxDeposition[3] << std::endl;
			maxDeposition.push_back(muonFlag);
			//store the point of largest energy deposition, the energy deposited and the muon number in the spallationVects vector
			//spallation vects has structure [[x, y, z, ene, muonNum], ..., [x_n, y_n, z_n, ene_n, muonNum_n]]
			spallationVects.push_back(maxDeposition);
			energyDep.clear();
			showParts.clear();
			muonFlag = muNum;
		}
		
		ShowerParticle currentPart;
		currentPart.muonID = muNum;
		currentPart.PID = flukaID;
		currentPart.Pos[0] = x;
		currentPart.Pos[1] = y;
		currentPart.Pos[2] = z;
		currentPart.Mom[0] = px;
		currentPart.Mom[1] = py;
		currentPart.Mom[2] = pz;
		showParts.push_back(currentPart);
		
	}
	
	int muonID = -9999;
	
	float entEne, exitEne, entx, enty, entz, exitx, exity, exitz;
	while (myMuons >> muCount >> muNum >> entEne >> exitEne >> entx >> enty >> entz >> exitx >> exity >> exitz) {
		if(muonID == -9999){
			inputTree->GetEntry(0);
//			std::cout << muNum << "	" << muonID << "	" << MC->pvtxvc[0][0] * 1000. << std::endl;
			entryNum = 1;
			muonID = std::round(MC->pvtxvc[0][0] * 1000);
		}
		//while(muNum == muonID){
//			std::cout << muNum << std::endl;
		//calculate the direction of the muon
		float mudir[3];
		mudir[0] = entx - exitx;
		mudir[1] = enty - exity;
		mudir[2] - entz - exitz;
		
		float muent[3];
		muent[0] = entx;
		muent[1] = enty;
		muent[2] = entz;
		
		//get the vertex of the LOWE event
		float loweVertex[4];
		
		std::copy(std::begin(LOWE->bsvertex), std::end(LOWE->bsvertex), loweVertex);
		
		float entToLowe[3];
		float ETLdotMuDir = 0;
		float MAGentToLowe = 0;
		float MAGmudir = 0;
		
		for(int i = 0; i < 3; i++){
			entToLowe[i] = loweVertex[i] - muent[i];
			ETLdotMuDir += entToLowe[i] * mudir[i];
			MAGentToLowe += entToLowe[i] * entToLowe[i];
			MAGmudir += mudir[i] * mudir[i];
		}
		
		MAGentToLowe = pow(MAGentToLowe, 0.5);
		MAGmudir = pow(MAGmudir, 0.5);
		
		spadlt = MAGentToLowe * sin(acos( ETLdotMuDir/(MAGentToLowe * MAGmudir) ));
		
		//std::cout << "spadlt			" << spadlt << std::endl;
		
		for(std::vector<float> deposition: spallationVects){
			if(deposition[4] == muNum){
				//get vector between the muon entry point and the point of spallation
				float mutoSpall[3];
				float MAGmutoSpall = 0;
				for(int i = 0; i < 3; i++){
					mutoSpall[i] = deposition[i] - muent[i];
					MAGmutoSpall += mutoSpall[i] * mutoSpall[i];
				}
				MAGmutoSpall = pow(MAGmutoSpall, 0.5);
				
//					std::cout << "MAGmutoSpall		" << MAGmutoSpall << std::endl;
//					std::cout << "ETLdotMuDir		" << ETLdotMuDir << std::endl;
//					std::cout << "MAGmudir		" << MAGmudir << std::endl;
				
				spadll = (ETLdotMuDir/MAGmudir) - MAGmutoSpall;
				
//					std::cout << "spadll		" << spadll << std::endl;
				break;
			}
		}
		
		//set all branch variables
		std::copy(std::begin(LOWE->bsvertex), std::end(LOWE->bsvertex), bsvertex);
		std::copy(std::begin(LOWE->bsdir),    std::end(LOWE->bsdir),    bsdir);
		bsgood = LOWE->bsgood[1];
		bsenergy = LOWE->bsenergy;
		bsn50 = LOWE->bsn50;
		
		atomicnumber = MC->pvtxvc[0][1];
		massnumber = MC->pvtxvc[0][2];
		
		outputTree->Fill();
		
		inputTree->GetEntry(entryNum);
		
		std::cout << muNum << "		" << muonID << "		" << entryNum << std::endl;
		
		//if(long((((float)entryNum / (float)nEntries) * 100)) % 10 == 0){
		//	std::cout << (((float)entryNum / (float)nEntries)) * 100. << "%" << std::endl;
		//}
		
		muonID = std::round(MC->pvtxvc[0][0] * 1000);
		entryNum++;
		if(entryNum >= nEntries){
			m_data->vars.Set("StopLoop", true);
			break;
		}
		//}
	}
	
	m_data->vars.Set("StopLoop", true);
	
	return true;
}


bool AddMuonInfo::Finalise(){
	outputFile->Write();
	outputFile->Close();
	inputFile->Close();
	return true;
}
