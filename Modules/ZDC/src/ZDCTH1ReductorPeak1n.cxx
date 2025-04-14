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
/// \file   ZDCTH1ReductorPeak1n.cxx
/// \author Piotr Konopka and ZDC code added by Stefan Cristi Zugravel
///

#include <TH1.h>
#include <TF1.h>
#include "ZDC/ZDCTH1ReductorPeak1n.h"

namespace o2::quality_control_modules::zdc
{

void* ZDCTH1ReductorPeak1n::getBranchAddress()
{
  return &mStats;
}

const char* ZDCTH1ReductorPeak1n::getBranchLeafList()
{
  return "mean/D:stddev:entries:position1n";
}

void ZDCTH1ReductorPeak1n::update(TObject* obj)
{
  // todo: use GetStats() instead?
  auto histo = dynamic_cast<TH1*>(obj);
  if (histo) {
    mStats.entries = histo->GetEntries();
    mStats.stddev = histo->GetStdDev();
    mStats.mean = histo->GetMean();
    TF1* fitfunction = histo->GetFunction("fitFunc");
    if (fitfunction) {
      mStats.position1n = fitfunction->GetParameter(3);
    }
  }
}

} // namespace o2::quality_control_modules::zdc
