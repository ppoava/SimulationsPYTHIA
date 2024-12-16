set -x

NEV=1000
o2-sim -j 20 -n ${NEV} -g pythia8 --configKeyValues 'SimUserDecay.pdglist=443;GeneratorPythia8.config=decay_jpsi.cfg;DecayerPythia8.verbose=true'
