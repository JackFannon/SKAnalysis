#ifndef PARTICLECAND_H
#define PARTICLECAND_H

struct ParticleCand {
	int EventNumber;
	int EventTime;
	int EntryNumber;
	float ReconEnergy;
	std::vector<int> matchedParticleEvNum;
	std::vector<float> matchedParticleEne;
};

#endif
