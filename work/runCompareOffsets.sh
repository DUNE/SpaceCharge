rm -rf CompareOffsets
g++ -Wall -std=c++11 CompareOffsets.C ../src/SpaceCharge.C ../TestFiles/SpaceChargeProtoDUNE.C ../TestFiles/SpaceChargeMicroBooNE.C `root-config --cflags --glibs` -o CompareOffsets;
./CompareOffsets

