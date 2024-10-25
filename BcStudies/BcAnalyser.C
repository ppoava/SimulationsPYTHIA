// BcAnalyser.C

// C++ libraries
#include <iostream>
#include <vector>
// ROOT libraries
#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TChain.h"
#include "TString.h"

#define PI 3.14159265
using namespace std;
using namespace TMath;

Double_t DeltaPhi(Double_t phi1, Double_t phi2) {
        // Returns delta phi in range (-pi/2 3pi/2)
        // Specific range is used to make the near- and away-side peak more visible
	return fmod(phi1-phi2+2.5*PI,2*PI)-0.5*PI;
	}

void status_file() {
	// This functions takes the trigger and associate id and creates a ROOT output file with the same filename
    // containing the histograms produced in this macro
	
	// Define the TChain
	TChain *ch1 = new TChain("tree");
	TFile *output = new TFile("analysed_output.root","RECREATE");
	
	// OPTION 1: SINGLE FILE
	ch1->Add("output_1e6.root");
	
	
	// OPTION 2: BATCH FILE STRUCTURE
	// Number of trees to be added to the TChain
	// This can be changed by the user
    /*
	int ntrees = 25; 
	
	for( int i = 1; i < ntrees+1;  i++){
	// One can change the file path accordingly
	  ch1->Add(Form("Group%i/output.root",i)); // Group%i/output.root
	}
    */
	// COMMENT TILL THIS LINE TO DISABLE OPTION 2

	// Now we define vectors that carry the information at event level
	vector<Int_t>* vID = 0;
	vector<Double_t>* vPt = 0;
	vector<Double_t>* vPhi = 0;
	vector<Double_t>* vStatus = 0;
	vector<Double_t>* vEta = 0;
	vector<Double_t>* vY = 0;
	vector<Double_t>* vMother1 = 0;
	vector<Double_t>* vMotherID = 0;
	Int_t MULTIPLICITY = 0;

	// Setting up chain branch addresses to the vectors defined above
	ch1->SetBranchAddress("ID",&vID);
	ch1->SetBranchAddress("PT",&vPt);
	ch1->SetBranchAddress("PHI",&vPhi);
	ch1->SetBranchAddress("ETA",&vEta);
	ch1->SetBranchAddress("Y",&vY);
	ch1->SetBranchAddress("STATUS",&vStatus);
	ch1->SetBranchAddress("MOTHER",&vMother1);
	ch1->SetBranchAddress("MOTHERID",&vMotherID);
	ch1->SetBranchAddress("MULTIPLICITY",&MULTIPLICITY);

	// Variables used in this script
	// s indicates signal particle variables
	// b indicates background particle variables
	Int_t id;
	Double_t pT,phi,status,eta,y,mother1,grandMother1,motherID,grandMotherID;
	int nParticles = 0;
	
	// There is a possiblity to change status settings for production studies
	// This is only used to double-check whether these histograms are consistent with their status
	// They can be removed from this script or the title of the plots can be changed in drawing scripts
	// This is not used for beauty angular correlations, but the settings can be changed easily to allow for a different production study
	// Double_t primary_status = 83;
	// Double_t secondary_status = 91;
	
	// Indicate the total number of events analysed
	int nEvents = ch1->GetEntries();
	cout << "The number of events for this analysis is: " << nEvents << endl;
	
	// Definitions of produced histograms
	// The histograms are divided into two categories: angular correlations and trigger spectra, the latter can be integrated over in local scripts to obtain the total number of triggers, which the yield is then normalised over.
	// The naming convention is as follows: 
	// h = histogram
	// TrPt = trigger spectra
	// DPhi = Delta phi (angular correlations)
	// Pr = primary status, Sc = secondary status (production mechanisms)

	// Furthermore, the trigger-associate pairs can be divided according to their pT, indicated as follows:
	// Low (L): 1 < pT < 3 GeV
	// Intermediate (I): 3 < pT < 8 GeV
	// High (H): pT > 8 GeV
	// Hence there are 6 total combinations for trigger-associate pairs (LL), (IL), (HL), (II), (HI) and (HH).

	// The histograms are defined compactly for clarity
	
	TH1D* hMULT = new TH1D("hMULT","Multiplicity per event;multiplicity;Counts",100,0,400);
    TH1D* hPt = new TH1D("hpT","Generic pT spectrum;pT;Counts",100,0,400);
    TH1D* hEta = new TH1D("hEta","Generic Eta spectrum;eta;Counts",100,0,400);
    TH1D* hY = new TH1D("hY","Generic rapidity spectrum;y;Counts",100,0,400);

    // Neutrinos
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

	// 2-dimensional histograms
	// TH2D* hTrPtEta = new TH2D("hPtEta",Form("pT and pseudorapidity trigger pT for trigger %s;p_{T};#eta;Counts",title),100,0,50,100,-4,4); 
	// TH2D* hTrPtY = new TH2D("hPtY",Form("pT and rapidity trigger pT for trigger %s;p_{T};y;Counts",title),100,0,50,100,-4,4);

	// Event loop
	// Retrieve variables for trigger particle from event, naming is straight-forward
	for(int iEvent = 0; iEvent < nEvents; iEvent++) {
		ch1->GetEntry(iEvent);
		int nparticles = vID->size();
		hMULT->Fill(MULTIPLICITY);

		for(int ipart = 0; ipart < nparticles; ipart++) {
			id = (*vID)[ipart];
			phi = (*vPhi)[ipart];
			pT = (*vPt)[ipart];
			status = (*vStatus)[ipart];
			eta =(*vEta)[ipart];
			y = (*vY)[ipart]; // not to be confused with the y-component of the momentum, denoted py!
			motherID = (*vMotherID)[ipart];
				
			if(pT >= 1.) {
				nParticles++;
				hEta->Fill(eta);
				hY->Fill(y);
				hPt->Fill(pT);

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

                // Background particles
			    if(id == 14) { // found muon neutrino (B_cˆ{+} background)
				    hPtNeutrinoB->Fill(pT);
				    hEtaNeutrinoB->Fill(eta);
				    hPhiNeutrinoB->Fill(phi);
			    }

			    if(id == -14) { // found anti-muon neutrino (B_cˆ{-} background)
				    hPtNeutrinoBarB->Fill(pT);
				    hEtaNeutrinoBarB->Fill(eta);
				    hPhiNeutrinoBarB->Fill(phi);
			    }

			} // pT > 1 GeV cut
		} // Particle loop
	} // Eventloop
	
	output->Write();
	output->Close();
	cout<<"The total number of triggers is: "<<nParticles<<endl;
	
	cout<<"File: "<<filename<<" has been created!"<<endl;
}


int BcAnalyser() {

    status_file();

	return 0;
}