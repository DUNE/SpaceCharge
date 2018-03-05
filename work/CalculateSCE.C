#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "math.h"
#include "stdio.h"

#include "../src/SpaceChargeExperiment.h"

using namespace std;

int main() {

  const double Edrift = 0.500; // in kV/cm
  const double dEdx = 2.1; // in MeV/cm
  const double ModBoxA = 0.930; // recomb constant A used in LArG4
  const double ModBoxB = 0.212; // recomb constant B used in LArG4

  //SpaceChargeMicroBooNE SCE; // for 273 V/cm by default
  //SpaceChargeExperiment SCE("../TestFiles/Mike_SCEoffsets_MicroBooNE_E500.root"); 
  SpaceChargeExperiment SCE("../OutputFiles/SCEoffsets_MicroBooNE_E500.root"); 
  // if you want to try different E fields (shown for 500 V/cm), though must change Edrfit constant above to match!

  ////////////////////
  //  Example to obtain spatial offsets
  //////////
    vector<double> mySoffsets = SCE.GetPosOffsets(170.0,0.0,518.0); // returns {dX, dY, dZ} at given point (example here: middle of TPC volume)
    cout << "Spatial Offsets at TPC Center:  " << -1.0*mySoffsets.at(0) << " " << mySoffsets.at(1) << " " << mySoffsets.at(2) << endl; // values in cm (note, MUST use -1.0 factor for dx offset)
  ////////////////////


  ////////////////////
  //  Example to obtain E field offsets
  //////////
    vector<double> myEoffsets = SCE.GetEfieldOffsets(256.0,0.0,518.0); // returns {dEx/|E_drift|, dEy/|E_drift|, dEz/|E_drift|} at given point (example here: middle of cathode)
    cout << "E Field Offsets at Cathode:  " << 1000.0*Edrift*myEoffsets.at(0) << " " << 1000.0*Edrift*myEoffsets.at(1) << " " << 1000.0*Edrift*myEoffsets.at(2) << endl; // values in V/cm
    //cout << "E Field Offsets at Cathode:  " << myEoffsets.at(0) << " " << myEoffsets.at(1) << " " << myEoffsets.at(2) << endl; // values in V/cm
  ////////////////////


  ////////////////////
  // Example to calculate impact of E field distortion on recombination level
  //////////
    double EfieldMag = sqrt(pow(Edrift*(1.0+myEoffsets.at(0)),2.0) + pow(Edrift*myEoffsets.at(1),2.0) + pow(Edrift*myEoffsets.at(2),2.0));
    
    double Xi = (ModBoxB * dEdx) / EfieldMag;
    double recomb = log(ModBoxA + Xi) / Xi;
    
    cout << "Recombination Factor at Cathode with SCE:  " << recomb << endl;
    
    double Xi_nominal = (ModBoxB * dEdx) / Edrift;
    double recomb_nominal = log(ModBoxA + Xi_nominal) / Xi_nominal;
    
    cout << "Recombination Factor w/o SCE (entire TPC):  " << recomb_nominal << endl;
  ////////////////////


  return 0;
}
