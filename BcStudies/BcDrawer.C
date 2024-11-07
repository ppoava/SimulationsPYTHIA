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
    TH1D* hPtSoloMuonSig = (TH1D*)rootFile->Get("hPtSoloMuonSig");
    TH1D* hPtSoloMuonBkg = (TH1D*)rootFile->Get("hPtSoloMuonBkg");
    TH1D* hEtaNeutrinoSig = (TH1D*)rootFile->Get("hEtaNeutrinoSig");
    TH1D* hEtaNeutrinoBkg = (TH1D*)rootFile->Get("hEtaNeutrinoBkg");
    TH1D* hPhiNeutrinoSig = (TH1D*)rootFile->Get("hPhiNeutrinoSig");
    TH1D* hPhiNeutrinoBkg = (TH1D*)rootFile->Get("hPhiNeutrinoBkg");
    TH1D* hEtaSoloMuonSig = (TH1D*)rootFile->Get("hEtaSoloMuonSig");
    TH1D* hEtaSoloMuonBkg = (TH1D*)rootFile->Get("hEtaSoloMuonBkg");
    TH1D* hPhiSoloMuonSig = (TH1D*)rootFile->Get("hPhiSoloMuonSig");
    TH1D* hPhiSoloMuonBkg = (TH1D*)rootFile->Get("hPhiSoloMuonBkg");
    TH1D* hPhiNeutrinoBarBkg = (TH1D*)rootFile->Get("hPhiNeutrinoBarBkg");
    TH1D* hDPhiDiMuons = (TH1D*)rootFile->Get("hDPhiDiMuons");
    TH1D* hDPhiDiMuonsBkg = (TH1D*)rootFile->Get("hDPhiDiMuonsBkg");
    TH1D* hDEtaDiMuons = (TH1D*)rootFile->Get("hDEtaDiMuons");
    TH1D* hDEtaDiMuonsBkg = (TH1D*)rootFile->Get("hDEtaDiMuonsBkg");
    TH1D* hDPhiSoloMuonBarDiMuons = (TH1D*)rootFile->Get("hDPhiSoloMuonBarDiMuons");
    TH1D* hDPhiSoloMuonBarDiMuonsBkg = (TH1D*)rootFile->Get("hDPhiSoloMuonBarDiMuonsBkg");
    TH1D* hDEtaSoloMuonBarDiMuons = (TH1D*)rootFile->Get("hDEtaSoloMuonBarDiMuons");
    TH1D* hDEtaSoloMuonBarDiMuonsBkg = (TH1D*)rootFile->Get("hDEtaSoloMuonBarDiMuonsBkg");

    /*
    TH1D*  = (TH1D*)rootFile->Get("");
    */

    hPtNeutrinoSig->Scale(1/hPtNeutrinoSig->Integral());
    hPtNeutrinoBkg->Scale(1/hPtNeutrinoBkg->Integral());
    hPtSoloMuonSig->Scale(1/hPtSoloMuonSig->Integral());
    hPtSoloMuonBkg->Scale(1/hPtSoloMuonBkg->Integral());
    hEtaNeutrinoSig->Scale(1/hEtaNeutrinoSig->Integral());
    hEtaNeutrinoBkg->Scale(1/hEtaNeutrinoBkg->Integral());
    hPhiNeutrinoSig->Scale(1/hPhiNeutrinoSig->Integral());
    hPhiNeutrinoBkg->Scale(1/hPhiNeutrinoBkg->Integral());
    hEtaSoloMuonSig->Scale(1/hEtaSoloMuonSig->Integral());
    hEtaSoloMuonBkg->Scale(1/hEtaSoloMuonBkg->Integral());
    hPhiSoloMuonSig->Scale(1/hPhiSoloMuonSig->Integral());
    hPhiSoloMuonBkg->Scale(1/hPhiSoloMuonBkg->Integral());
    hPhiNeutrinoBarBkg->Scale(1/hPhiNeutrinoBarBkg->Integral());
    hDPhiDiMuons->Scale(1/hDPhiDiMuons->Integral());
    hDPhiDiMuonsBkg->Scale(1/hDPhiDiMuonsBkg->Integral());
    hDEtaDiMuons->Scale(1/hDEtaDiMuons->Integral());
    hDEtaDiMuonsBkg->Scale(1/hDEtaDiMuonsBkg->Integral());
    hDPhiSoloMuonBarDiMuons->Scale(1/hDPhiSoloMuonBarDiMuons->Integral());
    hDPhiSoloMuonBarDiMuonsBkg->Scale(1/hDPhiSoloMuonBarDiMuonsBkg->Integral());
    hDEtaSoloMuonBarDiMuons->Scale(1/hDEtaSoloMuonBarDiMuons->Integral());
    hDEtaSoloMuonBarDiMuonsBkg->Scale(1/hDEtaSoloMuonBarDiMuonsBkg->Integral());

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
    hPtNeutrinoBkg->GetXaxis()->SetTitle("pT [GeV]");

    TLegend* lPtNeutrino = new TLegend(0.30, 0.6, 0.50, 0.75);
    lPtNeutrino->SetTextSize(0.05);
    lPtNeutrino->SetBorderSize(0);
    lPtNeutrino->SetMargin(0.5);
    lPtNeutrino->AddEntry(hPtNeutrinoSig, "signal", "l");
    lPtNeutrino->AddEntry(hPtNeutrinoBkg, "background", "l");

    cPtNeutrino->cd();
    // gPad->SetLogy();
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

    TCanvas* cPtSoloMuon = new TCanvas("cPtSoloMuon", "Compare signal and background solo muons", 1200, 1000);
    setCanvasMargins(cPtSoloMuon, 0.20, 0.1, 0.1, 0.05);
    hPtSoloMuonSig->SetLineColor(kRed);
    hPtSoloMuonBkg->SetLineColor(kBlack);
    hPtSoloMuonBkg->GetYaxis()->SetTitle("self-normalised counts");
    hPtSoloMuonBkg->GetXaxis()->SetTitle("pT [GeV]");

    TLegend* lPtSoloMuon = new TLegend(0.30, 0.6, 0.50, 0.75);
    lPtSoloMuon->SetTextSize(0.05);
    lPtSoloMuon->SetBorderSize(0);
    lPtSoloMuon->SetMargin(0.5);
    lPtSoloMuon->AddEntry(hPtSoloMuonSig, "signal", "l");
    lPtSoloMuon->AddEntry(hPtSoloMuonBkg, "background", "l");

    cPtSoloMuon->cd();
    gPad->SetLogy();
    hPtSoloMuonBkg->Draw("hist E");
    hPtSoloMuonSig->Draw("same hist E");
    lPtSoloMuon->Draw();

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

    TCanvas* cEtaSoloMuon = new TCanvas("cEtaSoloMuon", "Compare signal and background solo muons", 1200, 1000);
    setCanvasMargins(cEtaSoloMuon, 0.20, 0.1, 0.1, 0.05);
    hEtaSoloMuonSig->SetLineColor(kRed);
    hEtaSoloMuonBkg->SetLineColor(kBlack);
    hEtaSoloMuonSig->GetYaxis()->SetTitle("self-normalised counts");
    hEtaSoloMuonSig->GetXaxis()->SetTitle("eta");

    TLegend* lEtaSoloMuon = new TLegend(0.30, 0.6, 0.50, 0.75);
    lEtaSoloMuon->SetTextSize(0.05);
    lEtaSoloMuon->SetBorderSize(0);
    lEtaSoloMuon->SetMargin(0.5);
    lEtaSoloMuon->AddEntry(hEtaSoloMuonSig, "signal", "l");
    lEtaSoloMuon->AddEntry(hEtaSoloMuonBkg, "background", "l");

    cEtaSoloMuon->cd();
    hEtaSoloMuonSig->Draw("hist E");
    hEtaSoloMuonBkg->Draw("same hist E");
    lEtaSoloMuon->Draw();

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

    TCanvas* cPhiSoloMuon = new TCanvas("cPhiSoloMuon", "Compare signal and background solo muons", 1200, 1000);
    setCanvasMargins(cPhiSoloMuon, 0.20, 0.1, 0.1, 0.05);
    hPhiSoloMuonSig->SetLineColor(kRed);
    hPhiSoloMuonBkg->SetLineColor(kBlack);
    hPhiSoloMuonSig->GetYaxis()->SetTitle("self-normalised counts");
    hPhiSoloMuonSig->GetXaxis()->SetTitle("phi");

    TLegend* lPhiSoloMuon = new TLegend(0.30, 0.6, 0.50, 0.75);
    lPhiSoloMuon->SetTextSize(0.05);
    lPhiSoloMuon->SetBorderSize(0);
    lPhiSoloMuon->SetMargin(0.5);
    lPhiSoloMuon->AddEntry(hPhiSoloMuonSig, "signal", "l");
    lPhiSoloMuon->AddEntry(hPhiSoloMuonBkg, "background", "l");

    cPhiSoloMuon->cd();
    hPhiSoloMuonSig->Draw("hist E");
    hPhiSoloMuonBkg->Draw("same hist E");
    lPhiSoloMuon->Draw();

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
    hPhiNeutrinoBkg->SetLineColor(kBlack);
    hPhiNeutrinoBarBkg->SetLineColor(kRed);
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

    TCanvas* cDPhiDiMuons = new TCanvas("cDPhiDiMuons", "Compare #Delta#phi dimuons signal and background", 1200, 1000);
    setCanvasMargins(cDPhiDiMuons, 0.20, 0.1, 0.1, 0.05);
    hDPhiDiMuons->SetLineColor(kRed);
    hDPhiDiMuonsBkg->SetLineColor(kBlack);
    hDPhiDiMuons->GetYaxis()->SetTitle("self-normalised counts");
    hDPhiDiMuons->GetXaxis()->SetTitle("#delta#phi");

    TLegend* lDPhiDiMuons = new TLegend(0.30, 0.6, 0.50, 0.75);
    lDPhiDiMuons->SetTextSize(0.05);
    lDPhiDiMuons->SetBorderSize(0);
    lDPhiDiMuons->SetMargin(0.5);
    lDPhiDiMuons->AddEntry(hDPhiDiMuons, "signal", "l");
    lDPhiDiMuons->AddEntry(hDPhiDiMuonsBkg, "background", "l");

    cDPhiDiMuons->cd();
    hDPhiDiMuons->Draw("hist E");
    hDPhiDiMuonsBkg->Draw("same hist E");
    lDPhiDiMuons->Draw();

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

    TCanvas* cDEtaDiMuons = new TCanvas("cDEtaDiMuons", "Compare #Delta#eta dimuons signal and background", 1200, 1000);
    setCanvasMargins(cDEtaDiMuons, 0.20, 0.1, 0.1, 0.05);
    hDEtaDiMuons->SetLineColor(kRed);
    hDEtaDiMuonsBkg->SetLineColor(kBlack);
    hDEtaDiMuons->GetYaxis()->SetTitle("self-normalised counts");
    hDEtaDiMuons->GetXaxis()->SetTitle("#delta#eta");

    TLegend* lDEtaDiMuons = new TLegend(0.30, 0.6, 0.50, 0.75);
    lDEtaDiMuons->SetTextSize(0.05);
    lDEtaDiMuons->SetBorderSize(0);
    lDEtaDiMuons->SetMargin(0.5);
    lDEtaDiMuons->AddEntry(hDEtaDiMuons, "signal", "l");
    lDEtaDiMuons->AddEntry(hDEtaDiMuonsBkg, "background", "l");

    cDEtaDiMuons->cd();
    hDEtaDiMuons->Draw("hist E");
    hDEtaDiMuonsBkg->Draw("same hist E");
    lDEtaDiMuons->Draw();

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

    TCanvas* cDPhiSoloMuonBarDiMuons = new TCanvas("cDPhiSoloMuonBarDiMuons", "Compare #Delta#phi (solo muon,dimuons) signal and background", 1200, 1000);
    setCanvasMargins(cDPhiSoloMuonBarDiMuons, 0.20, 0.1, 0.1, 0.05);
    hDPhiSoloMuonBarDiMuons->SetLineColor(kRed);
    hDPhiSoloMuonBarDiMuonsBkg->SetLineColor(kBlack);
    hDPhiSoloMuonBarDiMuons->GetYaxis()->SetTitle("self-normalised counts");
    hDPhiSoloMuonBarDiMuons->GetXaxis()->SetTitle("#delta#phi");

    TLegend* lDPhiSoloMuonBarDiMuons = new TLegend(0.30, 0.6, 0.50, 0.75);
    lDPhiSoloMuonBarDiMuons->SetTextSize(0.05);
    lDPhiSoloMuonBarDiMuons->SetBorderSize(0);
    lDPhiSoloMuonBarDiMuons->SetMargin(0.5);
    lDPhiSoloMuonBarDiMuons->AddEntry(hDPhiSoloMuonBarDiMuons, "signal", "l");
    lDPhiSoloMuonBarDiMuons->AddEntry(hDPhiSoloMuonBarDiMuonsBkg, "background", "l");

    cDPhiSoloMuonBarDiMuons->cd();
    hDPhiSoloMuonBarDiMuons->Draw("hist E");
    hDPhiSoloMuonBarDiMuonsBkg->Draw("same hist E");
    lDPhiSoloMuonBarDiMuons->Draw();

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

    TCanvas* cDEtaSoloMuonBarDiMuons = new TCanvas("cDEtaSoloMuonBarDiMuons", "Compare #Delta#eta (solo muon,dimuons) signal and background", 1200, 1000);
    setCanvasMargins(cDEtaSoloMuonBarDiMuons, 0.20, 0.1, 0.1, 0.05);
    hDEtaSoloMuonBarDiMuons->SetLineColor(kRed);
    hDEtaSoloMuonBarDiMuonsBkg->SetLineColor(kBlack);
    hDEtaSoloMuonBarDiMuons->GetYaxis()->SetTitle("self-normalised counts");
    hDEtaSoloMuonBarDiMuons->GetXaxis()->SetTitle("#delta#eta");

    TLegend* lDEtaSoloMuonBarDiMuons = new TLegend(0.30, 0.6, 0.50, 0.75);
    lDEtaSoloMuonBarDiMuons->SetTextSize(0.05);
    lDEtaSoloMuonBarDiMuons->SetBorderSize(0);
    lDEtaSoloMuonBarDiMuons->SetMargin(0.5);
    lDEtaSoloMuonBarDiMuons->AddEntry(hDEtaSoloMuonBarDiMuons, "signal", "l");
    lDEtaSoloMuonBarDiMuons->AddEntry(hDEtaSoloMuonBarDiMuonsBkg, "background", "l");

    cDEtaSoloMuonBarDiMuons->cd();
    hDEtaSoloMuonBarDiMuons->Draw("hist E");
    hDEtaSoloMuonBarDiMuonsBkg->Draw("same hist E");
    lDEtaSoloMuonBarDiMuons->Draw();

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

    // add pT comparison solo muon signal + background

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