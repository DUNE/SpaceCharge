#ifndef MAP_SCE_H
#define MAP_SCE_H

#include <TPaveText.h>

using namespace std;

class MapSCE
{
 private :

    TString InputFile;
    TString OutputDirectory;
    string HistoDirectory;
    double DriftField;
    double BinningFactor;

 public :

    // With default values
    MapSCE(const TString input = "NONE", const TString output = "NONE", const string histo = "NONE", const double drift = 0.0, const double bin = 1.0);

    // To set values of choice
    void SetInputFile(const TString input);
    void SetOutputDirectory(const TString output);
    void SetHistoDirectory(const string histo);
    void SetDriftField(const double drift);
    void SetBinningFactor(const double bin);

    // To perform transformation
    void PerformTransformation(string FieldToTransform, string DimensionToTransform, int rawFitPolN, int intermediateFitPolN);

    // To print fit info in histogram
    TPaveText* fitInfo()
    {
        TPaveText *text = new TPaveText(0.9, 0.65, 1.0, 1.0, "brNDC");
        text->SetTextSize(0.025);
        text->SetBorderSize(0);
        text->SetTextFont(42);
        text->SetFillColor(10);
        text->SetTextColor(1);
        text->SetTextAlign(12);
        return text;
    }
};

#endif
