// #include <iostream>
// #include <vector>
// #include "TFile.h"
// #include "TTree.h"
// #include "SimulationDataFormat/MCTrack.h"
// #include "Steer/Steer.h"
// #include "Steer/Steer.h"
// #include "MCUtils.h"
// #include "TMCProcess.h"
// #include "CommonDataFormat/MCTrack.h"

using namespace o2;
// using namespace o2::steer;

bool isFinalState(o2::MCTrack* track) {
    // A final-state particle will have no daughters and no mother
    return (track->getFirstDaughterTrackId() == -1 && track->getLastDaughterTrackId() == -1);
}

int readKinematics() {


 // TODO: do this on hyperloop

 // Jpsi = 443
 // Bc+ = 541


  TH1F *hID = new TH1F("hID","PDG ID for track",20000,-10000,10000);
  TH1F *hPtJpsi = new TH1F("hPtJpsi","Pt spectrum jpsi",100,0,10);
  TH1F *hPtPromptJpsi = new TH1F("hPtPromptJpsi","Pt spectrum prompt jpsi",100,0,10);
  TH1F *hPtNonPromptJpsi = new TH1F("hPtNonPromptJpsi","Pt spectrum non-prompt jpsi",100,0,10);
  TH1F *hVxJpsi = new TH1F("hVxJpsi","Vx for jpsi",100,-10,10);
  TH1F *hVxPromptJpsi = new TH1F("hVxPromptJpsi","Vx for prompt jpsi",100,-10,10);
  TH1F *hVxNonPromptJpsi = new TH1F("hVxNonPromptJpsi","Vx for non-prompt jpsi",100,-10,10);
  TH1F *hDiMuonTRUEMass = new TH1F("hDiMuonTRUEMass","Di-muon TRUE mass [GeV]",100,3.095,3.099);
  TH1F *hDiMuonMass = new TH1F("hDiMuonMass","Di-muon mass [GeV]",100,0,10);
  TH1F *hDiMuonCandMass = new TH1F("hDiMuonCandMass","Di-muon candidate mass [GeV]",100,2.0,4.5);
  TH1F *hDiMuonMatchedMass = new TH1F("hDiMuonMatchedMass","Di-muon matched < 10.0 mass [GeV]",100,0,10);
  TH1F *hDiMuonVxDiff = new TH1F("hDiMuonVxDiff","Di-muon vx difference of pairs",100,-11,11);


  Int_t trackId = 0;
  Int_t nJpsi=0;Int_t nPromptJpsi=0;Int_t nNonPromptJpsi=0;Int_t nMuon=0;Int_t nAntiMuon=0;
  Int_t nMisId=0;
  Int_t particlePdgCode,motherTrackId,motherPdgCode,triggerMotherTrackId,associatePdgCode,associateMotherTrackId,associateMotherPdgCode;
  Int_t firstDaughterTrackId,firstDaughterPdgCode;
  Int_t lastDaughterTrackId,lastDaughterPdgCode;
  Double_t pT,eta,triggerVx,triggerVy,triggerVz,associateVx,associateVy,associateVz;


  Double_t VxCut = 0.1; // cut on di-muon production vertex
  Double_t lowMassCut = 2.5; // jpsi mass cuts on dimuon matching
  Double_t highMassCut = 4.0;


  // optional misID setting
  // probability can be adjusted
  Double_t misIdProb = 0.01; // 1% misid chance
  TRandom3 randGen;
  randGen.SetSeed(0);


  TFile inputKine("JpsiStudies/1e5_o2sim_Kine.root","READ");
  auto tree = (TTree*)inputKine.Get("o2sim");
  std::vector<o2::MCTrack>* tracks{};
  tree->SetBranchAddress("MCTrack", &tracks);


  // TO DO 
  // ADD RANDOM COMBINATIONS OF MUONS (OR SEMI-RANDOM) OR AT LEAST MOTIVATED BY EXPERIMENT 
  // (e.g. similar origin vertex, etc.)


  for (int ev = 0; ev < tree->GetEntries(); ++ev) {


      tree->GetEntry(ev);
      // std::cout<<"NEXT EVENT"<<std::endl<<std::endl;


      for (auto& track : *tracks) {

        
        trackId++;

        // if (!isFinalState(&track)) { continue; }

        pT = track.GetPt();
        eta = track.GetEta();
        triggerVx = track.Vx();
        triggerVy = track.Vy();
        triggerVz = track.Vz();

        // if (eta < -3.6 || eta > -2.5) { continue; } // MFT pseudorapidity cut

        hID->Fill(track.GetPdgCode());
        particlePdgCode = track.GetPdgCode();
        motherTrackId = track.getMotherTrackId();
        const o2::MCTrack& motherTrack = (*tracks)[motherTrackId];
        motherPdgCode = motherTrack.GetPdgCode();

        firstDaughterTrackId = track.getFirstDaughterTrackId();
        const o2::MCTrack& firstDaughterTrack = (*tracks)[firstDaughterTrackId];
        firstDaughterPdgCode = firstDaughterTrack.GetPdgCode();

        lastDaughterTrackId = track.getLastDaughterTrackId();
        const o2::MCTrack& lastDaughterTrack = (*tracks)[lastDaughterTrackId];
        lastDaughterPdgCode = lastDaughterTrack.GetPdgCode();
        
        // std::cout<<"pdg code = "<<particlePdgCode<<std::endl;


        // Muon misId initialisation
        Double_t randomValue = randGen.Uniform(0.0, 1.0);
        Int_t triggerDetectorPdgCode = 13; // muon
        Int_t associateDetectorPdgCode = -13; // anti-muon


        // for testing only
        /*
        if (particlePdgCode == 3322) { // xi0
          std::cout<<"I am a "<<particlePdgCode<<std::endl;
          std::cout<<"firstDaughter = "<<firstDaughterPdgCode<<std::endl;
          std::cout<<"lastDaughter = "<<lastDaughterPdgCode<<std::endl<<std::endl;
          // std::cout<<"me = "<<particlePdgCode<<std::endl;
          // std::cout<<"my mom = "<<motherPdgCode<<std::endl<<std::endl;
        }
        */
        

        /*
        if (particlePdgCode == 3122) { // lambda
          std::cout<<"firstDaughter = "<<firstDaughterPdgCode<<std::endl;
          std::cout<<"lastDaughter = "<<lastDaughterPdgCode<<std::endl<<std::endl;
          // std::cout<<"me = "<<particlePdgCode<<std::endl;
          // std::cout<<"my mom = "<<motherPdgCode<<std::endl<<std::endl;
        }
        */


        if (particlePdgCode == 541) { // bc+
          // std::cout<<"firstDaughter Bc+ = "<<firstDaughterPdgCode<<std::endl;
          // std::cout<<"lastDaughter Bc+ = "<<lastDaughterPdgCode<<std::endl<<std::endl;
          // for (int daughterTrackId = firstDaughterTrackId; daughterTrackId <= lastDaughterTrackId; ++daughterTrackId) {
            // const o2::MCTrack& daughterTrack = (*tracks)[daughterTrackId];
            // int daughterPdgCode = daughterTrack.GetPdgCode();
            // std::cout << "Bc+ Daughter PDG: " << daughterPdgCode << std::endl;
          // }
          // std::cout<<std::endl;
          // std::cout<<"me = "<<particlePdgCode<<std::endl;
          // std::cout<<"my mom = "<<motherPdgCode<<std::endl<<std::endl;
        }


        if (particlePdgCode == 443) { // Jpsi (use code == 2212 (proton) for non-prompt/prompt distinction)
          nJpsi++;
          hPtJpsi->Fill(pT);
          hVxJpsi->Fill(triggerVx);
          if (track.isPrimary()) { 
            nPromptJpsi++;
            hPtPromptJpsi->Fill(pT);
            hVxPromptJpsi->Fill(triggerVx); 
          }
          if (track.isSecondary()) { 
            nNonPromptJpsi++;
            hPtNonPromptJpsi->Fill(pT);
            hVxNonPromptJpsi->Fill(triggerVx);
          }
          // std::cout<<"firstDaughter Jpsi = "<<firstDaughterPdgCode<<std::endl;
          // std::cout<<"lastDaughter Jpsi = "<<lastDaughterPdgCode<<std::endl<<std::endl;
          // for (int daughterTrackId = firstDaughterTrackId; daughterTrackId <= lastDaughterTrackId; ++daughterTrackId) {
            // const o2::MCTrack& daughterTrack = (*tracks)[daughterTrackId];
            // int daughterPdgCode = daughterTrack.GetPdgCode();
            // std::cout << "Jpsi Daughter PDG: " << daughterPdgCode << std::endl;
          // }
          // std::cout<<std::endl;
          // std::cout<<"me = "<<particlePdgCode<<std::endl;
          // std::cout<<"my mom = "<<motherPdgCode<<std::endl<<std::endl;
        }


        // MC TRUE dimuon matching
        if (particlePdgCode == 13) { // mu-
          nMuon++;
          TLorentzVector trigger4P;
          track.Get4Momentum(trigger4P);
          // std::cout<<"me = "<<particlePdgCode<<std::endl;
          // std::cout<<"my mom = "<<motherPdgCode<<std::endl<<std::endl;
          for (auto& track : *tracks) { // couple with mu+ now
            associatePdgCode = track.GetPdgCode();
            associateMotherTrackId = track.getMotherTrackId();
            const o2::MCTrack& associateMotherTrack = (*tracks)[associateMotherTrackId];
            associateMotherPdgCode = motherTrack.GetPdgCode();
            // std::cout<<"associateMother = "<<associateMother<<std::endl;
            if (associatePdgCode == -13) { // mu+
              TLorentzVector associate4P;
              associateVx = track.Vx();
              associateVy = track.Vy();
              associateVz = track.Vz();
              if (associateMotherTrackId == motherTrackId) { // true dimuon pair found
                nAntiMuon++;
                std::cout<<"me = "<<associatePdgCode<<std::endl;
                std::cout<<"my mom = "<<associateMotherPdgCode<<std::endl;
                std::cout<<"and my trigger mom = "<<motherPdgCode<<std::endl;
                std::cout<<"my trigger production coordinates = "<<"("<<triggerVx<<","<<triggerVy<<","<<triggerVz<<")"<<std::endl<<std::endl;
                std::cout<<"my associate production coordinates = "<<"("<<associateVx<<","<<associateVy<<","<<associateVz<<")"<<std::endl<<std::endl;
                track.Get4Momentum(associate4P);
                TLorentzVector dimuon4P = trigger4P+associate4P;
                Double_t dimuonM = dimuon4P.M();
                std::cout<<"Di-muon mass = "<<dimuonM<<std::endl;
                hDiMuonTRUEMass->Fill(dimuonM);
              }
            }
          }
        } // end of 'TRUE' di-muon coupling


        if (randomValue < misIdProb) { 
          nMisId++;
          triggerDetectorPdgCode = -211; 
          associateDetectorPdgCode = 211;
        } // pion misID

        // Muon matching with potential detector effects included
        // NOTE: seems like there is misID effects, sometimes coupling gives e.g. an eta->mumu
        if (particlePdgCode == triggerDetectorPdgCode) { // mu-
          TLorentzVector trigger4P;
          track.Get4Momentum(trigger4P);
          // std::cout<<"me = "<<particlePdgCode<<std::endl;
          // std::cout<<"my mom = "<<motherPdgCode<<std::endl<<std::endl;
          for (auto& track : *tracks) { // couple with mu+ now
            associatePdgCode = track.GetPdgCode();
            associateMotherTrackId = track.getMotherTrackId();
            const o2::MCTrack& associateMotherTrack = (*tracks)[associateMotherTrackId];
            associateMotherPdgCode = motherTrack.GetPdgCode();
            // std::cout<<"associateMother = "<<associateMother<<std::endl;
            if (associatePdgCode == associateDetectorPdgCode) { // mu+
              TLorentzVector associate4P;
              associateVx = track.Vx();
              associateVy = track.Vy();
              associateVz = track.Vz();
              if (associateMotherTrackId == motherTrackId) { // true dimuon pair found
                std::cout<<"me = "<<associatePdgCode<<std::endl;
                std::cout<<"my mom = "<<associateMotherPdgCode<<std::endl;
                std::cout<<"and my trigger mom = "<<motherPdgCode<<std::endl;
                std::cout<<"my trigger production coordinates = "<<"("<<triggerVx<<","<<triggerVy<<","<<triggerVz<<")"<<std::endl<<std::endl;
                std::cout<<"my associate production coordinates = "<<"("<<associateVx<<","<<associateVy<<","<<associateVz<<")"<<std::endl<<std::endl;
                track.Get4Momentum(associate4P);
                TLorentzVector dimuon4P = trigger4P+associate4P;
                Double_t dimuonM = dimuon4P.M();
                std::cout<<"Di-muon mass = "<<dimuonM<<std::endl;
                hDiMuonMass->Fill(dimuonM);
              }
              if (abs(triggerVx - associateVx) < VxCut) { // matching of dimuon candidates
                track.Get4Momentum(associate4P);
                TLorentzVector dimuon4P = trigger4P+associate4P;
                Double_t dimuonM = dimuon4P.M();
                std::cout<<"candidate matching mass = "<<dimuonM<<std::endl;
                if (dimuonM < 2) {
                  std::cout<<"trigger mom = "<<motherPdgCode<<std::endl;
                  std::cout<<"trigger mom track id = "<<motherTrackId<<std::endl;
                  std::cout<<"associate mom = "<<associateMotherPdgCode<<std::endl;
                  std::cout<<"associate mom track id = "<<associateMotherTrackId<<std::endl<<std::endl;
                }
                if (dimuonM > lowMassCut && dimuonM < highMassCut) { hDiMuonCandMass->Fill(dimuonM); }
              }
              if (abs(triggerVx - associateVx) < 10.0) { // "minimum bias"
                track.Get4Momentum(associate4P);
                TLorentzVector dimuon4P = trigger4P+associate4P;
                Double_t dimuonM = dimuon4P.M();
                // std::cout<<"candidate fake mass = "<<dimuonM<<std::endl<<std::endl;
                hDiMuonMatchedMass->Fill(dimuonM);
                hDiMuonVxDiff->Fill(triggerVx-associateVx);
              }
            }
          }
        } // end of 'normal' di-muon coupling


      } // end of tracks


    } // end of event


  std::cout<<"Analysed "<<tree->GetEntries()<<" events"<<std::endl;
  std::cout<<"Number of jpsi found = "<<nJpsi<<std::endl;
  std::cout<<"Number of prompt jpsi found = "<<nPromptJpsi<<std::endl;
  std::cout<<"Number of non-prompt jpsi found = "<<nNonPromptJpsi<<std::endl;
  std::cout<<"Number of muons found = "<<nMuon<<std::endl;
  std::cout<<"Number of di-muons found = "<<nAntiMuon<<std::endl;
  std::cout<<"Number of misIds found = "<<nMisId<<std::endl;


  TFile outputFile("JpsiStudies/output.root", "RECREATE");
  hID->Write();
  hPtJpsi->Write();
  hPtPromptJpsi->Write();
  hPtNonPromptJpsi->Write();
  hVxJpsi->Write();
  hVxPromptJpsi->Write();
  hVxNonPromptJpsi->Write();
  hDiMuonTRUEMass->Write();
  hDiMuonMass->Write();
  hDiMuonCandMass->Write();
  hDiMuonMatchedMass->Write();
  hDiMuonVxDiff->Write();


  return 0;


}