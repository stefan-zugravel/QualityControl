// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// \file   ZDCRecBeautifyPlots.cxx
/// \author Stefan Cristi Zugravel
///

#include "ZDC/ZDCRecBeautifyPlots.h"
#include "QualityControl/MonitorObject.h"
#include "QualityControl/Quality.h"
#include "QualityControl/QcInfoLogger.h"
// ROOT
#include <TH1.h>
#include <TF1.h>
#include <TH2.h>
#include <TText.h>
#include <TLine.h>
#include <TMarker.h>

#include <DataFormatsQualityControl/FlagType.h>
#include <DataFormatsQualityControl/FlagTypeFactory.h>

using namespace std;
using namespace o2::quality_control;

namespace o2::quality_control_modules::zdc
{

void ZDCRecBeautifyPlots::configure()
{
  // THUS FUNCTION BODY IS AN EXAMPLE. PLEASE REMOVE EVERYTHING YOU DO NOT NEED.
  // This method is called whenever CustomParameters are set.

  // Example of retrieving a custom parameter
  std::string parameter = mCustomParameters.atOrDefaultValue("myOwnKey1", "default");
}

Quality ZDCRecBeautifyPlots::check(std::map<std::string, std::shared_ptr<MonitorObject>>* moMap)
{
  Quality result = Quality::Null;
  return result;
}

std::string ZDCRecBeautifyPlots::getAcceptedType()
{
  // This method is a remnant of early interface prototype and will be removed in the scope of ticket QC-373
  return "TH1";
}

void ZDCRecBeautifyPlots::beautify(std::shared_ptr<MonitorObject> mo, Quality checkResult)
{
  if (mo->getName() == "h_CENTR_ZNA" || mo->getName() == "h_CENTR_ZNC" || mo->getName() == "h_CENTR_ZNA_cut_ZEM" || mo->getName() == "h_CENTR_ZNC_cut_ZEM") {
    auto* h = dynamic_cast<TH2F*>(mo->getObject());
    if (h == nullptr) {
      ILOG(Error, Support) << "could not cast '" << mo->getName() << "' to TH2*" << ENDM;
      return;
    }
    auto* lineH = new TLine(0.5, 0, -0.5, 0);
    auto* lineV = new TLine(0, 0.5, 0, -0.5);
    auto* marker = new TMarker(h->GetMean(1), h->GetMean(2), 20);
    lineH->SetLineColor(kBlack);
    lineV->SetLineColor(kBlack);
    lineH->SetLineWidth(2);
    lineV->SetLineWidth(2);
    marker->SetMarkerColor(2);
    h->GetListOfFunctions()->Add(lineH);
    h->GetListOfFunctions()->Add(lineV);
    h->GetListOfFunctions()->Add(marker);
  }
  if (mo->getName() == "h_TDC_ZNA_TC_A_H" || mo->getName() == "h_TDC_ZNA_SUM_A_H" || mo->getName() == "h_TDC_ZNC_TC_A_H" || mo->getName() == "h_TDC_ZNC_SUM_A_H") {
    auto* h = dynamic_cast<TH1F*>(mo->getObject());
    if (h == nullptr) {
      ILOG(Error, Support) << "Could not cast `h_TDC_ZNA_TC_A_H` to TH1*, skipping" << ENDM;
      return;
    }
    TF1 *fitFunc = new TF1("fitFunc", "[0]*exp(-x/[1]) + [2]*exp(-0.5*((x-[3])/[4])^2) + [5]*exp(-0.5*((x-[6])/[7])^2) + [8]*exp(-0.5*((x-[9])/[10])^2) + [11]*exp(-0.5*((x-[12])/[13])^2) + [14]*exp(-0.5*((x-[15])/[16])^2)", 12, 150);
    fitFunc->SetParName(0, "Exp_A");
    fitFunc->SetParName(1, "Exp_tau");
    fitFunc->SetParName(2, "G1_A");
    fitFunc->SetParName(3, "G1_mean");
    fitFunc->SetParName(4, "G1_sigma");
    fitFunc->SetParName(5, "G2_A");
    fitFunc->SetParName(6, "G2_mean");
    fitFunc->SetParName(7, "G2_sigma");
    fitFunc->SetParName(8, "G3_A");
    fitFunc->SetParName(9, "G3_mean");
    fitFunc->SetParName(10,"G3_sigma");
    fitFunc->SetParName(11,"G4_A");
    fitFunc->SetParName(12,"G4_mean");
    fitFunc->SetParName(13,"G4_sigma");
    fitFunc->SetParName(14,"G5_A");
    fitFunc->SetParName(15,"G5_mean");
    fitFunc->SetParName(16,"G5_sigma");
    fitFunc->SetParameter(0, 0.5*h->GetMaximum());          // Exp_A
    fitFunc->SetParameter(1, 75);             // Exp_tau
    fitFunc->SetParameter(2, 0.5*h->GetMaximum());          // CB1_A
    fitFunc->SetParameter(3, 31);              // CB1_mean
    fitFunc->SetParameter(4, 4);               // CB1_sigma
    fitFunc->SetParameter(5, 0.2*h->GetMaximum());          // CB2_A
    fitFunc->SetParameter(6, 70);              // CB2_mean
    fitFunc->SetParameter(7, 8);              // CB2_sigma
    //fitFunc->SetParameter(8, 0.1*h->GetMaximum());          // CB3_A
    fitFunc->SetParameter(9, 90);              // CB3_mean
    fitFunc->SetParameter(10, 9);              // CB3_sigma
    //fitFunc->SetParameter(11, 0.05*h->GetMaximum());          // CB4_A
    fitFunc->SetParameter(12, 120);              // CB4_mean
    fitFunc->SetParameter(13, 11);              // CB4_sigma
    //fitFunc->SetParameter(14, 0.05*h->GetMaximum());          // CB4_A
    fitFunc->SetParameter(15, 150);              // CB4_mean
    fitFunc->SetParameter(16, 15);              // CB4_sigma
    fitFunc->SetParLimits(0,  0, 0.8*h->GetMaximum());  // Exp_A
    fitFunc->SetParLimits(1, 50, 175);       // Exp_tau
    fitFunc->SetParLimits(2, 0, h->GetMaximum()); // CB1_A
    fitFunc->SetParLimits(3, 10, 50);          // CB1_mean
    fitFunc->SetParLimits(4, 3, 9);         // CB1_sigma
    fitFunc->SetParLimits(5, 0, h->GetMaximum());  // CB2_A
    fitFunc->SetParLimits(6, 50, 90);         // CB2_mean
    fitFunc->SetParLimits(7, 5, 15);        // CB2_sigma
    fitFunc->SetParLimits(8, 0, h->GetMaximum());  // CB3_A
    fitFunc->SetParLimits(9, 70, 110);         // CB3_mean
    fitFunc->SetParLimits(10, 5, 15);        // CB3_sigma
    fitFunc->SetParLimits(11, 0, h->GetMaximum());  // CB4_A
    fitFunc->SetParLimits(12, 100, 140);         // CB4_mean
    fitFunc->SetParLimits(13, 5, 15);        // CB4_sigma
    fitFunc->SetParLimits(14, 0, h->GetMaximum());  // CB4_A
    fitFunc->SetParLimits(15, 110, 170);         // CB4_mean
    fitFunc->SetParLimits(16, 10, 30);        // CB4_sigma
    h->Fit(fitFunc, "R");
    TLine *line = new TLine(fitFunc->GetParameter(3), h->GetMinimum(), fitFunc->GetParameter(3), h->GetMaximum());
    line->SetLineColor(kBlack); // Set line color to red
    line->SetLineWidth(3);      // Set line width
    std::ostringstream position;
    position << "1n position  : " << fitFunc->GetParameter(3);
    std::ostringstream sigma;
    sigma << "1n sigma     : " << fitFunc->GetParameter(4);
    std::ostringstream resolution;
    resolution << "1n resolution: " << (fitFunc->GetParameter(4) / fitFunc->GetParameter(3)) * 100 << " %";
    TText *text1 = new TText(fitFunc->GetParameter(3) + 50, h->GetMaximum(), position.str().c_str());
    text1->SetTextColor(kBlack); // Set text color
    text1->SetTextSize(0.04);    // Set the text size
    TText *text2 = new TText(fitFunc->GetParameter(3) + 50, h->GetMaximum()/2, sigma.str().c_str());
    text2->SetTextColor(kBlack); // Set text color
    text2->SetTextSize(0.04);    // Set the text size
    TText *text3 = new TText(fitFunc->GetParameter(3) + 50, h->GetMaximum()/4, resolution.str().c_str());
    text3->SetTextColor(kBlack); // Set text color
    text3->SetTextSize(0.04);    // Set the text size
    h->GetListOfFunctions()->Add(text1);
    h->GetListOfFunctions()->Add(text2);
    h->GetListOfFunctions()->Add(text3);
    h->GetListOfFunctions()->Add(line);
    h->GetListOfFunctions()->Add(fitFunc);
  }
  if (mo->getName() == "h_TDC_ZPA_TC_A_H" || mo->getName() == "h_TDC_ZPA_SUM_A_H" || mo->getName() == "h_TDC_ZPC_TC_A_H" || mo->getName() == "h_TDC_ZPC_SUM_A_H") {
    auto* h2 = dynamic_cast<TH1F*>(mo->getObject());
    if (h2 == nullptr) {
      ILOG(Error, Support) << "Could not cast `h_TDC_ZPA_TC_A_H` to TH1*, skipping" << ENDM;
      return;
    }
    TF1 *fitFunc1p = new TF1("fitFunc", "[0]*exp(-x/[1]) + [2]*exp(-0.5*((x-[3])/[4])^2) + [5]*exp(-0.5*((x-[6])/[7])^2) + [8]*exp(-0.5*((x-[9])/[10])^2) ", 15, 250);
    fitFunc1p->SetParName(0, "Exp_A");
    fitFunc1p->SetParName(1, "Exp_tau");
    fitFunc1p->SetParName(2, "G1_A");
    fitFunc1p->SetParName(3, "G1_mean");
    fitFunc1p->SetParName(4, "G1_sigma");
    fitFunc1p->SetParName(5, "G2_A");
    fitFunc1p->SetParName(6, "G2_mean");
    fitFunc1p->SetParName(7, "G2_sigma");
    fitFunc1p->SetParName(8, "G3_A");
    fitFunc1p->SetParName(9, "G3_mean");
    fitFunc1p->SetParName(10,"G3_sigma");
    fitFunc1p->SetParameter(0, 0.5*h2->GetMaximum());          // Exp_A
    fitFunc1p->SetParameter(1, 125);             // Exp_tau
    fitFunc1p->SetParameter(2, 0.5*h2->GetMaximum());          // CB1_A
    fitFunc1p->SetParameter(3, 17);              // CB1_mean
    fitFunc1p->SetParameter(4, 6);               // CB1_sigma
    fitFunc1p->SetParameter(5, 0.4*h2->GetMaximum());          // CB2_A
    fitFunc1p->SetParameter(6, 73);              // CB2_mean
    fitFunc1p->SetParameter(7, 10);              // CB2_sigma
    fitFunc1p->SetParameter(8, 0.1*h2->GetMaximum());          // CB2_A
    fitFunc1p->SetParameter(9, 140);              // CB2_mean
    fitFunc1p->SetParameter(10,15);              // CB2_sigma
    fitFunc1p->SetParLimits(0,  0.2*h2->GetMaximum(), 0.8*h2->GetMaximum());  // Exp_A
    fitFunc1p->SetParLimits(1, 75, 175);       // Exp_tau
    fitFunc1p->SetParLimits(2, 0.2*h2->GetMaximum(), 0.8*h2->GetMaximum()); // CB1_A
    fitFunc1p->SetParLimits(3, 10, 30);          // CB1_mean
    fitFunc1p->SetParLimits(4, 3, 9);         // CB1_sigma
    fitFunc1p->SetParLimits(5, 0.1*h2->GetMaximum(), 0.7*h2->GetMaximum());  // CB2_A
    fitFunc1p->SetParLimits(6, 50, 100);         // CB2_mean
    fitFunc1p->SetParLimits(7, 5, 15);        // CB2_sigma
    fitFunc1p->SetParLimits(8, 0.01*h2->GetMaximum(), 0.3*h2->GetMaximum());  // CB2_A
    fitFunc1p->SetParLimits(9, 100, 150);         // CB2_mean
    fitFunc1p->SetParLimits(10,10, 25);        // CB2_sigma  
    h2->Fit(fitFunc1p, "R");
    TLine *line2 = new TLine(fitFunc1p->GetParameter(6), h2->GetMinimum(), fitFunc1p->GetParameter(6), h2->GetMaximum());
    line2->SetLineColor(kBlack); // Set line color to red
    line2->SetLineWidth(3);    // Set line width
    std::ostringstream position;
    position << "1p position  : " << fitFunc1p->GetParameter(6);
    std::ostringstream sigma;
    sigma << "1p sigma     : " << fitFunc1p->GetParameter(7);
    std::ostringstream resolution;
    resolution << "1p resolution: " << (fitFunc1p->GetParameter(7) / fitFunc1p->GetParameter(6)) * 100 << " %";    
    TText *text4 = new TText(fitFunc1p->GetParameter(6) + 150, h2->GetMaximum(), position.str().c_str());
    text4->SetTextColor(kBlack); // Set text color
    text4->SetTextSize(0.04);    // Set the text size
    TText *text5 = new TText(fitFunc1p->GetParameter(6) + 150, h2->GetMaximum()/2, sigma.str().c_str());
    text5->SetTextColor(kBlack); // Set text color
    text5->SetTextSize(0.04);    // Set the text size
    TText *text6 = new TText(fitFunc1p->GetParameter(6) + 150, h2->GetMaximum()/4, resolution.str().c_str());
    text6->SetTextColor(kBlack); // Set text color
    text6->SetTextSize(0.04);    // Set the text size
    h2->GetListOfFunctions()->Add(text4);
    h2->GetListOfFunctions()->Add(text5);
    h2->GetListOfFunctions()->Add(text6);
    h2->GetListOfFunctions()->Add(line2);
    h2->GetListOfFunctions()->Add(fitFunc1p);
  }
}

void ZDCRecBeautifyPlots::reset()
{
  // THUS FUNCTION BODY IS AN EXAMPLE. PLEASE REMOVE EVERYTHING YOU DO NOT NEED.
  ILOG(Debug, Devel) << "ZDCRecBeautifyPlots::reset" << ENDM;
  // please reset the state of the check here to allow for reuse between consecutive runs.
}

void ZDCRecBeautifyPlots::startOfActivity(const Activity& activity)
{
  // THUS FUNCTION BODY IS AN EXAMPLE. PLEASE REMOVE EVERYTHING YOU DO NOT NEED.
  ILOG(Debug, Devel) << "ZDCRecBeautifyPlots::start : " << activity.mId << ENDM;
}

void ZDCRecBeautifyPlots::endOfActivity(const Activity& activity)
{
  // THUS FUNCTION BODY IS AN EXAMPLE. PLEASE REMOVE EVERYTHING YOU DO NOT NEED.
  ILOG(Debug, Devel) << "ZDCRecBeautifyPlots::end : " << activity.mId << ENDM;
}

} // namespace o2::quality_control_modules::zdc
