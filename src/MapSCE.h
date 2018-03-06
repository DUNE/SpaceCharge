#ifndef MAP_SCE_H
#define MAP_SCE_H

using namespace std;

class MapSCE
{
 private :

    TString InputFile;
    TString OutputDirectory;
    string HistoDirectory;
    double DriftField;

 public :

    // With default values
    MapSCE(const TString input = "NONE", const TString output = "NONE", const string histo = "NONE", const double drift = 0.0);

    // To set values of choice
    void SetInputFile(const TString input);
    void SetOutputDirectory(const TString output);
    void SetHistoDirectory(const string histo);
    void SetDriftField(const double drift);

    // To perform transformation
    void PerformTransformation(string FieldToTransform, string DimensionToTransform, int rawFitPolN, int intermediateFitPolN);
};

#endif
