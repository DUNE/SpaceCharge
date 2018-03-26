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
    histo->GetYaxis()->SetTitleOffset(0.95);
    histo->GetYaxis()->SetTitleFont(62);
    histo->GetYaxis()->SetTickLength(0.04);
    histo->GetYaxis()->SetLabelFont(62);
    histo->GetYaxis()->SetLabelSize(0.045);

    histo->SetTitle("");
}

void plotOffsetsComparision()
{
    //gStyle->SetOptStat(0);
    gStyle->SetErrorX(0);
    gStyle->SetTitleFontSize(0.1);

    string experiment[2] = {"ProtoDUNE", "MicroBooNE"};

    TFile *inputFile = new TFile();
    for(unsigned int i = 0; i <= 1; i++)
        {
	    if(i==1) continue;
            if(i == 0)
                {
                    inputFile = new TFile("../work/ProtoDUNEOffsetsComparision.root", "r");
                }
            else if(i == 1)
                {
                    inputFile = new TFile("../work/MicroBooNEOffsetsComparision.root", "r");
                }

	    TCanvas *cComparisionD = new TCanvas(Form("cComparisionD_%u", i), "", 1500, 1500);
	    cComparisionD->Divide(3, 3);
	    TH1D *iDx = (TH1D*)inputFile->Get("iDx");
	    setHisto(iDx, "Dx", "Count");
	    iDx->SetTitle("Input");
	    TH1D *mDx = (TH1D*)inputFile->Get("mDx");
            setHisto(mDx, "Dx", "");
            mDx->SetTitle("New");
	    TH1D *oDx = (TH1D*)inputFile->Get("oDx");
            setHisto(oDx, "Dx", "");
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
            setHisto(iDy, "Dy", "Count");
            TH1D *mDy = (TH1D*)inputFile->Get("mDy");
            setHisto(mDy, "Dy", "");
            TH1D *oDy = (TH1D*)inputFile->Get("oDy");
            setHisto(oDy, "Dy", "");

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
            setHisto(iDz, "Dz", "Count");
            TH1D *mDz = (TH1D*)inputFile->Get("mDz");
            setHisto(mDz, "Dz", "");
            TH1D *oDz = (TH1D*)inputFile->Get("oDz");
            setHisto(oDz, "Dz", "");

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
	    setHisto(iEx, "Ex", "Count");
	    iEx->SetTitle("Input");
	    TH1D *mEx = (TH1D*)inputFile->Get("mEx");
            setHisto(mEx, "Ex", "");
            mEx->SetTitle("New");
	    TH1D *oEx = (TH1D*)inputFile->Get("oEx");
            setHisto(oEx, "Ex", "");
            oEx->SetTitle("Mike's");

	    cComparisionE->cd(1);
	    iEx->Draw();
	    cComparisionE->cd(2);
            mEx->Draw();
	    cComparisionE->cd(3);
            oEx->Draw();

	    TH1D *iEy = (TH1D*)inputFile->Get("iEy");
            setHisto(iEy, "Ey", "Count");
            TH1D *mEy = (TH1D*)inputFile->Get("mEy");
            setHisto(mEy, "Ey", "");
            TH1D *oEy = (TH1D*)inputFile->Get("oEy");
            setHisto(oEy, "Ey", "");

            cComparisionE->cd(4);
            iEy->Draw();
            cComparisionE->cd(5);
            mEy->Draw();
            cComparisionE->cd(6);
            oEy->Draw();

	    TH1D *iEz = (TH1D*)inputFile->Get("iEz");
            setHisto(iEz, "Ez", "Count");
            TH1D *mEz = (TH1D*)inputFile->Get("mEz");
            setHisto(mEz, "Ez", "");
            TH1D *oEz = (TH1D*)inputFile->Get("oEz");
            setHisto(oEz, "Ez", "");

            cComparisionE->cd(7);
            iEz->Draw();
            cComparisionE->cd(8);
            mEz->Draw();
            cComparisionE->cd(9);
            oEz->Draw();
	    cComparisionE->Update();

	    if(i == 0)
                {
		    cComparisionD->SaveAs("ProtoDUNE_D.png");
		    //cComparisionE->SaveAs("ProtoDUNE_E.png");
		}
	    if(i==1){
		//cComparisionD->SaveAs("MicroBooNE_D.png");
		//cComparisionE->SaveAs("MicroBooNE_E.png");
	    }
        }
}
