// BcDrawer.C
// C++ libraries
#include <iostream>
#include <vector>
// ROOT libraries
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH1D.h"
#include "TTree.h"
#include "TChain.h"
#include "TString.h"
#include "TLorentzVector.h"

using namespace std;
using namespace TMath;

void setCanvasMargins(TCanvas *canvas, double_t LeftMargin, double_t RightMargin, double_t TopMargin, double_t BottomMargin) {
  canvas->SetLeftMargin(LeftMargin);
  canvas->SetRightMargin(RightMargin);
  canvas->SetTopMargin(TopMargin);
  canvas->SetBottomMargin(BottomMargin);
}



    

void drawCanvas() {

    std::string path = "/Users/pv280546/alice/SimulationsPYTHIA/BcStudies/";

    TFile* rootFile = new TFile((path + "analysed_output_31_OKT_2024.root").c_str());
    TH1D* hInvMass = (TH1D*)rootFile->Get("hInvMass");
    TH1D* hInvMassWithoutNeutrino = (TH1D*)rootFile->Get("hInvMassWithoutNeutrino");
    
    // hBpBm_pT1->Scale(1/hBpBmTrPt_pT1->Integral());

    TCanvas* cInvMass = new TCanvas("cInvMass", "Compare invariant mass with and w/o neutrino", 1200, 1000);
    setCanvasMargins(cInvMass, 0.20, 0.1, 0.1, 0.05);
    hInvMassWithoutNeutrino->SetLineColor(kRed);
    hInvMassWithoutNeutrino->GetYaxis()->SetRangeUser(0, 45e3);

    cInvMass->cd();
    hInvMassWithoutNeutrino->Draw("hist E");
    hInvMass->Draw("same hist E");
    
    // -------------------------------------------------------------------- //
    // -------------------------------------------------------------------- //
    // -------------------------------------------------------------------- //
    // -------------------------------------------------------------------- //
    // -------------------------------------------------------------------- //
    // -------------------------------------------------------------------- //
    // -------------------------------------------------------------------- //
    // -------------------------------------------------------------------- //
    // -------------------------------------------------------------------- //
    // -------------------------------------------------------------------- //

    TCanvas* cDMass = new TCanvas("cDMass", "#Delta mass effect of excluding neutrino in reconstruction", 1200, 1000);
    setCanvasMargins(cDMass, 0.20, 0.1, 0.1, 0.05);

    // Create a new histogram to store delta mass values
    int nEntries = hInvMass->GetEntries(); // Assuming hist1 and hist2 have the same binning
    std::cout<<nEntries<<std::endl;
    TH1F *hDMass = new TH1F("hDMass", "#Delta mass effect of excluding neutrino in reconstruction;Counts", nEntries, -10, 10);
    
    // Loop over the bins and calculate the delta mass
    for (int i = 1; i <= nEntries; ++i) {
        double mass1 = hInvMass->GetBinContent(i); // Get the center of the bin for hist1
        double mass2 = hInvMassWithoutNeutrino->GetBinContent(i); // Get the center of the bin for hist2
        std::cout<<mass2<<std::endl;
        double deltaMass = mass1 - mass2;       // Calculate delta mass
        std::cout<<deltaMass<<std::endl;

        // Fill the delta histogram with the counts
        double count1 = hInvMass->GetBinContent(i); // Count from hist1
        double count2 = hInvMassWithoutNeutrino->GetBinContent(i); // Count from hist2

        hDMass->Fill(deltaMass); // Fill deltaHist with the difference of counts
    }
    hDMass->GetXaxis()->SetRangeUser(-1,1);

    cDMass->cd();
    hDMass->Draw("hist E");

    /*
    TLegend* lhists_DPhi_pTHatMin = new TLegend(0.30, 0.6, 0.50, 0.75);
    lhists_DPhi_pTHatMin->SetTextSize(0.05);
    lhists_DPhi_pTHatMin->SetBorderSize(0);
    // lhists_DPhi_pTHatMin->SetLineWidth(2);
    lhists_DPhi_pTHatMin->SetMargin(0.5);
    lhists_DPhi_pTHatMin->AddEntry(hBpBm, "B^{+}B^{-}", "l");
    */

    // cInvMass->cd(); l00_DPhi_pTHatMin->Draw();
}

int BcDrawer() {
    drawCanvas();
    return 0;
}