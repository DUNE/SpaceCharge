#ifndef MAP_SCE_H
#define MAP_SCE_H

using namespace std;

class MapSCE
{
 private :

    TString InputFile;
    TString OutputDirectory;
    string HistoDirectory;

 public :

    // With default values
    MapSCE(const TString input = "NONE", const TString output = "NONE", const string histo = "NONE");

    // To set values of choice
    void SetInputFile(const TString input);
    void SetOutputDirectory(const TString output);
    void SetHistoDirectory(const string histo);

    // To perform transformation
    void PerformTransformation(string FieldToTransform, string DimensionToTransform, int rawFitPolN, int intermediateFitPolN);
};

#endif
