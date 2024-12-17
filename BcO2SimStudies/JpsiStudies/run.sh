set -x

NEV=10
NODES=10

### "minimum bias with jpsi->mumu forced"
# o2-sim -j ${NODES} -n ${NEV} -g pythia8 --configKeyValues "SimUserDecay.pdglist=443;GeneratorPythia8.config=decay_jpsi.cfg;DecayerPythia8.verbose=true"

### "minimum bias with bc+->mu nu jpsi->mumu forced"
# doesn't work???
# also careful: needs a lot of statistics to make sense
# o2-sim -j ${NODES} -n ${NEV} -g pythia8 --configKeyValues "SimUserDecay.pdglist=541 443;GeneratorPythia8.config=decay_bcp.cfg;DecayerPythia8.verbose=true"

### boxgen insertion 1 jpsi per event, jpsi->mumu forced
# DISABLE CHARMONIUM SETTING IN RUN SCRIPT!!!
# doesn't work???
# o2-sim -j ${NODES} -n ${NEV} -g boxgen --configKeyValues \
# "BoxGun.pdg=443;BoxGun.number=1;SimUserDecay.pdglist=443 443;DecayerPythia8.config[1]=decay_jpsi.cfg;DecayerPythia8.verbose=true"

### B_c+ study, can also be used for non-prompt jpsi studies/background studies
# o2-sim -j ${NODES} -n ${NEV} -g boxgen --configKeyValues \
# "BoxGun.pdg=541;BoxGun.number=1;SimUserDecay.pdglist=541 443;DecayerPythia8.config[1]=decay_bcp.cfg;DecayerPythia8.verbose=true"

# change eta setting in boxgun?
# BoxGun.eta[0]=-4.5;BoxGun.eta[1]=-2.5