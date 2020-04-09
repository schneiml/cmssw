#!/bin/bash
set -e
DB=datasets2017.db
RUNS=$(echo "select distinct run from indexentry;" | sqlite3 $DB)
cmsDriver.py step4  -s  HARVESTING:@rerecoZeroBiasFakeHLT+@miniAODDQM --conditions auto:run2_data --data  --filetype DQM --scenario pp --era Run2_2017 --no_exec > cmsDriver.log
CONFIG=step4_HARVESTING.py

for run in $RUNS; do
  mkdir -p reharvest_$run
  cd  reharvest_$run
  cp ../$CONFIG $CONFIG
  echo "process.source.fileNames = cms.untracked.vstring(" >> $CONFIG
  echo "select distinct '\"' || substr(name, 31) || '\",' from file inner join indexentry on indexentry.file = file.id where run = $run;" | sqlite3 ../$DB >> $CONFIG
  echo ")" >> $CONFIG
  echo "process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('$run:1-$(($run+1)):0')" >> $CONFIG
  cat <<EOF > run${run}.sh
#!/bin/bash
set -e
cd $PWD
cmsRun $CONFIG > cmsRun.log 2>&1
mv dqmio_endjob.root run${run}.root
EOF
  cd ..
done

cat <<'EOF' > Makefile
TASKS = $(wildcard reharvest_*/run*.sh)
OUT = $(TASKS:.sh=.root)

.PHONY: all
.SUFFIXES:

all: $(OUT)
	echo 'Done. Per-run root files are in reharvest_*/run*.root."

%.root: %.sh
	bash $<
EOF

echo 'Run `make -j<n>` to run parallel harvesting jobs.'






