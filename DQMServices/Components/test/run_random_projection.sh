DATASET=/ZeroBias/Run2018C-12Nov2019_UL2018-v2/DQMIO
WD=reharvest/
mkdir -p $WD
cp DQMServices/Components/test/random_projection_cfg.py $WD
cd $WD

for f in $(dasgoclient -query "file dataset=$DATASET"); do
  file=$(basename $f)
  echo "set -e" > $file.sh
  echo "xrdcp 'root://cms-xrd-global.cern.ch/$f' ." >> $file.sh
  echo "cmsRun random_projection_cfg.py file://$file > $file.dump" >> $file.sh
  echo "rm $file" >> $file.sh
  echo "mv $file.dump $file.out" >> $file.sh
done

cat <<'EOF' > Makefile
TASKS = $(wildcard *.sh)
OUT = $(TASKS:.sh=.out)

.PHONY: all
.SUFFIXES:

all: $(OUT)
	cat *.out > out.csv
	echo 'Done. Results are in out.csv.'

%.out: %.sh
	bash $<
EOF

echo "Prepared job in $WD, run using 'cd $WD; make -j16'."

