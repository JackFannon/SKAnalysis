#include "RelicMuonMatching.h"
#include "fortran_routines.h"
#include "Constants.h"
#include "skheadC.h"
#include "ParticleCand.h"

RelicMuonMatching::RelicMuonMatching():Tool(){}


bool RelicMuonMatching::Initialise(std::string configfile, DataModel &data){
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


bool RelicMuonMatching::Execute(){
	
	std::cout << "MADE IT HERE" << std::endl;
	
	myTreeReader->Get("HEADER", myHeader);
	myTreeReader->Get("LOWE", myLowe);
	
	m_data->vars.Get("newMuon", muonFlag);
	m_data->vars.Get("newRelic", relicFlag);
	
	
	/*
	Event is a muon candidate! Now need to:
	1)  Check if there have not been any relic candidates yet (i.e. the first event is a muon candidate) then 
	    save the muon candidate's event # and time to the deque muonCands
	2)  If there are already relic candidates written to the relicCands vector:
	2a) Check the time difference between the muon and the oldest relic candidate - if the time difference
	    is larger than 30s and if the relic already has muons tagged to it then set the relic to be written
	    out as we have found all of the muons within +-30s of it.then set the relic, and its set of muons as
	    they have all been tagged to a relic candidate, to be written out.
	
	ParticleCand is a struct, defined in ParticleCand.h, with structure:
		struct ParticleCand {
			int EventNumber;
			int EventTime;
			std::vector<int> matchedParticleEvNum;
		};
	*/
	
	
	if(muonFlag){
		RelicMuonMatch("newMuon", m_data->muonCandDeque, m_data->relicCandDeque);
	}else if(relicFlag){
		RelicMuonMatch("newRelic", m_data->relicCandDeque, m_data->muonCandDeque);
	}else{
		Log("Event is not a relic candidate or a muon candidate! Something has gone wrong further up in the chain"
		, v_error, verbosity);
		return false;
	}
	
	return true;
}


bool RelicMuonMatching::Finalise(){
	
	return true;
}

unsigned long long int RelicMuonMatching::bitshiftTime(unsigned long long int t0Time, unsigned long long int hardwareTime){
	
	unsigned long long int shiftedt0Time, shiftedhardwareTime, oneint;
	
	// equivalent to 00000000000000001111111111111111 in binary
	oneint = 65535;
	
	shiftedt0Time = t0Time >> 16;
	shiftedt0Time = shiftedt0Time << 16;
	
	shiftedt0Time = shiftedt0Time | (t0Time & oneint);
	
	shiftedhardwareTime = hardwareTime >> 17;
	shiftedhardwareTime = shiftedhardwareTime << 32;
	
	shiftedt0Time = shiftedt0Time + shiftedhardwareTime;
	
	t0Time = shiftedt0Time;
	
	return t0Time;
}

/*float RelicMuonMatching::rollOver(unsigned long long int currentTime, unsigned long long int targetTime){
	unsigned long long int bitOne = 1;
	unsigned long long int tDiff;
	tDiff = currentTime - targetTime;
	tDiff = (bitOne << 47) + tDiff;
	
}*/

bool RelicMuonMatching::RelicMuonMatch(std::string particleType, std::deque<ParticleCand>& currentDeque, std::deque<ParticleCand>& targetDeque){
	m_data->vars.Set(particleType, false);
	unsigned long long int currentTime = bitshiftTime(skheadqb_.it0xsk, skheadqb_.nevhwsk);
	ParticleCand currentParticle;
	currentParticle.EventNumber = myHeader->nevsk;
	currentParticle.EventTime = currentTime;
	currentParticle.EntryNumber = myTreeReader->GetEntryNumber();
	currentParticle.ReconEnergy = myLowe->bsenergy;
	currentDeque.push_back(currentParticle);
	if(! targetDeque.size()){
		return true;
	}
	for(int i = 0; i < targetDeque.size(); i++){
		//find time to each target candidate (if the event is a muon cand then the target is a relic cand) oldest 
		//to newest
		if(currentTime > targetDeque[i].EventTime){
			timeDiff = (currentTime - targetDeque[i].EventTime)/COUNT_PER_NSEC;
		}else{
			timeDiff = ((bitOne << 47) + currentTime - targetDeque[i].EventTime)/COUNT_PER_NSEC;
		}
		if(abs(timeDiff) < 30.){
			//add the event # of the current event to the target particle's "matched particle" list and add the
			//if time difference is less than +-30s then the events need investigating to see if they are correlated
			//event # of the target particle to the current particle's "matched particle" list
			currentParticle.matchedParticleEvNum.push_back(targetDeque[i].EventNumber);
			currentParticle.matchedParticleEne.push_back(targetDeque[i].ReconEnergy);
			targetDeque[i].matchedParticleEvNum.push_back(currentParticle.EventNumber);
			targetDeque[i].matchedParticleEne.push_back(currentParticle.ReconEnergy);
			if(particleType == "newMuon"){
				m_data->matchedMuonEntryNums.push_back(currentParticle.EntryNumber);
			}
			if(particleType == "newRelic" && targetDeque[i].matchedParticleEvNum.size() == 1){
				m_data->matchedMuonEntryNums.push_back(targetDeque[i].EntryNumber);
			}
		}else if(timeDiff > 30.){
			if(targetDeque[i].matchedParticleEvNum.size()){
				//if the current event comes more than 30 seconds after any of the other events then no other
				//events can be tagged to it. So check that the older event has been tagged to other events and
				//set it to be written out.
				m_data->writeOut.push_back(targetDeque[i].EntryNumber);
			}
			//if it has been tagged to no events then it does not need to be written out and can be removed from
			//the deque as no more muons/relics will be tagged to it
			targetDeque.pop_front();
		}
	}
	return true;
}
