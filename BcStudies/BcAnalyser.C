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
    vector<Double_t>* vGrandMother1 = 0;
	vector<Double_t>* vMotherID = 0;
    vector<Double_t>* vGrandMotherID = 0;
	Int_t MULTIPLICITY = 0;

	// Setting up chain branch addresses to the vectors defined above
	ch1->SetBranchAddress("ID",&vID);
	ch1->SetBranchAddress("PT",&vPt);
	ch1->SetBranchAddress("PHI",&vPhi);
	ch1->SetBranchAddress("ETA",&vEta);
	ch1->SetBranchAddress("Y",&vY);
	ch1->SetBranchAddress("STATUS",&vStatus);
	ch1->SetBranchAddress("MOTHER",&vMother1);
    ch1->SetBranchAddress("GRANDMOTHER",&vGrandMother1);
	ch1->SetBranchAddress("MOTHERID",&vMotherID);
    ch1->SetBranchAddress("GRANDMOTHERID",&vGrandMotherID);
	ch1->SetBranchAddress("MULTIPLICITY",&MULTIPLICITY);

	// Variables used in this analyser
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
    TH1D* hPt = new TH1D("hpT","Generic pT spectrum;pT;Counts",50,0,10);
    TH1D* hEta = new TH1D("hEta","Generic Eta spectrum;eta;Counts",50,2.5,4);
    // TH1D* hY = new TH1D("hY","Generic rapidity spectrum;y;Counts",50,2.5,4);
    TH1D* hPhi = new TH1D("hPhi","Generic phi spectrum;phi;Counts",50,-PI,PI);

    // Neutrinos
    // --- Split in particle - antiparticle and signal - background
	TH1D* hPtNeutrinoSig = new TH1D("hPtNeutrinoSig","pT spectrum neutrino signal",50,0,10); // muon neutrino
	TH1D* hPtNeutrinoBarSig = new TH1D("hPtNeutrinoBarSig","pT spectrum anti-neutrino signal",50,0,10); // muon anti-neutrino
	TH1D* hEtaNeutrinoSig = new TH1D("hEtaNeutrinoSig","eta spectrum neutrino signal",50,2.5,4); 
	TH1D* hEtaNeutrinoBarSig = new TH1D("hEtaNeutrinoBarSig","eta spectrum anti-neutrino signal",50,2.5,4);
	TH1D* hPhiNeutrinoSig = new TH1D("hPhiNeutrinoSig","phi spectrum neutrino signal",50,-PI,PI); 
	TH1D* hPhiNeutrinoBarSig = new TH1D("hPhiNeutrinoBarSig","phi spectrum anti-neutrino signal",50,-PI,PI);  

	TH1D* hPtNeutrinoBkg = new TH1D("hPtNeutrinoBkg","pT spectrum neutrino background",50,0,10); 
	TH1D* hPtNeutrinoBarBkg = new TH1D("hPtNeutrinoBarBkg","pT spectrum anti-neutrino background",50,0,10); 
	TH1D* hEtaNeutrinoBkg = new TH1D("hEtaNeutrinoBkg","eta spectrum neutrino background",50,2.5,4); 
	TH1D* hEtaNeutrinoBarBkg = new TH1D("hEtaNeutrinoBarBkg","eta spectrum anti-neutrino background",50,2.5,4); 
	TH1D* hPhiNeutrinoBkg = new TH1D("hPhiNeutrinoBkg","phi spectrum neutrino background",50,-PI,PI); 
	TH1D* hPhiNeutrinoBarBkg = new TH1D("hPhiNeutrinoBarBkg","phi spectrum anti-neutrino background",50,PI,-PI); 

    // Standalone muon (solo)
    TH1D* hPtSoloMuonSig = new TH1D("hPtSoloMuonSig","pT spectrum standalone muon signal",50,0,10);
    TH1D* hPtSoloMuonBarSig = new TH1D("hPtSoloMuonBarSig","pT spectrum stanalone anti-muon signal",50,0,10);
    TH1D* hEtaSoloMuonSig = new TH1D("hEtaSoloMuonSig","eta spectrum standalone muon signal",50,2.5,4); 
	TH1D* hEtaSoloMuonBarSig = new TH1D("hEtaSoloMuonBarSig","eta spectrum standalone anti-muon signal",50,2.5,4);
	TH1D* hPhiSoloMuonSig = new TH1D("hPhiSoloMuonSig","phi spectrum standalone muon signal",50,-PI,PI); 
	TH1D* hPhiSoloMuonBarSig = new TH1D("hPhiSoloMuonBarSig","phi spectrum standalone anti-muon signal",50,-PI,PI); 

    TH1D* hPtSoloMuonBkg = new TH1D("hPtSoloMuonBkg","pT spectrum standalone muon background",50,0,10);
    TH1D* hPtSoloMuonBarBkg = new TH1D("hPtSoloMuonBarBkg","pT spectrum stanalone anti-muon background",50,0,10);
    TH1D* hEtaSoloMuonBkg = new TH1D("hEtaSoloMuonBkg","eta spectrum standalone muon background",50,2.5,4); 
	TH1D* hEtaSoloMuonBarBkg = new TH1D("hEtaSoloMuonBarBkg","eta spectrum standalone anti-muon background",50,2.5,4);
	TH1D* hPhiSoloMuonBkg = new TH1D("hPhiSoloMuonBkg","phi spectrum standalone muon background",50,-PI,PI); 
	TH1D* hPhiSoloMuonBarBkg = new TH1D("hPhiSoloMuonBarBkg","phi spectrum standalone anti-muon background",50,-PI,PI);

    // Dimuons from J/psi
    // TODO: change names to ... and ...bar like above
    // TODO: split into histograms with J/psi from Bc+ and from Bc- (needs statistics...)
    TH1D* hPtDiMuon1Sig = new TH1D("hPtDiMuon1Sig","pT spectrum dimuon1 signal",50,0,10);
    TH1D* hPtDiMuon2Sig = new TH1D("hPtDiMuon2Sig","pT spectrum dimuon2 signal",50,0,10);
    TH1D* hEtaDiMuon1Sig = new TH1D("hEtaDiMuon1Sig","eta spectrum dimuon1 signal",50,2.5,4); 
	TH1D* hEtaDiMuon2Sig = new TH1D("hEtaDiMuon2Sig","eta spectrum dimuon2 signal",50,2.5,4);
	TH1D* hPhiDiMuon1Sig = new TH1D("hPhiDiMuon1Sig","phi spectrum dimuon1 signal",50,-PI,PI); 
	TH1D* hPhiDiMuon2Sig = new TH1D("hPhiDiMuon2Sig","phi spectrum dimuon2 signal",50,-PI,PI); 

    TH1D* hPtDiMuon1Bkg = new TH1D("hPtDiMuon1Bkg","pT spectrum dimuon1 background",50,0,10);
    TH1D* hPtDiMuon2Bkg = new TH1D("hPtDiMuon2Bkg","pT spectrum dimuon2 background",50,0,10);
    TH1D* hEtaDiMuon1Bkg = new TH1D("hEtaDiMuon1Bkg","eta spectrum dimuon1 background",50,2.5,4); 
	TH1D* hEtaDiMuon2Bkg = new TH1D("hEtaDiMuon2Bkg","eta spectrum dimuon2 background",50,2.5,4);
	TH1D* hPhiDiMuon1Bkg = new TH1D("hPhiDiMuon1Bkg","phi spectrum dimuon1 background",50,-PI,PI); 
	TH1D* hPhiDiMuon2Bkg = new TH1D("hPhiDiMuon2Bkg","phi spectrum dimuon2 background",50,-PI,PI);


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
            mother1 = (*vMother1) [ipart];
            grandMother1 = (*vGrandMother1) [ipart];
			motherID = (*vMotherID)[ipart];
            grandMotherID = (*vGrandMotherID) [ipart];
				
			if(pT >= 1.) {
				nParticles++;
				hEta->Fill(eta);
				// hY->Fill(y);
				hPt->Fill(pT);
                hPhi->Fill(phi);

                // Signal particles
			    if(motherID == 541 && id == 14) { // found muon neutrino (B_cˆ{+} signal)
                    std::cout<<"This is my neutrino mom (from Bc+): "<<motherID<<std::endl;
                    std::cout<<"iEvent = "<<iEvent<<std::endl;
				    hPtNeutrinoSig->Fill(pT);
				    hEtaNeutrinoSig->Fill(eta);
				    hPhiNeutrinoSig->Fill(phi);
			    }

			    if(motherID == -541 && id == -14) { // found muon neutrino (B_cˆ{-} signal)
                    std::cout<<"This is my anti-neutrino mom (from Bc-): "<<motherID<<std::endl;
                    std::cout<<"iEvent = "<<iEvent<<std::endl;
				    hPtNeutrinoBarSig->Fill(pT);
				    hEtaNeutrinoBarSig->Fill(eta);
				    hPhiNeutrinoBarSig->Fill(phi);
			    }

                // TODO : make sure standalone muon and muon neutrino have same mother!
                if(motherID == 541 && id == -13) { // found standalone anti-muon (from B_cˆ{+})
                    std::cout<<"This is my muon+ mom (from Bc+): "<<motherID<<std::endl;
                    std::cout<<"iEvent = "<<iEvent<<std::endl;
                    hPtSoloMuonSig->Fill(pT);
				    hEtaSoloMuonSig->Fill(eta);
				    hPhiSoloMuonSig->Fill(phi);
                }

                if(motherID == -541 && id == 13) { // found standalone muon (from B_cˆ{-})
                    std::cout<<"This is my muon- mom (from Bc-): "<<motherID<<std::endl;
                    std::cout<<"iEvent = "<<iEvent<<std::endl;
                    hPtSoloMuonBarSig->Fill(pT);
				    hEtaSoloMuonBarSig->Fill(eta);
				    hPhiSoloMuonBarSig->Fill(phi);
                }

                // TODO: J/psi decay to dimuons: confirm they have the same grandmother
                // TODO: also think about if they have to come from B+ or B- (by splitting in more histograms,
                // one from Bc+ decay and one from Bc- decay...)
                if(grandMotherID == 541 || grandMotherID == -541 && motherID == 443 && id == 13) { // found anti-muon in di-muon pair (from J/psi)
                    hPtDiMuon1Sig->Fill(pT);
				    hEtaDiMuon1Sig->Fill(eta);
				    hPhiDiMuon1Sig->Fill(phi);
                }

                if(grandMotherID == 541 || grandMotherID == -541 && motherID == 443 && id == 13) { // found anti-muon in di-muon pair (from J/psi)
                    hPtDiMuon2Sig->Fill(pT);
				    hEtaDiMuon2Sig->Fill(eta);
				    hPhiDiMuon2Sig->Fill(phi);
                }

                // Background particles
			    if(id == 14) { // found muon neutrino (B_cˆ{+} background)
				    hPtNeutrinoBkg->Fill(pT);
				    hEtaNeutrinoBkg->Fill(eta);
				    hPhiNeutrinoBkg->Fill(phi);
			    }

			    if(id == -14) { // found anti-muon neutrino (B_cˆ{-} background)
				    hPtNeutrinoBarBkg->Fill(pT);
				    hEtaNeutrinoBarBkg->Fill(eta);
				    hPhiNeutrinoBarBkg->Fill(phi);
			    }

                if(motherID != 443 && id == -13) { // found standalone anti-muon (from B_cˆ{+} background)
                    hPtSoloMuonBkg->Fill(pT);
				    hEtaSoloMuonBkg->Fill(eta);
				    hPhiSoloMuonBkg->Fill(phi);
                }

                if(motherID != 443 && id == 13) { // found standalone muon (from B_cˆ{-} background)
                    hPtSoloMuonBarBkg->Fill(pT);
				    hEtaSoloMuonBarBkg->Fill(eta);
				    hPhiSoloMuonBarBkg->Fill(phi);
                }

			} // pT > 1 GeV cut
		} // Particle loop
	} // Eventloop
	
	output->Write();
	output->Close();
	cout<<"The total number of Particles is: "<<nParticles<<endl;

    cout<<"The lepton flavour violation is violated in PYTHIA by about +/- 400 particles. Don't expect the same number of muons and neutrinos to be produced from Bc decay"<<endl;

    std::cout<<std::endl;
    std::cout<<"Analysis terminated. Succesful?"<<std::endl;
}


int BcAnalyser() {

    status_file();

	return 0;
}