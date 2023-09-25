#ifndef SHOWERPARTICLE_H
#define SHOWERPARTICLE_H

struct ShowerParticle {
	//ID of muon that produced the shower particle
	int muonID;
	//Particle FLUKA ID
	int PID;
	//Initial position x, y, z (cm)
	float Pos[3];
	//Initial momentum Px, Py, Pz (GeV/c)
	float Mom[3];
};

#endif