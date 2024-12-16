set -x

NEV=1000
NODES=10

### "minimum bias with jpsi->mumu forced"
### o2-sim -j ${NODES} -n ${NEV} -g pythia8 --configKeyValues "SimUserDecay.pdglist=443;GeneratorPythia8.config=decay_jpsi.cfg;DecayerPythia8.verbose=true"

### boxgen insertion 1 jpsi per event, jpsi->mumu forced
o2-sim -j ${NODES} -n ${NEV} -g boxgen --configKeyValues "BoxGun.pdg=443;BoxGun.number=1;SimUserDecay.pdglist=443;DecayerPythia8.config[1]=decay_jpsi.cfg;DecayerPythia8.verbose=true"