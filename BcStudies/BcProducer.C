// bbbarcorrelations_status.cpp
// Script that used PYTHIA to simulate collision events according to the settings given in the pythiasettings_Hard_Low_bb.cmnd file.

// The variables saved are necessary to create azimuthal angular correlation plots for beauty hadrons, which are defined as follows:
// pT = transverse momentum
// eta = pseudorapidity
// phi = azimuthal angle
// ID = PDG ID of particle
// mother = index of mother particle 
// motherID = PDG ID of mother particle

// Other variables are saved to be used to create a few histograms that provide quick checks on the quality of the simulated output

// C++ libraries
#include <iostream>
#include <cmath>
#include <cstring>
#include <chrono>
#include <vector>
// PYTHIA library
#include "Pythia8/Pythia.h"
// ROOT libraries
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TTree.h"

#define PI 3.14159265
using namespace std;
using namespace Pythia8;

int BcProducer() {
	// For larger simulations it is essential to know the duration of the execution of this script
	// Start time here
	auto start = chrono::high_resolution_clock::now();
	
	// Create output file
	TFile* output = new TFile("output.root","RECREATE");
	/*
	if(!output->IsOpen()){
		cout<<"Error: File "<<"output.root"<<"already exists terminating program!"<<endl;
		return 1;
	}
	*/

	// Define TTree that will contain event information
	TTree *tree = new TTree("tree","bbbar correlations");
	
	// Define variables that will be generated
	Double_t pT,m,eta,y,phi,status,mother,motherID,grandMother,grandMotherID;
	Int_t  id,idBeauty,MULTIPLICITY,nEvents,motherIndex,grandMotherIndex;

	// Define vectors that contain event-level information on the particles
	vector<Int_t> vID;
	vector<Double_t> vPt;
	vector<Double_t> vM;
	vector<Double_t> vEta; // pseudorapidity
	vector<Double_t> vY; // rapidity
	vector<Double_t> vPhi;
	vector<Double_t> vStatus;
	// vector<Double_t> vMotherIndex;
	// vector<Double_t> vGrandMotherIndex;
	vector<Double_t> vMother1;
	vector<Double_t> vGrandMother1;
	vector<Double_t> vMotherID;
	vector<Double_t> vGrandMotherID;
	
	// Setting up tree branches
	// The histograms are used to provide quick checks on the output
	tree->Branch("ID",&vID);
	tree->Branch("PT",&vPt);
	tree->Branch("M",&vM);
	tree->Branch("ETA",&vEta);
	tree->Branch("Y",&vY);
	tree->Branch("PHI",&vPhi);
	tree->Branch("STATUS",&vStatus);
	// tree->Branch("MOTHERINDEX",&vMotherIndex);
	// tree->Branch("GRANDMOTHERINDEX",&vGrandMotherIndex);
	tree->Branch("MOTHER",&vMother1);
	tree->Branch("GRANDMOTHER",&vGrandMother1);
	tree->Branch("MOTHERID",&vMotherID);
	tree->Branch("GRANDMOTHERID",&vGrandMotherID);
	tree->Branch("MULTIPLICITY",&MULTIPLICITY,"x/I");
	TH1D* hMULTIPLICITY = new TH1D("hMULTIPLICITY","Multiplicity",301,-0.5,300.5);
	TH1D* hBeautyPart = new TH1D("hBeautyPart", "Beauty Particles Per Event",200,-0.5,200.5);
	// TH1D* hidBeauty = new TH1D("hidBeauty","PDG Codes for Beauty hadrons",12000,-6000,6000);

	// Bc meson
	/*
	TH1D* hPtBcP = new TH1D("hPtBcP","pT spectrum B^{+}_{c}",50,0,10);
	TH1D* hPtBcM = new TH1D("hPtBcM","pT spectrum B^{-}_{c}",50,0,10);
	*/

	// Neutrinos
	// For now included to test the production output
	TH1D* hPtNeutrinoS = new TH1D("hPtNeutrinoS","pT spectrum neutrino signal",50,0,10); // muon neutrino
	TH1D* hPtNeutrinoBarS = new TH1D("hPtNeutrinoBarS","pT spectrum anti-neutrino signal",50,0,10); // muon anti-neutrino

	TH1D* hEtaNeutrinoS = new TH1D("hEtaNeutrinoS","eta spectrum neutrino signal",50,2.5,4); 
	TH1D* hEtaNeutrinoBarS = new TH1D("hEtaNeutrinoBarS","eta spectrum anti-neutrino signal",50,2.5,4);

	TH1D* hPhiNeutrinoS = new TH1D("hPhiNeutrinoS","phi spectrum neutrino signal",50,-PI,PI); 
	TH1D* hPhiNeutrinoBarS = new TH1D("hPhiNeutrinoBarS","phi spectrum anti-neutrino signal",50,-PI,PI);  

	TH1D* hPtNeutrinoB = new TH1D("hPtNeutrinoB","pT spectrum neutrino background",50,0,10); 
	TH1D* hPtNeutrinoBarB = new TH1D("hPtNeutrinoBarB","pT spectrum anti-neutrino background",50,0,10); 

	TH1D* hEtaNeutrinoB = new TH1D("hEtaNeutrinoB","eta spectrum neutrino background",50,2.5,4); 
	TH1D* hEtaNeutrinoBarB = new TH1D("hEtaNeutrinoBarB","eta spectrum anti-neutrino background",50,2.5,4); 

	TH1D* hPhiNeutrinoB = new TH1D("hPhiNeutrinoB","phi spectrum neutrino background",50,-PI,PI); 
	TH1D* hPhiNeutrinoBarB = new TH1D("hPhiNeutrinoBarB","phi spectrum anti-neutrino background",50,PI,-PI);  

	// J/psi
	/*
	TH1D* hPtJpsiS = new TH1D("hPtJpsiS","pT spectrum J/psi signal",50,0,10);
	TH1D* hEtaJpsiS = new TH1D("hEtaJpsiS","eta spectrum J/psi signal",50,2.5,4);
	TH1D* hPhiJpsiS = new TH1D("hPhiJpsiS","phi spectrum J/psi signal",50,-PI,PI);

	TH1D* hPtJpsiB = new TH1D("hPtJpsiB","pT spectrum J/psi background",50,0,10);
	TH1D* hEtaJpsiB = new TH1D("hEtaJpsiB","eta spectrum J/psi background",50,2.5,4);
	TH1D* hPhiJpsiB = new TH1D("hPhiJpsiB","phi spectrum J/psi background",50,-PI,PI);
	*/

	// Kinematics constraints
	const Double_t pTmin = 0.15; // minimum pT
	const Double_t etaMin = 2.5; // muon acceptance in ALICE = (2.5,4)
	const Double_t etaMax = 4;
	
	// Get PYTHIA
	Pythia8::Pythia pythia;
	
	// Simulation settings from pythiasettings_Hard_Low_bb.cmnd
	// The settings used are documented in that file
	pythia.readFile("pythiasettings_Hard_Low_bb.cmnd");
	nEvents = pythia.mode("Main:numberOfEvents");
	
	// Create a random seed so that the outcome will be truly random
	Int_t proccessid = getpid();
	string seedstr = "Random:seed = "+std::to_string((time(0)+proccessid)%900000000);
	pythia.readString("Random:setSeed = on");
	pythia.readString(seedstr);
	
	// Initializing simulation
	pythia.init();
	cout<<"Generating "<<nEvents<<" events!"<<endl;
	
	// Event loop
	for(int iEvent = 0; iEvent<nEvents; iEvent++) {
	        if(!pythia.next()) continue;
		int nPart = pythia.event.size(); // Number of particles produced in this event
		MULTIPLICITY = 0; // Initialiazing for multiplicity plot
		motherIndex = -1;
		grandMotherIndex = -1;
		// Initializing vectors for event-level information
		vID.clear();
		vPt.clear();
		vM.clear();
		vEta.clear();
		vY.clear();
		vPhi.clear();
		vStatus.clear();
		// vMotherIndex.clear();
		// vGrandMotherIndex.clear();
		vMother1.clear();
		vGrandMother1.clear();
		vMotherID.clear();
		vGrandMotherID.clear();

		// Particle loop
		for(int iPart = 0; iPart<nPart; iPart++) {
			const Particle &particle = pythia.event[iPart];
			if(!particle.isFinal()) continue; // Skip if the particle is not in its final state
			id = particle.id();
			if(id != 13 && id != -13 &&	id != 14 && id != -14) continue; // Only keep muons and muon neutrinos
			
			pT = particle.pT();
			m = particle.m();
			eta = particle.eta();
			y = particle.y();
			phi = particle.phi();
			status = static_cast<Double_t> (particle.status());
			motherIndex = particle.mother1();
			grandMotherIndex = pythia.event[motherIndex].mother1();
			mother = static_cast<Double_t> (motherIndex);
			grandMother = static_cast<Double_t> (grandMotherIndex);
			motherID = static_cast<Double_t> (pythia.event[motherIndex].id());
			grandMotherID = static_cast<Double_t>(pythia.event[grandMotherIndex].id());

			// Kinematics check
			if(pT < pTmin || eta < etaMin || eta > etaMax) continue;
			
			// Multiplicity is defined as the number of charged primary particles in the final state of the event
			// Particles to be considered primaries here are: electron, muon, pion, kaon, proton
			if(abs(id) == 11 || abs(id) == 13 || abs(id) == 211 || abs(id) == 321 || abs(id) == 2212) { // Needs to be a primary particle
			  if(81 <= status && status >= 89) { // Needs to be prompt
			    MULTIPLICITY++; 
			  }
			}

			// Filling vectors
			vID.push_back(id);
			vPt.push_back(pT);
			vM.push_back(m);
			vEta.push_back(eta);
			vY.push_back(y);
			vPhi.push_back(phi);
			vStatus.push_back(status);
			vMother1.push_back(mother);
			vGrandMother1.push_back(grandMother);
			vMotherID.push_back(motherID);
			vGrandMotherID.push_back(grandMotherID);

			/*
			if(id == 541) { // found a B_cˆ{+}
				hPtBcP->Fill(pT);
			}

			if(id == -541) { // found a B_cˆ{-}
				hPtBcM->Fill(pT);
			}
			*/

			// Signal particles
			if(motherID == 541 && id == 14) { // found muon neutrino (B_cˆ{+} signal)
				hPtNeutrinoS->Fill(pT);
				hEtaNeutrinoS->Fill(eta);
				hPhiNeutrinoS->Fill(phi);
			}

			if(motherID == -541 && id == -14) { // found muon neutrino (B_cˆ{-} signal)
				hPtNeutrinoBarS->Fill(pT);
				hEtaNeutrinoBarS->Fill(eta);
				hPhiNeutrinoBarS->Fill(phi);
			}

			/*
			if(motherID == 541 && id == 443) { // found J/psi (B_cˆ{+} signal)
				hPtJpsiS->Fill(pT);
				hEtaJpsiS->Fill(eta);
				hPhiJpsiS->Fill(phi);
			}
			*/
			
			// Background particles
			if(motherID != 541 && id == 14) { // found muon neutrino (B_cˆ{+} background)
				hPtNeutrinoB->Fill(pT);
				hEtaNeutrinoB->Fill(eta);
				hPhiNeutrinoB->Fill(phi);
			}

			if(motherID != -541 && id == -14) { // found anti-muon neutrino (B_cˆ{-} background)
				hPtNeutrinoBarB->Fill(pT);
				hEtaNeutrinoBarB->Fill(eta);
				hPhiNeutrinoBarB->Fill(phi);
			}

			/*
			if(id == 443) { // found J/psi (B_cˆ{+} signal)
				hPtJpsiB->Fill(pT);
				hEtaJpsiB->Fill(eta);
				hPhiJpsiB->Fill(phi);
			}
			*/

		} // 1st particle loop

		hMULTIPLICITY->Fill((Double_t) MULTIPLICITY);

		// In order not to fill tree with empty vectors.
		if(vID.empty() || vPt.empty() || vM.empty() || vEta.empty() || vY.empty() || vPhi.empty() || vStatus.empty() || vMother1.empty() || vMotherID.empty()) continue; 
		tree->Fill();
	} // End of event loop
	
	// Write output and close it
	output->Write();
	cout<<"File has been created and its name is: "<<output->GetName()<<endl;
	output->Close();
	
	// Return duration of the simulation
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::minutes>(end - start);
        cout << "This script took " << duration.count() << " minutes to run." << endl;

	return 0;
}




