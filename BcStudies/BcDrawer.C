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
    TH1D* hDMass = (TH1D*)rootFile->Get("hDMass");
    TH1D* hPtNeutrinoSig = (TH1D*)rootFile->Get("hPtNeutrinoSig");
    TH1D* hPtNeutrinoBkg = (TH1D*)rootFile->Get("hPtNeutrinoBkg");
    TH1D* hEtaNeutrinoSig = (TH1D*)rootFile->Get("hEtaNeutrinoSig");
    TH1D* hEtaNeutrinoBkg = (TH1D*)rootFile->Get("hEtaNeutrinoBkg");
    TH1D* hPhiNeutrinoSig = (TH1D*)rootFile->Get("hPhiNeutrinoSig");
    TH1D* hPhiNeutrinoBkg = (TH1D*)rootFile->Get("hPhiNeutrinoBkg");
    TH1D* hPhiNeutrinoBarBkg = (TH1D*)rootFile->Get("hPhiNeutrinoBarBkg");

    /*
    TH1D*  = (TH1D*)rootFile->Get("");
    */

    hPtNeutrinoSig->Scale(1/hPtNeutrinoSig->Integral());
    hPtNeutrinoBkg->Scale(1/hPtNeutrinoBkg->Integral());
    hEtaNeutrinoSig->Scale(1/hEtaNeutrinoSig->Integral());
    hEtaNeutrinoBkg->Scale(1/hEtaNeutrinoBkg->Integral());
    hPhiNeutrinoSig->Scale(1/hPhiNeutrinoSig->Integral());
    hPhiNeutrinoBkg->Scale(1/hPhiNeutrinoBkg->Integral());
    hPhiNeutrinoBarBkg->Scale(1/hPhiNeutrinoBarBkg->Integral());

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

    TCanvas* cInvMass = new TCanvas("cInvMass", "Compare invariant mass with and w/o neutrino", 1200, 1000);
    setCanvasMargins(cInvMass, 0.20, 0.1, 0.1, 0.05);
    hInvMassWithoutNeutrino->SetLineColor(kRed);
    hInvMassWithoutNeutrino->GetYaxis()->SetRangeUser(0, 45e3);
    hInvMassWithoutNeutrino->GetXaxis()->SetTitle("mass [GeV]");

    TLegend* lInvMass = new TLegend(0.30, 0.6, 0.50, 0.75);
    lInvMass->SetTextSize(0.05);
    lInvMass->SetBorderSize(0);
    lInvMass->SetMargin(0.5);
    lInvMass->AddEntry(hInvMass, "3 muons + neutrino", "l");
    lInvMass->AddEntry(hInvMassWithoutNeutrino, "3 muons", "l");

    cInvMass->cd();
    hInvMassWithoutNeutrino->Draw("hist E");
    hInvMass->Draw("same hist E");
    lInvMass->Draw();

    // Mass difference
    TCanvas* cDMass = new TCanvas("cDMass", "#Delta mass effect of excluding neutrino in reconstruction", 1200, 1000);
    setCanvasMargins(cDMass, 0.20, 0.1, 0.1, 0.05);
    hDMass->GetXaxis()->SetRangeUser(0, 3.2);
    hDMass->GetXaxis()->SetTitle("#Delta mass [GeV]");

    cDMass->cd();
    hDMass->Draw("hist E");
    
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

    TCanvas* cPtNeutrino = new TCanvas("cPtNeutrino", "Compare signal and background neutrinos", 1200, 1000);
    setCanvasMargins(cPtNeutrino, 0.20, 0.1, 0.1, 0.05);
    hPtNeutrinoSig->SetLineColor(kRed);
    hPtNeutrinoBkg->SetLineColor(kBlack);
    hPtNeutrinoBkg->GetYaxis()->SetTitle("self-normalised counts");
    hPtNeutrinoBkg->GetXaxis()->SetTitle("mass [GeV]");

    TLegend* lPtNeutrino = new TLegend(0.30, 0.6, 0.50, 0.75);
    lPtNeutrino->SetTextSize(0.05);
    lPtNeutrino->SetBorderSize(0);
    lPtNeutrino->SetMargin(0.5);
    lPtNeutrino->AddEntry(hPtNeutrinoSig, "signal", "l");
    lPtNeutrino->AddEntry(hPtNeutrinoBkg, "background", "l");

    cPtNeutrino->cd();
    hPtNeutrinoBkg->Draw("hist E");
    hPtNeutrinoSig->Draw("same hist E");
    lPtNeutrino->Draw();

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

    TCanvas* cEtaNeutrino = new TCanvas("cEtaNeutrino", "Compare signal and background neutrinos", 1200, 1000);
    setCanvasMargins(cEtaNeutrino, 0.20, 0.1, 0.1, 0.05);
    hEtaNeutrinoSig->SetLineColor(kRed);
    hEtaNeutrinoBkg->SetLineColor(kBlack);
    hEtaNeutrinoSig->GetYaxis()->SetTitle("self-normalised counts");
    hEtaNeutrinoSig->GetXaxis()->SetTitle("eta");

    TLegend* lEtaNeutrino = new TLegend(0.30, 0.6, 0.50, 0.75);
    lEtaNeutrino->SetTextSize(0.05);
    lEtaNeutrino->SetBorderSize(0);
    lEtaNeutrino->SetMargin(0.5);
    lEtaNeutrino->AddEntry(hEtaNeutrinoSig, "signal", "l");
    lEtaNeutrino->AddEntry(hEtaNeutrinoBkg, "background", "l");

    cEtaNeutrino->cd();
    hEtaNeutrinoSig->Draw("hist E");
    hEtaNeutrinoBkg->Draw("same hist E");
    lEtaNeutrino->Draw();

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

    TCanvas* cPhiNeutrino = new TCanvas("cPhiNeutrino", "Compare signal and background neutrinos", 1200, 1000);
    setCanvasMargins(cPhiNeutrino, 0.20, 0.1, 0.1, 0.05);
    hPhiNeutrinoSig->SetLineColor(kRed);
    hPhiNeutrinoBkg->SetLineColor(kBlack);
    hPhiNeutrinoSig->GetYaxis()->SetTitle("self-normalised counts");
    hPhiNeutrinoSig->GetXaxis()->SetTitle("phi");

    TLegend* lPhiNeutrino = new TLegend(0.30, 0.6, 0.50, 0.75);
    lPhiNeutrino->SetTextSize(0.05);
    lPhiNeutrino->SetBorderSize(0);
    lPhiNeutrino->SetMargin(0.5);
    lPhiNeutrino->AddEntry(hPhiNeutrinoSig, "signal", "l");
    lPhiNeutrino->AddEntry(hPhiNeutrinoBkg, "background", "l");

    cPhiNeutrino->cd();
    hPhiNeutrinoSig->Draw("hist E");
    hPhiNeutrinoBkg->Draw("same hist E");
    lPhiNeutrino->Draw();

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

    TCanvas* cPhiNeutrinoNeutrinoBar = new TCanvas("cPhiNeutrinoNeutrinoBar", "Compare neutrino and anti-neutrino", 1200, 1000);
    setCanvasMargins(cPhiNeutrinoNeutrinoBar, 0.20, 0.1, 0.1, 0.05);
    hPhiNeutrinoBkg->SetLineColor(kRed);
    hPhiNeutrinoBarBkg->SetLineColor(kBlack);
    hPhiNeutrinoBarBkg->GetYaxis()->SetTitle("self-normalised counts");
    hPhiNeutrinoBarBkg->GetXaxis()->SetTitle("phi");

    TLegend* lPhiNeutrinoNeutrinoBar = new TLegend(0.30, 0.6, 0.50, 0.75);
    lPhiNeutrinoNeutrinoBar->SetTextSize(0.05);
    lPhiNeutrinoNeutrinoBar->SetBorderSize(0);
    lPhiNeutrinoNeutrinoBar->SetMargin(0.5);
    lPhiNeutrinoNeutrinoBar->AddEntry(hPhiNeutrinoBkg, "neutrino", "l");
    lPhiNeutrinoNeutrinoBar->AddEntry(hPhiNeutrinoBarBkg, "anti-neutrino", "l");

    cPhiNeutrinoNeutrinoBar->cd();
    hPhiNeutrinoBarBkg->Draw("hist E");
    hPhiNeutrinoBkg->Draw("same hist E");
    lPhiNeutrinoNeutrinoBar->Draw();

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

}

int BcDrawer() {
    drawCanvas();
    return 0;
}