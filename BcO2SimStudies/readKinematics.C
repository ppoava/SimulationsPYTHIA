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
  /*
  // init the reader from the transport kinematics file (assuming here prefix o2sim)
  o2::steer::MCKinematicsReader reader("o2sim", o2::steer::MCKinematicsReader::Mode::kMCKine);

  // loop over all events in the file
  for (int event = 0; event < reader.getNEvents(0); ++event) {
    // get all Monte Carlo tracks for this event; not that this is the short version of
    // std::vector<MCTrack> const& tracks = reader.getTracks(source, event);
    std::vector<MCTrack> const& tracks = reader.getTracks(event);

    // analyse tracks
    for (auto& t : tracks) {
      // analyse tracks; fetch mother track of each track (in the pool of all tracks)
      auto mother = o2::mcutil::MCTrackNavigator::getMother(t, tracks);
      if (mother) {
        std::cout << "This track has a mother"<<std::endl;;
      }
      // fetch the (backward first) primary particle from which this track derives
      auto primary = o2::mcutil::MCTrackNavigator::getFirstPrimary(t, tracks);
      }
  }
  return 0;
  */

 // Jpsi = 443
 // Bc+ = 541

  TH1F *hID = new TH1F("hID","PDG ID for track",20000,-10000,10000);
  TH1F *hDiMuonMass = new TH1F("hDiMuonMass","Di-muon mass [GeV]",100,3.095,3.099);
  Int_t nJpsi = 0;
  Int_t nMuon = 0;
  Int_t nAntiMuon = 0;
  Int_t particlePdgCode,motherTrackId,motherPdgCode,triggerMotherTrackId,associatePdgCode,associateMotherTrackId,associateMotherPdgCode;
  Int_t firstDaughterTrackId,firstDaughterPdgCode;
  Int_t lastDaughterTrackId,lastDaughterPdgCode;
  Double_t pT,eta,triggerVx,triggerVy,triggerVz,associateVx,associateVy,associateVz;


  TFile inputKine("Lambda0ForcedDecay/o2sim_Kine.root","READ");
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


        // for testing only
        if (particlePdgCode == 3122) { // lambda
          std::cout<<"firstDaughter = "<<firstDaughterPdgCode<<std::endl;
          std::cout<<"lastDaughter = "<<lastDaughterPdgCode<<std::endl<<std::endl;
          // std::cout<<"me = "<<particlePdgCode<<std::endl;
          // std::cout<<"my mom = "<<motherPdgCode<<std::endl<<std::endl;
        }


        if (particlePdgCode == 541) { // bc+
          // std::cout<<"firstDaughter Bc+ = "<<firstDaughterPdgCode<<std::endl;
          // std::cout<<"lastDaughter Bc+ = "<<lastDaughterPdgCode<<std::endl<<std::endl;
          for (int daughterTrackId = firstDaughterTrackId; daughterTrackId <= lastDaughterTrackId; ++daughterTrackId) {
            const o2::MCTrack& daughterTrack = (*tracks)[daughterTrackId];
            int daughterPdgCode = daughterTrack.GetPdgCode();
            std::cout << "Bc+ Daughter PDG: " << daughterPdgCode << std::endl;
          }
          std::cout<<std::endl;
          // std::cout<<"me = "<<particlePdgCode<<std::endl;
          // std::cout<<"my mom = "<<motherPdgCode<<std::endl<<std::endl;
        }


        if (particlePdgCode == 443) { // Jpsi (use code == 2212 (proton) for non-prompt/prompt distinction)
          nJpsi++;
          // std::cout<<"firstDaughter Jpsi = "<<firstDaughterPdgCode<<std::endl;
          // std::cout<<"lastDaughter Jpsi = "<<lastDaughterPdgCode<<std::endl<<std::endl;
          for (int daughterTrackId = firstDaughterTrackId; daughterTrackId <= lastDaughterTrackId; ++daughterTrackId) {
            const o2::MCTrack& daughterTrack = (*tracks)[daughterTrackId];
            int daughterPdgCode = daughterTrack.GetPdgCode();
            std::cout << "Jpsi Daughter PDG: " << daughterPdgCode << std::endl;
          }
          std::cout<<std::endl;
          // std::cout<<"me = "<<particlePdgCode<<std::endl;
          // std::cout<<"my mom = "<<motherPdgCode<<std::endl<<std::endl;
        }


        // NOTE: seems like there is misID effects, sometimes coupling gives e.g. an eta->mumu
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
            if (associatePdgCode == -13 && associateMotherTrackId == motherTrackId) { // mu+ and same mother as mu-
              nAntiMuon++;
              TLorentzVector associate4P;
              associateVx = track.Vx();
              associateVy = track.Vy();
              associateVz = track.Vz();
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
          }
        } // end of di-muon coupling


      } // end of tracks


    } // end of event


  std::cout<<"Number of jpsi found = "<<nJpsi<<std::endl;
  std::cout<<"Number of muons found = "<<nMuon<<std::endl;
  std::cout<<"Number of di-muons found = "<<nAntiMuon<<std::endl;
  TFile outputFile("JpsiStudies/output.root", "RECREATE");
  hID->Write();
  hDiMuonMass->Write();


  return 0;


}