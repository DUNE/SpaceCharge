rm -rf CompareOffsets
g++ -Wall -std=c++11 CompareOffsets.C ../../SourceCode/SpaceCharge.C ../../OldFiles/ProtoDUNE/SpaceChargeProtoDUNE.C ../../OldFiles/MicroBooNE/SpaceChargeMicroBooNE.C `root-config --cflags --glibs` -o CompareOffsets;
./CompareOffsets
rm -rf CompareOffsets
