#include <string>
#include <vector>
#include <TGraph.h>
#include <TF1.h>
#include <TFile.h>

class SpaceCharge
{

 public:

    SpaceCharge(std::string filename,
                const int initialSpatialPolN[3], const int intermediateSpatialPolN[3], const int initialEFieldPolN[3], const int intermediateEFieldPolN[3],
                const std::string eName);
    ~SpaceCharge();

    bool Configure(std::string filename);
    std::vector<double> GetPosOffsets(double xVal, double yVal, double zVal) const;
    std::vector<double> GetEfieldOffsets(double xVal, double yVal, double zVal) const;

 protected:
    int initialSpatialFitPolN[3], intermediateSpatialFitPolN[3];
    int initialEFieldFitPolN[3], intermediateEFieldFitPolN[3];
    std::string ExperimentName;//MicroBooNE, ProtoDUNE, SBND

    std::vector<double> GetPosOffsetsParametric(double xVal, double yVal, double zVal) const;
    double GetOnePosOffsetParametric(double xVal, double yVal, double zVal, std::string axis) const;
    std::vector<double> GetEfieldOffsetsParametric(double xVal, double yVal, double zVal) const;
    double GetOneEfieldOffsetParametric(double xVal, double yVal, double zVal, std::string axis) const;
    double TransformX(double xVal) const;
    double TransformY(double yVal) const;
    double TransformZ(double zVal) const;
    bool IsInsideBoundaries(double xVal, double yVal, double zVal) const;

    bool fEnableSimSpatialSCE;
    bool fEnableSimEfieldSCE;
    bool fEnableCorrSCE;

    TGraph *gSpatialGraphX[99][99];
    TF1 *intermediateSpatialFitFunctionX[99];
    TF1 *initialSpatialFitFunctionX;
    TGraph *gSpatialGraphY[99][99];
    TF1 *intermediateSpatialFitFunctionY[99];
    TF1 *initialSpatialFitFunctionY;
    TGraph *gSpatialGraphZ[99][99];
    TF1 *intermediateSpatialFitFunctionZ[99];
    TF1 *initialSpatialFitFunctionZ;

    TGraph *gEFieldGraphX[99][99];
    TF1 *intermediateEFieldFitFunctionX[99];
    TF1 *initialEFieldFitFunctionX;
    TGraph *gEFieldGraphY[99][99];
    TF1 *intermediateEFieldFitFunctionY[99];
    TF1 *initialEFieldFitFunctionY;
    TGraph *gEFieldGraphZ[99][99];
    TF1 *intermediateEFieldFitFunctionZ[99];
    TF1 *initialEFieldFitFunctionZ;
}; // class SpaceCharge
