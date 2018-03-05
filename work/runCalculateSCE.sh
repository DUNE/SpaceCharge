rm -rf CalculateSCE
g++ -Wall -std=c++11 CalculateSCE.C ../src/SpaceChargeExperiment.C `root-config --cflags --glibs` -o CalculateSCE;
./CalculateSCE

