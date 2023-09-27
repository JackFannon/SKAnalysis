#include "evPlot.h"

#include "TROOT.h"
#include "sktqC.h"

evPlot::evPlot() : Tool() {}

bool evPlot::Initialise(std::string configfile, DataModel &data) {

  gROOT->SetBatch(1);

  if (configfile != "")
    m_variables.Initialise(configfile);
  // m_variables.Print();

  m_data = &data;
  m_log = m_data->Log;

  // Get the config variables: TreeReader, which plots to create, output
  // directory, etc.
  m_variables.Get("treeReaderName", treeReaderName);
  m_variables.Get("outputDir", outputDir);

  // Get the TreeReader
  if (m_data->Trees.count(treeReaderName) == 0) {
    Log("ERROR: TreeReader " + treeReaderName + " not found!", v_error,
        verbosity);
    return false;
  }

  myTreeReader = m_data->Trees.at(treeReaderName);

  if (!m_variables.Get("verbose", m_verbose))
    m_verbose = 1;

  // Initialise the histograms
  hitTimes = new TH1D("hitTimes", "Hit Times", 1000, 0, 15000);
  hitTimes->SetLineColor(38);
  hitTimes->SetFillColor(38);
  hitTimes->GetXaxis()->SetTitle("Time (ns)");

  hitCharges = new TH1D("hitCharges", "Hit Charges", 100, 0, 1100);
  hitCharges->SetLineColor(46);
  hitCharges->SetFillColor(46);
  hitCharges->GetXaxis()->SetTitle("Charge (pC)");

  hitTimesAndCharges = new TH2D("hitTimesAndCharges", "Hit Times and Charges",
                                150, 0, 1500, 110, 0, 1100);
  hitTimesAndCharges->GetXaxis()->SetTitle("Time (ns)");
  hitTimesAndCharges->GetYaxis()->SetTitle("Charge (pC)");

  /* Create the canvas and divide it up
    -------------------
   | ----------------- |
   ||     HEATMAP     ||
   ||     T vs Q      ||
   | ----------------- |
   | -------   ------- |
   ||  TIME | | CHRGE ||
   | -------   ------- |
    -------------------
  */

  plotCanvas = new TCanvas("displayCanvas", "Display Canvas", 1920, 1080);
  plotCanvas->Divide(1, 2);
  plotPad = plotCanvas->cd(2);
  plotPad->Divide(2, 1);
  plotCanvas->cd(2);

  // Get the TQReal branch
  myTreeReader->Get("TQREAL", myTQReal);

  return true;
}

bool evPlot::Execute() {

  // Get the number of hit PMTs from the TQReal branch
  // totalPMTsHit = myTQReal->cables.size();
  totalPMTsHit = sktqz_.nqiskz;
  std::cout << "THIS NUMBER OF CABLES " << totalPMTsHit << std::endl;

  if (totalPMTsHit == 0) {
    std::cout << "No PMTs hit in this event!" << std::endl;
    return true;
  }

  // If number of actual hits is less than 10000 then skip
  if (sktqz_.nqiskz < 10000) {
    std::cout << "Number of hits is less than 10000, skipping..." << std::endl;
    return true;
  }

  // Loop over the hit PMTs and fill the histograms, call the iterator pmtNumber
  for (int pmtNumber = 0; pmtNumber < totalPMTsHit; ++pmtNumber) {
    // cableNumber = myTQReal->cables.at(pmtNumber);
    // charge = myTQReal->Q.at(pmtNumber);
    // time = myTQReal->T.at(pmtNumber);
    cableNumber = sktqz_.icabiz[pmtNumber];
    charge = sktqz_.qiskz[cableNumber];
    time = sktqz_.tiskz[cableNumber];
    // Fill the histograms
    hitTimes->Fill(time);
    hitCharges->Fill(charge);
    hitTimesAndCharges->Fill(time, charge);
  }

  double minval = hitTimes->GetMinimum();
  double maxval = hitTimes->GetMaximum();
  hitTimes->GetXaxis()->SetRangeUser(minval, maxval);
  hitTimesAndCharges->GetXaxis()->SetRangeUser(minval, maxval);

  minval = hitCharges->GetMinimum();
  maxval = hitCharges->GetMaximum();
  hitCharges->GetXaxis()->SetRangeUser(minval, maxval);
  hitTimesAndCharges->GetYaxis()->SetRangeUser(minval, maxval);

  // Draw the histograms
  plotPad->cd(1);
  hitTimes->Draw();
  plotPad->cd(2);
  hitCharges->Draw();
  plotCanvas->cd(1);
  hitTimesAndCharges->Draw("COLZ");

  plotCanvas->cd();
  plotCanvas->Modified();
  plotCanvas->Update();

  plotCanvas->SaveAs("testOutput.png");

  std::cout << "Hit enter to continue..." << std::endl;
  std::cin.get();

  // Clear histograms
  hitTimes->Reset();
  hitCharges->Reset();
  hitTimesAndCharges->Reset();

  return true;
}

bool evPlot::Finalise() { return true; }
