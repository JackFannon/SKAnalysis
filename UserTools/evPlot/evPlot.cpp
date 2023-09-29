#include "evPlot.h"

#include "TError.h"
#include "TGaxis.h"
#include "TH2.h"
#include "TROOT.h"
#include "TStyle.h"

#include "skheadC.h"
#include "sktqC.h"
#include "type_name_as_string.h"

#include <bitset>
#include <string>

evPlot::evPlot() : Tool() {
  // Get the name of the tool
  toolName = type_name<decltype(this)>();
  toolName.pop_back();
}

bool evPlot::Initialise(std::string configfile, DataModel &data) {

  gROOT->SetBatch(1);

  gErrorIgnoreLevel = kWarning;

  if (configfile != "")
    m_variables.Initialise(configfile);
  // m_variables.Print();

  m_data = &data;
  m_log = m_data->Log;

  // Get the config variables: TreeReader, which plots to create, output
  // directory, etc.
  m_variables.Get("treeReaderName", treeReaderName);
  m_variables.Get("outputDir", outputDir);
  m_variables.Get("outputName", outputName);
  m_variables.Get("dataSrc", dataSrc);
  m_variables.Get("maskTriggers", maskTriggers);
  m_variables.Get("Tmin", Tmin);
  m_variables.Get("Tmax", Tmax);
  m_variables.Get("Qmin", Qmin);
  m_variables.Get("Qmax", Qmax);

  // Get the TreeReader
  if (m_data->Trees.count(treeReaderName) == 0) {
    Log("ERROR: TreeReader " + treeReaderName + " not found!", v_error,
        verbosity);
    return false;
  }

  myTreeReader = m_data->Trees.at(treeReaderName);

  if (!m_variables.Get("verbose", m_verbose))
    m_verbose = 1;

  // --- GStyle stuff ---
  // No axis ticks
  gStyle->SetTickLength(0., "Y");
  // No stats box
  gStyle->SetOptStat(0);

  // Calculate the number of bins for the histograms
  int Tbins = (Tmax - Tmin) / 10;
  int Qbins = (Qmax - Qmin) * 10;

  // Initialise the histograms
  hitTimes = new TH1D("hitTimes", "Hit Times", Tbins, Tmin, Tmax);
  hitTimes->SetLineColor(38);
  hitTimes->SetFillColor(38);
  hitTimes->GetXaxis()->SetTitle("Time (ns)");

  hitCharges = new TH1D("hitCharges", "Hit Charges", Qbins, Qmin, Qmax);
  hitCharges->SetLineColor(46);
  hitCharges->SetFillColor(46);
  hitCharges->GetXaxis()->SetTitle("Charge (pC)");

  // hitTimesAndCharges =
  // new TH2D("hitTimesAndCharges", "Hit Times and Charges", Tbins / 2, Tmin,
  // Tmax,
  //           Qbins / 100, Qmin, 10 * Qmax);
  // hitTimesAndCharges->GetXaxis()->SetTitle("Time (ns)");
  // hitTimesAndCharges->GetYaxis()->SetTitle("Charge (pC)");

  /* Create the canvas and divide it up
    -------------------
   | ----------------- |
   ||     HEATMAP     ||
   ||     T vs Q      ||
   | ----------------- |
   | -------   ------- |
   ||  TIME | | CHRGE ||
   | -------   ------- |
   | ----------------- |
   ||      GRAPH      ||
   ||     T vs Q      ||
   | ----------------- |
    -------------------
  */

  plotCanvas = new TCanvas("displayCanvas", "Display Canvas", 1920, 1080);
  plotCanvas->Divide(1, 3);
  plotPad = plotCanvas->cd(2);
  plotPad->Divide(2, 1);
  plotCanvas->cd(2);

  plotCanvas->Print((outputName + "[").c_str());

  // Get Header and TQReal branches
  myTreeReader->Get("Header", myHeader);
  myTreeReader->Get("TQREAL", myTQReal);

  return true;
}

