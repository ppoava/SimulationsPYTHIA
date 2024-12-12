set -x

NEV=100
o2-sim -n ${NEV} -g pythia8 --configKeyValues "GeneratorPythia8.config=decay_jpsi.cfg"