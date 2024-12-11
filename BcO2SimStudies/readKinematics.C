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
  TFile inputKine("1e3_o2sim_Kine.root","READ");
    auto tree = (TTree*)inputKine.Get("o2sim");
    std::vector<o2::MCTrack>* tracks{};
    tree->SetBranchAddress("MCTrack", &tracks);
    for (int ev = 0; ev < tree->GetEntries(); ++ev) {


        tree->GetEntry(ev);


        for (auto& track : *tracks) {


          hID->Fill(track.GetPdgCode());
          Int_t particlePdgCode = track.GetPdgCode();
          Int_t motherTrackId = track.getMotherTrackId();
          const o2::MCTrack& motherTrack = (*tracks)[motherTrackId];
          Int_t motherPdgCode = motherTrack.GetPdgCode();

          if (particlePdgCode == 443) { // Jpsi
            std::cout<<"me = "<<particlePdgCode<<std::endl;
            std::cout<<"my mom = "<<motherPdgCode<<std::endl<<std::endl;
          }

          if (particlePdgCode == 13) { // mu-
            std::cout<<"me = "<<particlePdgCode<<std::endl;
            std::cout<<"my mom = "<<motherPdgCode<<std::endl<<std::endl;
          }
    

        }
    }
    TFile outputFile("output.root", "RECREATE");
    hID->Write();

    return 0;
}