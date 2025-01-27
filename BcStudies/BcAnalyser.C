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
#include "TLorentzVector.h"

#define PI 3.14159265
using namespace std;
using namespace TMath;

Double_t DeltaPhi(Double_t phi1, Double_t phi2) {
        // Returns delta phi in range (-pi/2 3pi/2)
        // Specific range is used to make the near- and away-side peak more visible
	return fmod(phi1-phi2+2.5*PI,2*PI)-0.5*PI;
	}

Double_t DeltaEta(Double_t eta1, Double_t eta2) {
        // For eta it is easier..
	return (eta1 - eta2);
	}	

Double_t InvariantMass(Int_t neutrinoIndex, Int_t soloMuonIndex, Int_t diMuon1Index, Int_t diMuon2Index,
						vector<Double_t>* vPt, vector<Double_t>* vEta, vector<Double_t>* vPhi,
						vector<Double_t>* vM) {
	TLorentzVector lorentzNeutrino,lorentzSoloMuon,lorentzDiMuon1,lorentzDiMuon2;

	lorentzNeutrino.SetPtEtaPhiM(
		(*vPt)[neutrinoIndex], 
		(*vEta)[neutrinoIndex], 
		(*vPhi)[neutrinoIndex], 
		(*vM)[neutrinoIndex]
	);
	lorentzSoloMuon.SetPtEtaPhiM(
		(*vPt)[soloMuonIndex], 
		(*vEta)[soloMuonIndex], 
		(*vPhi)[soloMuonIndex], 
		(*vM)[soloMuonIndex]
	);
	lorentzDiMuon1.SetPtEtaPhiM(
		(*vPt)[diMuon1Index], 
		(*vEta)[diMuon1Index], 
		(*vPhi)[diMuon1Index], 
		(*vM)[diMuon1Index]
	);
	lorentzDiMuon2.SetPtEtaPhiM(
		(*vPt)[diMuon2Index], 
		(*vEta)[diMuon2Index], 
		(*vPhi)[diMuon2Index], 
		(*vM)[diMuon2Index]
	);

	// Sum the four vectors to get the total four-momentum
	TLorentzVector total = lorentzNeutrino + lorentzSoloMuon +
							lorentzDiMuon1  + lorentzDiMuon2;

	// Calculate the invariant mass of the four-particle system
	return total.M();
}

Double_t InvariantMassWithoutNeutrino(Int_t soloMuonIndex, Int_t diMuon1Index, Int_t diMuon2Index,
									vector<Double_t>* vPt, vector<Double_t>* vEta, vector<Double_t>* vPhi,
									vector<Double_t>* vM) {
	TLorentzVector lorentzSoloMuon,lorentzDiMuon1,lorentzDiMuon2;

	lorentzSoloMuon.SetPtEtaPhiM(
		(*vPt)[soloMuonIndex], 
		(*vEta)[soloMuonIndex], 
		(*vPhi)[soloMuonIndex], 
		(*vM)[soloMuonIndex]
	);
	lorentzDiMuon1.SetPtEtaPhiM(
		(*vPt)[diMuon1Index], 
		(*vEta)[diMuon1Index], 
		(*vPhi)[diMuon1Index], 
		(*vM)[diMuon1Index]
	);
	lorentzDiMuon2.SetPtEtaPhiM(
		(*vPt)[diMuon2Index], 
		(*vEta)[diMuon2Index], 
		(*vPhi)[diMuon2Index], 
		(*vM)[diMuon2Index]
	);

	// Sum the four vectors to get the total four-momentum
	TLorentzVector total = lorentzSoloMuon + lorentzDiMuon1 + lorentzDiMuon2;

	// Calculate the invariant mass of the four-particle system
	return total.M();
}

