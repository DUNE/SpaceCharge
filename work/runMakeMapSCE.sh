rm -rf MapSCE
rm -rf ../HistoDirectory/*.pdf
rm -rf ../OutputFiles/*
g++ -Wall -std=c++11 MakeMapSCE.C ../src/MapSCE.C `root-config --cflags --glibs` -o MapSCE;
./MapSCE

