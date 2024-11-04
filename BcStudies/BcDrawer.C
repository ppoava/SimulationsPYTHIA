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