void status_file() {
	// This functions takes the trigger and associate id and creates a ROOT output file with the same filename
    // containing the histograms produced in this macro
	
	// Define the TChain
	TChain *ch1 = new TChain("tree");
	TFile *output = new TFile("analysed_output.root","RECREATE");
	
	// OPTION 1: SINGLE FILE
    // INPUT
	ch1->Add("output.root");
	// ch1->Add("output.root");
	
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

    // Debugging
    Bool_t VERBOSE = false;

	// Now we define vectors that carry the information at event level
	vector<Int_t>* vID = 0;
	vector<Double_t>* vPt = 0;
    vector<Double_t>* vM = 0;
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
    ch1->SetBranchAddress("M",&vM);
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
	Int_t id,neutrinoIndex,neutrinoBarIndex,soloMuonIndex,soloMuonBarIndex,diMuon1BcpIndex,diMuon2BcpIndex,diMuon1BcmIndex,diMuon2BcmIndex;
	Int_t neutrinoBkgIndex,neutrinoBarBkgIndex,soloMuonBkgIndex,soloMuonBarBkgIndex,diMuon1BkgIndex,diMuon2BkgIndex;
	Double_t pT,m,phi,status,eta,y,mother1,grandMother1,motherID,grandMotherID,DPhiDiMuons,DEtaDiMuons,DPhiDiMuonsBkg,DEtaDiMuonsBkg;
	TLorentzVector LVdiMuon1,LVdiMuon2,LVjpsi;
	int nParticles = 0;
	
	// There is a possiblity to change status settings for production studies
	// This is only used to double-check whether these histograms are consistent with their status
	// They can be removed from this script or the title of the plots can be changed in drawing scripts
	// This is not used for beauty angular correlations, but the settings can be changed easily to allow for a different production study
	// Double_t primary_status = 83;
	// Double_t secondary_status = 91;
	
	// Indicate the total number of events analysed
	int nEvents = ch1->GetEntries();
	std::cout << "The number of events for this analysis is: " << nEvents << std::endl;

	TH1D* hMULT = new TH1D("hMULT","Multiplicity per event;multiplicity;Counts",100,0,400);
    TH1D* hPt = new TH1D("hpT","Generic pT spectrum;pT;Counts",50,0,10);
    TH1D* hEta = new TH1D("hEta","Generic Eta spectrum;eta;Counts",50,-4,4);
    // TH1D* hY = new TH1D("hY","Generic rapidity spectrum;y;Counts",50,2.5,4);
    TH1D* hPhi = new TH1D("hPhi","Generic phi spectrum;phi;Counts",50,-PI,PI);
	// TH1D* hMass = new TH1D("hMass","Generic mass spectrum;phi;Counts",50,0,0.1);
	TH1D* hInvMass = new TH1D("hInvMass","Invariant mass of Bc+;mass [GeV];Counts",50,0,8);
	TH1D* hInvMassWithoutNeutrino = new TH1D("hInvMassWithoutNeutrino","Invariant mass of Bc+ w/o neutrino;mass [GeV];Counts",50,0,8);
	TH1D* hDMass = new TH1D("hDMass","#Delta mass effect of excluding neutrino;mass [GeV];Counts",50,0,6);
	TH1D* hInvMassJpsi = new TH1D("hInvMassJpsi","Invariant mass of Jpsi;mass [GeV];Counts",50,0,8);

    // Neutrinos
    // --- Split in particle - antiparticle and signal - background
	TH1D* hPtNeutrinoSig = new TH1D("hPtNeutrinoSig","pT spectrum neutrino signal",50,0,10); // muon neutrino
	TH1D* hPtNeutrinoBarSig = new TH1D("hPtNeutrinoBarSig","pT spectrum anti-neutrino signal",50,0,10); // muon anti-neutrino
	TH1D* hEtaNeutrinoSig = new TH1D("hEtaNeutrinoSig","eta spectrum neutrino signal",50,-4,4); 
	TH1D* hEtaNeutrinoBarSig = new TH1D("hEtaNeutrinoBarSig","eta spectrum anti-neutrino signal",50,-4,4);
	TH1D* hPhiNeutrinoSig = new TH1D("hPhiNeutrinoSig","phi spectrum neutrino signal",50,-PI,PI); 
	TH1D* hPhiNeutrinoBarSig = new TH1D("hPhiNeutrinoBarSig","phi spectrum anti-neutrino signal",50,-PI,PI);  

	TH1D* hPtNeutrinoBkg = new TH1D("hPtNeutrinoBkg","pT spectrum neutrino background",50,0,10); 
	TH1D* hPtNeutrinoBarBkg = new TH1D("hPtNeutrinoBarBkg","pT spectrum anti-neutrino background",50,0,10); 
	TH1D* hEtaNeutrinoBkg = new TH1D("hEtaNeutrinoBkg","eta spectrum neutrino background",50,-4,4); 
	TH1D* hEtaNeutrinoBarBkg = new TH1D("hEtaNeutrinoBarBkg","eta spectrum anti-neutrino background",50,-4,4); 
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

	// Correlations
	TH1D* hDPhiNeutrinoBarSoloMuon = new TH1D("hDPhiNeutrinoBarSoloMuon","#Delta#phi for semi-leptonic Bc- daughters;#Delta#phi (rad);Counts",100,-PI/2,3*PI/2);
	TH1D* hDPhiNeutrinoSoloMuonBar = new TH1D("hDPhiNeutrinoSoloMuonBar","#Delta#phi for semi-leptonic Bc+ daughters;#Delta#phi (rad);Counts",100,-PI/2,3*PI/2);
	TH1D* hDEtaNeutrinoBarSoloMuon = new TH1D("hDEtaNeutrinoBarSoloMuon","#Delta#eta for semi-leptonic Bc- daughters;#Delta#eta;Counts",100,-4,4);
	TH1D* hDEtaNeutrinoSoloMuonBar = new TH1D("hDEtaNeutrinoSoloMuonBar","#Delta#eta for semi-leptonic Bc+ daughters;#Delta#eta;Counts",100,-4,4);

    // Dimuons from J/psi
    // TODO: change names to ... and ...bar like above
    // TODO: split into histograms with J/psi from Bc+ and from Bc- (needs statistics...)
    // 1 = muon- (-13)
    // 2 = muon+ (+13)
    TH1D* hPtDiMuon1BcpSig = new TH1D("hPtDiMuon1BcpSig","pT spectrum dimuon1 from Bc+ signal",50,0,10);
    TH1D* hPtDiMuon2BcpSig = new TH1D("hPtDiMuon2BcpSig","pT spectrum dimuon2 from Bc+ signal",50,0,10);
    TH1D* hEtaDiMuon1BcpSig = new TH1D("hEtaDiMuon1BcpSig","eta spectrum dimuon1 from Bc+ signal",50,2.5,4);
	TH1D* hEtaDiMuon2BcpSig = new TH1D("hEtaDiMuon2BcpSig","eta spectrum dimuon2 from Bc+ signal",50,2.5,4);
	TH1D* hPhiDiMuon1BcpSig = new TH1D("hPhiDiMuon1BcpSig","phi spectrum dimuon1 from Bc+ signal",50,-PI,PI);
	TH1D* hPhiDiMuon2BcpSig = new TH1D("hPhiDiMuon2BcpSig","phi spectrum dimuon2 from Bc+ signal",50,-PI,PI);

	TH1D* hPtDiMuon1BcmSig = new TH1D("hPtDiMuon1BcmSig","pT spectrum dimuon1 from Bc- signal",50,0,10);
    TH1D* hPtDiMuon2BcmSig = new TH1D("hPtDiMuon2BcmSig","pT spectrum dimuon2 from Bc- signal",50,0,10);
    TH1D* hEtaDiMuon1BcmSig = new TH1D("hEtaDiMuon1BcmSig","eta spectrum dimuon1 from Bc- signal",50,2.5,4);
	TH1D* hEtaDiMuon2BcmSig = new TH1D("hEtaDiMuon2BcmSig","eta spectrum dimuon2 from Bc- signal",50,2.5,4);
	TH1D* hPhiDiMuon1BcmSig = new TH1D("hPhiDiMuon1BcmSig","phi spectrum dimuon1 from Bc- signal",50,-PI,PI);
	TH1D* hPhiDiMuon2BcmSig = new TH1D("hPhiDiMuon2BcmSig","phi spectrum dimuon2 from Bc- signal",50,-PI,PI);

    TH1D* hPtDiMuon1Bkg = new TH1D("hPtDiMuon1Bkg","pT spectrum dimuon1 background",50,0,10);
    TH1D* hPtDiMuon2Bkg = new TH1D("hPtDiMuon2Bkg","pT spectrum dimuon2 background",50,0,10);
    TH1D* hEtaDiMuon1Bkg = new TH1D("hEtaDiMuon1Bkg","eta spectrum dimuon1 background",50,2.5,4);
	TH1D* hEtaDiMuon2Bkg = new TH1D("hEtaDiMuon2Bkg","eta spectrum dimuon2 background",50,2.5,4);
	TH1D* hPhiDiMuon1Bkg = new TH1D("hPhiDiMuon1Bkg","phi spectrum dimuon1 background",50,-PI,PI);
	TH1D* hPhiDiMuon2Bkg = new TH1D("hPhiDiMuon2Bkg","phi spectrum dimuon2 background",50,-PI,PI);

	TH1D* hDPhiDiMuons = new TH1D("hDPhiDiMuons","#Delta#phi for dimuon pair;#Delta#Phi (rad);Counts",100,-PI/2,3*PI/2);
	TH1D* hDEtaDiMuons = new TH1D("hDEtaDiMuons","#Delta#eta for dimuon pair;#Delta#eta;Counts",100,-2.5,4);
	TH1D* hDPhiSoloMuonDiMuons = new TH1D("hDPhiSoloMuonDiMuons","#Delta#phi for muon and di-muon pair from Bc-;#Delta#phi (rad);Counts",100,-PI/2,3*PI/2);
	TH1D* hDPhiSoloMuonBarDiMuons = new TH1D("hDPhiSoloMuonBarDiMuons","#Delta#phi for muon and di-muon pair from Bc+;#Delta#phi (rad);Counts",100,-PI/2,3*PI/2);
	TH1D* hDEtaSoloMuonDiMuons = new TH1D("hDEtaSoloMuonDiMuons","#Delta#eta for muon and di-muon pair from Bc-;#Delta#eta;Counts",100,-2,2);
	TH1D* hDEtaSoloMuonBarDiMuons = new TH1D("hDEtaSoloMuonBarDiMuons","#Delta#eta for muon and di-muon pair from Bc+;#Delta#eta;Counts",100,-2,2);
	//
	TH1D* hDPhiDiMuonsBkg = new TH1D("hDPhiDiMuonsBkg","#Delta#phi for dimuon background pair;#Delta#Phi (rad);Counts",100,-PI/2,3*PI/2);
	TH1D* hDEtaDiMuonsBkg = new TH1D("hDEtaDiMuonsBkg","#Delta#eta for dimuon background pair;#Delta#eta;Counts",100,-2.5,4);
	TH1D* hDPhiSoloMuonDiMuonsBkg = new TH1D("hDPhiSoloMuonDiMuonsBkg","#Delta#phi for muon and di-muon pair background;#Delta#phi (rad);Counts",100,-PI/2,3*PI/2);
	TH1D* hDPhiSoloMuonBarDiMuonsBkg = new TH1D("hDPhiSoloMuonBarDiMuonsBkg","#Delta#phi for anti-muon and di-muon pair background;#Delta#phi (rad);Counts",100,-PI/2,3*PI/2);
	TH1D* hDEtaSoloMuonDiMuonsBkg = new TH1D("hDEtaSoloMuonDiMuonsBkg","#Delta#eta for muon and di-muon pair background;#Delta#eta;Counts",100,-2,2);
	TH1D* hDEtaSoloMuonBarDiMuonsBkg = new TH1D("hDEtaSoloMuonBarDiMuonsBkg","#Delta#eta for anti-muon and di-muon pair background;#Delta#eta;Counts",100,-2,2);

	// Background mass study
	TH1D* hInvMassFakeSoloMuon = new TH1D("hInvMassFakeSoloMuon","Invariant mass of Bc+ with fake solo muon;mass [GeV];Counts",50,0,8);
	TH1D* hInvMassWithoutNeutrinoFakeSoloMuon = new TH1D("hInvMassWithoutNeutrinoFakeSoloMuon","Invariant mass of Bc+ with fake solo muon w/o neutrino;mass [GeV];Counts",50,0,8);
	TH1D* hDMassFakeSoloMuon = new TH1D("hDMassFakeSoloMuon","#Delta mass effect with fake solo muon and excluding neutrino;mass [GeV];Counts",50,0,6);

	TH1D* hInvMassFakeJpsi = new TH1D("hInvMassFakeJpsi","Invariant mass of Bc+ with fake Jpsi;mass [GeV];Counts",50,0,8);
	TH1D* hInvMassWithoutNeutrinoFakeJpsi = new TH1D("hInvMassWithoutNeutrinoFakeJpsi","Invariant mass of Bc+ with fake Jpsi w/o neutrino;mass [GeV];Counts",50,0,8);
	TH1D* hDMassFakeJpsi = new TH1D("hDMassFakeJpsi","#Delta mass effect with fake Jpsi and excluding neutrino;mass [GeV];Counts",50,0,6);

	// 2-dimensional histograms
	// TH2D* hTrPtEta = new TH2D("hPtEta",Form("pT and pseudorapidity trigger pT for trigger %s;p_{T};#eta;Counts",title),100,0,50,100,-4,4); 
	// TH2D* hTrPtY = new TH2D("hPtY",Form("pT and rapidity trigger pT for trigger %s;p_{T};y;Counts",title),100,0,50,100,-4,4);

	// Event loop
	// Retrieve variables for trigger particle from event, naming is straight-forward
	for(int iEvent = 0; iEvent < nEvents; iEvent++) {
		ch1->GetEntry(iEvent);
		int nparticles = vID->size();
		hMULT->Fill(MULTIPLICITY);
        
        // The indexing is used to check if candidates came from the same mother/vertex
        neutrinoIndex = -1;
        neutrinoBarIndex = -1;
        soloMuonIndex = -1;
        soloMuonBarIndex = -1;
        diMuon1BcpIndex = -1;
        diMuon2BcpIndex = -1;
		diMuon1BcmIndex = -1;
        diMuon2BcmIndex = -1;
		//
		neutrinoBkgIndex = -1;
        neutrinoBarBkgIndex = -1;
        soloMuonBkgIndex = -1;
        soloMuonBarBkgIndex = -1;
        diMuon1BkgIndex = -1;
        diMuon2BkgIndex = -1;

		for(int ipart = 0; ipart < nparticles; ipart++) {
            // TODO: this can be done easier later on, no?
            // TODO: add mass for missing neutrino mass plot
			id = (*vID)[ipart];
            m = (*vM)[ipart];
			phi = (*vPhi)[ipart];
			pT = (*vPt)[ipart];
			status = (*vStatus)[ipart];
			eta =(*vEta)[ipart];
			y = (*vY)[ipart]; // not to be confused with the y-component of the momentum, denoted py!
            mother1 = (*vMother1)[ipart];
            grandMother1 = (*vGrandMother1)[ipart];
			motherID = (*vMotherID)[ipart];
            grandMotherID = (*vGrandMotherID)[ipart];
				
			if(pT >= 0.15) { // same cut as in simulation
				nParticles++;
				hEta->Fill(eta);
				// hY->Fill(y);
				hPt->Fill(pT);
                hPhi->Fill(phi);

                // Signal particles
				// muon neutrino
			    if(motherID == 541 && id == 14) { // found muon neutrino (B_cˆ{+} signal)
                    if(VERBOSE) {
                    // std::cout<<"This is my neutrino mom (from Bc+): "<<motherID<<std::endl;
                    // std::cout<<"iEvent = "<<iEvent<<std::endl;
                    }
                    neutrinoIndex = ipart;
                }

				// anti-muon
                if(motherID == 541 && id == -13) { // found standalone anti-muon (from B_cˆ{+})
                    if(VERBOSE) {
                    // std::cout<<"This is my muon+ mom (from Bc+): "<<motherID<<std::endl;
                    // std::cout<<"iEvent = "<<iEvent<<std::endl;
                    }
                    soloMuonBarIndex = ipart;
                }

				// J/psi
				if(grandMotherID == 541 && motherID == 443 && id == -13) { // found muon in di-muon pair (from J/psi)
                    diMuon1BcpIndex = ipart;
                    // if(VERBOSE) { std::cout<<"1 = "<<diMuon1Index<<std::endl; }
                }

                if(grandMotherID == 541 && motherID == 443 && id == 13) { // found anti-muon in di-muon pair (from J/psi)
                    diMuon2BcpIndex = ipart;
                    // if(VERBOSE) { std::cout<<"2 = "<<diMuon2Index<<std::endl; }
                }

                // Candidate matching
                // TODO: match this also with J/psi!!! (needs statistics...)
                // Bcˆ{+} -> μ+ ν J/ψ -> μ+μ-
                if(neutrinoIndex != -1 && soloMuonBarIndex != -1 &&
                    (*vMother1)[neutrinoIndex] == (*vMother1)[soloMuonBarIndex]) {
					hDPhiNeutrinoSoloMuonBar->Fill(DeltaPhi((*vPhi)[neutrinoIndex],(*vPhi)[soloMuonBarIndex]));
					hDEtaNeutrinoSoloMuonBar->Fill(DeltaEta((*vEta)[neutrinoIndex],(*vEta)[soloMuonBarIndex]));

                	hPtNeutrinoSig->Fill((*vPt)[neutrinoIndex]);
				    hEtaNeutrinoSig->Fill((*vEta)[neutrinoIndex]);
				    hPhiNeutrinoSig->Fill((*vPhi)[neutrinoIndex]);
                    hPtSoloMuonBarSig->Fill((*vPt)[soloMuonBarIndex]);
				    hEtaSoloMuonBarSig->Fill((*vEta)[soloMuonBarIndex]);
				    hPhiSoloMuonBarSig->Fill((*vPhi)[soloMuonBarIndex]);

					// The dimuons candidates are correlated
					// TODO: add this to part above in if-statement
					// TODO: angle incorrect, create Jpsi first!
					// J/psi -> μ+μ-
					if(diMuon1BcpIndex != -1 && diMuon2BcpIndex != -1 &&
						(*vMother1)[diMuon1BcpIndex] == (*vMother1)[diMuon2BcpIndex]) {
						DPhiDiMuons = DeltaPhi((*vPhi)[diMuon1BcpIndex],(*vPhi)[diMuon2BcpIndex]);
						DEtaDiMuons = DeltaEta((*vEta)[diMuon1BcpIndex],(*vEta)[diMuon2BcpIndex]);
						hDPhiDiMuons->Fill(DPhiDiMuons);
						hDEtaDiMuons->Fill(DEtaDiMuons);
						LVdiMuon1.SetPtEtaPhiM(
							(*vPt)[diMuon1BcpIndex], 
							(*vEta)[diMuon1BcpIndex], 
							(*vPhi)[diMuon1BcpIndex], 
							(*vM)[diMuon1BcpIndex]
						);
						LVdiMuon2.SetPtEtaPhiM(
							(*vPt)[diMuon2BcpIndex], 
							(*vEta)[diMuon2BcpIndex], 
							(*vPhi)[diMuon2BcpIndex], 
							(*vM)[diMuon2BcpIndex]
						);
						LVjpsi = LVdiMuon1+LVdiMuon2;
						hInvMassJpsi->Fill(LVjpsi.M());
						// hEtaJpsi->Fill(LVjpsi.Eta());
						hDPhiSoloMuonBarDiMuons->Fill(DeltaPhi((*vPhi)[soloMuonBarIndex],LVjpsi.Phi()));
						hDEtaSoloMuonBarDiMuons->Fill(DeltaEta((*vEta)[soloMuonBarIndex],LVjpsi.Eta()));

						hPtDiMuon1BcpSig->Fill((*vPt)[diMuon1BcpIndex]);
						hEtaDiMuon1BcpSig->Fill((*vEta)[diMuon1BcpIndex]);
						hPhiDiMuon1BcpSig->Fill((*vPhi)[diMuon1BcpIndex]);

						hPtDiMuon2BcpSig->Fill((*vPt)[diMuon2BcpIndex]);
						hEtaDiMuon2BcpSig->Fill((*vEta)[diMuon2BcpIndex]);
						hPhiDiMuon2BcpSig->Fill((*vPhi)[diMuon2BcpIndex]);

						// Full decay reconstructed: invariant mass
						Double_t invariantMass = InvariantMass(neutrinoIndex,soloMuonBarIndex, 
																diMuon1BcpIndex,diMuon2BcpIndex,
																vPt,vEta,vPhi,vM);
						hInvMass->Fill(invariantMass);
						if(VERBOSE) { std::cout<<"invariant mass Bc+ = "<<invariantMass<<std::endl; }
						double invariantMassWithoutNeutrino = InvariantMassWithoutNeutrino(
																soloMuonBarIndex, 
																diMuon1BcpIndex,diMuon2BcpIndex,
																vPt,vEta,vPhi,vM);
						hInvMassWithoutNeutrino->Fill(invariantMassWithoutNeutrino);
						hDMass->Fill(invariantMass-invariantMassWithoutNeutrino);
					}
				}
			    
				// anti-muon neutrino
			    if(motherID == -541 && id == -14) { // found anti-muon neutrino (B_cˆ{-} signal)
                    if(VERBOSE) {
                    // std::cout<<"This is my anti-neutrino mom (from Bc-): "<<motherID<<std::endl;
                    // std::cout<<"iEvent = "<<iEvent<<std::endl;
                    }
                    neutrinoBarIndex = ipart;
                }

				// muon
                if(motherID == -541 && id == 13) { // found standalone muon (from B_cˆ{-})
                    if(VERBOSE) {
                    // std::cout<<"This is my muon- mom (from Bc-): "<<motherID<<std::endl;
                    // std::cout<<"iEvent = "<<iEvent<<std::endl;
                    }
                    soloMuonIndex = ipart;
                }

				// J/psi
				if(grandMotherID == -541 && motherID == 443 && id == -13) { // found muon in di-muon pair (from J/psi)
                    diMuon1BcmIndex = ipart;
                    // if(VERBOSE) { std::cout<<"1 = "<<diMuon1Index<<std::endl; }
                }

                if(grandMotherID == -541 && motherID == 443 && id == 13) { // found anti-muon in di-muon pair (from J/psi)
                    diMuon2BcmIndex = ipart;
                    // if(VERBOSE) { std::cout<<"2 = "<<diMuon2Index<<std::endl; }
                }

                // Candidate matching
                // Bcˆ{-} -> μ- ν-bar J/ψ -> μ+μ-
                if(neutrinoBarIndex != -1 && soloMuonIndex != -1 &&
                    (*vMother1)[neutrinoBarIndex] == (*vMother1)[soloMuonIndex]) {
					hDPhiNeutrinoBarSoloMuon->Fill(DeltaPhi((*vPhi)[neutrinoBarIndex],(*vPhi)[soloMuonIndex]));
					hDEtaNeutrinoBarSoloMuon->Fill(DeltaEta((*vEta)[neutrinoBarIndex],(*vEta)[soloMuonIndex]));

                    hPtNeutrinoBarSig->Fill((*vPt)[neutrinoBarIndex]);
				    hEtaNeutrinoBarSig->Fill((*vEta)[neutrinoBarIndex]);
				    hPhiNeutrinoBarSig->Fill((*vPhi)[neutrinoBarIndex]);
                    hPtSoloMuonSig->Fill((*vPt)[soloMuonIndex]);
				    hEtaSoloMuonSig->Fill((*vEta)[soloMuonIndex]);
				    hPhiSoloMuonSig->Fill((*vPhi)[soloMuonIndex]);

					// The dimuons candidates are correlated
					// J/psi -> μ+μ-
					if(diMuon1BcmIndex != -1 && diMuon2BcmIndex != -1 &&
						(*vMother1)[diMuon1BcmIndex] == (*vMother1)[diMuon2BcmIndex]) {
						DPhiDiMuons = DeltaPhi((*vPhi)[diMuon1BcmIndex],(*vPhi)[diMuon2BcmIndex]);
						DEtaDiMuons = DeltaEta((*vEta)[diMuon1BcmIndex],(*vEta)[diMuon2BcmIndex]);
						hDPhiDiMuons->Fill(DPhiDiMuons);
						hDEtaDiMuons->Fill(DEtaDiMuons);
						// hDPhiSoloMuonDiMuons->Fill(DeltaPhi((*vPhi)[soloMuonIndex],DPhiDiMuons));
						// hDEtaSoloMuonDiMuons->Fill(DeltaEta((*vEta)[soloMuonIndex],DEtaDiMuons));

						hPtDiMuon1BcmSig->Fill((*vPt)[diMuon1BcmIndex]);
						if (VERBOSE) { std::cout<<"pT dimuon = "<<(*vPt)[diMuon1BcmIndex]<<std::endl; }
						hEtaDiMuon1BcmSig->Fill((*vEta)[diMuon1BcmIndex]);
						hPhiDiMuon1BcmSig->Fill((*vPhi)[diMuon1BcmIndex]);

						hPtDiMuon2BcmSig->Fill((*vPt)[diMuon2BcmIndex]);
						hEtaDiMuon2BcmSig->Fill((*vEta)[diMuon2BcmIndex]);
						hPhiDiMuon2BcmSig->Fill((*vPhi)[diMuon2BcmIndex]);
    
    					// Calculate the invariant mass of the four-particle system
						// Only the Bc+ system is used from now on
						// Double_t invariantMass = InvariantMass(neutrinoBarIndex,soloMuonIndex, 
						//										diMuon1BcmIndex,diMuon2BcmIndex,
						//										vPt,vEta,vPhi,vM);
						// hInvMass->Fill(invariantMass);
						// if(VERBOSE) { std::cout<<"invariant mass Bc- = "<<invariantMass<<std::endl; }

						// total = lorentzSoloMuon + lorentzDiMuon1 + lorentzDiMuon2;
						// double invariantMassWithoutNeutrino = total.M();
						// hInvMassWithoutNeutrino->Fill(invariantMassWithoutNeutrino);

						// hDMass->Fill(invariantMass-invariantMassWithoutNeutrino);
					}
				}

                // Background particles
                // TODO: think if the checks need to include not from J/psi filter
			    if(motherID != 541 && id == 14) { // found muon neutrino (B_cˆ{+} background)
					neutrinoBkgIndex = ipart;
				    hPtNeutrinoBkg->Fill(pT);
				    hEtaNeutrinoBkg->Fill(eta);
				    hPhiNeutrinoBkg->Fill(phi);
			    }

			    if(motherID != -541 && id == -14) { // found anti-muon neutrino (B_cˆ{-} background)
					neutrinoBarBkgIndex = ipart;
				    hPtNeutrinoBarBkg->Fill(pT);
				    hEtaNeutrinoBarBkg->Fill(eta);
				    hPhiNeutrinoBarBkg->Fill(phi);
			    }

                if(motherID != 541 && motherID != 443 && id == -13) { // found standalone anti-muon (from B_cˆ{+} background)
					soloMuonBkgIndex = ipart;
                    hPtSoloMuonBkg->Fill(pT);
				    hEtaSoloMuonBkg->Fill(eta);
				    hPhiSoloMuonBkg->Fill(phi);
                }

                if(motherID != -541 && motherID != 443 && id == 13) { // found standalone muon (from B_cˆ{-} background)
					soloMuonBarBkgIndex = ipart;
                    hPtSoloMuonBarBkg->Fill(pT);
				    hEtaSoloMuonBarBkg->Fill(eta);
				    hPhiSoloMuonBarBkg->Fill(phi);
                }

                if(grandMotherID != 541 && grandMotherID != -541 && motherID == 443 && id == -13) { // found anti-muon in di-muon pair (from J/psi)
					diMuon1BkgIndex = ipart;
                    hPtDiMuon1Bkg->Fill(pT);
				    hEtaDiMuon1Bkg->Fill(eta);
				    hPhiDiMuon1Bkg->Fill(phi);
                }

                if(grandMotherID != 541 && grandMotherID != -541 && motherID == 443 && id == 13) { // found anti-muon in di-muon pair (from J/psi)
					diMuon2BkgIndex = ipart;
                    hPtDiMuon2Bkg->Fill(pT);
				    hEtaDiMuon2Bkg->Fill(eta);
				    hPhiDiMuon2Bkg->Fill(phi);
                }		

				// Calculate uncorrelated delta phi and delta eta
				// TODO: not completely analogous to signal treatment
				if(soloMuonBkgIndex != -1 && diMuon1BkgIndex != -1 && diMuon2BkgIndex != -1) {
					DPhiDiMuonsBkg = DeltaPhi((*vPhi)[diMuon1BkgIndex],(*vPhi)[diMuon2BkgIndex]);
					DEtaDiMuonsBkg = DeltaEta((*vEta)[diMuon1BkgIndex],(*vEta)[diMuon2BkgIndex]);
					hDPhiDiMuonsBkg->Fill(DPhiDiMuons);
					hDEtaDiMuonsBkg->Fill(DEtaDiMuons);

					LVdiMuon1.SetPtEtaPhiM(
							(*vPt)[diMuon1BkgIndex], 
							(*vEta)[diMuon1BkgIndex], 
							(*vPhi)[diMuon1BkgIndex], 
							(*vM)[diMuon1BkgIndex]
						);
						LVdiMuon2.SetPtEtaPhiM(
							(*vPt)[diMuon2BkgIndex], 
							(*vEta)[diMuon2BkgIndex], 
							(*vPhi)[diMuon2BkgIndex], 
							(*vM)[diMuon2BkgIndex]
						);
						LVjpsi = LVdiMuon1+LVdiMuon2;
						hDPhiSoloMuonDiMuonsBkg->Fill(DeltaPhi((*vPhi)[soloMuonBkgIndex],LVjpsi.Phi()));
						hDEtaSoloMuonDiMuonsBkg->Fill(DeltaEta((*vEta)[soloMuonBkgIndex],LVjpsi.Eta()));
				}

				if(soloMuonBarBkgIndex != -1 && diMuon1BkgIndex != -1 && diMuon2BkgIndex != -1) {
					DPhiDiMuonsBkg = DeltaPhi((*vPhi)[diMuon1BkgIndex],(*vPhi)[diMuon2BkgIndex]);
					DEtaDiMuonsBkg = DeltaEta((*vEta)[diMuon1BkgIndex],(*vEta)[diMuon2BkgIndex]);
					hDPhiDiMuonsBkg->Fill(DPhiDiMuons);
					hDEtaDiMuonsBkg->Fill(DEtaDiMuons);

					LVdiMuon1.SetPtEtaPhiM(
							(*vPt)[diMuon1BkgIndex], 
							(*vEta)[diMuon1BkgIndex], 
							(*vPhi)[diMuon1BkgIndex], 
							(*vM)[diMuon1BkgIndex]
						);
						LVdiMuon2.SetPtEtaPhiM(
							(*vPt)[diMuon2BkgIndex], 
							(*vEta)[diMuon2BkgIndex], 
							(*vPhi)[diMuon2BkgIndex], 
							(*vM)[diMuon2BkgIndex]
						);
						LVjpsi = LVdiMuon1+LVdiMuon2;
						hDPhiSoloMuonBarDiMuonsBkg->Fill(DeltaPhi((*vPhi)[soloMuonBarBkgIndex],LVjpsi.Phi()));
						hDEtaSoloMuonBarDiMuonsBkg->Fill(DeltaEta((*vEta)[soloMuonBarBkgIndex],LVjpsi.Eta()));
				}

				// Study backgrounds : reconstruction with fake solo muon (including and excluding neutrinos)
				if(neutrinoIndex != -1 && soloMuonBarBkgIndex != -1 && diMuon1BcpIndex != -1 && diMuon2BcpIndex != -1) {// fake J/psi, real muon
					if(VERBOSE) {std::cout<<"Found fake solo muon"<<std::endl;}
					Double_t invariantMass = InvariantMass(neutrinoIndex,soloMuonBarBkgIndex, 
															diMuon1BcpIndex,diMuon2BcpIndex,
															vPt,vEta,vPhi,vM);
					hInvMassFakeSoloMuon->Fill(invariantMass);
					Double_t invariantMassWithoutNeutrino = InvariantMassWithoutNeutrino(
															soloMuonBarBkgIndex, 
															diMuon1BcpIndex,diMuon2BcpIndex,
															vPt,vEta,vPhi,vM);
					hInvMassWithoutNeutrinoFakeSoloMuon->Fill(invariantMassWithoutNeutrino);
					hDMassFakeSoloMuon->Fill(invariantMass-invariantMassWithoutNeutrino);
				}

				// Study backgrounds : reconstruction with fake J/psi (including and excluding neutrinos)
				if(neutrinoIndex != -1 && soloMuonBarIndex != -1 && diMuon1BkgIndex != -1 && diMuon2BkgIndex != -1) {// fake J/psi, real muon
					if(VERBOSE) {std::cout<<"Found fake Jpsi"<<std::endl;}
					Double_t invariantMass = InvariantMass(neutrinoIndex,soloMuonBarIndex, 
															diMuon1BkgIndex,diMuon2BkgIndex,
															vPt,vEta,vPhi,vM);
					hInvMassFakeJpsi->Fill(invariantMass);
					Double_t invariantMassWithoutNeutrino = InvariantMassWithoutNeutrino(
															soloMuonBarIndex, 
															diMuon1BkgIndex,diMuon2BkgIndex,
															vPt,vEta,vPhi,vM);
					hInvMassWithoutNeutrinoFakeJpsi->Fill(invariantMassWithoutNeutrino);
					hDMassFakeJpsi->Fill(invariantMass-invariantMassWithoutNeutrino);
				}

			} // pT > 1 GeV cut
		} // Particle loop
	} // Eventloop
	
	output->Write();
	output->Close();
	std::cout<<"The total number of Particles is: "<<nParticles<<std::endl;

	/*
    if(VERBOSE) { 
        std::cout<<"The lepton flavour violation is violated in PYTHIA by about +/- 400 particles. Don't expect the same number of muons and neutrinos to be produced from Bc decay"<<endl;
    }
	*/

    std::cout<<std::endl;
    std::cout<<"Analysis terminated. Succesful?"<<std::endl;
}

int BcAnalyser() {

    status_file();

	return 0;
}