#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TRatioPlot.h>
#include <iostream>
#include <string>

using namespace std;

void setHisto(TH1 *histo, TString xAxis, TString yAxis)
{
    histo->GetXaxis()->SetTitle(xAxis);
    histo->GetXaxis()->SetTitleSize(0.065);
    histo->GetXaxis()->SetTitleOffset(0.75);
    histo->GetXaxis()->SetTitleFont(62);
    histo->GetXaxis()->SetTickLength(0.04);
    histo->GetXaxis()->SetLabelFont(62);
    histo->GetXaxis()->SetLabelSize(0.045);

    histo->GetYaxis()->SetTitle(yAxis);
    histo->GetYaxis()->SetTitleSize(0.06);
    histo->GetYaxis()->SetTitleOffset(0.85);
    histo->GetYaxis()->SetTitleFont(62);
    histo->GetYaxis()->SetTickLength(0.04);
    histo->GetYaxis()->SetLabelFont(62);
    histo->GetYaxis()->SetLabelSize(0.045);

    histo->SetMinimum(1.0);
    histo->SetTitle("");
}

void PlotOffsetsComparision()
{
    gStyle->SetOptStat(1110);
    gStyle->SetTitleFontSize(0.1);

    string experiment[2] = {"ProtoDUNE", "MicroBooNE"};

    TFile *inputFile = new TFile();
    for(unsigned int i = 0; i <= 1; i++)
        {
            if(i == 0)
                {
                    inputFile = new TFile("../../OutputFiles/OffsetsComparision_ProtoDUNE.root", "r");
                }
            else if(i == 1)
                {
                    inputFile = new TFile("../../OutputFiles/OffsetsComparision_MicroBooNE.root", "r");
                }

            TCanvas *cComparisionD = new TCanvas(Form("cComparisionD_%u", i), "", 1500, 1500);
            cComparisionD->Divide(3, 3);
            TH1D *iDx = (TH1D*)inputFile->Get("iDx");
            setHisto(iDx, "dDx (m)", "Count");
            iDx->SetTitle("Input");
            TH1D *mDx = (TH1D*)inputFile->Get("mDx");
            setHisto(mDx, "dDx (cm)", "");
            mDx->SetTitle("New");
            TH1D *oDx = (TH1D*)inputFile->Get("oDx");
            setHisto(oDx, "dDx (cm)", "");
            oDx->SetTitle("Mike's");

            cComparisionD->cd(1);
            gPad->SetLogy();
            iDx->Draw();
            cComparisionD->cd(2);
            gPad->SetLogy();
            mDx->Draw();
            cComparisionD->cd(3);
            gPad->SetLogy();
            oDx->Draw();

            TH1D *iDy = (TH1D*)inputFile->Get("iDy");
            setHisto(iDy, "dDy (m)", "Count");
            TH1D *mDy = (TH1D*)inputFile->Get("mDy");
            setHisto(mDy, "dDy (cm)", "");
            TH1D *oDy = (TH1D*)inputFile->Get("oDy");
            setHisto(oDy, "dDy (cm)", "");

            cComparisionD->cd(4);
            gPad->SetLogy();
            iDy->Draw();
            cComparisionD->cd(5);
            gPad->SetLogy();
            mDy->Draw();
            cComparisionD->cd(6);
            gPad->SetLogy();
            oDy->Draw();

            TH1D *iDz = (TH1D*)inputFile->Get("iDz");
            setHisto(iDz, "dDz (m)", "Count");
            TH1D *mDz = (TH1D*)inputFile->Get("mDz");
            setHisto(mDz, "dDz (cm)", "");
            TH1D *oDz = (TH1D*)inputFile->Get("oDz");
            setHisto(oDz, "dDz (cm)", "");

            cComparisionD->cd(7);
            gPad->SetLogy();
            iDz->Draw();
            cComparisionD->cd(8);
            gPad->SetLogy();
            mDz->Draw();
            cComparisionD->cd(9);
            gPad->SetLogy();
            oDz->Draw();
            cComparisionD->Update();

            TCanvas *cComparisionE = new TCanvas(Form("cComparisionE_%u", i), "", 1500, 1500);
            cComparisionE->Divide(3, 3);
            TH1D *iEx = (TH1D*)inputFile->Get("iEx");
            setHisto(iEx, "dEx (V/m)", "Count");
            iEx->SetTitle("Input");
            TH1D *mEx = (TH1D*)inputFile->Get("mEx");
            setHisto(mEx, "-dEx/E", "");
            mEx->SetTitle("New");
            TH1D *oEx = (TH1D*)inputFile->Get("oEx");
            setHisto(oEx, "-dEx/E", "");
            oEx->SetTitle("Mike's");

            cComparisionE->cd(1);
            gPad->SetLogy();
            iEx->Draw();
            cComparisionE->cd(2);
            gPad->SetLogy();
            mEx->Draw();
            cComparisionE->cd(3);
            gPad->SetLogy();
            oEx->Draw();

            TH1D *iEy = (TH1D*)inputFile->Get("iEy");
            setHisto(iEy, "dEy (V/m)", "Count");
            TH1D *mEy = (TH1D*)inputFile->Get("mEy");
            setHisto(mEy, "-dEy/E", "");
            TH1D *oEy = (TH1D*)inputFile->Get("oEy");
            setHisto(oEy, "-dEy/E", "");

            cComparisionE->cd(4);
            gPad->SetLogy();
            iEy->Draw();
            cComparisionE->cd(5);
            gPad->SetLogy();
            mEy->Draw();
            cComparisionE->cd(6);
            gPad->SetLogy();
            oEy->Draw();

            TH1D *iEz = (TH1D*)inputFile->Get("iEz");
            setHisto(iEz, "dEz (V/m)", "Count");
            TH1D *mEz = (TH1D*)inputFile->Get("mEz");
            setHisto(mEz, "-dEz/E", "");
            TH1D *oEz = (TH1D*)inputFile->Get("oEz");
            setHisto(oEz, "-dEz/E", "");

            cComparisionE->cd(7);
            gPad->SetLogy();
            iEz->Draw();
            cComparisionE->cd(8);
            gPad->SetLogy();
            mEz->Draw();
            cComparisionE->cd(9);
            gPad->SetLogy();
            oEz->Draw();
            cComparisionE->Update();

            if(i == 0)
                {
                    cComparisionD->SaveAs("ProtoDUNE_Spatial.png");
                    cComparisionE->SaveAs("ProtoDUNE_EField.png");
                }
            if(i == 1)
                {
                    cComparisionD->SaveAs("MicroBooNE_Spatial.png");
                    cComparisionE->SaveAs("MicroBooNE_EField.png");
                }
        }
}
