#include "evPlot.h"

evPlot::evPlot() : Tool() {}

bool evPlot::Initialise(std::string configfile, DataModel &data) {

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
  } else {
    myTreeReader = m_data->Trees.at(treeReaderName);
  }

  if (!m_variables.Get("verbose", m_verbose))
    m_verbose = 1;

  // Initialise the histograms
  hitTimes = new TH1D("hitTimes", "Hit Times", 100, 0, 1500);
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

  return true;
}

bool evPlot::Execute() {
  std::cout << "THIS NUMBER OF CABLES " << std::endl;

  // Get the number of hit PMTs from the TQReal branch
  totalPMTsHit = myTQReal->cables.size();

  if (totalPMTsHit == 0) {
    std::cout << "No PMTs hit in this event!" << std::endl;
    return true;
  }

  /*
  // Loop over the hit PMTs and fill the histograms, call the iterator pmtNumber
  for (int pmtNumber = 0; pmtNumber < totalPMTsHit; ++pmtNumber) {
    cableNumber = myTQReal->cables.at(pmtNumber);
    charge = myTQReal->Q.at(pmtNumber);
    time = myTQReal->T.at(pmtNumber);

    // Fill the histograms
    hitTimes->Fill(time);
    hitCharges->Fill(charge);
    hitTimesAndCharges->Fill(time, charge);
  }

    // Draw the histograms
    plotPad->cd(1);
    hitTimes->Draw();
    plotPad->cd(2);
    hitCharges->Draw();
    plotCanvas->cd(1);
    hitTimesAndCharges->Draw("COLZ");

    gPad->WaitPrimitive();
  */
  return true;
}

bool evPlot::Finalise() { return true; }
