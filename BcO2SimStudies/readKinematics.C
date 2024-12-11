#include <iostream>
#include <vector>
#include "SimulationDataFormat/MCTrack.h"
#include "Steer/Steer.h"
#include "MCUtils.h"
#include "CommonDataFormat/MCTrack.h"

using o2::steer;
using o2;

int readKinematics() {
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
}