bool evPlot::Execute() {

  // Got to declare the TGraph here; there is no function to delete all points
  // from a TGraph. Have to delete the pointer and reallocate.
  TGraph *hitTimesVsCharges = new TGraph();

  std::bitset<sizeof(int) * 8> triggerID;
  triggerID = skhead_.idtgsk;
  // initlialise string Trigs to store trigger strings in
  std::string Trigs;
  // loop over the 31 trigger IDs and check if the trigger bit for each ID, i,
  // is 1 pull trigger strings from Trigger_ID_To_Trigger using
  // Constants::TriggerIDToTrigger and append them to trigs
  for (int i = 0; i <= 31; i++) {
    if (triggerID.test(i))
      Trigs += " " + TriggerIDToTrigger(i) + ",";
  }

  // Only pass if the HE and OD triggers are set
  if (!(triggerID.test(1) && triggerID.test(3)) && maskTriggers) {
    return true;
  }

  switch (dataSrc) {
  case 0: {
    // sktq common
    totalPMTsHit = sktqz_.nqiskz;
    if (totalPMTsHit == 0) {
      std::cout << "No PMTs hit in this event!" << std::endl;
      return true;
    }
    // If number of actual hits is less than 10000 then skip
    if (totalPMTsHit < 0) {
      std::cout << "Number of hits is less than 10000, skipping..."
                << std::endl;
      return true;
    }
    // Loop over the hits
    for (int hitNumber = 0; hitNumber < totalPMTsHit; ++hitNumber) {
      cableNumber = sktqz_.icabiz[hitNumber];
      charge = sktqz_.qiskz[hitNumber];
      time = sktqz_.tiskz[hitNumber];
      // Fill the histograms
      if (charge != 0) {
        hitTimes->Fill(time);
        hitCharges->Fill(charge);
        timeVec.push_back(time);
        chargeVec.push_back(charge);
        // Find max charge
        if (charge > maxQ) {
          maxQ = charge;
        }
        if (time > Tmin && time < Tmax && charge > Qmin) {
          hitTimesVsCharges->SetPoint(hitNumber, time, charge);
        }
      }
    }
    break;
  }
  case 1: {
    // skt and skq commons
    totalPMTsHit = skq_.nqisk;
    if (totalPMTsHit == 0) {
      std::cout << "No PMTs hit in this event!" << std::endl;
      return true;
    }
    // If number of actual hits is less than 10000 then skip
    if (totalPMTsHit < 0) {
      std::cout << "Number of hits is less than 10000, skipping..."
                << std::endl;
      return true;
    }
    // Loop over the hits
    for (int hitNumber = 0; hitNumber < totalPMTsHit; ++hitNumber) {
      cableNumber = skchnl_.ihcab[hitNumber];
      charge = skq_.qisk[cableNumber - 1];
      time = skt_.tisk[cableNumber - 1];
      // Fill the histograms
      if (charge != 0) {
        hitTimes->Fill(time);
        hitCharges->Fill(charge);
        timeVec.push_back(time);
        chargeVec.push_back(charge);
        // Find max charge
        if (charge > maxQ) {
          maxQ = charge;
        }
        if (time > Tmin && time < Tmax && charge > Qmin) {
          hitTimesVsCharges->SetPoint(hitNumber, time, charge);
        }
      }
    }
    break;
  }
  case 2: {
    // tqreal branch
    totalPMTsHit = myTQReal->cables.size();
    std::cout << totalPMTsHit << std::endl;
    if (totalPMTsHit == 0) {
      std::cout << "No PMTs hit in this event!" << std::endl;
      return true;
    }
    // If number of actual hits is less than 10000 then skip
    if (totalPMTsHit < 0) {
      std::cout << "Number of hits is less than 10000, skipping..."
                << std::endl;
      return true;
    }
    // Loop over the hits
    for (int hitNumber = 0; hitNumber < totalPMTsHit; ++hitNumber) {
      cableNumber = myTQReal->cables.at(hitNumber);
      charge = myTQReal->Q.at(hitNumber);
      time = myTQReal->T.at(hitNumber);
      // Fill the histograms
      if (charge != 0) {
        hitTimes->Fill(time);
        hitCharges->Fill(charge);
        timeVec.push_back(time);
        chargeVec.push_back(charge);
        // Find max charge
        if (charge > maxQ) {
          maxQ = charge;
        }
        if (time > Tmin && time < Tmax && charge > Qmin) {
          hitTimesVsCharges->SetPoint(hitNumber, time, charge);
        }
      }
    }
    break;
  }
  default: {
    Log(toolName + " ERROR: Invalid data source - " + std::to_string(dataSrc),
        v_error, verbosity);
    break;
  }
  }

  TH2D *hitTimesAndCharges =
      new TH2D("hitTimesAndCharges", "Hit Times and Charges", 100, 0, Tmax, 100,
               0, maxQ);

  // Loop over timeVec and chargeVec and fill hitTimesAndCharges
  for (int i = 0; i < timeVec.size(); i++) {
    hitTimesAndCharges->Fill(timeVec.at(i), chargeVec.at(i));
  }

  // Draw the histograms
  plotPad->cd(1);
  hitTimes->Draw();
  plotPad->cd(2);
  hitCharges->Draw();
  plotCanvas->cd(1);
  hitTimesAndCharges->Draw("COLZ");
  plotCanvas->cd(3);
  gPad->SetLogy(1);
  hitTimesVsCharges->Draw("AP");

  hitTimesVsCharges->GetXaxis()->SetTitle("Time (ns)");
  hitTimesVsCharges->GetYaxis()->SetTitle("Charge (pC)");
  hitTimesVsCharges->GetXaxis()->SetLimits(1.0 * Tmin, 1.0 * Tmax);
  hitTimesVsCharges->GetYaxis()->SetLimits(1.0 * Qmin, 1.0 * Qmax);

  plotCanvas->cd();
  plotCanvas->Modified();
  plotCanvas->Update();

  // Append the plot to a pdf on a new page
  plotCanvas->Print(outputName.c_str());

  // Clear histograms
  hitTimes->Reset();
  hitCharges->Reset();
  hitTimesAndCharges->Reset();

  // Delete the TGraph
  delete hitTimesVsCharges;

  return true;
}

bool evPlot::Finalise() {
  plotCanvas->Print((outputName + "]").c_str());
  return true;
}
