rm -rf CalculateSCE
g++ -Wall -std=c++11 CalculateSCE.C ../src/SpaceCharge.C ../TestFiles/SpaceChargeMicroBooNE.C `root-config --cflags --glibs` -o CalculateSCE;
./CalculateSCE

