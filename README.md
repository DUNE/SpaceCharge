# Parameterization of Space Charge Effect (SCE)

### Space Charge Effect
The space charge effect is the build-up of slow-moving positive ions in a detector due to, for instance, ionization from cosmic rays, leading to a distortion of the electric field within the detector. This effect leads to a displacement in the reconstructed position of signal ionization electrons in LArTPC (Liquid Argon Time Projection Chamber) detectors.

### Source 
MapSCE::PerformTransformation performs the transformation. Field to transform can be Spatial or E-Field and dimension to transform can be X, Y, or Z. Produced histograms are saved in HistoDirectory and ROOT output files in OutputFiles directory.
All the parameters are set in MakeMapSCE.C and 
```
./WorkArea/MakeMap/runMakeMapSCE.sh 
```
runs the the code to produce the SCE offsets. 

SpaceCharge is used for accessing the SCE. Currently, there is an option to choose between MicroBooNE and ProtoDUNE experiments. WorkArea/CompareOffsets/CompareOffsets.C accesses the SCE and 
```
./WorkArea/CompareOffsets/runCompareOffsets.sh
```
runs it.

### Extra
OldFiles/MicroBooNE and OldFiles/ProtoDUNE contains previously produced ROOT offset files and codes to access them.
